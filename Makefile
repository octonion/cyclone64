# Native Banshee64 and Cyclone64. All generated files stay below BUILD.
.DEFAULT_GOAL := all
ifeq ($(origin CC),default)
CC := $(shell native/select-cc.sh)
endif
LIBC_CC ?= $(shell native/select-cc.sh)
AR ?= ar
PYTHON ?= python3
BUILD ?= build/native
ARCH ?= $(shell native/select-arch.sh "$(CC)")
CFLAGS ?= -O2 -g
CPPFLAGS ?=
LDFLAGS ?=
NATIVE_GC ?= system
PREFIX ?= /usr/local
prefix ?= $(PREFIX)
DESTDIR ?=
export CC LIBC_CC AR BUILD ARCH CFLAGS CPPFLAGS LDFLAGS NATIVE_GC prefix DESTDIR
# JOBS overrides -j; otherwise the coordinator uses make's requested job count.
export JOBS
.PHONY: all check install clean help
all check install clean:
	+@$(PYTHON) native/unified-build.py $@
help:
	@echo 'make all      Build independent Banshee64 and bootstrap Cyclone64'
	@echo 'make check    Run Banshee, Cyclone, runtime and convergence checks'
	@echo 'make install  Install libraries, headers, pkg-config and cyclone64 driver'
	@echo 'make clean    Remove only this managed BUILD directory'
	@echo 'Overrides: BUILD CC LIBC_CC AR ARCH CFLAGS CPPFLAGS LDFLAGS JOBS NATIVE_GC PREFIX DESTDIR'
