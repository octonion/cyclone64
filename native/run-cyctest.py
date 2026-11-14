#!/usr/bin/env python3
"""Translate, link, and run the upstream library suite, retaining each failure."""
import argparse, json, os, pathlib, shlex, subprocess, sys
from runtime_build import runtime_objects, runtime_libs
p=argparse.ArgumentParser();p.add_argument('--cflags',default='-O2 -g');p.add_argument('--direct',action='store_true',help='run without ptrace for sanitizer compatibility');p.add_argument('--build',default='native/build');p.add_argument('--stage-dir');p.add_argument('--compiler');p.add_argument('--out',default='native/compiler-tests/cyctest');p.add_argument('--cc',default=os.environ.get('CC') or subprocess.check_output(['native/select-cc.sh'],text=True).strip());a=p.parse_args()
b=pathlib.Path(a.build);stage=pathlib.Path(a.stage_dir) if a.stage_dir else None;out=pathlib.Path(a.out);out.mkdir(parents=True,exist_ok=True);compiler=a.compiler or str((stage or b)/'cyclone64')
for artifact in ('cyctest.cyp','cyctest.c','cyctest'): (out/artifact).unlink(missing_ok=True)
objects=sorted((b/'cyclone/stage0/library/std').rglob('*.o'))+runtime_objects(b)
if stage: objects=sorted((stage/'library/std').glob('*.o'))+[stage/'native/generated/lineno.o',stage/'native/generated/libc/cycstubs.o',stage/'cstubs.o',stage/'boot_cstubs.o']+runtime_objects(b)
commands=[('cpp',[a.cc,'-E','-x','c','-nostdinc','-Inative/generated','-Inative/include','-Inative/compat','-isystem','native/generated/libc','testsuite/cyctest/cyctest.cyc','-o',str(out/'cyctest.cyp')]),('translate',[compiler,'--cifc-inst-tvar','--better-widen','-o',str(out/'cyctest.c'),str(out/'cyctest.cyp')]),('link',[a.cc,'-std=gnu11','-fno-strict-aliasing',*shlex.split(a.cflags),'-march=native','-Wno-error=incompatible-pointer-types','-Ilibrary/std/cyc-lib',str(out/'cyctest.c'),*map(str,objects),*runtime_libs(b),'-o',str(out/'cyctest')]),('run',['gdb','-q','-batch','-ex','set debuginfod enabled off','-ex','handle SIGUSR1 nostop noprint pass','-ex','handle SIGUSR2 nostop noprint pass','-ex','run','-ex','bt','--args',str(out/'cyctest')])]
if a.direct: commands[-1] = ('run',[str(out/'cyctest')])
(out/'commands.json').write_text(json.dumps(commands,indent=2)+'\n')
for phase,cmd in commands:
    try:q=subprocess.run(cmd,capture_output=True,text=True,timeout=120)
    except subprocess.TimeoutExpired: print(phase+': TIMEOUT');sys.exit(1)
    text=q.stdout+q.stderr;(out/(phase+'.log')).write_text(text)
    ok=q.returncode==0 if phase!='run' or a.direct else ('exited normally' in text and 'received signal' not in text)
    if phase=='run': ok = ok and 'All tests passed.' in text
    else: ok = ok and (out/{'cpp':'cyctest.cyp','translate':'cyctest.c','link':'cyctest'}[phase]).is_file()
    print(phase+': '+('PASS' if ok else 'FAIL'),flush=True)
    if not ok:sys.exit(1)
