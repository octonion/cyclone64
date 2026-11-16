# Existential-array lowering repair

Base: `478e840f73a1b922de5b2921e9a2d5c541a03027` (PR #1).
Investigation and local validation: 2026-11-16. No repository push is required
to apply or test this patch.

## Failure and correction

The unmodified source-built stage2 compiler reproduces the exact failure on
`testsuite/tests/structarray.cyc`: aggregate removal encounters
``Internal Compiler Error: bad exp after translation to C: valueof(`j)``.
This is not a missing substitution of the existential witness.

1. Pattern checking opens `Ptr` with a fresh integer type variable. Its tag
   field has type `tag_t<j>` and its array view has bound `valueof(j)`.
   The tag remains an unsigned integer at runtime; `j` is erased.
2. The verification-condition analysis relates the tag and array extent.
   Inside `offset < s`, InsertChecks proves the thin-pointer access safe and
   records `NoCheck` (or `NullOnly` when nullness still needs checking).
   An unproved access with a non-C-evaluable bound is rejected.
3. The native-layout branch in `ptr_use_to_c` ignored that annotation and
   unconditionally injected `_check_known_subscript_null` using the original
   type-level bound. That newly introduced `Valueof_e` survives Toc and violates
   RemoveAggrs' C-expression invariant.
4. The repair respects the annotation for thin pointers and computes the
   native byte offset. Required null checks remain. Fat cursors and thin
   pointers requiring runtime checks retain their existing checked path.

Execution after removing the ICE exposed two further necessary corrections
in the same file, `library/compiler/toc.cyc`:

- An unsized aggregate cannot be copied into its fixed C header. Pattern
  extraction now retains a pointer to the original object; otherwise a nested
  tag is read beyond a header-only temporary. Abstract member addresses pass
  through `void *` before the requested structure cast, and both resulting
  dereference nodes receive the correct type. This avoids incorrectly typed
  temporaries in the later C lowering pass.
- Existential aggregate initialization must retain boxed storage, matching
  `add_struct_type_body`. Previously an `int` instantiation used an array at
  offset 4 while the generic `void *` array view used offset 8 on LP64. The
  generated storage now retains the erased word layout; lengths and abstract
  memory types are still specialized. Initializers and accesses continue to
  use the packed logical element width.

## Safety and ABI argument

For a well-typed package with extent `n`, native element width `w`, and data
address `b`, an accepted proved access has `0 <= i < n`. The existing checked
allocation establishes space for the logical `n*w` bytes (the erased storage
may reserve more). Thus `b + i*w` points to an entire valid element; native
load/store still transfers exactly `w` bytes. Zero-length packages admit no
access. Nested projection now uses the original allocation, preserving this
premise and the actual tag/array relationship.

No typechecking rule, witness field, function signature, tag width, runtime
helper, or allocation-overflow check changes. The initializer correction
restores the existing generic ABI's storage offsets. The 32-bit prover still
does not authorize wide-index check elimination. Such an access through an
erased thin bound remains a compile-time rejection; conversion using the actual
tag to a fat pointer permits checked dynamic indexing.

## Regression coverage

`structarray.cyc` is unchanged and must now translate, link and execute its
original assertions. `run-applications.py` has no expected-failure exception.

`native/layout-tests/generic-existential-array.cyc` adds:

- Lengths 0, 1 and 17; first/last-element reads, writes, and assignment results.
- Heap and region allocation, dynamic comprehensions and generic construction.
- Int, full-width long and pointer elements.
- Three nested structure levels, prefix sentinels, reads and writes.
- Index equal to length, maximum unsigned index, negative and >32-bit fat
  indices, nullable thin-pointer dereference and checked fixed-array indices.

Two compile-failure regressions ensure unguarded and wide-index existential
thin-pointer accesses are rejected. The rejection runner now preprocesses
inputs so semantic diagnostics have source positions, and checks their specific
bounds diagnostic alongside the existing four literal-overflow rejections.

## Validation

Executed in this Debian 13 x86-64 workspace, using portable `-march=x86-64`:

| Check | GCC 15 | Clang 21 |
| --- | --- | --- |
| Fresh stage0 -> seed -> stage1 -> stage2 bootstrap | PASS | PASS |
| Generated-C convergence | 81/81 | 81/81 |
| Full `make check` | PASS | PASS |
| Historical applications (including original structarray) | 31/31 | 31/31 |
| Native/layout executables | 36/36 | 36/36 |
| Historical diagnostics / rejection cases | 8/8 + 6/6 | 8/8 + 6/6 |
| Upstream library suite | PASS | PASS |
| Installation smoke tests and relocated installed-client suite | PASS | PASS |

Toolchains: GCC `15.3.1 20261115` (Debian `15.3.0-6`) and Clang `21.1.8`
(Debian backport `3~bpo13+2`); libc bindings preprocessed with GCC 15 for both.
The relocated installation paths contain spaces. The unmodified baseline was
also source-bootstrapped with GCC 14.2 and converged 81/81 before reproduction.

A separately instrumented current compiler, standard library and runtime passed
all 36 native/layout executables, 8 diagnostics, 6 rejection cases, the upstream
library suite and all 31 applications with Clang ASan + UBSan. Flags were
`-O1 -g -fsanitize=address,undefined -fno-omit-frame-pointer
-fno-sanitize-recover=all`; function sanitization was not disabled. The two
existential-array executables also passed with an instrumented malloc-backed
runtime, providing ASan heap redzones independently of conservative GC.
Banshee solver/region tests and the allocation, native-width allocation,
bounds and libffi callback runtime units also passed ASan + UBSan.

LeakSanitizer cannot complete process inspection in this sandbox: an explicit
`detect_leaks=1` probe fails with its fatal ptrace/environment diagnostic.
Successful sanitizer runs therefore set `ASAN_OPTIONS=detect_leaks=0` and
`UBSAN_OPTIONS=halt_on_error=1:print_stacktrace=1`. They do **not** establish
leak freedom. The existing system-GC bridge also disables ASan fake stacks for
collector compatibility; the malloc-backed tests do not use that bridge.

Pending workstation validation: leak detection in an unrestricted environment,
Ryzen `-march=native`, and the optional multi-GiB allocation stress. None is
claimed as executed here. Per-command logs, convergence JSON and result lists
are included in the review bundle.

## Reproduction on a workstation

```sh
make BUILD=build/existential-gcc CC=gcc-15 LIBC_CC=gcc-15 JOBS=8 all check
make BUILD=build/existential-clang CC=clang-21 LIBC_CC=gcc-15 JOBS=8 all check
```

Use a fresh build directory. For instrumented compiler/library/runtime tests,
use the current-compiler procedure in `REVIEW-VALIDATION.md`; do not execute
the instrumented historical stage0 tools. The new layout and rejection probes
are automatically included in the standard runners and installed-client checks.
