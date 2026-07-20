from __future__ import annotations

import argparse
import random

from common import PUZZLES


def build_parser() -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(
        description="Print built-in sample puzzles. Real puzzle generation can live here later."
    )
    parser.add_argument("--name", choices=sorted(PUZZLES), help="Puzzle name to print.")
    parser.add_argument("--random", action="store_true", help="Print a random built-in puzzle.")
    parser.add_argument("--list", action="store_true", help="List built-in puzzle names.")
    return parser


def main() -> int:
    parser = build_parser()
    args = parser.parse_args()

    if args.list:
        for name in sorted(PUZZLES):
            print(name)
        return 0

    name = args.name
    if args.random or name is None:
        name = random.choice(sorted(PUZZLES))

    print(f"{name}:")
    print(PUZZLES[name])
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
