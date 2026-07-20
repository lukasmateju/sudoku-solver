from __future__ import annotations

import argparse
import time

from common import PUZZLES, SOLVERS, get_puzzle, run_solver


def build_parser() -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(description="Benchmark Sudoku solvers on one puzzle.")
    parser.add_argument(
        "--puzzle",
        default="easy",
        help="Puzzle name or 81-character puzzle string. Use 0 or . for empty cells.",
    )
    parser.add_argument(
        "--repeat",
        type=int,
        default=1,
        help="Number of times to run each solver.",
    )
    parser.add_argument(
        "--with-steps",
        action="store_true",
        help="Collect step paths while benchmarking.",
    )
    parser.add_argument(
        "--list-puzzles",
        action="store_true",
        help="List built-in puzzle names.",
    )
    return parser


def main() -> int:
    parser = build_parser()
    args = parser.parse_args()

    if args.list_puzzles:
        print("Built-in puzzles:")
        for name in sorted(PUZZLES):
            print(f"  {name}")
        return 0

    if args.repeat < 1:
        parser.error("--repeat must be at least 1")

    puzzle_name, puzzle_text = get_puzzle(args.puzzle)
    print(f"Puzzle: {puzzle_name}")
    print(f"Repeat: {args.repeat}")
    print(f"Collect steps: {args.with_steps}")
    print()
    print(f"{'Solver':14s} {'Solved':8s} {'Steps':>10s} {'Avg ms':>10s}")
    print("-" * 46)

    for solver_name in sorted(SOLVERS):
        total_time = 0.0
        last_result = None

        for _ in range(args.repeat):
            start = time.perf_counter()
            last_result = run_solver(solver_name, puzzle_text, collect_steps=args.with_steps)
            total_time += time.perf_counter() - start

        avg_ms = (total_time / args.repeat) * 1000.0
        solved = "yes" if last_result and last_result.solved else "no"
        steps = last_result.totalSteps if last_result else 0
        print(f"{solver_name:14s} {solved:8s} {steps:10d} {avg_ms:10.3f}")

    return 0


if __name__ == "__main__":
    raise SystemExit(main())
