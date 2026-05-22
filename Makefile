PYTHON ?= python3
CMAKE ?= cmake
PYTHON_ARCH := $(shell $(PYTHON) -c "import platform; print(platform.machine())")
CMAKE_COMMON_FLAGS := -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_OSX_ARCHITECTURES=$(PYTHON_ARCH)

SRC_DIR := core
BUILD_DIR := build
DEBUG_BUILD_DIR := build-debug
RELEASE_BUILD_DIR := build-release
TMP_DIR := tmp
BT_TEST := $(TMP_DIR)/backtracking_test
BT_TEST_OUTPUT := $(TMP_DIR)/backtracking_test_output.txt

.PHONY: help configure build debug release check import-test bt-test run cli visualize benchmark clean

help:
	@echo "Sudoku solver commands:"
	@echo "  make configure    Configure the default build directory"
	@echo "  make build        Build the C++ Python extension"
	@echo "  make debug        Configure and build with C++ debug symbols"
	@echo "  make release      Configure and build an optimized release extension"
	@echo "  make check        Syntax-check core C++ files"
	@echo "  make import-test  Build and verify Python can import sudoku_cpp"
	@echo "  make bt-test      Build and run temporary C++ backtracking test"
	@echo "  make run          Build and run app/cli.py"
	@echo "  make visualize    Build and run app/visualize.py"
	@echo "  make benchmark    Build and run app/benchmark.py"
	@echo "  make clean        Remove generated build directories"

configure:
	$(CMAKE) -S $(SRC_DIR) -B $(BUILD_DIR) $(CMAKE_COMMON_FLAGS)
	ln -sf $(BUILD_DIR)/compile_commands.json compile_commands.json

build: configure
	$(CMAKE) --build $(BUILD_DIR)

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
	c++ -std=c++17 -fsyntax-only core/dlx.cpp
	c++ -std=c++17 -fsyntax-only core/annealing.cpp

run: build
	PYTHONPATH=$(BUILD_DIR) $(PYTHON) app/cli.py

cli: run

visualize: build
	PYTHONPATH=$(BUILD_DIR) $(PYTHON) app/visualize.py

benchmark: build
	PYTHONPATH=$(BUILD_DIR) $(PYTHON) app/benchmark.py

clean:
	$(CMAKE) -E rm -rf $(BUILD_DIR) $(DEBUG_BUILD_DIR) $(RELEASE_BUILD_DIR) core/build
	$(CMAKE) -E rm -f $(BT_TEST)
	$(CMAKE) -E rm -f compile_commands.json
