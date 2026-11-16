# Cyclone64

[![Native build and tests](https://github.com/octonion/cyclone64/actions/workflows/native.yml/badge.svg)](https://github.com/octonion/cyclone64/actions/workflows/native.yml)

Native 64-bit Cyclone compiler and Banshee constraint solver, with GNU Make
builds and tests. Cyclone is a safe dialect of C; its compiler translates
Cyclone source into C.

This is a development fork of [TokTok/cyclone](https://github.com/TokTok/cyclone),
preserving its history. The LP64 conversion starts from upstream commit
`c19c449365671fa10acad6307a95ffffae86dc69`. Both components live here:

- **Cyclone64**: a source-bootstrapped compiler, runtime, standard library and
  installed `cyclone64` compile/link driver.
- **Banshee64**: an independently buildable `libbanshee64.a` constraint solver,
  including region allocation, unification, SetIF/SetST, rollback and checkpoints.

## Status and supported platform

The current candidate has passed native Linux x86-64 builds with GCC 15 and
Clang 21 on Ubuntu 26.04, self-bootstrap convergence across 81 generated C files,
solver/runtime tests, compiler regressions and installed-client checks. Earlier
compiler/runtime validation also includes ASan/UBSan. CI runs GCC and Clang builds
and installed-client tests, plus Banshee and runtime sanitizer tests.

This is an actively developed candidate, not a claim that every historical
Cyclone feature or platform has been verified. Native objects use the new LP64
calling convention and must be rebuilt together. Banshee checkpoints are for
trusted input from the same build and ABI; custom expression types need codecs.
The arithmetic prover still uses 32-bit terms, so wide operations retain
conservative runtime checks. See [native/README.md](native/README.md) for
semantics, supported driver options, validation details and remaining limits.
Cyclone allocation sizes are native width with checked overhead; objects must
be smaller than `PTRDIFF_MAX`. All 31 historical top-level applications,
including existential arrays, are required to pass; see
[application coverage](native/APPLICATIONS.txt).

## Dependencies

Use GNU/Linux on x86-64 with GNU Make, Python 3.9 or newer, GCC, binutils,
pkg-config, and development files for libbacktrace, libffi, OpenSSL/libcrypto and
Boehm-Demers-Weiser GC. Python is also required by the installed driver.
A GCC installation is required even when Clang compiles the C objects: the
legacy libc binding generator uses GCC to preprocess system headers.

On Ubuntu 26.04:

```sh
sudo apt-get update
sudo apt-get install build-essential gcc-15 libgcc-15-dev python3 pkg-config \
  libffi-dev libssl-dev libgc-dev git
# Optional alternative C compiler:
sudo apt-get install clang-21
```

`libgcc-15-dev` supplies libbacktrace on this platform. Other distributions may
package it separately. The build does not install packages or require Bazel.

## Clone, build, test and install

```sh
git clone https://github.com/octonion/cyclone64.git
cd cyclone64
make all
make check
make install PREFIX="$HOME/.local"
export PATH="$HOME/.local/bin:$PATH"
export PKG_CONFIG_PATH="$HOME/.local/lib/pkgconfig${PKG_CONFIG_PATH:+:$PKG_CONFIG_PATH}"
```

Build products and logs stay under `build/native`. The bootstrap uses tracked
sources; no downloaded bootstrap binary or previous Cyclone installation is
needed. `make check` covers Banshee, region persistence, runtime bounds and GC,
Cyclone diagnostics and rejection tests, native layouts, the upstream library suite and historical application tests,
compiler units, and bootstrap convergence. `make install` also compiles and runs
small clients using the installed libraries and headers.

For example, save this as `hello.cyc`:

```c
#include <core.h>
#include <stdio.h>
using Core;
int main(int argc, string_t ?argv) {
  printf("Hello from Cyclone64!\n");
  return 0;
}
```

```sh
cyclone64 hello.cyc -o hello
./hello
```

The default chooses the newest installed stable GCC and native CPU architecture
on Ryzen. Compiler and architecture overrides are explicit:

```sh
make BUILD=build/portable CC=gcc-15 ARCH=-march=x86-64 JOBS=4 all check
make BUILD=build/clang CC=clang-21 LIBC_CC=gcc-15 ARCH=-march=x86-64 all check
```

Use `ARCH=-march=x86-64` for binaries intended for other x86-64 machines.
The selector reserves GCC 14 and `-march=knl` for detected Xeon Phi 7250 hardware
when available; that hardware path has not yet been tested.

`BUILD`, `CC`, `LIBC_CC`, `AR`, `ARCH`, `CFLAGS`, `CPPFLAGS`, `LDFLAGS`, and `JOBS`
are configurable. Repeat overrides on subsequent invocations, or export them.
Changing compiler or flags requires a fresh `BUILD` or `make clean`.
`make clean` removes only the selected managed build directory.

Staged installation and the full installed-client check:

```sh
make install PREFIX=/opt/cyclone64 DESTDIR="$PWD/build/staging"
python3 native/check-install.py --prefix build/staging/opt/cyclone64 \
  --out build/installed-checks
```

Installations can be relocated. They contain `bin/cyclone64`, both static
libraries, public headers, Cyclone standard-library headers, and pkg-config
metadata. A Banshee C client can use:

```sh
cc client.c $(pkg-config --cflags --libs banshee64) -o client
```

## Build Banshee independently

```sh
make -f native/Makefile BUILD=build/banshee all check rlib-check
```

This produces `build/banshee/libbanshee64.a` without bootstrapping Cyclone.
The independent build accepts the same compiler and architecture overrides.
See [native/tests](native/tests) for client and checkpoint examples.

## Development and provenance

- [Native build and ABI guide](native/README.md)
- [Native calling convention](native/NATIVE-LAYOUTS.txt)
- [Diagnostic reference rationale](native/DIAGNOSTICS.txt)
- [Historical workstation validation notes](native/STATUS.txt) and
  [32-bit comparison notes](native/REFERENCE32.txt). Paths and logs in these
  records describe the original validation workspace, not prerequisites for a clone.
- [Compiler sources](library/compiler), [Banshee sources](library/banshee),
  [standard library](library/std), and [test suite](testsuite)

Please include a minimal reproducer, the commit, OS, compiler versions and build
overrides when reporting a problem. Changes should pass `make check`; installation
changes should also pass `native/check-install.py`. CI uses portable x86-64 flags
and preserves logs as workflow artifacts. GNU Make is the supported native build
entry point. Obsolete Bazel definitions and experimental reference wrappers have
been removed; their history remains in Git. The checked-in `stage0/` C sources
are still required for bootstrapping. See [native/CLEANUP.txt](native/CLEANUP.txt)
for the retained source and regression coverage.

## License and attribution

Original authorship and copyright notices are retained. See [LICENSE](LICENSE)
(GPLv3), [LICENSE.LIB](LICENSE.LIB) (LGPLv2.1), and the notices in individual files,
including Banshee and bundled tools. This fork does not relicense those components.
