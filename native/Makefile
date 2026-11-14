# Invoke from repository root: make -f native/Makefile check
ifeq ($(origin CC),default)
CC := $(shell native/select-cc.sh)
endif
AR ?= ar
BUILD ?= native/build
ARCH ?= $(shell native/select-arch.sh "$(CC)")
CFLAGS ?= -O2 -g
CPPFLAGS += -DBANSHEE_ROLLBACK -Ddeletes= -Dtraditional= -Dsameregion= -Dparentptr= -Ilibrary/banshee -Ilibrary/banshee/rlib
WARN = -Wall -Wextra -Wno-unused-parameter -Wno-unused-function -Werror=pointer-to-int-cast -Werror=int-to-pointer-cast -Werror=implicit-function-declaration
SOURCES := $(filter-out library/banshee/bounds.c,$(wildcard library/banshee/*.c)) library/banshee/rlib/regions.c library/banshee/rlib/dhash.c
OBJECTS := $(patsubst %.c,$(BUILD)/%.o,$(SOURCES))
TESTS := $(patsubst native/tests/%.c,$(BUILD)/test-%,$(wildcard native/tests/*.c))
.PHONY: all check
all: $(BUILD)/libbanshee64.a
$(BUILD)/libbanshee64.a: $(OBJECTS)
	$(AR) rcsD $@ $^
$(BUILD)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) -std=gnu11 $(ARCH) $(CPPFLAGS) $(CFLAGS) $(WARN) -MMD -MP -c $< -o $@
$(BUILD)/test-%: native/tests/%.c $(BUILD)/libbanshee64.a
	$(CC) -std=gnu11 $(ARCH) $(CPPFLAGS) $(CFLAGS) $(WARN) $< $(BUILD)/libbanshee64.a $(LDFLAGS) -o $@
check: $(TESTS)
	@set -e; for test in $(TESTS); do echo "RUN $$test"; "$$test"; done
-include $(OBJECTS:.o=.d)

# Select explicitly in a fresh BUILD directory; nogc keeps ASan redzones.
NATIVE_GC ?= nogc
CYC_LIBS = -lbacktrace -ldl -lm -lpthread $(shell pkg-config --libs libffi libcrypto)
ifeq ($(NATIVE_GC),system)
CYC_GC_EXCLUDE = library/runtime/nogc.c
CYC_GC_FLAGS = -DCYC_USE_SYSTEM_GC $(shell pkg-config --cflags bdw-gc)
CYC_LIBS += $(shell pkg-config --libs bdw-gc)
else
CYC_GC_EXCLUDE = library/runtime/system_gc.c
endif
.PHONY: runtime-config
runtime-config:
	python3 native/runtime_build.py $(BUILD) $(NATIVE_GC)

# Stage0 uses the checked-in generated C; no Bazel workspace required.
CYC_SOURCES := $(filter-out $(CYC_GC_EXCLUDE) stage0/library/compiler/config.c,$(wildcard stage0/library/compiler/*.c stage0/library/std/*.c stage0/library/std/libc/*.c library/runtime/*.c)) library/compiler/inference/cycinf.c library/compiler/inference/type_name.c stage0/tools/cyclone/cyclone.c
CYC_OBJECTS := $(patsubst %.c,$(BUILD)/cyclone/%.o,$(CYC_SOURCES))
# Legacy generated C uses erased callback types; keep the diagnostics visible.
CYC_FLAGS = -DCYC_LIBFFI $(shell pkg-config --cflags libffi libcrypto) $(CYC_GC_FLAGS) -fno-strict-aliasing -Wno-error=incompatible-pointer-types -Ilibrary/std/cyc-lib -D_POSIX_C_SOURCE=200809L -D_DEFAULT_SOURCE -DCYC_STATIC_MAIN
# libbacktrace's public header ships in GCC's include directory on Kubuntu.
# Clang finds the library but needs that header path for this one source file.
BACKTRACE_CPPFLAGS ?= -idirafter $(shell $(shell native/select-cc.sh) -print-file-name=include)
$(BUILD)/cyclone/library/runtime/runtime_stacktrace.o: CYC_FLAGS += $(BACKTRACE_CPPFLAGS)
.PHONY: cyclone
cyclone: runtime-config $(BUILD)/cyclone64
$(BUILD)/cyclone/%.o: %.c | runtime-config
	@mkdir -p $(dir $@)
	$(CC) -std=gnu11 $(ARCH) $(CPPFLAGS) $(CFLAGS) $(CYC_FLAGS) -MMD -MP -c $< -o $@
$(BUILD)/make-config: library/compiler/make-config.c
	@mkdir -p $(dir $@)
	$(CC) -std=gnu11 $(ARCH) $< -o $@
$(BUILD)/config.c: $(BUILD)/make-config
	$< > $@
$(BUILD)/config.o: $(BUILD)/config.c
	$(CC) $(ARCH) $(CFLAGS) -c $< -o $@
$(BUILD)/cyclone64: $(CYC_OBJECTS) $(BUILD)/config.o $(BUILD)/libbanshee64.a
	$(CC) $(ARCH) $(CFLAGS) $^ $(LDFLAGS) $(CYC_LIBS) -o $@
# Generator objects use the same runtime header; rebuild them when it changes.
CYC_TOOL_OBJECTS := $(patsubst %.c,$(BUILD)/cyclone/%.o,$(wildcard stage0/tools/*/*.c))
-include $(CYC_OBJECTS:.o=.d) $(CYC_TOOL_OBJECTS:.o=.d)

RLIB_TESTS := $(addprefix $(BUILD)/rlib-test-,0 1 2 3 4 5 6 7)
$(BUILD)/rlib-test-%: library/banshee/rlib/test/%/driver.c native/rlib-main.c $(BUILD)/libbanshee64.a
	$(CC) -std=gnu11 $(ARCH) $(CPPFLAGS) $(CFLAGS) $< native/rlib-main.c $(BUILD)/libbanshee64.a $(LDFLAGS) -o $@
.PHONY: rlib-check
rlib-check: $(RLIB_TESTS)
	@set -e; for test in $(RLIB_TESTS); do \
	  mkdir -p "$$test.run"; echo "RUN $$test"; \
	  (cd "$$test.run" && ../$$(basename "$$test")); \
	done

CYC_COMMON := $(filter-out $(BUILD)/cyclone/stage0/tools/cyclone/cyclone.o,$(CYC_OBJECTS))
$(BUILD)/buildlib64: $(CYC_COMMON) $(BUILD)/cyclone/stage0/tools/buildlib/buildlib.o $(BUILD)/config.o $(BUILD)/libbanshee64.a
	$(CC) $(ARCH) $(CFLAGS) $^ $(LDFLAGS) $(CYC_LIBS) -o $@
$(BUILD)/cyclex64: $(filter-out $(BUILD)/cyclone/stage0/library/compiler/% $(BUILD)/cyclone/library/compiler/inference/%,$(CYC_COMMON)) $(patsubst %.c,$(BUILD)/cyclone/%.o,$(wildcard stage0/tools/cyclex/*.c)) $(BUILD)/config.o $(BUILD)/libbanshee64.a
	$(CC) $(ARCH) $(CFLAGS) $^ $(LDFLAGS) $(CYC_LIBS) -o $@

$(BUILD)/blocker-%: native/blockers/%.c $(BUILD)/libbanshee64.a
	$(CC) -std=gnu11 $(ARCH) $(CPPFLAGS) $(CFLAGS) $(WARN) $< $(BUILD)/libbanshee64.a $(LDFLAGS) -o $@

CYC_RUNTIME := $(filter-out $(BUILD)/cyclone/stage0/library/compiler/% $(BUILD)/cyclone/library/compiler/inference/%,$(CYC_COMMON))
$(BUILD)/bison64: $(CYC_RUNTIME) $(patsubst %.c,$(BUILD)/cyclone/%.o,$(wildcard stage0/tools/bison/*.c)) $(BUILD)/config.o $(BUILD)/libbanshee64.a
	$(CC) $(ARCH) $(CFLAGS) $^ $(LDFLAGS) $(CYC_LIBS) -o $@

$(BUILD)/check-constant-fold: native/compiler-checks/constant-fold.c $(CYC_COMMON) $(BUILD)/config.o $(BUILD)/libbanshee64.a
	$(CC) -std=gnu11 $(ARCH) $(CPPFLAGS) $(CFLAGS) $(CYC_FLAGS) $< $(filter-out $(BUILD)/cyclone/stage0/library/compiler/assndef.o,$(filter %.o %.a,$^)) $(LDFLAGS) $(CYC_LIBS) -o $@

$(BUILD)/errorgen64: native/generated/errorgen.c $(CYC_RUNTIME)
	$(CC) -std=gnu11 $(ARCH) $(CFLAGS) $(CYC_FLAGS) $^ $(LDFLAGS) $(CYC_LIBS) -o $@

$(BUILD)/check-runtime-bounds: native/compiler-checks/runtime-bounds.c library/runtime/runtime_zeroterm.c library/std/cyc-lib/cyc_include.h
	@mkdir -p $(dir $@)
	$(CC) -std=gnu11 $(ARCH) $(CFLAGS) -Ilibrary/std/cyc-lib $(filter %.c,$^) $(LDFLAGS) -o $@

$(BUILD)/check-gc-allocation: native/compiler-checks/gc-allocation.c library/runtime/nogc.c library/runtime/runtime_memory.c library/runtime/runtime_stack.c library/runtime/bget.c library/std/cyc-lib/cyc_include.h
	@mkdir -p $(dir $@)
	$(CC) -std=gnu11 $(ARCH) $(CFLAGS) -ffunction-sections -fdata-sections -Ilibrary/std/cyc-lib $(filter %.c,$^) -Wl,--gc-sections $(LDFLAGS) -o $@

$(BUILD)/check-gc-collection: native/compiler-checks/gc-collection.c library/runtime/system_gc.c library/runtime/runtime_memory.c library/runtime/bget.c library/std/cyc-lib/cyc_include.h
	@mkdir -p $(dir $@)
	$(CC) -std=gnu11 $(ARCH) $(CFLAGS) -DCYC_USE_SYSTEM_GC -ffunction-sections -fdata-sections -Ilibrary/std/cyc-lib $(filter %.c,$^) -Wl,--gc-sections $(LDFLAGS) $(shell pkg-config --cflags --libs bdw-gc) -o $@

$(BUILD)/check-callback-ffi: native/compiler-checks/callback-ffi.c library/runtime/runtime_callback.c
	@mkdir -p $(dir $@)
	$(CC) -std=gnu11 $(ARCH) $(CFLAGS) -DCYC_LIBFFI $^ $(LDFLAGS) $(shell pkg-config --cflags --libs libffi) -lpthread -o $@

$(BUILD)/check-type-name: native/compiler-checks/type-name.c library/compiler/inference/type_name.c
	@mkdir -p $(dir $@)
	$(CC) -std=gnu11 $(ARCH) $(CFLAGS) $^ $(LDFLAGS) $(shell pkg-config --cflags --libs libcrypto) -o $@

# A representation prototype only; not yet emitted by the compiler.
$(BUILD)/check-polyref: native/compiler-checks/polyref.c native/experimental/polyref.c native/experimental/polyref.h library/std/cyc-lib/cyc_include.h
	@mkdir -p $(dir $@)
	$(CC) -std=gnu11 $(ARCH) $(CFLAGS) -Ilibrary/std/cyc-lib $(filter %.c,$^) $(LDFLAGS) -o $@
