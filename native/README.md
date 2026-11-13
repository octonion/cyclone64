# Native LP64 work on seymour

Experimental port based on `c19c449365671fa10acad6307a95ffffae86dc69`.
The independent Banshee archive passes the scoped tests below. Cyclone stage0
runs, but the complete native conversion and self-hosting bootstrap are not
finished. No prior LP64 kit was used: `/mnt/data` did not exist on this machine.

Run commands from the repository root. No Bazel workspace, system installation,
or modification of another checkout is required.

```sh
make -f native/Makefile -j16 check rlib-check
make -f native/Makefile -j16 cyclone
native/run-cyclone-tests.py
```

The last command currently returns failure for documented compiler regressions.
It retains preprocessed inputs, generated C, diagnostics, diffs, and runtime
results under `native/build/probes/`. It does not rewrite upstream references.

## Toolchain and reproduction

The Makefile chooses the highest installed non-prerelease versioned GCC, with
`-march=native` on Ryzen. `CC`, `AR`, `ARCH`, `CPPFLAGS`, `CFLAGS`, `LDFLAGS`, and
`BUILD` can be overridden. Use a **new BUILD directory when changing flags or
compilers**, since Make does not track command-line changes in existing objects.

```sh
make -f native/Makefile BUILD=native/gcc14 CC=gcc-14 check
make -f native/Makefile BUILD=native/clang CC=clang check
make -f native/Makefile BUILD=native/asan \
  CFLAGS='-O1 -g -fsanitize=address,undefined -fno-omit-frame-pointer -fno-sanitize-recover=all' \
  check rlib-check
```

LeakSanitizer needs process inspection permission outside the desktop sandbox.
Do not disable leak checks to turn a failing run into a pass. The explicit
compiler override takes precedence. Intel family 6/model 87 selects GCC 14 and
`-march=knl` when available; a newer compiler on KNL defaults to portable
`-march=x86-64`. The KNL path is untested on hardware.

`native/build/libbanshee64.a` is independent of Cyclone. Clients need the headers
in `library/banshee` and `library/banshee/rlib`, and the annotation definitions
used by the Makefile. The five C tests are standalone client examples. Objects
are archived deterministically (`ar rcsD`). A forced rebuild in the same directory
produced the same SHA-256; this is not a claim of cross-toolchain reproducibility.

## Changes and verification

* Sparse, full-uintptr page maps replace fixed 32-bit ownership/translation arrays.
  Freed pages lose ownership even with `NMEMDEBUG` enabled.
* Alignment masks retain upper address bits. Size multiplication and narrowing
  are checked; big allocations reserve their one-past-end address.
* Integer hash payloads and non-pointer list scanners/array conversions no longer
  write pointer-sized values into four-byte integers. One-bucket hashing avoids
  a shift by the word width. String serialization writes a correctly sized length.
* Rollback kind checks compare rather than assign. Region restore updates the
  active allocation base and closes descriptors on open failures.
* The runtime declares/calls `Cyc_main` using the generated `_fat_ptr` argument ABI.

Verified on Ryzen 9 5950X, Ubuntu 26.04.1: GCC 15.2, GCC 14.3, Clang 21.1.8.
All five new Banshee tests pass with each compiler and with GCC ASan/UBSan.
All eight upstream region persistence drivers pass with GCC 15 and ASan/UBSan,
including six million linked nodes and large arrays. The `NMEMDEBUG` build passes
all five new tests. Tests exercise high address bits, allocation/reuse/overflow,
integer payloads, union-find, term unification, setif rollback, setst propagation,
region graph relocation, and object graph persistence of union-find classes.
These tests do not constitute coverage of every solver operation.

## Remaining blockers and limits

1. **SetST rollback is absent upstream.** The public backtrack operation leaves
   SetST constraints in place; `setst_rollback` and its persistence hooks are stubs.
   Reproduce the behavioral failure (exit 1):
   `make -f native/Makefile native/build/blocker-setst-rollback && native/build/blocker-setst-rollback`.
2. **Full solver object persistence is incomplete.** In the default specialized
   build, `gen_e_serialize` returns false, `gen_e_deserialize` returns NULL, and
   SetST serialization does nothing. The passing tests cover region snapshots and
   supported union-find object graphs, not a full solver checkpoint.
3. **Cyclone integer semantics still assume ILP32.** `long-literal.cyc` rejects
   `0x123456789abcdef0UL`. In `lex.cyl:intconst`, the constant representation and
   suffix handling collapse long into 32-bit Int_c. ASan/UBSan additionally reports
   a shift by 32 on `unsigned int` in generated `Cyc_AssnDef_binop`; the source
   constant folder in `assndef.cyc` uses 32-bit Uint terms. Simply widening one
   cast does not repair the type/kind, constant-folding, and proof semantics.
4. **Stage1 is partial.** 37 of 41 hand-written compiler modules preprocess,
   translate, and compile to native C objects. `absynpp`, `bansheeif`, and `currgn`
   stop at the generated assert.h error; `parse_errors` needs generated parser and
   error-table headers. Generated lexer compilation, parser/error-table generation, rebuilt standard library, final
   stage1 linking, stage2 comparison, and the full regression suite remain undone.
5. **Cyclone sanitizers are not clean.** Besides the shift error, the selected
   upstream `nogc.c` runtime produces LeakSanitizer failures. The native ABI probe
   passes without sanitizers; it does not waive these failures. Generated C still
   has pointer-width and callback-type diagnostics. Only its callback incompatibility
   diagnostic is downgraded from error to warning for bootstrap compilation.
6. **No 32-bit reference:** `gcc-15 -m32` cannot link because Scrt1.o, crti.o, and
   32-bit libgcc are unavailable. No 32/64 differential claim is made.
7. **Allocator/persistence scope:** allocation alignment is pointer-word alignment;
   over-aligned objects are unsupported. Individual requests above roughly 2 GiB
   fail explicitly while page/snapshot sizes remain int. Snapshots are raw native
   layout, same-ABI trusted files, without format versioning or malformed-input
   validation. Neither 32-bit snapshot migration nor cross-build function-ID
   compatibility is established. Legacy callback casts in generic containers and
   generated function tables need a wider C-ABI audit.

Six of eight compiler diagnostic references match exactly after path normalization.
`fail3` differs by one source column; `zeroptr` retains the expected compiler
exception but reports the current source line (1777 vs 1732). Neither reference
was updated to hide the difference. The native cyclex executable also generated the compiler lexer (209 states).
The native ABI probe checks 8-byte long/pointer,
4-byte int, high addresses, C calls, argv, and region array bounds/length.

## Partial bootstrap reproduction

```sh
make -f native/Makefile -j16 native/build/buildlib64
mkdir -p native/generated/libc native/include
# The include alias is also created by run-cyclone-tests.py.
ln -s ../.. native/include/cyclone  # only if absent
native/build/buildlib64 -cc gcc-15 -d native/generated/libc \
  -log native/generated/libc/buildlib.log \
  -cstubs native/generated/libc/cstubs.c \
  -cycstubs native/generated/libc/cycstubs.cyc library/std/libc/libc.cys
native/bootstrap-check.py
```

The binding generator exits successfully while recording unsupported assert.h
and stdarg.h, so its exit status alone is insufficient. The bootstrap checker
returns nonzero for unresolved modules and preserves the logs in `native/bootstrap`.
Next work should address integer semantics and generated bindings, implement SetST
rollback/full persistence, then complete and compare the compiler stages.
