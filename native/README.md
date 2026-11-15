# Native LP64 build and validation

Cyclone64 is a Linux x86-64 development fork based on TokTok/cyclone commit
`c19c449365671fa10acad6307a95ffffae86dc69`. See the root [README](../README.md)
for dependencies and cloning. The original TokTok import does not establish an
exact original Cyclone release or SVN revision.

## Unified Make interface

From the repository root:

```sh
make all
make check
make install PREFIX="$HOME/.local"
```

`make all` builds both archives and the native compiler in `build/native`,
regenerates bootstrap sources, and verifies 81-file self-bootstrap convergence.
`make check` runs the Banshee regressions/region drivers, Cyclone diagnostics,
literal rejections, native/layout probes, upstream suite, compiler units and
runtime/collector checks. `make install` also compiles and runs small clients
against the installed Cyclone and Banshee interfaces.

Requirements: GNU Make, Python 3.9+ (also used by the installed driver), a native
GCC installation, pkg-config, and development files for libbacktrace, libffi,
OpenSSL/libcrypto and BDW-GC. Clang can compile all C objects through `CC`.
The legacy glibc declaration parser uses GCC preprocessing for libc bindings;
`LIBC_CC` selects that separate tool and defaults to the detected GCC. No package
installation is performed by the build.

```sh
# Explicit compiler and portable x86-64 architecture; separate output tree.
make BUILD=build/clang CC=clang-21 ARCH=-march=x86-64 JOBS=6 all check

# Stage a conventional /opt prefix without writing to /opt.
make install PREFIX=/opt/cyclone64 DESTDIR="$PWD/build/staging"

# Verify all executable native/layout probes using the installed driver.
python3 native/check-install.py --prefix build/staging/opt/cyclone64 \
  --out build/installed-checks
```

`PREFIX` (or lowercase `prefix`) defaults to `/usr/local`. `DESTDIR` is only a
staging root; installed paths are relative to the installation, so the prefix
can be relocated. Prefixes containing spaces are supported. Install creates:

- `bin/cyclone64`: preprocessing, translation, C compilation and linking.
- `libexec/cyclone64/cyclone64-compiler`: the native translator.
- `lib/libbanshee64.a`, `lib/libcyclone64.a` and `lib/pkgconfig/*.pc`.
- `include/banshee64`, `include/cyclone64`, and Cyclone-language headers and
  configuration under `share/cyclone64`.

Basic installed use:

```sh
cyclone64 hello.cyc -o hello
cyclone64 module.cyc -c -o module.o
cyclone64 module.cyc --emit-c -o module.c
cyclone64 hello.cyc -E -o hello.cyp
cc client.c $(pkg-config --cflags --libs banshee64) -o client
```

The driver accepts `.cyc`, preprocessed `.cyp`, `.c`, `.o` and `.a` inputs,
`-I/-D/-U/-L/-l`, C compiler flags, and `--cyclone-flag=--option` for translator
options. `--cc`, `--arch`, and the `CC/ARCH/CFLAGS/CPPFLAGS/LDFLAGS` environment
variables override the installed compiler defaults. It is a native driver with
this documented option set, not a replacement for every historical driver flag.
Both public pkg-config interfaces select GNU C11; generated C also needs
`-fno-strict-aliasing`. Add the installed `lib/pkgconfig` directory to
`PKG_CONFIG_PATH` for nonstandard prefixes. Static Cyclone C linkage uses
`pkg-config --cflags --libs --static cyclone64`.

Build overrides: `CC`, `LIBC_CC`, `AR`, `ARCH`, `CFLAGS`, `CPPFLAGS`, `LDFLAGS`,
`BUILD`, `NATIVE_GC` and `JOBS`. `JOBS` controls inner builds; otherwise the
requested `make -jN` count is used, with a bounded CPU-based default.
`NATIVE_GC=system` is the normal default. `nogc` is for focused allocation tests,
not general programs that require collection.

Repeat compiler/flag overrides on later invocations, or export them. A changed
configuration requires a fresh `BUILD` or `make clean`; the build refuses to mix
objects. Generated sources and logs are isolated under `BUILD/src` and
`BUILD/logs`. Repeating an unchanged build is a no-op. Compiler/runtime source
changes rebuild the bootstrap pipeline; test/documentation-only changes do not.
This stage-level rebuilding is conservative because there is no complete Cyclone
include-dependency scanner. `make clean` removes only a directory marked as a
managed build belonging to this checkout. Existing manual builds under `native/`
remain separate. The low-level scripts remain available for diagnosis.

## Scope and limits

Generic values carry memory-width arguments; references preserve native C
addresses. First-class generic functions use libffi adapters. Rebuild compiler,
runtime, libraries and client objects together after ABI changes. See
[NATIVE-LAYOUTS.txt](NATIVE-LAYOUTS.txt) for the calling convention.

Cyclone allocation counts and byte sizes use native widths. Heap, region and
reap operations validate products, headers and alignment before allocation;
object sizes must be below `PTRDIFF_MAX` and available memory still applies.
The old 256 MiB and 2 GiB Cyclone allocation caps are removed. Region byte-count
APIs now return `unsigned long`. Banshee's separate region allocator retains its
near-2-GiB per-allocation limit. See [ALLOCATIONS.txt](ALLOCATIONS.txt).

The arithmetic prover uses 32-bit terms. Wide expressions retain conservative
runtime checks. Adapted variadic, union, bitfield and int128 callback signatures
remain unsupported; concrete tuple/structure callbacks are covered. Long and
long-long constant patterns and untagged existential-union pattern unpacking
remain historical language restrictions.

Banshee checkpoints are trusted-input, same-build, same-ABI snapshots. Custom
and flowrow expression codecs are caller responsibilities; the old serialize_cs
helper is a stub. Region alignment is pointer-word alignment, not arbitrary
user-requested over-alignment.

Production uses BDW-GC. Its objects do not have malloc-ASan payload redzones;
separate malloc-backed runtime tests cover those checks. The sanitizer bridge
disables ASan fake stacks for the collector, not bounds checks or leak detection.
Callback caches live until explicit shutdown, which requires quiescent callers.
They use a linear, mutex-protected lookup; this is a known scaling limitation.

## Tests

[REVIEW-VALIDATION.md](REVIEW-VALIDATION.md) records the current compatibility
review results and the instrumented-compiler reproduction procedure.

`make check` runs Banshee/region tests, runtime units, the eight historical
compiler diagnostic cases, native rejection and executable probes, generic
layout regressions, the upstream `cyctest` library suite, compiler units, all
31 top-level programs in `testsuite/tests`, and bootstrap convergence.
[APPLICATIONS.txt](APPLICATIONS.txt) records the one expected application failure.
Expected failures require the exact phase, status and diagnostic; an unexpected
success also fails the runner so the exception cannot silently become stale.
The signal application is expected to terminate with SIGTERM after its checks.
Test-local crash reporting controls do not change desktop/system settings.

Normal allocation checks use small real allocations plus full-width overflow
units. For real large-object validation, run the built `large-allocation64`
probe with `CYC_ALLOCATION_COUNT=4294967313` and
`CYC_INITIALIZED_COUNT=268435473`. This exercises allocations beyond 4 GiB and an
initialized comprehension beyond 256 MiB; it requires several GiB of memory.

Historical workstation paths, crash-report counts and earlier candidate logs
are retained in [HISTORICAL-VALIDATION.md](HISTORICAL-VALIDATION.md) and
[HISTORY.txt](HISTORY.txt). They are audit records, not prerequisites.
