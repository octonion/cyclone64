#!/usr/bin/env python3
"""Link a boundary regression against the stage's actual generated prover."""
import argparse,json,pathlib,subprocess,sys
p=argparse.ArgumentParser();p.add_argument('--stage-dir',required=True);p.add_argument('--out',required=True);a=p.parse_args()
stage=pathlib.Path(a.stage_dir);out=pathlib.Path(a.out);out.mkdir(parents=True,exist_ok=True)
cmd=json.loads((stage/'commands.json').read_text())[-1]
if cmd[-2:]!=['-o',str(stage/'cyclone64')]: raise SystemExit('stage manifest has no final compiler link')
cmd=[s for s in cmd if not any(x in s for x in ['/tools/cyclone/cyclone.o','/library/compiler/pratt_prover.o'])]
cmd[1:1]=['-Ilibrary/std/cyc-lib','-DPRATT_GENERATED="'+str((stage/'library/compiler/pratt_prover.c').resolve())+'"','native/compiler-checks/pratt-constants.c']
cmd[-1]=str(out/'pratt-constants')
(out/'commands.json').write_text(json.dumps([cmd,[cmd[-1]]],indent=2)+'\n')
for label,argv in [('link',cmd),('run',[cmd[-1]])]:
 q=subprocess.run(argv,capture_output=True,text=True,timeout=90)
 (out/(label+'.log')).write_text(q.stdout+q.stderr)
 print(label+': '+('PASS' if q.returncode==0 else 'FAIL'))
 if q.returncode: sys.exit(1)
