"""Keep runtime backend selection consistent across native build drivers."""
import json
import pathlib
import subprocess
import sys
import shlex

def configuration(build):
    path = pathlib.Path(build) / 'runtime-config.json'
    return json.loads(path.read_text()) if path.exists() else {'backend': 'nogc', 'ldflags': []}

def runtime_objects(build):
    build = pathlib.Path(build)
    config = configuration(build)
    inactive = 'nogc.o' if config['backend'] == 'system' else 'system_gc.o'
    objects = sorted((build / 'cyclone/library/runtime').glob('*.o'))
    if any(p.name == inactive for p in objects):
        raise SystemExit(f'{build}: mixed allocator objects; use a fresh BUILD directory')
    return objects

def runtime_libs(build):
    return ['-lbacktrace', '-ldl', '-lm', '-lpthread', *configuration(build)['ldflags']]

def configure(build, backend):
    if backend not in ('nogc', 'system'):
        raise SystemExit('NATIVE_GC must be nogc or system')
    build = pathlib.Path(build)
    config = {'backend': backend, 'ldflags': [], 'cflags': []}
    if backend == 'system':
        for key, flag in [('ldflags', '--libs'), ('cflags', '--cflags')]:
            config[key] = shlex.split(subprocess.check_output(['pkg-config', flag, 'bdw-gc'], text=True))
        config['version'] = subprocess.check_output(['pkg-config', '--modversion', 'bdw-gc'], text=True).strip()
    path = build / 'runtime-config.json'
    if path.exists() and json.loads(path.read_text()) != config:
        raise SystemExit(f'{build}: runtime configuration changed; use a fresh BUILD directory')
    if not path.exists():
        inactive = 'nogc.o' if backend == 'system' else 'system_gc.o'
        if (build / 'cyclone/library/runtime' / inactive).exists():
            raise SystemExit(f'{build}: incompatible allocator objects; use a fresh BUILD directory')
        build.mkdir(parents=True, exist_ok=True)
        path.write_text(json.dumps(config, indent=2) + '\n')

if __name__ == '__main__':
    configure(*sys.argv[1:])
