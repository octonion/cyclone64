# Native LP64 build and validation guide

Cyclone64 preserves TokTok/cyclone history, with the LP64 work based on
`c19c449365671fa10acad6307a95ffffae86dc69`. See the root [README](../README.md)
for clone instructions and distribution dependencies.

Historical validation ran on the `seymour` workstation, on branch
`native-lp64-seymour`. Referenced local builds and logs are historical evidence,
not files needed to build a fresh clone. The earlier experimental build kit was
not available there and was not used as evidence.

See [STATUS.txt](STATUS.txt) for the validated candidate paths and scope, and
[NATIVE-LAYOUTS.txt](NATIVE-LAYOUTS.txt) for the corrected generic calling
convention. Historical failed baselines are retained in logs and Git history.
A successful bootstrap alone does not establish correct LP64 semantics.

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

# Verify all 30 executable native/layout probes using the installed driver.
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
remain separate. The low-level workflows below remain available for diagnosis.

## Independent Banshee build

Run from the repository root:

```sh
make -f native/Makefile -j12 check rlib-check
make -f native/Makefile BUILD=native/gcc14 CC=gcc-14 check
make -f native/Makefile BUILD=native/clang CC=clang check
make -f native/Makefile BUILD=native/asan \
  CFLAGS='-O1 -g -fsanitize=address,undefined -fno-omit-frame-pointer -fno-sanitize-recover=all' \
  check rlib-check
```

The result is `native/build/libbanshee64.a`, independent of Cyclone. Clients use
`library/banshee` and `library/banshee/rlib` headers with the annotation definitions
in the Makefile. `native/tests` contains seven standalone client programs.

The Makefile chooses the highest installed non-prerelease versioned GCC and
`-march=native` on Ryzen. `CC`, `AR`, `ARCH`, `CPPFLAGS`, `CFLAGS`, `LDFLAGS`, and
`BUILD` accept explicit overrides. **Use a fresh BUILD directory when changing
flags or compilers.** Intel family 6/model 87 chooses GCC 14/`-march=knl` if
available; newer GCC on that CPU defaults to portable x86-64. The KNL path has
not been tested on hardware.

Verified here: Ryzen 9 5950X, Ubuntu 26.04.1, GCC 15.2, GCC 14.3, Clang 21.1.8.
All seven tests pass with those compilers, GCC ASan/UBSan with leak detection,
and `NMEMDEBUG`. All eight upstream region persistence drivers pass under
ASan/UBSan, including six million linked nodes. The archive is deterministic
under a forced same-directory rebuild; no cross-toolchain claim is made.

The allocator's two expected-abort checks install a **child-only SIGABRT handler**
that exits with a distinguished status. The parent still verifies rejection;
missing aborts and other signals fail. No system crash-report settings are
changed, and unexpected failures remain visible. Desktop crash records remain at 30; no new reports are produced by these
expected-abort checks.

## Banshee changes and scope

* Sparse full-uintptr ownership/translation maps, pointer-width alignment masks,
  checked multiplication/narrowing, big-allocation one-past-end reservation,
  and ownership removal on free even with `NMEMDEBUG`.
* Typed adapters preserve callback signatures in list and union-find operations;
  string hash callbacks use the hash-table key signature. Clang standalone
  ASan/UBSan checks include function-type checking and regressions for integer
  callbacks, nested sorting, empty-list filtering and union-find rollback.
* Correct pointer-sized container payload handling; no writes through int-sized
  output buffers. Hash copies allocate buckets in their destination region.
* SetST bounds use snapshots through union-find history. Tests cover nested
  rollback, cyclic variable merging/splitting, branch reuse, sinks, intersections,
  and projection caches. The old blocker program now passes.
* Built-in term, SetIF, and SetST object codecs replace the specialized build's
  empty generic-expression codec. SetST state/history is persisted. LP64 record
  fields are written individually where packed-size assumptions dropped padding
  and pointer bytes. Restored strings have region ownership; pointer-valued hash
  keys are relocated and rehashed. Term hash-consing state is also restored.
* The checkpoint test relocates all three sorts twice, discards original solver
  regions before the second restore, verifies shared identities and pointer-key
  lookup, then checks propagation and rollback after restoration.
* `banshee_serialize_finish()` reports codec failure so unsupported client
  expressions cannot silently produce a successful checkpoint. Discard failed
  output. The legacy void finish exits unsuccessfully on codec failure; legacy
  `fwrite_s` hooks can still terminate on I/O errors.

Persistence is **same-build, same-ABI, trusted-input** checkpointing. Built-in
expression sort detection uses owning regions; arbitrary client constructors and
flowrow expressions need their own codec. The old `serialize_cs` convenience
function remains a stub: use the module/object API illustrated by
`native/tests/solver-persistence.c`. Cross-ABI snapshots, malformed-input handling,
and cross-build function-ID compatibility are not established. Region allocation
is pointer-word aligned, not over-aligned; individual allocations remain capped
near 2 GiB. Passing these tests is not exhaustive verification of the solver.

## Cyclone build from tracked sources

Requirements are the installed GCC, Make, Python 3, pkg-config, libbacktrace,
libffi, OpenSSL/libcrypto and BDW-GC development files. No host packages are
installed by these scripts. Use fresh build directories for a different compiler,
allocator or flags. The compiler selector chooses the newest installed stable
GCC; explicit `CC` or `--cc` overrides it. Ryzen builds use `-march=native`.

```sh
mkdir -p native/logs
NATIVE_GC=system BUILD=native/build-system native/prepare-bootstrap.sh \
  > native/logs/prepare-system.log 2>&1
python3 native/bootstrap-stage1.py --build native/build-system \
  --out native/bootstrap/seed
python3 native/bootstrap-stage1.py --build native/build-system \
  --compiler native/bootstrap/seed/cyclone64 \
  --out native/bootstrap/native1
python3 native/bootstrap-stage1.py --build native/build-system \
  --compiler native/bootstrap/native1/cyclone64 \
  --out native/bootstrap/native2
python3 native/compare-bootstrap.py --left native/bootstrap/native1 \
  --right native/bootstrap/native2 --out native/logs/native-convergence.json
python3 native/run-cyclone-tests.py --build native/build-system \
  --stage-dir native/bootstrap/native2
python3 native/run-cyclone-tests.py --skip-diagnostics --skip-rejections \
  --build native/build-system --stage-dir native/bootstrap/native2 \
  --probe-dir native/layout-tests \
  --output native/compiler-tests/native2-layout
python3 native/run-cyctest.py --direct --build native/build-system \
  --stage-dir native/bootstrap/native2
```

Stage0 is only a bootstrap compiler. It retains its old constant AST and ABI.
Source-built stages default to the native convention. `--native-layouts` remains
an explicit spelling; `--legacy-layouts` is for bootstrap compatibility only.
Never mix standard-library or compiler objects emitted in the two conventions.
Native buildlib, cyclex, Bison and errorgen regenerate
libc bindings, lexers, parser and error tables under `native/generated/`.
The build driver records every preprocessing, translation, C compilation and
link command, with separate logs. Compare all 81 generated C files between
successive complete stages before claiming convergence.

For a new compiler ABI or seed, set `CYCLONE_COMPILER` to its absolute path and
pass `--compiler native/run-compiler-under-gdb.sh`. GDB contains unexpected fatal
signals. Instrumented compilers should run directly for effective leak checking.

## Sanitizer validation

Create a separate runtime build with `NATIVE_GC=system`, `CC=clang-21` and
`CFLAGS='-O1 -g -fsanitize=address,undefined,function -fno-sanitize-recover=all'`.
Build the source compiler/stdlib with `bootstrap-stage1.py`, passing that build,
a verified source compiler, `--cc clang-21`
and the same `--cflags`. Run both regression drivers with the matching stage,
runtime, compiler flags and C flags. The upstream driver accepts `--direct`.

Also run the emitted native and layout probes against a separate
`NATIVE_GC=nogc` sanitizer runtime. Its payloads have exact malloc-ASan redzones;
these probes use stack/static/region lifetimes and must not silently depend on
GC collection. The production collector supplies collection semantics but its
heap does not provide those redzones. Its bridge disables ASan fake stacks,
which BDW-GC does not scan; malloc-backed probes retain fake-stack checking.
Leak detection stays enabled in both cases. Libffi closure records are registered
as live roots while cached, and explicit shutdown frees them. The callback unit
includes live-cache and deliberate-leak controls.

Focused units include `check-runtime-bounds`, `check-gc-allocation`,
`check-gc-collection`, `check-callback-ffi` and `check-type-name` Makefile targets.
`run-pratt-constants.py` and `run-pointer-compare.py` link against a source stage
to verify proof arithmetic and full-width identity against the actual compiler.

## Semantics and limits

Native integral ranks, literal suffixes and range checks distinguish LP64 long
from int. Pointer arithmetic, subscripts, allocation counts and fat-pointer
extents preserve their widths until validation. Fat-pointer indexing checks the
final selected element, including temporary before-base and past-end cursors.
The new calling convention carries concrete boxed-type widths through generic
functions, allocation, references, arrays, callbacks and existential packages.

The arithmetic proof engine remains based on 32-bit terms. Wide arithmetic and
complete fat-pointer bounds must not be justified by that model: conservative
runtime checks are retained, including the cases in `wide-proof64.cyc`. Consult
STATUS.txt for the validated stages and any remaining candidate work.

Long/long-long constant patterns and untagged-union patterns remain existing
language restrictions. Adapted variadic, union, bitfield and int128 callback
signatures are rejected when the runtime cannot represent the conversion.
Concrete C structure arguments/returns and supported bound callbacks have
separate interoperability tests.

`--skip-diagnostics` only skips exact diagnostic text. All eight current
references pass after three audited updates: integral-bound wording, documented GCC/Clang
macro-expansion columns, and successful null-pointer typedef conversion replacing an
internal compiler exception. The runner also checks exit status. See
[DIAGNOSTICS.txt](DIAGNOSTICS.txt) for the complete rationale and baseline logs.
Historical expectations remain in Git and the detached ILP32 checkout.

A working historical ILP32 reference is available in an isolated checkout and
private extracted sysroot; see [REFERENCE32.txt](REFERENCE32.txt). Common probes
compare behavior without installing host multilib packages. Known inherited
failures are distinguished from native conversion regressions in the logs.
