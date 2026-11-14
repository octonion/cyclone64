#!/usr/bin/env python3
"""Exercise installed tools from a temporary directory without source includes."""
import argparse
import json
import os
from pathlib import Path
import shlex
import shutil
import subprocess
import tempfile

p=argparse.ArgumentParser(description=__doc__)
p.add_argument('--prefix',type=Path,required=True)
p.add_argument('--out',type=Path,required=True)
a=p.parse_args(); prefix=a.prefix.resolve(); out=a.out.resolve(); out.mkdir(parents=True,exist_ok=True)
root=Path(__file__).resolve().parent.parent
driver=prefix/'bin/cyclone64'
commands=[]
env=os.environ.copy()
for key in ('CPATH','C_INCLUDE_PATH','CPLUS_INCLUDE_PATH','PKG_CONFIG_SYSROOT_DIR'):
    env.pop(key,None)
env['PKG_CONFIG_PATH']=str(prefix/'lib/pkgconfig')
def run(cmd,label,cwd):
    cmd=list(map(str,cmd)); commands.append({'label':label,'argv':cmd})
    (out/'commands.json').write_text(json.dumps(commands,indent=2)+'\n')
    q=subprocess.run(cmd,cwd=cwd,env=env,capture_output=True,text=True,timeout=120)
    (out/(label+'.log')).write_text(q.stdout+q.stderr)
    if q.returncode: raise SystemExit(f'{label}: FAIL ({q.returncode}); see {out/(label+".log")}')
    print(label+': PASS',flush=True)
    return q.stdout
with tempfile.TemporaryDirectory(prefix='cyclone64-installed-') as tmp:
    tmp=Path(tmp)
    shutil.copy2(root/'native/cyclone-tests/abi-helper.c',tmp/'abi-helper.c')
    for group in ('cyclone-tests','layout-tests'):
        for source in sorted((root/'native'/group).glob('*.cyc')):
            label=group+'-'+source.stem
            target=tmp/(label+'.cyc');shutil.copy2(source,target)
            run([driver,target,'abi-helper.c','-o',label],label+'-compile',tmp)
            run([tmp/label],label+'-run',tmp)
    simple=tmp/'stdio.cyc'
    simple.write_text('#include <core.h>\n#include <stdio.h>\nusing Core;\nint main(int argc,string_t ?argv) { printf("installed stdio: %d\\n",42); return 0; }\n')
    run([driver,simple,'-o','stdio'],'stdio-compile',tmp)
    assert 'installed stdio: 42' in run([tmp/'stdio'],'stdio-run',tmp)
    run([driver,simple,'-c','-o','stdio.o'],'compile-only',tmp)
    run([driver,'stdio.o','-o','object-program'],'link-object',tmp)
    run([tmp/'object-program'],'object-run',tmp)
    run([driver,simple,'-E','-o','stdio.cyp'],'preprocess',tmp)
    run([driver,'stdio.cyp','--emit-c','-o','stdio.c'],'emit-c',tmp)
    flags=shlex.split(run(['pkg-config','--cflags','--libs','--static','cyclone64'],'pkgconfig-cyclone',tmp))
    config=json.loads((prefix/'share/cyclone64/config.json').read_text())
    cc=shlex.split(env.get('CC',config['cc']))
    cflags=shlex.split(env.get('CFLAGS',config['cflags']))
    ldflags=shlex.split(env.get('LDFLAGS',config['ldflags']))
    run([*cc,*cflags,'stdio.c',*flags,*ldflags,'-o','c-program'],'compile-generated-c',tmp)
    run([tmp/'c-program'],'generated-c-run',tmp)
    shutil.copy2(root/'native/tests/solver.c',tmp/'solver.c')
    flags=shlex.split(run(['pkg-config','--cflags','--libs','banshee64'],'pkgconfig-banshee',tmp))
    run([*cc,*cflags,'solver.c',*flags,*ldflags,'-o','solver'],'solver-compile',tmp)
    run([tmp/'solver'],'solver-run',tmp)
print('Installed toolchain checks passed.',flush=True)
