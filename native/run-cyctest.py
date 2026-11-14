#!/usr/bin/env python3
"""Translate, link, and run the upstream library suite, retaining each failure."""
import argparse, os, pathlib, subprocess, sys
p=argparse.ArgumentParser();p.add_argument('--build',default='native/build');p.add_argument('--stage-dir');p.add_argument('--compiler');p.add_argument('--out',default='native/compiler-tests/cyctest');p.add_argument('--cc',default=os.environ.get('CC') or subprocess.check_output(['native/select-cc.sh'],text=True).strip());a=p.parse_args()
b=pathlib.Path(a.build);stage=pathlib.Path(a.stage_dir) if a.stage_dir else None;out=pathlib.Path(a.out);out.mkdir(parents=True,exist_ok=True);compiler=a.compiler or str((stage or b)/'cyclone64')
objects=sorted((b/'cyclone/stage0/library/std').rglob('*.o'))+sorted((b/'cyclone/library/runtime').glob('*.o'))
if stage: objects=sorted((stage/'library/std').glob('*.o'))+[stage/'native/generated/lineno.o',stage/'native/generated/libc/cycstubs.o',stage/'cstubs.o',stage/'boot_cstubs.o']+sorted((b/'cyclone/library/runtime').glob('*.o'))
commands=[('cpp',[a.cc,'-E','-x','c','-nostdinc','-Inative/generated','-Inative/include','-Inative/compat','-isystem','native/generated/libc','testsuite/cyctest/cyctest.cyc','-o',str(out/'cyctest.cyp')]),('translate',[compiler,'--cifc-inst-tvar','--better-widen','-o',str(out/'cyctest.c'),str(out/'cyctest.cyp')]),('link',[a.cc,'-std=gnu11','-fno-strict-aliasing','-O2','-g','-march=native','-Wno-error=incompatible-pointer-types','-Ilibrary/std/cyc-lib',str(out/'cyctest.c'),*map(str,objects),'-lbacktrace','-ldl','-lm','-lpthread','-o',str(out/'cyctest')]),('run',['gdb','-q','-batch','-ex','set debuginfod enabled off','-ex','handle SIGUSR1 nostop noprint pass','-ex','handle SIGUSR2 nostop noprint pass','-ex','run','-ex','bt','--args',str(out/'cyctest')])]
for phase,cmd in commands:
    try:q=subprocess.run(cmd,capture_output=True,text=True,timeout=120)
    except subprocess.TimeoutExpired: print(phase+': TIMEOUT');sys.exit(1)
    text=q.stdout+q.stderr;(out/(phase+'.log')).write_text(text)
    ok=q.returncode==0 if phase!='run' else ('exited normally' in text and 'received signal' not in text)
    print(phase+': '+('PASS' if ok else 'FAIL'),flush=True)
    if not ok:sys.exit(1)
