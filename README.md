# Cyclone64

[![Native build and tests](https://github.com/octonion/cyclone64/actions/workflows/native.yml/badge.svg)](https://github.com/octonion/cyclone64/actions/workflows/native.yml)

Cyclone64 brings the [Cyclone programming language](https://cyclone.thelanguage.org/)
and Banshee constraint solver to native 64-bit Linux, with GNU Make builds and
tests. Cyclone is a safe dialect of C whose compiler translates Cyclone source
into C.

Cyclone began as a joint research project of AT&T Labs Research and Greg
Morrisett's group at Cornell University. Credit for the language and its original
implementation belongs to the [Cyclone contributors](https://cyclone.thelanguage.org/wiki/People/).
This fork focuses on LP64 support, modern toolchains and regression testing; see
[original project and references](#original-project-and-references).

The repository preserves the history of [TokTok/cyclone](https://github.com/TokTok/cyclone).
Its LP64 conversion starts from upstream commit
[`c19c449365671fa10acad6307a95ffffae86dc69`](https://github.com/TokTok/cyclone/commit/c19c449365671fa10acad6307a95ffffae86dc69).
It contains:

- **Cyclone64**: a source-bootstrapped compiler, runtime, standard library and
  installed `cyclone64` compile/link driver.
- **Banshee64**: an independently buildable `libbanshee64.a` constraint solver,
  including region allocation, unification, SetIF/SetST, rollback and checkpoints.

## Status and supported platform

The supported target is GNU/Linux x86-64 (LP64). This development candidate has
passed GCC 15 and Clang 21 builds on Ubuntu 26.04 and Debian 13, including:

- Source bootstrap convergence across all 81 generated C files.
- All 31 historical top-level applications, including `structarray` and its
  existential arrays; see [application coverage](native/APPLICATIONS.txt).
- Solver/runtime tests, compiler regressions, the upstream library suite and
  installed-client checks.

CI runs both compiler builds and installed-client tests, plus Banshee and
runtime ASan/UBSan tests. Additional instrumented compiler, library and
application testing is documented in the
[existential-array repair and validation record](native/EXISTENTIAL-ARRAYS.md),
including environment limits and pending workstation checks.

Native objects use the LP64 calling convention and must be rebuilt together.
Cyclone allocation sizes are native width with checked overhead; objects must
be smaller than `PTRDIFF_MAX`. The arithmetic prover still uses 32-bit terms,
so wide operations retain conservative runtime checks. Banshee checkpoints
require trusted input from the same build and ABI; custom expression types
need codecs. See the [native guide](native/README.md) and
[current limits](native/STATUS.txt) for details and unverified features.

## Dependencies

Use GNU/Linux on x86-64 with GNU Make, Python 3.9 or newer, GCC, binutils,
pkg-config, and development files for libbacktrace, libffi, OpenSSL/libcrypto and
Boehm-Demers-Weiser GC. The installed driver also uses Python.
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
package it separately. The build does not install dependencies automatically.

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
needed. `make check` runs the native regression suites, historical applications
and bootstrap convergence checks. `make install` also compiles and runs small
clients using the installed libraries and headers.

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

By default, the build chooses the newest installed stable GCC and uses
`-march=native`. To select a compiler and portable architecture explicitly:

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

## Development

- [Native build and ABI guide](native/README.md)
- [Native calling convention](native/NATIVE-LAYOUTS.txt)
- [Diagnostic reference rationale](native/DIAGNOSTICS.txt)
- [Current status and limits](native/STATUS.txt)
- [Historical workstation validation](native/HISTORICAL-VALIDATION.md) and
  [32-bit comparison notes](native/REFERENCE32.txt). Paths and logs in these
  records describe earlier validation workspaces, not prerequisites for a clone.
- [Compiler sources](library/compiler), [Banshee sources](library/banshee),
  [standard library](library/std), and [test suite](testsuite)

Please include a minimal reproducer, the commit, OS, compiler versions and build
overrides when reporting a problem. Changes should pass `make check`; installation
changes should also pass `native/check-install.py`. CI uses portable x86-64 flags
and preserves logs as workflow artifacts. GNU Make is the supported native build
entry point. The checked-in `stage0/` C sources are required for bootstrapping.
See [native/CLEANUP.txt](native/CLEANUP.txt) for retained sources and regression
coverage.

## Original project and references

The original Cyclone project provides the
[language manual](https://cyclone.thelanguage.org/wiki/User%20Manual/),
[contributor credits](https://cyclone.thelanguage.org/wiki/People/) and
[research bibliography](https://cyclone.thelanguage.org/wiki/Papers/).
Use that manual for the language; use this repository's native guide for current
build instructions and ABI differences. The principal early papers are:

- Trevor Jim, Greg Morrisett, Dan Grossman, Michael Hicks, James Cheney and
  Yanling Wang. **[Cyclone: A Safe Dialect of C](https://www.usenix.org/conference/2002-usenix-annual-technical-conference/cyclone-safe-dialect-c)**.
  USENIX Annual Technical Conference, 2002, pp. 275–288.
- Dan Grossman, Greg Morrisett, Trevor Jim, Michael Hicks, Yanling Wang and
  James Cheney. **[Region-Based Memory Management in Cyclone](https://www.cs.umd.edu/projects/cyclone/papers/cyclone-regions.pdf)**.
  ACM SIGPLAN Conference on Programming Language Design and Implementation
  (PLDI), 2002, pp. 282–293.

The bundled solver derives from the
[Banshee project](https://banshee.sourceforge.net/), developed by John Kodumal
and Alex Aiken. Its design is described in:

- John Kodumal and Alex Aiken.
  **[Banshee: A Scalable Constraint-Based Analysis Toolkit](https://theory.stanford.edu/~aiken/publications/papers/sas05a.pdf)**.
  International Static Analysis Symposium (SAS), 2005, pp. 218–234.

When discussing Cyclone's language design or Banshee's analysis framework,
please cite the original work above. To identify this port in reproducible
experiments, also record the Cyclone64 repository URL and exact commit.

## License and attribution

Original authorship and copyright notices are retained. See [LICENSE](LICENSE)
(GPLv3), [LICENSE.LIB](LICENSE.LIB) (LGPLv2.1),
[Banshee's copyright and license](library/banshee/COPYRIGHT), and the notices in
individual files and bundled tools. This fork does not relicense those components.
