#!/usr/bin/env python3
"""Compile and run every top-level historical application; retain commands and logs."""
import argparse, concurrent.futures, json, os, pathlib, shlex, signal, subprocess, sys
from runtime_build import runtime_objects, runtime_libs
p=argparse.ArgumentParser(description=__doc__)
p.add_argument('--stage-dir',required=True);p.add_argument('--build',required=True)
p.add_argument('--cc',default=os.environ.get('CC','gcc'));p.add_argument('--cflags',default='-O2 -g')
p.add_argument('--out',default='native/compiler-tests/applications');p.add_argument('--jobs',type=int,default=4)
a=p.parse_args();out=pathlib.Path(a.out).resolve();out.mkdir(parents=True,exist_ok=True)
build=pathlib.Path(a.build).resolve();stage=pathlib.Path(a.stage_dir).resolve()
objects=sorted((stage/'library/std').glob('*.o'))+[stage/'native/generated/lineno.o',stage/'native/generated/libc/cycstubs.o',stage/'cstubs.o',stage/'boot_cstubs.o']+runtime_objects(build)
def execute(cmd,label,timeout=120):
    cmd=list(map(str,cmd));(out/(label+'.command.json')).write_text(json.dumps(cmd,indent=2)+'\n')
    try:
        r=subprocess.run(cmd,stdin=subprocess.DEVNULL,capture_output=True,text=True,timeout=timeout)
        text=r.stdout+r.stderr
    except subprocess.TimeoutExpired as e:
        text=(e.stdout or b'').decode(errors='replace')+(e.stderr or b'').decode(errors='replace')
        (out/(label+'.log')).write_text(text);return 124,text
    (out/(label+'.log')).write_text(text);return r.returncode,text

archive=out/'libapplication-runtime.a'
archive.unlink(missing_ok=True)
code,text=execute([os.environ.get('AR','ar'),'rcs',archive,*objects],'archive')
if code: raise SystemExit('application runtime archive failed: '+text)

def test(src):
    name=src.stem;pre=out/(name+'.cyp');gen=out/(name+'.c');exe=out/name
    for f in (pre,gen,exe):f.unlink(missing_ok=True)
    cmds=[('cpp',[a.cc,'-E','-x','c','-nostdinc','-Inative/generated','-Inative/include','-Inative/compat','-isystem','native/generated/libc',src,'-o',pre]),
          ('translate',[stage/'cyclone64','--cifc-inst-tvar','--better-widen','-o',gen,pre]),
          ('link',[a.cc,'-std=gnu11','-fno-strict-aliasing',*shlex.split(a.cflags),'-Werror=pointer-to-int-cast','-Ilibrary/std/cyc-lib',gen,'native/compiler-checks/test-process.c','-Wl,--start-group',archive,'-Wl,--end-group',*shlex.split(os.environ.get('LDFLAGS','')),*runtime_libs(build),'-o',exe])]
    for phase,cmd in cmds:
        code,text=execute(cmd,name+'-'+phase)
        if code:
            return name,f'{phase} FAIL ({code})'
    args=[str(exe)]
    if name=='histogram':args.append('testsuite/tests/histogram.txt')
    code,text=execute(args,name+'-run',60)
    if name=='sigaction_test':
        ok=code==-signal.SIGTERM and all(s in text for s in ['got a sigusr1!','got a sigusr2!','ignored sigterm']) and 'oops--should die' not in text
    else:ok=code==0
    if name.startswith('refcnt_'):ok=ok and 'All tests passed.' in text
    if name=='scheme-2':ok=ok and 'program returned:' in text and '##unspecified##' in text
    return name,'PASS' if ok else f'run FAIL ({code})'
with concurrent.futures.ThreadPoolExecutor(max_workers=a.jobs) as pool:
    results=list(pool.map(test,sorted(pathlib.Path('testsuite/tests').glob('*.cyc'))))
summary=''.join(f'{name}: {result}\n' for name,result in results)
(out/'results.txt').write_text(summary);print(summary,end='')
sys.exit(any(result!='PASS' for _,result in results))
