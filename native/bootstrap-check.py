#!/usr/bin/env python3
"""Attempt stage1 translation with generated native libc bindings, recording blockers."""
import argparse, os, pathlib, subprocess, sys
p = argparse.ArgumentParser()
p.add_argument("--build", default="native/build")
p.add_argument("--cc", default=os.environ.get("CC") or subprocess.check_output(["native/select-cc.sh"], text=True).strip())
a = p.parse_args()
out = pathlib.Path('native/bootstrap'); out.mkdir(exist_ok=True)
results=[]
for src in sorted(pathlib.Path('library/compiler').glob('*.cyc')):
    if src.name.endswith('.err.cyc') or src.stem in ('bansheeif_stub', 'rgnorder'): continue
    pre=out/(src.stem+'.cyp'); generated=out/(src.stem+'.c')
    commands=[[a.cc,'-E','-x','c','-nostdinc','-Inative/generated','-Inative/include','-Inative/compat','-isystem','native/generated/libc',str(src),'-o',str(pre)],[str(pathlib.Path(a.build)/"cyclone64"),'--cifc-inst-tvar','--better-widen','-o',str(generated),str(pre)],[a.cc,'-std=gnu11','-Wno-error=incompatible-pointer-types','-Ilibrary/std/cyc-lib','-c',str(generated),'-o',str(out/(src.stem+'.o'))]]
    for phase,cmd in zip(('preprocess','translate','compile'),commands):
        try: p=subprocess.run(cmd,capture_output=True,text=True,timeout=30)
        except subprocess.TimeoutExpired:
            results.append(f'{src.stem}: {phase} TIMEOUT');break
        (out/(src.stem+'-'+phase+'.log')).write_text(p.stdout+p.stderr)
        if p.returncode:
            results.append(f'{src.stem}: {phase} FAIL ({p.returncode})');break
    else: results.append(f'{src.stem}: PASS')
summary='\n'.join(results)+'\n'; (out/'results.txt').write_text(summary); print(summary,end='')
sys.exit(any('PASS' not in x for x in results))
