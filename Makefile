VENV_PYTHON := $(wildcard .venv/bin/python)
PYTHON ?= $(if $(VENV_PYTHON),$(VENV_PYTHON),python3)
CMAKE ?= cmake
PYTHON_ARCH := $(shell $(PYTHON) -c "import platform; print(platform.machine())")
PYTHON_EXECUTABLE := $(shell $(PYTHON) -c "import sys; print(sys.executable)")
PYTHON_ROOT_DIR := $(shell $(PYTHON) -c "import sys; print(sys.prefix)")
CMAKE_COMMON_FLAGS := -DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
	-DPython_EXECUTABLE=$(PYTHON_EXECUTABLE) \
	-DPython_ROOT_DIR=$(PYTHON_ROOT_DIR)

ifeq ($(shell uname -s),Darwin)
CMAKE_COMMON_FLAGS += -DCMAKE_OSX_ARCHITECTURES=$(PYTHON_ARCH)
endif

SRC_DIR := core
BUILD_DIR := build
DEBUG_BUILD_DIR := build-debug
RELEASE_BUILD_DIR := build-release
TMP_DIR := tmp
BT_TEST := $(TMP_DIR)/backtracking_test
BT_TEST_OUTPUT := $(TMP_DIR)/backtracking_test_output.txt

.PHONY: help configure build debug release check import-test test run cli visualize benchmark generate clean

help:
	@echo "Sudoku solver commands:"
	@echo "  make configure    Configure the default build directory"
	@echo "  make build        Build the C++ Python extension"
	@echo "  make debug        Configure and build with C++ debug symbols"
	@echo "  make release      Configure and build an optimized release extension"
	@echo "  make check        Syntax-check core C++ files"
	@echo "  make import-test  Build and verify Python can import sudoku_cpp"
	@echo "  make test         Build and run the automated test suite"
	@echo "  make run          Build and run app/cli.py"
	@echo "  make visualize    Build and run app/visualize.py"
	@echo "  make benchmark    Build and run app/benchmark.py"
	@echo "  make generate     Build and run app/generate.py"
	@echo "  make clean        Remove generated build directories"

configure:
	$(CMAKE) -S $(SRC_DIR) -B $(BUILD_DIR) $(CMAKE_COMMON_FLAGS)
	ln -sf $(BUILD_DIR)/compile_commands.json compile_commands.json

build: configure
	$(CMAKE) --build $(BUILD_DIR)

import-test: build
	PYTHONPATH=$(BUILD_DIR) $(PYTHON) -c "import sudoku_cpp; print('import ok')"

test: build
	PYTHONPATH=$(BUILD_DIR):app $(PYTHON) -m unittest discover -s tests -v

debug:
	$(CMAKE) -S $(SRC_DIR) -B $(DEBUG_BUILD_DIR) -DCMAKE_BUILD_TYPE=Debug $(CMAKE_COMMON_FLAGS)
	$(CMAKE) --build $(DEBUG_BUILD_DIR)

release:
	$(CMAKE) -S $(SRC_DIR) -B $(RELEASE_BUILD_DIR) -DCMAKE_BUILD_TYPE=Release $(CMAKE_COMMON_FLAGS)
	$(CMAKE) --build $(RELEASE_BUILD_DIR)

check:
	c++ -std=c++17 -fsyntax-only core/board.cpp
	c++ -std=c++17 -fsyntax-only core/backtracking.cpp
	c++ -std=c++17 -fsyntax-only core/constraint.cpp
	c++ -std=c++17 -fsyntax-only core/hybrid.cpp
	c++ -std=c++17 -fsyntax-only core/exactcover.cpp

run: build
	PYTHONPATH=$(BUILD_DIR) $(PYTHON) app/cli.py

visualize: build
	PYTHONPATH=$(BUILD_DIR) $(PYTHON) app/visualize.py

benchmark: build
	PYTHONPATH=$(BUILD_DIR) $(PYTHON) app/benchmark.py

generate: build
	PYTHONPATH=$(BUILD_DIR) $(PYTHON) app/generate.py

clean:
	$(CMAKE) -E rm -rf $(BUILD_DIR) $(DEBUG_BUILD_DIR) $(RELEASE_BUILD_DIR) core/build
	$(CMAKE) -E rm -f $(BT_TEST)
	$(CMAKE) -E rm -f compile_commands.json
