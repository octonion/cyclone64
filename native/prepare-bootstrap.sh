#!/bin/sh
# Run from the repository root. All generated files remain under native/.
set -eu
CC=${CC:-$(native/select-cc.sh)}
BUILD=${BUILD:-native/build}
# The legacy C-header parser expects GCC preprocessing of glibc float types.
# CC still builds every C object; LIBC_CC can select the header preprocessor.
LIBC_CC=${LIBC_CC:-$CC}
export CC
mkdir -p native/logs native/generated/libc native/generated/cyclone/library/compiler native/include
if [ ! -e native/include/cyclone ]; then ln -s ../.. native/include/cyclone; fi
make -f native/Makefile -j"${JOBS:-4}" "BUILD=$BUILD" cyclone "$BUILD/buildlib64" "$BUILD/cyclex64" "$BUILD/bison64"
"$BUILD/buildlib64" -cc "$LIBC_CC" -d native/generated/libc \
  -log native/logs/buildlib-generation.log -cstubs native/generated/libc/cstubs.c \
  -cycstubs native/generated/libc/cycstubs.cyc library/std/libc/libc.cys
# assert.h is provided by native/compat from the existing Cyclone implementation.
# stdarg.h is unsupported and unused by the rebuilt Cyclone standard library.
"$BUILD/cyclex64" library/compiler/lex.cyl native/generated/lex.cyc
"$BUILD/cyclex64" library/std/lineno.cyl native/generated/lineno.cyc
"$BUILD/bison64" -v -d library/compiler/parse.cyy -o native/generated/cyclone/library/compiler/parse_tab.cyc
"$CC" -E -x c -nostdinc -Inative/include -Inative/compat -isystem native/generated/libc \
  tools/errorgen/errorgen.cyc -o native/generated/errorgen.cyp
"$BUILD/cyclone64" --cifc-inst-tvar --better-widen -o native/generated/errorgen.c native/generated/errorgen.cyp
make -f native/Makefile "BUILD=$BUILD" "$BUILD/errorgen64"
"$BUILD/errorgen64" -c "$BUILD/cyclone64" library/compiler/parse_errors.err.cyc \
  > native/generated/cyclone/library/compiler/parse_errors_gen.h
