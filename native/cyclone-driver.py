#!/usr/bin/env python3
"""Installed native Cyclone compile/link driver (GCC-compatible basic options)."""
import argparse
import json
import os
from pathlib import Path
import shlex
import signal
import subprocess
import sys
import tempfile

config_path = (Path(__file__).resolve().parent / '@CONFIG_REL@').resolve()
config = json.loads(config_path.read_text())
base = config_path.parent
resolve = lambda key: str((base / config[key]).resolve())
p = argparse.ArgumentParser(description=__doc__)
p.add_argument('--version',action='version',version='Cyclone64 native LP64 candidate driver')
p.add_argument('inputs', nargs='*')
p.add_argument('-o', dest='output')
p.add_argument('-c', action='store_true', help='compile without linking')
p.add_argument('-E', action='store_true', help='preprocess only')
p.add_argument('--emit-c', action='store_true', help='translate to C without compiling')
p.add_argument('--cc', default=os.environ.get('CC', config['cc']))
p.add_argument('--arch', default=os.environ.get('ARCH', config['arch']))
p.add_argument('--cyclone-flag', action='append', default=[], help='translator option; use --cyclone-flag=--option')
p.add_argument('--print-config', action='store_true')
for flag in ('I', 'D', 'U', 'L', 'l'):
    p.add_argument('-'+flag, action='append', default=[])
a, extra = p.parse_known_intermixed_args()
if a.print_config:
    print(json.dumps(config, indent=2)); sys.exit(0)
if '--legacy-layouts' in a.cyclone_flag:
    p.error('the installed runtime requires native layouts')
if not a.inputs:
    p.error('supply .cyc, .cyp, .c, .o or .a inputs')
if any(not f.startswith('-') for f in extra):
    p.error('unsupported option argument; use -I/-D/-U/-L/-l for options with arguments')
if sum((a.c, a.E, a.emit_c)) > 1:
    p.error('-c, -E and --emit-c are mutually exclusive')
if (a.E or a.emit_c or (a.c and a.output)) and len(a.inputs) != 1:
    p.error('this output mode requires one input')
cc = shlex.split(a.cc)
cpp = shlex.split(os.environ.get('CPPFLAGS', config['cppflags']))
cpp += ['-'+flag+v for flag in ('I','D','U') for v in getattr(a, flag)]
cflags = shlex.split(a.arch) + shlex.split(os.environ.get('CFLAGS', config['cflags'])) + extra
ldflags = shlex.split(os.environ.get('LDFLAGS', config['ldflags']))
ldflags += ['-'+flag+v for flag in ('L','l') for v in getattr(a, flag)]

def run(argv):
    subprocess.run(argv, check=True)

try:
    with tempfile.TemporaryDirectory(prefix='cyclone64-') as tmp:
        products = []
        for i, name in enumerate(a.inputs):
            src = Path(name)
            if src.suffix not in ('.cyc', '.cyp', '.c', '.o', '.a'):
                p.error('unsupported input: '+name)
            if not src.is_file(): p.error('input does not exist: '+name)
            if src.suffix in ('.o', '.a'):
                if a.c or a.E or a.emit_c: p.error('object/archive input requires linking')
                products.append(str(src)); continue
            if src.suffix in ('.cyc', '.cyp'):
                pre = str(Path(tmp)/f'{i}.cyp')
                if src.suffix == '.cyc':
                    cmd = cc + ['-E','-x','c','-nostdinc',*cpp,
                        '-I'+resolve('include'), '-I'+resolve('std_include'),
                        '-I'+resolve('compat'), '-isystem',resolve('libc_include'), str(src)]
                    if a.E:
                        run(cmd + (['-o',a.output] if a.output else [])); continue
                    run(cmd + ['-o',pre])
                else:
                    pre = str(src)
                    if a.E:
                        if a.output: Path(a.output).write_bytes(src.read_bytes())
                        else: sys.stdout.buffer.write(src.read_bytes())
                        continue
                generated = a.output if a.emit_c and a.output else str(Path(tmp)/f'{i}.c')
                run([resolve('translator'), *a.cyclone_flag, '-o',generated,pre])
                if a.emit_c:
                    if not a.output: sys.stdout.write(Path(generated).read_text())
                    continue
                source = generated
            else:
                if a.emit_c: p.error('--emit-c requires Cyclone source')
                source = str(src)
                if a.E:
                    run(cc+['-E',*cpp,source]+(['-o',a.output] if a.output else [])); continue
            obj = (a.output or src.with_suffix('.o').name) if a.c else str(Path(tmp)/f'{i}.o')
            run(cc+['-std=gnu11','-fno-strict-aliasing',*cpp,*cflags,
                    '-I'+resolve('c_include'),'-c',source,'-o',obj])
            products.append(obj)
        if not (a.c or a.E or a.emit_c):
            run(cc+[*cflags,*products,'-Wl,--start-group',resolve('library'),
                    '-Wl,--end-group',*ldflags,*config['libs'],'-o',a.output or 'a.out'])
except subprocess.CalledProcessError as e:
    if e.returncode < 0:
        number = -e.returncode
        try: name = signal.Signals(number).name
        except ValueError: name = str(number)
        print(f"cyclone64: {Path(e.cmd[0]).name} terminated by signal {name} ({number})",
              file=sys.stderr)
        sys.exit(128 + number)
    sys.exit(e.returncode)
