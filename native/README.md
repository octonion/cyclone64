# Native LP64 work on seymour

Experimental conversion of TokTok/cyclone, based on
`c19c449365671fa10acad6307a95ffffae86dc69`, isolated on `native-lp64-seymour`.
No prior LP64 kit was used: `/mnt/data` did not exist on this workstation.

Banshee's scoped native tests pass. Source-built native Cyclone stages now link,
but language/runtime regressions below prevent calling this a solid conversion.
In particular, successful self-compilation does not establish correct LP64 semantics.

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
changed, and unexpected failures remain visible. Desktop crash records remained
at 28 throughout the follow-up verification.

## Banshee changes and scope

* Sparse full-uintptr ownership/translation maps, pointer-width alignment masks,
  checked multiplication/narrowing, big-allocation one-past-end reservation,
  and ownership removal on free even with `NMEMDEBUG`.
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

## Cyclone reproduction (wide indices and literals)

Use the latest installed stable GCC selected by `native/select-cc.sh`, or set
`CC` / pass `--cc`. Ryzen builds use `-march=native`. The independent Banshee
build and the isolated branch remain unchanged by bootstrap outputs.

```sh
native/prepare-bootstrap.sh > native/logs/prepare-bootstrap.log 2>&1
native/bootstrap-stage1.py --out native/bootstrap/wide-clean-stage1
native/bootstrap-stage1.py \
  --compiler native/bootstrap/wide-clean-stage1/cyclone64 \
  --out native/bootstrap/wide-clean-stage2
native/bootstrap-stage1.py \
  --compiler native/bootstrap/wide-clean-stage2/cyclone64 \
  --out native/bootstrap/wide-clean-stage3
native/run-cyclone-tests.py --skip-diagnostics \
  --stage-dir native/bootstrap/wide-clean-stage3
native/run-cyctest.py --stage-dir native/bootstrap/wide-clean-stage3
```

The fresh stage2/stage3 generated C is **81/81 byte-identical**; stage1/stage2
is 42/81. Each stage compiles 81 Cyclone inputs and two C support files and
links a native compiler. The bootstrap script records all commands and phase
logs, accepts `--jobs` (default 4), and supports `--cflags` for instrumentation.

All six native language regressions and four overflow-rejection inputs pass:
ABI/full-width values, wide subscripts, signed/unsigned promotions, literal
limits/suffixes, the original large-literal example, and null pointer typedefs.
`--skip-diagnostics` skips only exact historical diagnostic-text comparisons;
it does not skip these language or rejection tests. Without this option the
runner still returns failure for two historical reference differences, described
below. The upstream library runner still fails for the generic tuple bug.

The changes preserve integral subscript widths in type checking and keep runtime
checks for wide indices that the 32-bit proof model cannot justify. Runtime
checks reject invalid indices before forming pointers or multiplying offsets.
Fat pointers, bounded thin arrays, and zero-terminated scans are covered,
including negative offsets, LONG_MIN, ULONG_MAX, 2^32+1, one-past pointers,
128-bit indices at the runtime macro boundary, and single argument evaluation.
Zero-termination is inspected before translation erases its annotation.

The source compiler has a distinct Long_c constant variant. Lexing checks
multiply/add overflow and selects integer rank and signedness without dropping
upper bits; output preserves L/UL/LL/ULL and prints zero and signed minima
correctly. The usual arithmetic conversions distinguish LP64 signed long from
unsigned int, and unsigned long from same-width signed long long. Null pointer
translation now expands typedefs before looking up pointer bounds.

**Stage0 is a bootstrap compiler**, with minimal compatible fixes (including
zero-ULL printing). It still has the old constant AST and lexer. Build the
source-derived compiler for the complete literal fixes. Do not mix individual
new Long_c-based generated compiler modules into the old stage0 ABI.

Native buildlib, cyclex, Bison, and errorgen generate libc bindings, both lexers,
and parser/error headers. The existing compatibility assert header supplies the
unsupported binding; unused stdarg inclusion is excluded from the aggregate
module. No system compiler, library, or crash-report configuration is changed.

## Sanitizer checks

```sh
make -f native/Makefile native/build/check-runtime-bounds
native/build/check-runtime-bounds
make -f native/Makefile -j12 BUILD=native/asan \
  CFLAGS='-O1 -g -fsanitize=address,undefined -fno-omit-frame-pointer -fno-sanitize-recover=all' \
  cyclone check rlib-check native/asan/check-runtime-bounds
native/asan/check-runtime-bounds
native/run-cyclone-tests.py --skip-diagnostics --build native/asan \
  --compiler native/bootstrap/wide-clean-stage3/cyclone64 \
  --output native/asan/wide-clean-probes \
  --cflags '-O1 -g -fsanitize=address,undefined -fno-omit-frame-pointer -fno-sanitize-recover=all'
```

All ten language/rejection checks pass with ASan/UBSan instrumented emitted C,
standard-library support, and runtime. The direct bounds checks also pass with
GCC 15 and Clang 21. Seven Banshee regressions and eight upstream region tests
pass again in normal and ASan/UBSan builds, with leak detection enabled.
Intentional allocator-abort tests retain their child-only exit handler.

To instrument the source compiler itself, use `--build native/asan`, an explicit
non-sanitized `--compiler`, and the sanitizer `--cflags` above with
`native/bootstrap-stage1.py`. This build succeeds, but translating
`literal-limits.cyc` still exits 1 from LeakSanitizer: **1,185,131 bytes in
18,574 allocations**, originating in the upstream `nogc.c` allocation path.
That run reports no other ASan/UBSan error. Leak detection remains enabled;
passing emitted-program checks do not imply that the compiler is leak-free.

## Remaining verified blockers and limits

1. **Generic tuple/erased callback ABI:** the upstream Fn currying assertion at
   `testsuite/cyctest/suites/fn_suite.h:31` still fails. The minimized
   `native/cyclone-blockers/generic-tuple.cyc` reads an eight-byte concrete
   `(int,int)` pair through a sixteen-byte generic `(void*,void*)` layout.
   UBSan reports insufficient object space. This requires a consistent
   representation and access/call ABI, including writes and address-taking;
   padding one test's tuple is not a general repair.
2. **Pointer arithmetic remains separate from subscripting:**
   `native/cyclone-blockers/pointer-add.cyc` demonstrates `a + 0x100000001UL`
   still narrowing to int in `tcPlus`, then reading element 1. The corresponding
   `a[index]` regression now throws correctly. Pointer +/-/compound assignment,
   offset helpers, and allocation-size/proof arithmetic require further audit.
3. **Compiler allocation ownership/GC:** the source compiler still leaks as
   measured above. It uses the historical no-GC runtime. No lifetime workaround
   or disabled sanitizer is presented as a repair.
4. **Literal and proof scope:** integer literals are supported through 64 bits;
   overflow is diagnosed. Long/long-long constant patterns remain explicitly
   unsupported in the parser. The general proof engine still uses 32-bit terms;
   forcing wide subscript checks is conservative, not a full proof-engine port.
5. **Historical diagnostics:** six of eight reference files match exactly.
   `fail3` differs by one source column. `zeroptr` now compiles successfully;
   its old reference expected an internal Null_Exception. Upstream references
   have deliberately been retained, and `null-alias.cyc` checks the repaired
   behavior at runtime. The full reference runner therefore still exits 1.
6. **32-bit comparison:** multilib startup objects and libgcc remain unavailable;
   no working 32-bit reference or 32/64 equivalence is claimed. Native GCC C
   helpers independently check the literal values and ABI expectations.

To reproduce the remaining minimized failures with sanitizers, pass
`--probe-dir native/cyclone-blockers` to the native test runner, with the
instrumentation options above. Both inputs intentionally return a failing
status; UBSan stops the tuple case without an abort/core dump. The upstream
library runner uses GDB to contain unexpected fatal signals. The crash audit
found no new entries during this iteration (after 20:00 EST on November 13);
30 older records exist, most recently at 17:31:39, all preceding this work.

Bootstrap convergence is useful evidence, not a claim of complete LP64 safety.
