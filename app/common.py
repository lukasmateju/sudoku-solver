"""Shared Python helpers for the Sudoku C++ extension."""

from __future__ import annotations

from dataclasses import dataclass
from typing import Callable

import sudoku_cpp


PUZZLES: dict[str, str] = {
    "easy": "530070000600195000098000060800060003400803001700020006060000280000419005000080079",
    "medium": "000260701680070090190004500820100040004602900050003028009300074040050036703018000",
    "hard": "000000907000420180000705026100904000050000040000507009920108000034059000507000000",
}


SOLVERS: dict[str, Callable] = {
    "backtracking": sudoku_cpp.solve_backtracking,
    "constraint": sudoku_cpp.solve_constraint,
    "hybrid": sudoku_cpp.solve_hybrid,
    "exactcover": sudoku_cpp.solve_exactcover,
}


@dataclass
class RunConfig:
    solver_name: str
    puzzle_name: str
    puzzle_text: str
    collect_steps: bool


def parse_puzzle(text: str) -> sudoku_cpp.Board:
    cleaned = "".join(ch for ch in text if ch.isdigit() or ch == ".")

    if len(cleaned) != 81:
        raise ValueError("Puzzle must contain exactly 81 digits/dots")

    grid = []
    for row in range(9):
        row_values = []
        for col in range(9):
            ch = cleaned[row * 9 + col]
            row_values.append(0 if ch == "." else int(ch))
        grid.append(row_values)

    return sudoku_cpp.Board(grid)


def board_to_grid(board: sudoku_cpp.Board) -> list[list[int]]:
    return [list(row) for row in board.to_array()]


def board_to_text(board: sudoku_cpp.Board) -> str:
    grid = board_to_grid(board)
    lines = []

    for row, values in enumerate(grid):
        if row > 0 and row % 3 == 0:
            lines.append("------+-------+------")

        chunks = []
        for start in range(0, 9, 3):
            chunk = []
            for value in values[start:start + 3]:
                chunk.append("." if value == 0 else str(value))
            chunks.append(" ".join(chunk))

        lines.append(" | ".join(chunks))

    return "\n".join(lines)


def make_options(collect_steps: bool) -> sudoku_cpp.Options:
    options = sudoku_cpp.Options()
    options.collectSteps = collect_steps
    return options


def get_puzzle(name_or_text: str) -> tuple[str, str]:
    if name_or_text in PUZZLES:
        return name_or_text, PUZZLES[name_or_text]
    return "custom", name_or_text


def run_solver(solver_name: str, puzzle_text: str, collect_steps: bool = True):
    if solver_name not in SOLVERS:
        choices = ", ".join(sorted(SOLVERS))
        raise ValueError(f"Unknown solver '{solver_name}'. Choose one of: {choices}")

    board = parse_puzzle(puzzle_text)
    options = make_options(collect_steps)
    return SOLVERS[solver_name](board, options)


def action_name(action) -> str:
    text = str(action)
    return text.split(".")[-1]
