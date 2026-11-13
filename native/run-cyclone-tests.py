#!/usr/bin/env python3
"""Run stage0 diagnostics and native ABI probes; retain all generated code/logs."""
import argparse, difflib, os, pathlib, subprocess, sys
p = argparse.ArgumentParser()
p.add_argument('--build', default='native/build')
p.add_argument('--cc', default=os.environ.get('CC') or subprocess.check_output(['native/select-cc.sh'], text=True).strip())
p.add_argument('--cflags', default='-O2 -g')
a = p.parse_args()
build = pathlib.Path(a.build).resolve()
out = build / 'probes'; out.mkdir(exist_ok=True)
include = pathlib.Path('native/include'); include.mkdir(exist_ok=True)
if not (include / 'cyclone').exists(): (include / 'cyclone').symlink_to('../..')
results = []
def run(cmd, label):
    try:
        proc = subprocess.run(list(map(str, cmd)), capture_output=True, text=True, timeout=60)
        (out / (label + '.log')).write_text(proc.stdout + proc.stderr)
        return proc
    except subprocess.TimeoutExpired:
        raise SystemExit('TIMEOUT: ' + label)
for ref in sorted(pathlib.Path('testsuite/compiler').glob('*.ref')):
    src = ref.with_suffix('.cyc'); pre = out / (src.stem + '.cyp')
    q = run([a.cc, '-nostdinc', '-x', 'c', '-E', '-isystem', 'testsuite/compiler/lib', src, '-o', pre], src.stem + '-cpp')
    if q.returncode: results.append((src.stem, 'PREPROCESS FAIL')); continue
    q = run([build / 'cyclone64', '--manyerrors', pre, '-o', '/dev/null'], src.stem)
    got = (q.stdout + q.stderr).replace('testsuite/compiler/', 'cyclone/testsuite/compiler/')
    expected = ref.read_text()
    (out / (src.stem + '.diff')).write_text(''.join(difflib.unified_diff(expected.splitlines(True), got.splitlines(True))))
    results.append((src.stem, 'PASS' if got == expected else f'DIFF (exit {q.returncode})'))
objects = sorted((build / 'cyclone/stage0/library/std').rglob('*.o')) + sorted((build / 'cyclone/library/runtime').glob('*.o'))
for src in sorted(pathlib.Path('native/cyclone-tests').glob('*.cyc')):
    pre = out / (src.stem + '.cyp'); generated = out / (src.stem + '.c')
    q = run([a.cc, '-E', '-x', 'c', '-Inative/include', src, '-o', pre], src.stem + '-cpp')
    if q.returncode: results.append((src.stem, 'PREPROCESS FAIL')); continue
    q = run([build / 'cyclone64', '-o', generated, pre], src.stem + '-translate')
    if q.returncode: results.append((src.stem, f'TRANSLATE FAIL ({q.returncode})')); continue
    q = run([a.cc, '-std=gnu11', *a.cflags.split(), '-Ilibrary/std/cyc-lib', generated, 'native/cyclone-tests/abi-helper.c', *objects, '-lbacktrace', '-ldl', '-lm', '-o', out / src.stem], src.stem + '-link')
    if q.returncode: results.append((src.stem, 'LINK FAIL')); continue
    q = run([out / src.stem], src.stem + '-run')
    results.append((src.stem, 'PASS' if q.returncode == 0 else f'RUN FAIL ({q.returncode})'))
summary = ''.join(f'{name}: {status}\n' for name, status in results)
(out / 'results.txt').write_text(summary); print(summary, end='')
sys.exit(any(status != 'PASS' for _, status in results))
