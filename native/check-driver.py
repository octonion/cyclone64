#!/usr/bin/env python3
"""Check signal reporting and ordinary failure propagation without crash reports."""
import json, pathlib, subprocess, sys, tempfile
root=pathlib.Path(__file__).resolve().parent.parent
with tempfile.TemporaryDirectory(prefix='cyclone-driver-test-') as directory:
    d=pathlib.Path(directory)
    (d/'driver.py').write_text((root/'native/cyclone-driver.py').read_text().replace('@CONFIG_REL@','config.json'))
    (d/'input.cyp').write_text('int main() { return 0; }\n')
    (d/'config.json').write_text(json.dumps(dict(cc=sys.executable,arch='',cppflags='',cflags='',ldflags='',translator='translator')))
    fake=d/'translator'
    for body,expected,text in [('import ctypes,os,signal\nctypes.CDLL(None).prctl(4,0,0,0,0)\nos.kill(os.getpid(),signal.SIGSEGV)',139,'SIGSEGV (11)'),('import sys\nsys.exit(7)',7,'')]:
        fake.write_text('#!'+sys.executable+'\n'+body+'\n');fake.chmod(0o755)
        p=subprocess.run([sys.executable,d/'driver.py','--emit-c',d/'input.cyp'],capture_output=True,text=True)
        assert p.returncode==expected,(p.returncode,p.stderr)
        assert text in p.stderr,p.stderr
        if text: assert 'translator terminated by signal' in p.stderr,p.stderr
        else: assert not p.stderr,p.stderr
print('Driver signal and failure reporting: PASS')
