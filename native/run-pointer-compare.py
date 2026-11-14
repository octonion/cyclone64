#!/usr/bin/env python3
"""Exercise full-width identity against an actual source-built compiler."""
import argparse,json,pathlib,subprocess,sys
p=argparse.ArgumentParser();p.add_argument('--stage-dir',required=True);p.add_argument('--out',required=True);a=p.parse_args()
stage=pathlib.Path(a.stage_dir);out=pathlib.Path(a.out);out.mkdir(parents=True,exist_ok=True)
cmd=json.loads((stage/'commands.json').read_text())[-1]
if cmd[-2:]!=['-o',str(stage/'cyclone64')]: raise SystemExit('stage manifest has no final compiler link')
cmd=[s for s in cmd if '/tools/cyclone/cyclone.o' not in s]
if (stage/'library/compiler/toc.c').read_text().startswith('#define CYC_NATIVE_LAYOUTS'): cmd.insert(1,'-DCYC_NATIVE_LAYOUTS')
cmd[1:1]=['-Ilibrary/std/cyc-lib','native/compiler-checks/pointer-compare.c'];cmd[-1]=str(out/'pointer-compare')
(out/'commands.json').write_text(json.dumps([cmd,[cmd[-1]]],indent=2)+'\n')
for label,argv in [('link',cmd),('run',[cmd[-1]])]:
    q=subprocess.run(argv,capture_output=True,text=True,timeout=90)
    (out/(label+'.log')).write_text(q.stdout+q.stderr)
    print(label+': '+('PASS' if q.returncode==0 else 'FAIL'))
    if q.returncode: sys.exit(1)
