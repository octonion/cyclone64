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

# Stage0 uses the checked-in generated C; no Bazel workspace required.
CYC_SOURCES := $(filter-out stage0/library/compiler/config.c,$(wildcard stage0/library/compiler/*.c stage0/library/std/*.c stage0/library/std/libc/*.c library/runtime/*.c)) library/compiler/inference/cycinf.c stage0/tools/cyclone/cyclone.c
CYC_OBJECTS := $(patsubst %.c,$(BUILD)/cyclone/%.o,$(CYC_SOURCES))
# Legacy generated C uses erased callback types; keep the diagnostics visible.
CYC_FLAGS = -Wno-error=incompatible-pointer-types -Ilibrary/std/cyc-lib -D_POSIX_C_SOURCE=200809L -D_DEFAULT_SOURCE -DCYC_STATIC_MAIN
.PHONY: cyclone
cyclone: $(BUILD)/cyclone64
$(BUILD)/cyclone/%.o: %.c
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
	$(CC) $(ARCH) $(CFLAGS) $^ $(LDFLAGS) -lbacktrace -ldl -lm -o $@
-include $(CYC_OBJECTS:.o=.d)

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
	$(CC) $(ARCH) $(CFLAGS) $^ $(LDFLAGS) -lbacktrace -ldl -lm -o $@
$(BUILD)/cyclex64: $(filter-out $(BUILD)/cyclone/stage0/library/compiler/% $(BUILD)/cyclone/library/compiler/inference/%,$(CYC_COMMON)) $(patsubst %.c,$(BUILD)/cyclone/%.o,$(wildcard stage0/tools/cyclex/*.c)) $(BUILD)/config.o $(BUILD)/libbanshee64.a
	$(CC) $(ARCH) $(CFLAGS) $^ $(LDFLAGS) -lbacktrace -ldl -lm -o $@

$(BUILD)/blocker-%: native/blockers/%.c $(BUILD)/libbanshee64.a
	$(CC) -std=gnu11 $(ARCH) $(CPPFLAGS) $(CFLAGS) $(WARN) $< $(BUILD)/libbanshee64.a $(LDFLAGS) -o $@
