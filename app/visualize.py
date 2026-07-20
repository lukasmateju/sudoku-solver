from __future__ import annotations

import argparse
import tkinter as tk

from common import PUZZLES, SOLVERS, action_name, board_to_grid, get_puzzle, parse_puzzle, run_solver


CELL_SIZE = 58
BOARD_SIZE = CELL_SIZE * 9
PADDING = 18


COLORS = {
    "TRY_VALUE": "#fff3bf",
    "PLACE_VALUE": "#c3fae8",
    "REMOVE_VALUE": "#ffc9c9",
    "BACKTRACK": "#ffc9c9",
    "SOLVED": "#d0ebff",
}


class SudokuVisualizer:
    def __init__(self, root: tk.Tk, puzzle_text: str, solver_name: str, delay_ms: int) -> None:
        self.root = root
        self.solver_name = solver_name
        self.delay_ms = delay_ms
        self.start_board = parse_puzzle(puzzle_text)
        self.result = run_solver(solver_name, puzzle_text, collect_steps=True)
        self.grid = board_to_grid(self.start_board)
        self.fixed = [[value != 0 for value in row] for row in self.grid]
        self.step_index = 0
        self.paused = False

        root.title("Sudoku Solver Visualizer")

        self.header = tk.Label(root, text=self.status_text(), font=("Helvetica", 15, "bold"))
        self.header.pack(pady=(12, 4))

        self.canvas = tk.Canvas(root, width=BOARD_SIZE + PADDING * 2, height=BOARD_SIZE + PADDING * 2)
        self.canvas.pack()

        controls = tk.Frame(root)
        controls.pack(pady=(4, 12))

        self.toggle_button = tk.Button(controls, text="Pause", command=self.toggle_pause)
        self.toggle_button.pack(side=tk.LEFT, padx=4)

        self.step_button = tk.Button(controls, text="Step", command=self.step_once)
        self.step_button.pack(side=tk.LEFT, padx=4)

        self.reset_button = tk.Button(controls, text="Reset", command=self.reset)
        self.reset_button.pack(side=tk.LEFT, padx=4)

        self.draw_board()
        self.root.after(self.delay_ms, self.play)

    def status_text(self) -> str:
        return (
            f"{self.solver_name} | solved={self.result.solved} | "
            f"step {self.step_index}/{len(self.result.stepPath)} | total={self.result.totalSteps}"
        )

    def toggle_pause(self) -> None:
        self.paused = not self.paused
        self.toggle_button.config(text="Play" if self.paused else "Pause")

    def reset(self) -> None:
        self.grid = board_to_grid(self.start_board)
        self.step_index = 0
        self.header.config(text=self.status_text())
        self.draw_board()

    def play(self) -> None:
        if not self.paused:
            self.step_once()
        self.root.after(self.delay_ms, self.play)

    def step_once(self) -> None:
        if self.step_index >= len(self.result.stepPath):
            return

        step = self.result.stepPath[self.step_index]
        self.step_index += 1
        self.apply_step(step)
        self.draw_board(step)
        self.header.config(text=self.status_text())

    def apply_step(self, step) -> None:
        if step.row < 0 or step.col < 0:
            self.grid = board_to_grid(self.result.board)
            return

        action = action_name(step.action)
        if action in {"BACKTRACK", "REMOVE_VALUE"}:
            if not self.fixed[step.row][step.col]:
                self.grid[step.row][step.col] = 0
        else:
            self.grid[step.row][step.col] = step.value

    def draw_board(self, active_step=None) -> None:
        self.canvas.delete("all")

        active_row = active_step.row if active_step and active_step.row >= 0 else None
        active_col = active_step.col if active_step and active_step.col >= 0 else None
        active_action = action_name(active_step.action) if active_step else ""

        for row in range(9):
            for col in range(9):
                x1 = PADDING + col * CELL_SIZE
                y1 = PADDING + row * CELL_SIZE
                x2 = x1 + CELL_SIZE
                y2 = y1 + CELL_SIZE

                fill = "#f8f9fa" if self.fixed[row][col] else "#ffffff"
                if row == active_row and col == active_col:
                    fill = COLORS.get(active_action, "#ffffff")

                self.canvas.create_rectangle(x1, y1, x2, y2, fill=fill, outline="#ced4da")

                value = self.grid[row][col]
                if value:
                    weight = "bold" if self.fixed[row][col] else "normal"
                    color = "#212529" if self.fixed[row][col] else "#0b7285"
                    self.canvas.create_text(
                        x1 + CELL_SIZE / 2,
                        y1 + CELL_SIZE / 2,
                        text=str(value),
                        font=("Helvetica", 22, weight),
                        fill=color,
                    )

        for index in range(10):
            width = 3 if index % 3 == 0 else 1
            offset = PADDING + index * CELL_SIZE
            self.canvas.create_line(PADDING, offset, PADDING + BOARD_SIZE, offset, width=width)
            self.canvas.create_line(offset, PADDING, offset, PADDING + BOARD_SIZE, width=width)


def build_parser() -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(description="Animate a Sudoku solver step path.")
    parser.add_argument("--solver", choices=sorted(SOLVERS), default="hybrid")
    parser.add_argument("--puzzle", default="easy")
    parser.add_argument("--delay", type=int, default=40, help="Delay between steps in milliseconds.")
    parser.add_argument("--list-puzzles", action="store_true")
    return parser


def main() -> int:
    parser = build_parser()
    args = parser.parse_args()

    if args.list_puzzles:
        print("Built-in puzzles:")
        for name in sorted(PUZZLES):
            print(f"  {name}")
        return 0

    _, puzzle_text = get_puzzle(args.puzzle)
    root = tk.Tk()
    SudokuVisualizer(root, puzzle_text, args.solver, max(1, args.delay))
    root.mainloop()
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
