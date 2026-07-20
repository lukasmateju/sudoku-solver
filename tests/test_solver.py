from __future__ import annotations

import unittest

import sudoku_cpp

from common import PUZZLES, SOLVERS, board_to_grid, parse_puzzle, run_solver


EXPECTED_SOLUTIONS = {
    "easy": "534678912672195348198342567859761423426853791713924856961537284287419635345286179",
    "medium": "435269781682571493197834562826195347374682915951743628519326874248957136763418259",
    "hard": "462831957795426183381795426173984265659312748248567319926178534834259671517643892",
}


def board_digits(board: sudoku_cpp.Board) -> str:
    return "".join(str(value) for row in board_to_grid(board) for value in row)


class BoardTests(unittest.TestCase):
    def test_parse_accepts_dots_and_formatting(self) -> None:
        board = parse_puzzle(("." * 9 + "\n") * 9)
        self.assertEqual(board_digits(board), "0" * 81)

    def test_parse_rejects_wrong_length(self) -> None:
        with self.assertRaises(ValueError):
            parse_puzzle("0" * 80)

    def test_board_rejects_out_of_range_values(self) -> None:
        grid = [[0] * 9 for _ in range(9)]
        grid[0][0] = 10
        with self.assertRaises(ValueError):
            sudoku_cpp.Board(grid)


class SolverTests(unittest.TestCase):
    complete_solvers = ("backtracking", "hybrid", "exactcover")

    def test_complete_solvers_solve_all_samples(self) -> None:
        for solver_name in self.complete_solvers:
            for puzzle_name, expected in EXPECTED_SOLUTIONS.items():
                with self.subTest(solver=solver_name, puzzle=puzzle_name):
                    result = run_solver(solver_name, PUZZLES[puzzle_name], collect_steps=False)
                    self.assertTrue(result.solved)
                    self.assertTrue(result.board.is_complete())
                    self.assertEqual(board_digits(result.board), expected)
                    self.assertEqual(len(result.stepPath), 0)

    def test_constraint_solver_solves_easy_sample(self) -> None:
        result = run_solver("constraint", PUZZLES["easy"], collect_steps=True)
        self.assertTrue(result.solved)
        self.assertEqual(board_digits(result.board), EXPECTED_SOLUTIONS["easy"])
        self.assertGreater(len(result.stepPath), 0)

    def test_invalid_board_is_rejected_by_every_solver(self) -> None:
        invalid = "11" + "0" * 79
        for solver_name in SOLVERS:
            with self.subTest(solver=solver_name):
                result = run_solver(solver_name, invalid, collect_steps=False)
                self.assertFalse(result.solved)
                self.assertFalse(result.board.is_valid())

    def test_valid_but_unsolvable_board_is_not_solved(self) -> None:
        unsolvable = "123456780" + "000000009" + "0" * 63
        self.assertTrue(parse_puzzle(unsolvable).is_valid())
        for solver_name in self.complete_solvers:
            with self.subTest(solver=solver_name):
                result = run_solver(solver_name, unsolvable, collect_steps=False)
                self.assertFalse(result.solved)

    def test_unknown_solver_is_rejected(self) -> None:
        with self.assertRaises(ValueError):
            run_solver("unknown", PUZZLES["easy"])


if __name__ == "__main__":
    unittest.main()
