# Sudoku Solver

A Sudoku solver and visualizer built around a C++17 solving core with Python bindings. It includes several algorithms, a command-line interface, benchmarks, and an animated Tkinter visualization of each solving step.

## Features

- Recursive backtracking solver
- Constraint-propagation solver
- Hybrid constraint-propagation and backtracking solver
- Exact-cover solver
- Python API powered by pybind11
- CLI support for built-in and custom puzzles
- Solver benchmarking
- Step-by-step desktop visualization
- Automated correctness tests on Linux and macOS

## Requirements

- Python 3.9 or newer
- A C++17 compiler
- CMake 3.15 or newer
- `make`
- Tkinter, only when using the visualizer

## Setup

Create and activate a virtual environment, then install the build dependency:

```bash
python3 -m venv .venv
source .venv/bin/activate
python -m pip install -r requirements.txt
```

The Makefile automatically uses `.venv/bin/python` when it exists. You can select another interpreter explicitly with `make build PYTHON=/path/to/python`.

Build the C++ extension and verify that Python can import it:

```bash
make import-test
```

## Usage

Solve the default puzzle with the hybrid solver:

```bash
make run
```

Pass CLI options after building by running the Python entry point directly:

```bash
PYTHONPATH=build .venv/bin/python app/cli.py --solver exactcover --puzzle hard
```

Available solvers are `backtracking`, `constraint`, `hybrid`, and `exactcover`. Built-in puzzles are `easy`, `medium`, and `hard`.

You can also provide an 81-character puzzle string. Use `0` or `.` for an empty cell:

```bash
PYTHONPATH=build .venv/bin/python app/cli.py \
  --solver backtracking \
  --puzzle 530070000600195000098000060800060003400803001700020006060000280000419005000080079
```

Add `--show-steps` to print the collected solving path, or `--no-steps` to avoid collecting it.

## Visualization

Launch the Tkinter animation with:

```bash
make visualize
```

To customize the solver, puzzle, or animation speed:

```bash
PYTHONPATH=build .venv/bin/python app/visualize.py \
  --solver hybrid --puzzle medium --delay 75
```

## Benchmarking

Compare every solver on a sample or custom puzzle:

```bash
make benchmark
PYTHONPATH=build .venv/bin/python app/benchmark.py --puzzle hard --repeat 100
```

Timing results depend on the machine and build type. Use `make release` before manual performance comparisons when optimized binaries are important.

## Sample puzzles

`app/generate.py` selects and prints the bundled samples; it does not generate new Sudoku puzzles:

```bash
make generate
PYTHONPATH=build .venv/bin/python app/generate.py --list
```

## Tests

Build the extension and run the complete test suite:

```bash
make test
```

The tests cover parsing and board validation, known solutions, invalid inputs, unsolvable puzzles, step collection, and all complete solver implementations. The constraint-only solver is intentionally limited to puzzles solvable through propagation alone.

## Development commands

```text
make help         List available commands
make build        Build the extension
make debug        Build with debug symbols
make release      Build with compiler optimizations
make check        Syntax-check the C++ sources
make import-test  Verify the Python extension import
make test         Run automated tests
make clean        Remove generated build files
```

## Project structure

```text
core/   C++ board, solvers, pybind11 bindings, and CMake configuration
app/    Shared Python helpers, CLI, benchmark, sample selector, and visualizer
tests/  Python integration and solver correctness tests
```

## License

Released under the [MIT License](LICENSE).
