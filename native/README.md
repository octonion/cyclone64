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

## Cyclone reproduction

```sh
native/prepare-bootstrap.sh > native/logs/prepare-bootstrap.log 2>&1
native/bootstrap-stage1.py --out native/bootstrap/lp64-final-stage1
native/bootstrap-stage1.py \
  --compiler native/bootstrap/lp64-final-stage1/cyclone64 \
  --out native/bootstrap/lp64-final-stage2
native/bootstrap-stage1.py \
  --compiler native/bootstrap/lp64-final-stage2/cyclone64 \
  --out native/bootstrap/lp64-final-stage3
native/run-cyclone-tests.py --stage-dir native/bootstrap/lp64-final-stage2
native/run-cyctest.py --stage-dir native/bootstrap/lp64-final-stage1
```

Generation, translation, compilation, linking, commands, and diagnostics stay
under `native/`. The two test runners deliberately return failure for the
unresolved regressions; they never rewrite upstream reference outputs.
The upstream library suite runs under GDB so an unexpected fatal signal can be
examined without creating a desktop crash report. SIGUSR1/SIGUSR2 are passed
through for the suite's signal tests.

Native buildlib, cyclex, Bison, and errorgen produce libc wrappers, both lexers,
and parser/error headers. All 41 hand-written compiler modules, 35 standard
library modules, five generated/tool modules, and two C support files compile;
the native stage1, stage2, and stage3 compilers link. The binding generator still reports
unsupported `assert.h` and `stdarg.h`: assert is supplied by the existing Cyclone
implementation via `native/compat/assert.h`; unused C `va_list` inclusion was
removed from the aggregate library module. The sigaction wrapper now matches
the host declaration's leaf attribute.

The runtime calls `Cyc_main` with the actual generated `_fat_ptr` ABI. Compiler
fixes keep int/long type identities distinct, preserve wide casts during constant
evaluation, and prevent the proof-term folder from executing shifts >=32 or
INT_MIN/-1 division/remainder. Source and checked-in stage0 C are patched together.
The strengthened ABI test checks its 64-bit result against an independent C
constant, in addition to sizes, high addresses, argv, calls, and region arrays.
It passes both normally and with ASan/UBSan-instrumented generated C/runtime.
The direct constant-folder boundary test also passes under ASan/UBSan.

To reproduce runtime instrumentation independently of compiler leak failures:

```sh
make -f native/Makefile BUILD=native/asan \
  CFLAGS='-O1 -g -fsanitize=address,undefined -fno-omit-frame-pointer -fno-sanitize-recover=all' cyclone
native/run-cyclone-tests.py --build native/asan \
  --compiler native/bootstrap/lp64-final-stage1/cyclone64 \
  --output native/asan/runtime-final-probes \
  --cflags '-O1 -g -fsanitize=address,undefined -fno-omit-frame-pointer -fno-sanitize-recover=all'
```

## Remaining Cyclone blockers

1. **Large unsigned-long literals:** stage0 rejects `0x123456789abcdef0UL`;
   source-built stages accept it but emit only `2596069104U`, losing the upper
   bits. `native/cyclone-tests/long-literal.cyc` returns 3 with stage1/stage2.
   `lex.cyl:intconst` and the constant AST still lack a faithful LP64 long model.
2. **Bounds/index narrowing:** `bounds64.cyc` passes `0x100000001UL` as an index
   into three elements. Generated C converts it to int before
   `_check_fat_subscript`, accessing element 1 instead of throwing. It returns 1.
   The index conversion and runtime overflow/bounds arithmetic need a coordinated
   fix, not merely a wider literal parser.
3. **Arithmetic promotions:** `integer-promotion.cyc` demonstrates signed long -1
   compared with unsigned int 1 incorrectly. `tcutil.cyc:max_arithmetic_type`,
   precision checks, and the proof/range machinery retain ILP32 assumptions.
4. **Generic aggregate ABI:** the upstream suite's Fn currying assertion
   `apply(apply(addc,1),2) == 3` fails with both stage0 and rebuilt stage1 libraries.
   Generic tuples use pointer-sized fields while concrete `(int,int)` tuples use
   four-byte fields. Boxing/layout and erased callback ABI need further work.
   The suite reaches the remaining suites; one Fn assertion is reported failed.
5. **Compiler LeakSanitizer:** upstream `nogc.c` does not reclaim ordinary GC
   allocations. A sanitized compiler run of the ABI input reports leaks (for
   example 653282 bytes/17876 allocations before the final type changes).
   Leak detection has not been disabled to manufacture a passing result.
6. **Bootstrap convergence and remaining diagnostics:** stage1 versus stage2 has
   46/81 byte-identical generated C files; many differences replace `_throw` with
   `_rethrow`. Stage2 versus stage3 converges: **81/81 generated C files are
   byte-identical**. Six of eight upstream diagnostic references match; `fail3` differs by
   a column and `zeroptr` retains its reference's expected compiler exception with
   a changed source line. Pointer-width and callback warnings remain under audit.
7. **No working 32-bit reference:** missing Scrt1.o, crti.o, and 32-bit libgcc
   prevent `gcc-15 -m32` linking. No 32/64 behavioral equivalence is claimed.

Do not infer correctness from the bootstrap or the ABI smoke test alone. The
failing language cases above are retained as executable regressions for the next
iteration.
