#!/usr/bin/env python3
"""Build the historical checked-in C in an isolated checkout with an explicit ILP32 compiler."""
import argparse, concurrent.futures, json, pathlib, subprocess, sys
p=argparse.ArgumentParser();p.add_argument('--checkout',required=True);p.add_argument('--cc',required=True);p.add_argument('--out',required=True);p.add_argument('--jobs',type=int,default=8);a=p.parse_args()
root=pathlib.Path(a.checkout).resolve();out=pathlib.Path(a.out).resolve();out.mkdir(parents=True,exist_ok=True);cc=str(pathlib.Path(a.cc).resolve());commands=[]
flags=['-std=gnu11','-O2','-g','-fno-strict-aliasing','-D_POSIX_C_SOURCE=200809L','-D_DEFAULT_SOURCE','-DCYC_STATIC_MAIN','-DBANSHEE_ROLLBACK','-Ddeletes=','-Dtraditional=','-Dsameregion=','-Dparentptr=','-Wno-error=incompatible-pointer-types','-Wno-error=implicit-function-declaration','-Wno-error=int-conversion','-Ilibrary/std/cyc-lib','-Ilibrary/banshee','-Ilibrary/banshee/rlib']
def run(cmd,log):
    commands.append(cmd)
    with open(log,'w') as f:
        q=subprocess.run(cmd,cwd=root,stdout=f,stderr=subprocess.STDOUT,timeout=120)
    return q.returncode==0
sources=sorted(root.glob('stage0/library/compiler/*.c'))+sorted(root.glob('stage0/library/std/*.c'))+sorted(root.glob('stage0/library/std/libc/*.c'))+sorted(root.glob('library/runtime/*.c'))+sorted(root.glob('library/banshee/*.c'))+[root/s for s in ['library/banshee/rlib/regions.c','library/banshee/rlib/dhash.c','library/compiler/inference/cycinf.c','stage0/tools/cyclone/cyclone.c']]
sources=[s for s in sources if str(s.relative_to(root)) not in ('stage0/library/compiler/config.c','library/banshee/bounds.c')]
def compile_source(src):
    rel=src.relative_to(root);obj=out/rel.with_suffix('.o');obj.parent.mkdir(parents=True,exist_ok=True)
    obj.unlink(missing_ok=True)
    ok=run([cc,*flags,'-c',str(rel),'-o',str(obj)],str(obj)+'.log')
    result=f'{rel}: '+('PASS' if ok else 'FAIL');print(result,flush=True);return result,obj if ok else None
with concurrent.futures.ThreadPoolExecutor(max_workers=a.jobs) as pool: pairs=list(pool.map(compile_source,sources))
results=[r for r,_ in pairs];objects=[str(o) for _,o in pairs if o]
if all(o for _,o in pairs):
    ok=run([cc,'library/compiler/make-config.c','-o',str(out/'make-config')],out/'make-config.log')
    if ok:
        with open(out/'config.c','w') as f: subprocess.run([str(out/'make-config')],stdout=f,cwd=root,check=True)
        ok=run([cc,*flags,'-c',str(out/'config.c'),'-o',str(out/'config.o')],out/'config.log')
    if ok: ok=run([cc,*objects,str(out/'config.o'),'-lbacktrace','-ldl','-lm','-lpthread','-o',str(out/'cyclone32')],out/'link.log')
    results.append('stage0 link: '+('PASS' if ok else 'FAIL'))
(out/'commands.json').write_text(json.dumps(commands,indent=2)+'\n');(out/'results.txt').write_text('\n'.join(results)+'\n')
sys.exit(any('FAIL' in r for r in results))
