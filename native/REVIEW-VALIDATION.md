# Native compatibility and allocation validation

Validated on Linux x86-64, Ryzen 9 5950X, with GCC 15 and Clang 21.
This records the compatibility/allocation review changes; it is not a promise
that every historical feature is supported.

- Fresh GCC and Clang source bootstraps: 81/81 generated C files converge.
- `make all`, `make check`, and staged `make install`: passed with both backends.
- Relocated installations in directories containing spaces: all 35 native/layout
  executables, driver modes, generated-C linkage, and Banshee client passed.
- Eight historical diagnostic cases and four literal rejection cases: passed.
- All 31 top-level historical applications are integrated: 30 pass, one exact
  expected translation failure (`structarray`; see APPLICATIONS.txt).
- Banshee solver/region tests and allocation/bounds/callback runtime units pass
  under Clang ASan + UBSan, with nonrecovering undefined-behavior checks.
- An instrumented current compiler, standard library and runtime pass all 35
  native/layout executables, diagnostic/rejection cases, the upstream library
  suite, and the same historical application manifest.
- Real large-object check: `CYC_ALLOCATION_COUNT=4294967313` and
  `CYC_INITIALIZED_COUNT=268435473`. Heap/new/region extents, final-byte accesses,
  numelts, strlen/strcmp and initialized comprehension behavior passed.
- Region profiling mode compiles with native-width counters and matching formats.

## Reproduction

The standard reproducible entry points are `make all check install` and
`python3 native/check-install.py --prefix PREFIX --out OUTPUT`. See README.md
for isolated BUILD directories, compiler overrides, relocation and dependencies.
CI runs both native backends and independent Banshee/runtime sanitizer checks.
Normal CI allocations stay small; the multi-GiB run is opt-in (ALLOCATIONS.txt).

For instrumented compiler checks, first finish an ordinary source bootstrap.
Within its generated source tree (`BUILD/src`), build a separate sanitizer
support tree without executing the historical stage0 tools, then translate the
current sources with the validated stage2 compiler:

```sh
SAN='-O1 -g -fsanitize=address,undefined -fno-omit-frame-pointer -fno-sanitize-recover=all'
make -f native/Makefile BUILD=native/asan CC=clang-21 CFLAGS="$SAN" -j4 cyclone
python3 native/bootstrap-stage1.py --compiler native/bootstrap/stage2/cyclone64 \
  --build native/asan --out native/bootstrap/instrumented --cc clang-21 \
  --arch=-march=native --cflags="$SAN" --jobs 4
python3 native/run-cyclone-tests.py --stage-dir native/bootstrap/instrumented \
  --build native/asan --cc clang-21 --cflags="$SAN" \
  --output native/compiler-tests/instrumented-all
python3 native/run-cyclone-tests.py --stage-dir native/bootstrap/instrumented \
  --build native/asan --cc clang-21 --cflags="$SAN" --skip-diagnostics \
  --skip-rejections --probe-dir native/layout-tests \
  --output native/compiler-tests/instrumented-layout
python3 native/run-cyctest.py --direct --stage-dir native/bootstrap/instrumented \
  --build native/asan --cc clang-21 --cflags="$SAN" \
  --out native/compiler-tests/instrumented-upstream
python3 native/run-applications.py --stage-dir native/bootstrap/instrumented \
  --build native/asan --cc clang-21 --cflags="$SAN" \
  --out native/compiler-tests/instrumented-applications
```

A fully instrumented bootstrap starting by *executing* the preserved stage0
buildlib tool is not supported: Clang's function sanitizer reports its old
`Cyc__IO_vfprintf` indirect-call signature mismatch. The instrumented-current-
compiler procedure above does not execute that historical binary and does not
disable function sanitization. BDW-GC disables ASan fake stacks for collector
compatibility; malloc-backed runtime units supplement collector coverage.

## Remaining limits and warnings

The subsequent `structarray` repair is recorded in EXISTENTIAL-ARRAYS.md; the
30-pass/one-expected-failure counts above describe the original PR #1 validation.
The proof engine
and some historical container/string APIs retain 32-bit assumptions. The
allocation audit covers compiler allocation lowering, runtime byte accounting,
headers/alignment and bounds, not every standard-library container operation.
Banshee's separate per-object allocation cap is unchanged. Rebuild all client
objects for the changed runtime ABI and unsigned-long numelts/region counters.

Pointer-to-int cast warnings from generic unboxing were corrected and are errors
in native/layout/application regression builds. Other historical warnings remain,
including some source narrowing diagnostics, pointer-sign conversions, missing
C return paths after exceptions, and the unused tmpnam implementation when whole
object lists are linked. They are not globally suppressed. See STATUS.txt for
other callback, checkpoint and ABI limits.
