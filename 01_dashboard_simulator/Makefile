# ===============================================================================
# QNX Automotive Dashboard Simulator - Advanced Makefile
# ===============================================================================
# Features:
# - Multiple build configurations (Debug, Release, Automotive, Performance)
# - Cross-compilation support (x86, ARM, AArch64)
# - Quality assurance (Static analysis, Memory leak detection)
# - Professional build system with CI/CD integration
# ===============================================================================

# Project Configuration
PROJECT_NAME = qnx-dashboard-simulator
VERSION = 1.0.0
COPYRIGHT = 2025 QNX Automotive Projects Portfolio

# Directory Structure
SRC_DIR = src
INC_DIR = include
OBJ_DIR = build/obj
BIN_DIR = build/bin
LIB_DIR = lib
TEST_DIR = tests
SCRIPTS_DIR = scripts
RESOURCES_DIR = resources
DOC_DIR = docs

# Build Configuration (default: debug)
BUILD_CONFIG ?= debug
TARGET_ARCH ?= x86_64
QNX_VERSION ?= 7.1

# Compiler Configuration
CC = qcc
CXX = qcc
LD = qcc
AR = qnx-ar
STRIP = qnx-strip

# QNX System Includes
QNX_TARGET ?= /opt/qnx710/target/qnx7
QNX_HOST ?= /opt/qnx710/host/linux/x86_64

# Architecture-specific settings
ifeq ($(TARGET_ARCH),x86_64)
    ARCH_FLAGS = -Vgcc_ntox86_64
    ARCH_SUFFIX = x86_64
else ifeq ($(TARGET_ARCH),arm)
    ARCH_FLAGS = -Vgcc_ntoarmv7le
    ARCH_SUFFIX = armle-v7
else ifeq ($(TARGET_ARCH),aarch64)
    ARCH_FLAGS = -Vgcc_ntoaarch64le
    ARCH_SUFFIX = aarch64le
endif

# Source Files
MAIN_SOURCES = \
    $(SRC_DIR)/main.c \
    $(SRC_DIR)/gui_manager.c \
    $(SRC_DIR)/data_simulator.c \
    $(SRC_DIR)/config_manager.c \
    $(SRC_DIR)/dashboard_widgets.c \
    $(SRC_DIR)/performance_monitor.c \
    $(SRC_DIR)/utils.c

TEST_SOURCES = \
    $(TEST_DIR)/test_main.c \
    $(TEST_DIR)/test_data_simulator.c \
    $(TEST_DIR)/test_config_manager.c \
    $(TEST_DIR)/test_gui_manager.c

# Object Files
MAIN_OBJECTS = $(MAIN_SOURCES:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
TEST_OBJECTS = $(TEST_SOURCES:$(TEST_DIR)/%.c=$(OBJ_DIR)/test_%.o)

# Include Paths
INCLUDES = \
    -I$(INC_DIR) \
    -I$(QNX_TARGET)/usr/include \
    -I$(QNX_TARGET)/usr/include/photon

# Library Paths and Libraries
LIBS = \
    -L$(QNX_TARGET)/$(ARCH_SUFFIX)/lib \
    -L$(QNX_TARGET)/$(ARCH_SUFFIX)/usr/lib \
    -lph \
    -lphrender \
    -lphexlib \
    -lc \
    -lm \
    -lpthread

# Common Compiler Flags
COMMON_CFLAGS = \
    $(ARCH_FLAGS) \
    $(INCLUDES) \
    -Wall \
    -Wextra \
    -std=c99 \
    -fno-builtin \
    -DPROJECT_VERSION=\"$(VERSION)\" \
    -DTARGET_ARCH=\"$(TARGET_ARCH)\" \
    -DQN_VERSION=\"$(QNX_VERSION)\"

# Build Configuration Specific Flags
ifeq ($(BUILD_CONFIG),debug)
    CFLAGS = $(COMMON_CFLAGS) -g -O0 -DDEBUG -D_DEBUG -DVERBOSE_LOGGING
    LDFLAGS = $(ARCH_FLAGS) -g
    CONFIG_SUFFIX = debug
else ifeq ($(BUILD_CONFIG),release)
    CFLAGS = $(COMMON_CFLAGS) -O2 -DNDEBUG -DRELEASE_BUILD
    LDFLAGS = $(ARCH_FLAGS) -O2
    CONFIG_SUFFIX = release
else ifeq ($(BUILD_CONFIG),automotive)
    CFLAGS = $(COMMON_CFLAGS) -O2 -DAUTOMOTIVE_BUILD -DSAFETY_CRITICAL \
             -fstack-protector-strong -Wformat-security
    LDFLAGS = $(ARCH_FLAGS) -O2
    CONFIG_SUFFIX = automotive
else ifeq ($(BUILD_CONFIG),performance)
    CFLAGS = $(COMMON_CFLAGS) -O3 -DPERFORMANCE_BUILD -DHIGH_PERFORMANCE \
             -ffast-math -march=native
    LDFLAGS = $(ARCH_FLAGS) -O3
    CONFIG_SUFFIX = performance
endif

# Target Binary Names
MAIN_TARGET = $(BIN_DIR)/$(PROJECT_NAME)-$(CONFIG_SUFFIX)-$(ARCH_SUFFIX)
TEST_TARGET = $(BIN_DIR)/$(PROJECT_NAME)-test-$(CONFIG_SUFFIX)-$(ARCH_SUFFIX)

# Default target
.PHONY: all
all: info directories $(MAIN_TARGET)

# Information display
.PHONY: info
info:
	@echo "==============================================================================="
	@echo " QNX Automotive Dashboard Simulator Build System"
	@echo "==============================================================================="
	@echo " Project: $(PROJECT_NAME) v$(VERSION)"
	@echo " Config:  $(BUILD_CONFIG)"
	@echo " Arch:    $(TARGET_ARCH)"
	@echo " QNX:     $(QNX_VERSION)"
	@echo " Compiler: $(CC)"
	@echo " Target:  $(MAIN_TARGET)"
	@echo "==============================================================================="

# Create directory structure
.PHONY: directories
directories:
	@mkdir -p $(OBJ_DIR) $(BIN_DIR) $(DOC_DIR) $(LIB_DIR)

# Main application
$(MAIN_TARGET): $(MAIN_OBJECTS)
	@echo "🔗 Linking main application..."
	$(LD) $(LDFLAGS) -o $@ $^ $(LIBS)
	@echo "✅ Built: $@"

# Test application
$(TEST_TARGET): $(TEST_OBJECTS) $(filter-out $(OBJ_DIR)/main.o,$(MAIN_OBJECTS))
	@echo "🔗 Linking test application..."
	$(LD) $(LDFLAGS) -o $@ $^ $(LIBS)
	@echo "✅ Built: $@"

# Object file compilation
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@echo "🔨 Compiling $<..."
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/test_%.o: $(TEST_DIR)/%.c
	@echo "🔨 Compiling test $<..."
	$(CC) $(CFLAGS) -c $< -o $@

# Build configurations
.PHONY: debug release automotive performance
debug:
	$(MAKE) BUILD_CONFIG=debug

release:
	$(MAKE) BUILD_CONFIG=release

automotive:
	$(MAKE) BUILD_CONFIG=automotive

performance:
	$(MAKE) BUILD_CONFIG=performance

# Cross-compilation targets
.PHONY: x86_64 arm aarch64
x86_64:
	$(MAKE) TARGET_ARCH=x86_64

arm:
	$(MAKE) TARGET_ARCH=arm

aarch64:
	$(MAKE) TARGET_ARCH=aarch64

# Testing
.PHONY: test test-build
test-build: directories $(TEST_TARGET)
	@echo "✅ Test build complete"

test: test-build
	@echo "🧪 Running Dashboard Simulator Tests..."
	@if [ -f "$(TEST_TARGET)" ]; then \
		echo "Running unit tests..."; \
		$(TEST_TARGET); \
	else \
		echo "❌ Test binary not found: $(TEST_TARGET)"; \
		exit 1; \
	fi

# Quality Assurance
.PHONY: static-analysis lint
static-analysis:
	@echo "🔍 Running static analysis..."
	@if command -v cppcheck >/dev/null 2>&1; then \
		cppcheck --enable=all --std=c99 $(SRC_DIR); \
	else \
		echo "⚠️  cppcheck not available"; \
	fi

lint:
	@echo "🔍 Running code linting..."
	@if command -v clang-tidy >/dev/null 2>&1; then \
		clang-tidy $(MAIN_SOURCES) -- $(INCLUDES); \
	else \
		echo "⚠️  clang-tidy not available"; \
	fi

# Documentation
.PHONY: docs
docs:
	@echo "📚 Generating documentation..."
	@if command -v doxygen >/dev/null 2>&1; then \
		doxygen Doxyfile; \
	else \
		echo "⚠️  Doxygen not available"; \
	fi

# Installation
.PHONY: install
install: $(MAIN_TARGET)
	@echo "📦 Installing Dashboard Simulator..."
	@mkdir -p /opt/dashboard-simulator/bin
	@mkdir -p /opt/dashboard-simulator/resources
	@cp $(MAIN_TARGET) /opt/dashboard-simulator/bin/
	@cp -r $(RESOURCES_DIR)/* /opt/dashboard-simulator/resources/
	@echo "✅ Installation complete"

# Packaging
.PHONY: package
package: $(MAIN_TARGET)
	@echo "📦 Creating deployment package..."
	@mkdir -p package/dashboard-simulator
	@cp $(MAIN_TARGET) package/dashboard-simulator/
	@cp -r $(RESOURCES_DIR) package/dashboard-simulator/
	@cp README.md ARCHITECTURE.md LICENSE package/dashboard-simulator/
	@tar -czf dashboard-simulator-$(VERSION)-$(TARGET_ARCH).tar.gz -C package .
	@echo "✅ Package created: dashboard-simulator-$(VERSION)-$(TARGET_ARCH).tar.gz"

# Profiling and Performance
.PHONY: profile
profile: 
	$(MAKE) BUILD_CONFIG=performance CFLAGS="$(CFLAGS) -pg"
	@echo "🔬 Profiling build complete. Run with gprof for analysis."

# Benchmarking
.PHONY: benchmark
benchmark: $(MAIN_TARGET)
	@echo "⚡ Running performance benchmarks..."
	@if [ -f "$(SCRIPTS_DIR)/benchmark.sh" ]; then \
		$(SCRIPTS_DIR)/benchmark.sh $(MAIN_TARGET); \
	else \
		echo "⚠️  Benchmark script not found"; \
	fi

# Memory Analysis
.PHONY: memory-check
memory-check: $(MAIN_TARGET)
	@echo "🧠 Running memory analysis..."
	@if command -v valgrind >/dev/null 2>&1; then \
		valgrind --leak-check=full --show-leak-kinds=all $(MAIN_TARGET); \
	else \
		echo "⚠️  Valgrind not available"; \
	fi

# Clean targets
.PHONY: clean clean-all distclean
clean:
	@echo "🧹 Cleaning build files..."
	@rm -rf $(OBJ_DIR)/* $(BIN_DIR)/*

clean-all:
	@echo "🧹 Cleaning all generated files..."
	@rm -rf build/ package/ docs/html/ docs/latex/

distclean: clean-all
	@echo "🧹 Deep cleaning..."
	@rm -f *.tar.gz *.log core

# Help
.PHONY: help
help:
	@echo "==============================================================================="
	@echo " QNX Dashboard Simulator Build System - Help"
	@echo "==============================================================================="
	@echo " Build Targets:"
	@echo "   all          - Build default configuration (debug)"
	@echo "   debug        - Build debug version"
	@echo "   release      - Build optimized release version"
	@echo "   automotive   - Build automotive-grade version"
	@echo "   performance  - Build high-performance version"
	@echo ""
	@echo " Architecture Targets:"
	@echo "   x86_64       - Build for x86_64"
	@echo "   arm          - Build for ARM v7"
	@echo "   aarch64      - Build for ARM 64-bit"
	@echo ""
	@echo " Quality Assurance:"
	@echo "   test         - Run unit tests"
	@echo "   static-analysis - Run static code analysis"
	@echo "   lint         - Run code linting"
	@echo "   memory-check - Run memory leak detection"
	@echo ""
	@echo " Documentation & Packaging:"
	@echo "   docs         - Generate documentation"
	@echo "   package      - Create deployment package"
	@echo "   install      - Install to system"
	@echo ""
	@echo " Performance:"
	@echo "   profile      - Build with profiling support"
	@echo "   benchmark    - Run performance benchmarks"
	@echo ""
	@echo " Cleaning:"
	@echo "   clean        - Clean build files"
	@echo "   clean-all    - Clean all generated files"
	@echo "   distclean    - Deep clean everything"
	@echo ""
	@echo " Example Usage:"
	@echo "   make BUILD_CONFIG=release TARGET_ARCH=arm"
	@echo "   make automotive test"
	@echo "   make performance benchmark"
	@echo "==============================================================================="

# Dependencies
-include $(MAIN_OBJECTS:.o=.d)
-include $(TEST_OBJECTS:.o=.d)

# Automatic dependency generation
$(OBJ_DIR)/%.d: $(SRC_DIR)/%.c
	@$(CC) $(CFLAGS) -MM -MT $(@:.d=.o) $< > $@

$(OBJ_DIR)/test_%.d: $(TEST_DIR)/%.c
	@$(CC) $(CFLAGS) -MM -MT $(@:.d=.o) $< > $@ 