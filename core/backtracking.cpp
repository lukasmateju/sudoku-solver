#include "solver.h"

using namespace std;

class BacktrackingSolver : public Solver {
public:
    string name() const override {
        return "Backtracking";
    }

    Result solve(Board puzzle, const Options& options) override {
        Result result;

        if(!puzzle.is_valid()) {
            result.board = puzzle;
            return result;
        }

        result.solved = backtrack(puzzle, result, options);
        result.board = puzzle;

        if (result.solved) {
            add_step(result, options, -1, -1, 0, SOLVED);
        }

        return result;
    }

private:
    bool backtrack(Board& board, Result& result, const Options& options) {
        int row = 0;
        int col = 0;

        if (!board.find_empty(row, col)) {
            return board.is_complete();
        }

        vector<int> values = board.candidates(row, col);

        for (int value : values) {
            add_step(result, options, row, col, value, TRY_VALUE);

            board.set(row, col, value);
            add_step(result, options, row, col, value, PLACE_VALUE);

            if (backtrack(board, result, options)) {
                return true;
            }

            board.set(row, col, 0);
            add_step(result, options, row, col, 0, BACKTRACK);
        }

        return false;
    }
};

Result solve_backtracking(Board puzzle, const Options& options) {
    BacktrackingSolver solver;
    return solver.solve(puzzle, options);
}
