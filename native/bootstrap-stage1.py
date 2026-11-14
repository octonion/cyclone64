#!/usr/bin/env python3
"""Build source-derived Cyclone stage1; retain individual commands and diagnostics."""
import argparse, concurrent.futures, json, os, pathlib, shlex, subprocess, sys
from runtime_build import runtime_objects, compiler_libs
p=argparse.ArgumentParser()
p.add_argument('--jobs', type=int, default=4)
p.add_argument('--cflags', default='-O2 -g')
p.add_argument('--build',default='native/build')
p.add_argument('--out',default='native/bootstrap/stage1')
p.add_argument('--compiler',default=None)
p.add_argument('--cc',default=os.environ.get('CC') or subprocess.check_output(['native/select-cc.sh'],text=True).strip())
a=p.parse_args(); build=pathlib.Path(a.build); out=pathlib.Path(a.out); out.mkdir(parents=True,exist_ok=True)
compiler=a.compiler or str(build/'cyclone64'); results=[]; objects=[]; commands=[]
(out/'cyclone64').unlink(missing_ok=True)
def run(cmd, log):
    commands.append(list(map(str,cmd)))
    with open(log,'w') as f:
        try: q=subprocess.run(cmd,stdout=f,stderr=subprocess.STDOUT,timeout=90)
        except subprocess.TimeoutExpired: return False
    return q.returncode==0
sources=sorted(pathlib.Path('library/compiler').glob('*.cyc'))+sorted(pathlib.Path('library/std').glob('*.cyc'))
sources=[s for s in sources if not s.name.endswith('.err.cyc') and s.stem not in ('bansheeif_stub','rgnorder')]
sources += [pathlib.Path(s) for s in ['native/generated/lex.cyc','native/generated/lineno.cyc','native/generated/cyclone/library/compiler/parse_tab.cyc','native/generated/libc/cycstubs.cyc','tools/cyclone/cyclone.cyc']]
def compile_source(src):
    dest=out/src.with_suffix('');dest.parent.mkdir(parents=True,exist_ok=True)
    pre=str(dest)+'.cyp'; gen=str(dest)+'.c'; obj=str(dest)+'.o'
    for artifact in (pre, gen, obj): pathlib.Path(artifact).unlink(missing_ok=True)
    steps=[('preprocess',[a.cc,'-E','-x','c','-nostdinc','-Inative/generated','-Inative/include','-Inative/compat','-isystem','native/generated/libc',str(src),'-o',pre]),('translate',[compiler,'--cifc-inst-tvar','--better-widen','-o',gen,pre]),('compile',[a.cc,'-std=gnu11','-march=native','-fno-strict-aliasing',*shlex.split(a.cflags),'-Wno-error=incompatible-pointer-types','-Ilibrary/std/cyc-lib','-c',gen,'-o',obj])]
    for phase,cmd in steps:
        if not run(cmd,str(dest)+'-'+phase+'.log') or not pathlib.Path({'preprocess':pre,'translate':gen,'compile':obj}[phase]).is_file():
            result=f'{src}: {phase} FAIL';break
    else:
        result=f'{src}: PASS'
    print(result,flush=True)
    return result, obj if result.endswith(": PASS") else None
with concurrent.futures.ThreadPoolExecutor(max_workers=a.jobs) as pool:
    for result, obj in pool.map(compile_source, sources):
        results.append(result)
        if obj: objects.append(obj)
for src in ['native/generated/libc/cstubs.c','library/std/boot_cstubs.c']:
    obj=out/(pathlib.Path(src).stem+'.o')
    ok=run([a.cc,'-std=gnu11',*shlex.split(a.cflags),'-D_POSIX_C_SOURCE=200809L','-D_DEFAULT_SOURCE','-Ilibrary/std/cyc-lib','-c',src,'-o',str(obj)],str(obj)+'.log')
    results.append(f'{src}: '+('PASS' if ok else 'compile FAIL'))
    if ok: objects.append(str(obj))
if all('PASS' in r for r in results):
    support=runtime_objects(build)+[build/'cyclone/library/compiler/inference/cycinf.o',build/'cyclone/library/compiler/inference/type_name.o',build/'config.o',build/'libbanshee64.a']
    ok=run([a.cc,'-march=native',*shlex.split(a.cflags),*objects,*map(str,support),*compiler_libs(build),'-o',str(out/'cyclone64')],out/'link.log')
    results.append('stage link: '+('PASS' if ok else 'FAIL'))
(out/'commands.json').write_text(json.dumps(commands,indent=2)+'\n')
(out/'results.txt').write_text('\n'.join(results)+'\n')
sys.exit(any('PASS' not in r for r in results))
