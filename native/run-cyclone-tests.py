#!/usr/bin/env python3
"""Run stage0 diagnostics and native ABI probes; retain all generated code/logs."""
import argparse, difflib, json, os, pathlib, shlex, subprocess, sys
from runtime_build import runtime_objects, runtime_libs
p = argparse.ArgumentParser()
p.add_argument('--skip-rejections', action='store_true', help='omit native-only literal diagnostics for a historical reference')
p.add_argument('--skip-diagnostics', action='store_true', help='skip exact upstream diagnostic text comparisons')
p.add_argument('--probe-dir', default='native/cyclone-tests')
p.add_argument('--build', default='native/build')
p.add_argument('--compiler', help='explicit translator; keep BUILD standard library/runtime objects')
p.add_argument('--output', help='separate probe log directory')
p.add_argument('--stage-dir', help='source-built stage directory with cyclone64 and standard library objects')
p.add_argument('--cc', default=os.environ.get('CC') or subprocess.check_output(['native/select-cc.sh'], text=True).strip())
p.add_argument('--compiler-flags', default='')
p.add_argument('--cflags', default='-O2 -g')
p.add_argument('--runtime-include', default='library/std/cyc-lib')
a = p.parse_args()
build = pathlib.Path(a.build).resolve()
stage = pathlib.Path(a.stage_dir).resolve() if a.stage_dir else None
compiler = pathlib.Path(a.compiler).resolve() if a.compiler else (stage or build) / 'cyclone64'
out = pathlib.Path(a.output) if a.output else (stage or build) / 'probes'; out.mkdir(parents=True,exist_ok=True)
include = pathlib.Path('native/include'); include.mkdir(exist_ok=True)
if not (include / 'cyclone').exists(): (include / 'cyclone').symlink_to('../..')
results = []
commands = []
def run(cmd, label):
    commands.append({'label':label,'argv':list(map(str,cmd))})
    (out/'commands.json').write_text(json.dumps(commands,indent=2)+'\n')
    try:
        proc = subprocess.run(list(map(str, cmd)), capture_output=True, text=True, timeout=60)
        (out / (label + '.log')).write_text(proc.stdout + proc.stderr)
        return proc
    except subprocess.TimeoutExpired:
        raise SystemExit('TIMEOUT: ' + label)
diagnostic_variant = ''
if not a.skip_diagnostics:
    version = run([a.cc, '--version'], 'diagnostic-preprocessor-version')
    if version.returncode: raise SystemExit('cannot identify diagnostic preprocessor')
    if 'clang' in version.stdout.lower(): diagnostic_variant = 'clang'
for ref in ([] if a.skip_diagnostics else sorted(pathlib.Path('testsuite/compiler').glob('*.ref'))):
    src = ref.with_suffix('.cyc'); pre = out / (src.stem + '.cyp')
    q = run([a.cc, '-nostdinc', '-x', 'c', '-E', '-isystem', 'testsuite/compiler/lib', src, '-o', pre], src.stem + '-cpp')
    if q.returncode: results.append((src.stem, 'PREPROCESS FAIL')); continue
    q = run([compiler, *shlex.split(a.compiler_flags), '--manyerrors', pre, '-o', '/dev/null'], src.stem)
    got = (q.stdout + q.stderr).replace('testsuite/compiler/', 'cyclone/testsuite/compiler/')
    variant = pathlib.Path('native/diagnostics') / (ref.stem+'.'+diagnostic_variant+'.ref')
    expected = (variant if diagnostic_variant and variant.is_file() else ref).read_text()
    (out / (src.stem + '.diff')).write_text(''.join(difflib.unified_diff(expected.splitlines(True), got.splitlines(True))))
    expected_exit = 1 if 'COMPILATION FAILED!' in expected else 0
    results.append((src.stem, 'PASS' if got == expected and q.returncode == expected_exit else f'DIFF (exit {q.returncode}, expected {expected_exit})'))
# Overflow must be diagnosed rather than silently wrapping in the lexer.
for src in ([] if a.skip_rejections else sorted(pathlib.Path('native/cyclone-reject').glob('*.cyc'))):
    (out/(src.stem+'.c')).unlink(missing_ok=True)
    q = run([compiler, *shlex.split(a.compiler_flags), '-o', out / (src.stem + '.c'), src], src.stem)
    ok = q.returncode == 1 and 'integer constant too large' in q.stdout + q.stderr
    results.append((src.stem, 'PASS' if ok else f'REJECTION FAIL ({q.returncode})'))
objects = sorted((build / 'cyclone/stage0/library/std').rglob('*.o')) + runtime_objects(build)
if stage:
    objects = sorted((stage/'library/std').glob('*.o')) + [stage/'native/generated/lineno.o',stage/'native/generated/libc/cycstubs.o',stage/'cstubs.o',stage/'boot_cstubs.o'] + runtime_objects(build)
for src in sorted(pathlib.Path(a.probe_dir).glob('*.cyc')):
    pre = out / (src.stem + '.cyp'); generated = out / (src.stem + '.c')
    for artifact in (pre, generated, out/src.stem): artifact.unlink(missing_ok=True)
    q = run([a.cc, '-E', '-x', 'c', '-Inative/include', src, '-o', pre], src.stem + '-cpp')
    if q.returncode: results.append((src.stem, 'PREPROCESS FAIL')); continue
    q = run([compiler, *shlex.split(a.compiler_flags), '-o', generated, pre], src.stem + '-translate')
    if q.returncode or not generated.is_file(): results.append((src.stem, f'TRANSLATE FAIL ({q.returncode})')); continue
    q = run([a.cc, '-std=gnu11', '-fno-strict-aliasing', '-Werror=pointer-to-int-cast', *shlex.split(a.cflags), '-I'+a.runtime_include, generated, 'native/cyclone-tests/abi-helper.c', *objects, *shlex.split(os.environ.get('LDFLAGS','')), *runtime_libs(build), '-o', out / src.stem], src.stem + '-link')
    if q.returncode: results.append((src.stem, 'LINK FAIL')); continue
    q = run([out / src.stem], src.stem + '-run')
    results.append((src.stem, 'PASS' if q.returncode == 0 else f'RUN FAIL ({q.returncode})'))
summary = ''.join(f'{name}: {status}\n' for name, status in results)
(out / 'results.txt').write_text(summary); print(summary, end='')
sys.exit(any(status != 'PASS' for _, status in results))
