#include "solver.h"

using namespace std;

class HybridSolver : public Solver {
public:
    string name() const override {
        return "Constraint Propagation + Backtracking";
    }

    Result solve(Board puzzle, const Options& options) override {
        Result result;

        if(!puzzle.is_valid()) {
            result.board = puzzle;
            return result;
        }

        result.solved = hybrid(puzzle, result, options);
        result.board = puzzle;

        if (result.solved) {
            add_step(result, options, -1, -1, 0, SOLVED);
        }

        return result;
    }

private:
    bool hybrid(Board& board, Result& result, const Options& options) {
        // Constraint Propagation Logic
        bool boardChange = true;

        while (boardChange) {
            boardChange = false;

            for (int row = 0; row < Board::SIZE; row++) {
                for (int col = 0; col < Board::SIZE; col++) {
                    if (!board.is_empty(row, col)) { continue; }

                    vector<int> curr = board.candidates(row, col);

                    if (curr.size() == 1) {
                        board.set(row, col, curr[0]);
                        add_step(result, options, row, col, curr[0], PLACE_VALUE);
                        boardChange = true;
                    }
                }
            }
        }
        
        if (board.is_complete()) { return true; }
        
        // Backtracking Logic
        int row = 0;
        int col = 0;

        if (!board.find_empty(row, col)) {
            return board.is_complete();
        }

        vector<int> values = board.candidates(row, col);

        for (int value : values) {
            add_step(result, options, row, col, value, TRY_VALUE);

            Board attemptBoard = board;
            attemptBoard.set(row, col, value);

            add_step(result, options, row, col, value, PLACE_VALUE);

            if (hybrid(attemptBoard, result, options)) {
                board = attemptBoard;
                return true;
            }

            add_step(result, options, row, col, 0, BACKTRACK);
        }

        return false;
    }
};

Result solve_hybrid(Board puzzle, const Options& options) {
    HybridSolver solver;
    return solver.solve(puzzle, options);
}
