from __future__ import annotations

import argparse

from common import PUZZLES, SOLVERS, action_name, board_to_text, get_puzzle, parse_puzzle, run_solver


def build_parser() -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(description="Run a Sudoku solver from the command line.")
    parser.add_argument(
        "--solver",
        choices=sorted(SOLVERS),
        default="hybrid",
        help="Solver algorithm to run.",
    )
    parser.add_argument(
        "--puzzle",
        default="easy",
        help="Puzzle name or 81-character puzzle string. Use 0 or . for empty cells.",
    )
    parser.add_argument(
        "--no-steps",
        action="store_true",
        help="Do not collect the step path.",
    )
    parser.add_argument(
        "--show-steps",
        action="store_true",
        help="Print every collected solving step.",
    )
    parser.add_argument(
        "--list-puzzles",
        action="store_true",
        help="List built-in puzzle names.",
    )
    return parser


def print_steps(result) -> None:
    for index, step in enumerate(result.stepPath, start=1):
        print(
            f"{index:5d}: {action_name(step.action):12s} "
            f"row={step.row + 1 if step.row >= 0 else '-'} "
            f"col={step.col + 1 if step.col >= 0 else '-'} "
            f"value={step.value}"
        )


def main() -> int:
    parser = build_parser()
    args = parser.parse_args()

    if args.list_puzzles:
        print("Built-in puzzles:")
        for name in sorted(PUZZLES):
            print(f"  {name}")
        return 0

    puzzle_name, puzzle_text = get_puzzle(args.puzzle)
    start_board = parse_puzzle(puzzle_text)
    result = run_solver(args.solver, puzzle_text, collect_steps=not args.no_steps)

    print(f"Puzzle: {puzzle_name}")
    print(f"Solver: {args.solver}")
    print()
    print("Starting board:")
    print(board_to_text(start_board))
    print()
    print("Final board:")
    print(board_to_text(result.board))
    print()
    print(f"Solved: {result.solved}")
    print(f"Total steps: {result.totalSteps}")
    print(f"Collected steps: {len(result.stepPath)}")

    if args.show_steps:
        print()
        print("Steps:")
        print_steps(result)

    return 0 if result.solved else 1


if __name__ == "__main__":
    raise SystemExit(main())
