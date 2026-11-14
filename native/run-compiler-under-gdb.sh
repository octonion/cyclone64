#!/bin/sh
# Use for the first executions of a compiler built with changed internal ABI.
# GDB contains unexpected fatal signals instead of invoking desktop crash tools.
set -eu
: "${CYCLONE_COMPILER:?set CYCLONE_COMPILER to the native compiler under test}"
if [ ! -x "$CYCLONE_COMPILER" ]; then
  printf 'Compiler is missing or not executable: %s\n' "$CYCLONE_COMPILER" >&2
  exit 127
fi
exec gdb -q -batch --return-child-result \
  -ex 'set debuginfod enabled off' \
  -ex 'handle SIGUSR1 nostop noprint pass' \
  -ex 'handle SIGUSR2 nostop noprint pass' \
  -ex run -ex 'thread apply all bt' --args "$CYCLONE_COMPILER" "$@"
