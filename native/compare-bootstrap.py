#!/usr/bin/env python3
"""Compare every generated C file in two completed native bootstrap stages."""
import argparse
import hashlib
import json
from pathlib import Path
import sys

p = argparse.ArgumentParser(description=__doc__)
p.add_argument('--left', type=Path, required=True)
p.add_argument('--right', type=Path, required=True)
p.add_argument('--out', type=Path, required=True)
a = p.parse_args()
for stage in (a.left, a.right):
    results = stage / 'results.txt'
    if not results.is_file() or not (stage / 'cyclone64').is_file():
        p.error(f'{stage}: incomplete stage')
    lines = results.read_text().splitlines()
    if not lines or lines[-1] != 'stage link: PASS' or any(not s.endswith(': PASS') for s in lines):
        p.error(f'{stage}: failed stage')

def hashes(stage):
    return {str(path.relative_to(stage)): hashlib.sha256(path.read_bytes()).hexdigest()
            for path in sorted(stage.rglob('*.c'))}

left, right = hashes(a.left), hashes(a.right)
paths = sorted(left.keys() | right.keys())
records = [{'path': path, 'left_sha256': left.get(path), 'right_sha256': right.get(path),
            'identical': path in left and path in right and left[path] == right[path]}
           for path in paths]
identical = sum(record['identical'] for record in records)
report = {'left': str(a.left.resolve()), 'right': str(a.right.resolve()),
          'file_count': len(paths), 'identical_count': identical, 'files': records}
a.out.parent.mkdir(parents=True, exist_ok=True)
a.out.write_text(json.dumps(report, indent=2) + '\n')
print(f'{identical}/{len(paths)} generated C files byte-identical')
for record in records:
    if not record['identical']:
        print('DIFF: ' + record['path'])
sys.exit(not paths or identical != len(paths))
