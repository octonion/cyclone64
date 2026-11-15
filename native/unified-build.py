#!/usr/bin/env python3
"""Coordinate the native Make interface in a private generated source tree."""
import fcntl
import hashlib
import json
import os
from pathlib import Path
import re
import shlex
import shutil
import subprocess
import sys
import tempfile

ROOT = Path(__file__).resolve().parent.parent
BUILD = Path(os.environ.get('BUILD', 'build/native')).resolve()
ACTION = sys.argv[1]
if BUILD == ROOT or ROOT.is_relative_to(BUILD):
    raise SystemExit('BUILD must not contain the source checkout')
marker = BUILD / 'unified-build.json'
if BUILD.exists() and any(p.name != 'lock' for p in BUILD.iterdir()) and not marker.is_file():
    raise SystemExit(f'{BUILD}: not a managed native build; choose a fresh BUILD directory')
if ACTION == 'clean':
    if marker.is_file():
        with (BUILD/'lock').open('a') as lock:
            fcntl.flock(lock, fcntl.LOCK_EX)
            owned=json.loads(marker.read_text())
            if owned.get('format') != 1 or owned.get('source') != str(ROOT):
                raise SystemExit('Refusing to clean a build owned by a different source tree')
            shutil.rmtree(BUILD)
    print('Cleaned managed build:', BUILD)
    sys.exit(0)
BUILD.mkdir(parents=True, exist_ok=True)
lock = (BUILD/'lock').open('a')
fcntl.flock(lock, fcntl.LOCK_EX)
if not marker.exists(): marker.write_text(json.dumps({'format':1,'source':str(ROOT)})+'\n')
state = json.loads(marker.read_text())
if state.get('format') != 1 or state.get('source') != str(ROOT):
    raise SystemExit('BUILD belongs to a different source tree')
cc = os.environ.get('CC') or subprocess.check_output([ROOT/'native/select-cc.sh'],text=True).strip()
arch = os.environ.get('ARCH')
if arch is None: arch = subprocess.check_output([ROOT/'native/select-arch.sh',cc],text=True).strip()
match = re.search(r'(?:^|\s)(?:-j|--jobs=)(\d+)',os.environ.get('MAKEFLAGS',''))
jobs = int(os.environ.get('JOBS') or (match[1] if match else min(os.cpu_count() or 2,8)))
if jobs < 1: raise SystemExit('JOBS must be positive')
config = {key:os.environ.get(key,default) for key,default in
          [('LIBC_CC',cc),('AR','ar'),('CFLAGS','-O2 -g'),('CPPFLAGS',''),('LDFLAGS',''),('NATIVE_GC','system')]}
config.update(CC=cc, ARCH=arch, compiler_version=subprocess.check_output([cc,'--version'],text=True),
              libc_compiler_version=subprocess.check_output([config['LIBC_CC'],'--version'],text=True))
if 'config' in state and state['config'] != config:
    raise SystemExit('Build configuration changed. Use a fresh BUILD directory or make clean first.')
state['config'] = config
marker.write_text(json.dumps(state,indent=2)+'\n')
env = os.environ.copy()
env.update({k:v for k,v in config.items() if k not in ('compiler_version','libc_compiler_version')})
env.update(JOBS=str(jobs), BUILD='native/build', MAKEFLAGS='', MFLAGS='', MAKELEVEL='0')
work = BUILD/'src'
logs = BUILD/'logs'; logs.mkdir(exist_ok=True)

def run(cmd, label, cwd=None, environment=None):
    cmd = list(map(str,cmd))
    print(label, flush=True)
    (logs/(label+'.command.json')).write_text(json.dumps(cmd,indent=2)+'\n')
    with (logs/(label+'.log')).open('w') as log:
        result = subprocess.run(cmd,cwd=cwd or work,env=environment or env,stdout=log,stderr=subprocess.STDOUT)
    if result.returncode:
        print((logs/(label+'.log')).read_text()[-6000:],file=sys.stderr)
        raise SystemExit(f'{label} failed ({result.returncode}); see {logs/(label+".log")}')

# Explicit source roots keep generated files and old experimental builds out.
# This also works with a source archive: no Git command is required to build.
def source_files():
    roots = [ROOT/d for d in ('library','stage0','tools','testsuite')]
    roots += [ROOT/'native'/d for d in ('compat','tests','compiler-checks','cyclone-tests',
              'cyclone-reject','cyclone-blockers','layout-tests','diagnostics')]
    for tree in roots:
        for path in sorted(tree.rglob('*')):
            if path.is_file() and '__pycache__' not in path.parts and path.suffix not in ('.o','.pyc'):
                yield path
    for path in sorted((ROOT/'native').iterdir()):
        if path.is_file() and (path.suffix in ('.py','.sh','.c','.txt','.md') or path.name=='Makefile'):
            yield path
    for name in ('LICENSE','LICENSE.LIB'):
        yield ROOT/name

files = list(source_files())
def build_input(path):
    parts=Path(path).parts
    return parts[0] in ('library','stage0','tools') or path.startswith('native/compat/') or path in {
        'native/Makefile','native/prepare-bootstrap.sh','native/bootstrap-stage1.py',
        'native/select-cc.sh','native/select-arch.sh','native/runtime_build.py'}

hash_state = hashlib.sha256()
changed = False
new_files={str(source.relative_to(ROOT)) for source in files}
for old in state.get('source_files',[]):
    if old not in new_files:
        target=work/old
        if target.is_file() or target.is_symlink(): target.unlink()
        changed = changed or build_input(old)
for source in files:
    relative=str(source.relative_to(ROOT)); data=source.read_bytes()
    if build_input(relative): hash_state.update(relative.encode()+b'\0'+data)
    dest=work/relative
    if not dest.is_file() or dest.read_bytes()!=data:
        changed = changed or build_input(relative)
        dest.parent.mkdir(parents=True,exist_ok=True)
        if dest.is_symlink(): dest.unlink()
        shutil.copy2(source,dest)
state['source_files']=sorted(new_files)
signature=hash_state.hexdigest()
needs_build = changed or state.get('dirty',False) or not (BUILD/'lib/libcyclone64.a').is_file() or not (BUILD/'lib/libbanshee64.a').is_file() or not (BUILD/'bin/cyclone64-compiler').is_file()
if needs_build:
    state['dirty']=True
    marker.write_text(json.dumps(state,indent=2)+'\n')
    # Stale generated sources/objects must not survive a changed source set.
    for name in ('build','generated','include','bootstrap','compiler-tests'):
        target=work/'native'/name
        if target.exists(): shutil.rmtree(target)
    run(['sh','native/prepare-bootstrap.sh'],'prepare')
    previous='native/build/cyclone64'
    for name in ('seed','stage1','stage2'):
        stage='native/bootstrap/'+name
        run([sys.executable,'native/bootstrap-stage1.py','--compiler',previous,
             '--build','native/build','--out',stage,'--cc',cc,'--arch='+arch,
             '--cflags='+config['CFLAGS'],'--jobs',str(jobs)],'bootstrap-'+name)
        previous=stage+'/cyclone64'
    run([sys.executable,'native/compare-bootstrap.py','--left','native/bootstrap/stage1',
         '--right','native/bootstrap/stage2','--out','native/bootstrap/convergence.json'],'convergence')
    (BUILD/'bin').mkdir(exist_ok=True); (BUILD/'lib').mkdir(exist_ok=True)
    shutil.copy2(work/previous,BUILD/'bin/cyclone64-compiler')
    shutil.copy2(work/'native/build/libbanshee64.a',BUILD/'lib/libbanshee64.a')
    stage=work/'native/bootstrap/stage2'
    objects=sorted((stage/'library/std').glob('*.o'))
    objects += [stage/p for p in ('native/generated/lineno.o','native/generated/libc/cycstubs.o','cstubs.o','boot_cstubs.o')]
    objects += sorted((work/'native/build/cyclone/library/runtime').glob('*.o'))
    archive=BUILD/'lib/libcyclone64.a'
    archive.unlink(missing_ok=True)
    run([config['AR'],'rcsD',archive,*objects],'archive-runtime')
    state.update(source_hash=signature,dirty=False)
    marker.write_text(json.dumps(state,indent=2)+'\n')
else:
    marker.write_text(json.dumps(state,indent=2)+'\n')
    print('Build is up to date:', BUILD, flush=True)

if ACTION == 'check':
    run([sys.executable,'native/check-driver.py'],'check-driver')
    run(['make','-f','native/Makefile','-j'+str(jobs),'check','rlib-check',
         'native/build/check-runtime-bounds','native/build/check-gc-allocation',
         'native/build/check-callback-ffi','native/build/check-type-name','native/build/check-allocation-width'],'check-banshee-build-runtime')
    for name in ('runtime-bounds','gc-allocation','callback-ffi','type-name','allocation-width'):
        run(['native/build/check-'+name],'check-'+name)
    if config['NATIVE_GC']=='system':
        run(['make','-f','native/Makefile','native/build/check-gc-collection'],'build-gc-collection')
        run(['native/build/check-gc-collection'],'check-gc-collection')
    options=['--stage-dir','native/bootstrap/stage2','--build','native/build','--cc',cc,
             '--cflags='+arch+' '+config['CFLAGS']]
    run([sys.executable,'native/run-cyclone-tests.py',*options,'--output','native/compiler-tests/all'],'check-cyclone')
    run([sys.executable,'native/run-cyclone-tests.py',*options,'--skip-diagnostics','--skip-rejections',
         '--probe-dir','native/layout-tests','--output','native/compiler-tests/layout'],'check-layout')
    run([sys.executable,'native/run-cyctest.py','--direct',*options,'--out','native/compiler-tests/upstream'],'check-upstream')
    run([sys.executable,'native/run-applications.py',*options,'--out','native/compiler-tests/applications'], 'check-applications')
    for name in ('pratt-constants','pointer-compare'):
        run([sys.executable,'native/run-'+name+'.py','--stage-dir','native/bootstrap/stage2',
             '--out','native/compiler-tests/'+name],'check-'+name)
    run([sys.executable,'native/compare-bootstrap.py','--left','native/bootstrap/stage1',
         '--right','native/bootstrap/stage2','--out','native/bootstrap/convergence.json'],'check-convergence')
    print('All native checks passed.',flush=True)

if ACTION == 'install':
    prefix=Path(os.environ.get('prefix','/usr/local'))
    if not prefix.is_absolute(): raise SystemExit('PREFIX/prefix must be absolute')
    destdir=Path(os.environ.get('DESTDIR') or '/')
    dest=(destdir / prefix.relative_to('/')).resolve()
    installed=[]
    def copy(source,relative):
        target=dest/relative; target.parent.mkdir(parents=True,exist_ok=True)
        shutil.copy2(source,target); installed.append(str(relative)); return target
    def write(relative,text,executable=False):
        target=dest/relative; target.parent.mkdir(parents=True,exist_ok=True)
        target.write_text(text); target.chmod(0o755 if executable else 0o644); installed.append(str(relative))
    copy(BUILD/'bin/cyclone64-compiler','libexec/cyclone64/cyclone64-compiler')
    for name in ('libbanshee64.a','libcyclone64.a'): copy(BUILD/'lib'/name,'lib/'+name)
    for path in (work/'library/banshee').rglob('*.h'):
        copy(path,Path('include/banshee64')/path.relative_to(work/'library/banshee'))
    for source,relative in [(work/'library/std','share/cyclone64/include/cyclone/library/std'),
                            (work/'native/generated/libc','share/cyclone64/libc'),
                            (work/'native/compat','share/cyclone64/compat'),
                            (work/'library/std/cyc-lib','include/cyclone64')]:
        for path in source.rglob('*.h'): copy(path,Path(relative)/path.relative_to(source))
    runtime=json.loads((work/'native/build/runtime-config.json').read_text())
    driver_config={'cc':cc,'arch':arch,'cflags':config['CFLAGS'],'cppflags':config['CPPFLAGS'],
        'ldflags':config['LDFLAGS'],'translator':'../../libexec/cyclone64/cyclone64-compiler',
        'library':'../../lib/libcyclone64.a','include':'include',
        'std_include':'include/cyclone/library/std','c_include':'../../include/cyclone64',
        'libc_include':'libc','compat':'compat',
        'libs':['-lbacktrace','-ldl','-lm','-lpthread',*runtime['ldflags']]}
    write('share/cyclone64/config.json',json.dumps(driver_config,indent=2)+'\n')
    write('bin/cyclone64',(ROOT/'native/cyclone-driver.py').read_text().replace('@CONFIG_REL@','../share/cyclone64/config.json'),True)
    pc='prefix=${pcfiledir}/../..\nlibdir=${prefix}/lib\nincludedir=${prefix}/include\n\n'
    write('lib/pkgconfig/banshee64.pc',pc+'Name: Banshee64\nDescription: Native LP64 constraint solver\nVersion: 0.1\nLibs: -L${libdir} -lbanshee64\nCflags: -std=gnu11 -I${includedir}/banshee64 -I${includedir}/banshee64/rlib -DBANSHEE_ROLLBACK -Ddeletes= -Dtraditional= -Dsameregion= -Dparentptr=\n')
    write('lib/pkgconfig/cyclone64.pc',pc+'Name: Cyclone64\nDescription: Native Cyclone runtime and standard library\nVersion: 0.1\nLibs: -L${libdir} -lcyclone64\nLibs.private: '+' '.join(driver_config['libs'])+'\nCflags: -std=gnu11 -I${includedir}/cyclone64 -fno-strict-aliasing\n')
    for name in ('README.md','STATUS.txt','NATIVE-LAYOUTS.txt'):
        copy(ROOT/'native'/name,'share/doc/cyclone64/'+name)
    for name in ('LICENSE','LICENSE.LIB'): copy(ROOT/name,'share/doc/cyclone64/'+name)
    write('share/cyclone64/install-manifest.json',json.dumps({'prefix':str(prefix),'files':installed},indent=2)+'\n')
    print('Installed:',dest,flush=True)
    # A native installation is only successful after public headers, archives
    # and the relocated driver compile and run clients outside the source tree.
    with tempfile.TemporaryDirectory(prefix='install-check-',dir=BUILD) as tmp:
        tmp=Path(tmp)
        (tmp/'hello.cyc').write_text('#include <core.h>\nusing Core;\nint main(int argc,string_t ?argv) { return sizeof(void *)==8 && sizeof(long)==8 ? 0 : 1; }\n')
        run([dest/'bin/cyclone64','hello.cyc','-o','hello'],'install-cyclone-compile',tmp)
        run([tmp/'hello'],'install-cyclone-run',tmp)
        pkg_env=env.copy(); pkg_env['PKG_CONFIG_PATH']=str(dest/'lib/pkgconfig')
        pkg_env.pop('PKG_CONFIG_SYSROOT_DIR',None)
        args=shlex.split(subprocess.check_output(['pkg-config','--cflags','--libs','banshee64'],env=pkg_env,text=True))
        shutil.copy2(ROOT/'native/tests/solver.c',tmp/'banshee.c')
        run([cc,*shlex.split(arch+' '+config['CFLAGS']),'banshee.c',*args,*shlex.split(config['LDFLAGS']),'-o','banshee'],'install-banshee-compile',tmp)
        run([tmp/'banshee'],'install-banshee-run',tmp)
    print('Installed Banshee64 and Cyclone64 clients passed.',flush=True)
