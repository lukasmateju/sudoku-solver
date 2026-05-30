#include "solver.h"

using namespace std;

class ExactCoverSolver : public Solver {
public:
    string name() const override {
        return "Exact Cover";
    }

    Result solve(Board puzzle, const Options& options) override {
        Result result;

        if(!puzzle.is_valid()) {
            result.board = puzzle;
            return result;
        }

        result.solved = exactcover(puzzle, result, options);
        result.board = puzzle;

        if (result.solved) {
            add_step(result, options, -1, -1, 0, SOLVED);
        }

        return result;
    }

private:
    static const int TOTAL_CONSTRAINTS = 324;
    static const int COLUMNS_PER_PLACEMENT = 4;

    struct Placement {
        int row = 0;
        int col = 0;
        int value = 0;
        int columns[COLUMNS_PER_PLACEMENT];
    };

    vector<Placement> placements;

    vector<int> columnToPlacements[TOTAL_CONSTRAINTS];

    Placement create_placement(int row, int col, int value) {
        Placement curr;

        curr.row = row;
        curr.col = col;
        curr.value = value;
        curr.columns[0] = cell_constraint(row, col);
        curr.columns[1] = row_constraint(row, value);
        curr.columns[2] = col_constraint(col, value);
        curr.columns[3] = box_constraint(row, col, value);
        
        return curr;
    }

    void add_placement(Placement pIn) {
        int index = placements.size();
        placements.push_back(pIn);

        for (int i = 0; i < COLUMNS_PER_PLACEMENT; i++) {
            columnToPlacements[pIn.columns[i]].push_back(index);
        }
    }

    void build_placements(Board& board) {
        placements.clear();

        for (size_t i = 0; i < TOTAL_CONSTRAINTS; i++) {
            columnToPlacements[i].clear();
        }
        
        for (int row = 0; row < Board::SIZE; row++) {
            for (int col = 0; col < Board::SIZE; col++) {
                int curr = board.get(row, col);

                if (curr != Board::EMPTY) {
                    add_placement(create_placement(row, col, curr));
                } else {
                    for (int value = 1; value <= Board::SIZE; value++) {
                        if (board.is_valid_placement(row, col, value)) {
                            add_placement(create_placement(row, col, value));
                        }
                    }
                }
            }
        }
    }

    int choose_column(const array<bool, TOTAL_CONSTRAINTS>& coveredCols, const vector<bool>& blockedPlacements) {
        int bestCount = placements.size() + 1;
        int bestCol = -1;

        for(int col = 0; col < TOTAL_CONSTRAINTS; col++) {
            if (coveredCols[col]) { continue; }

            int count = 0;

            for (const int indx : columnToPlacements[col]) {
                if (!blockedPlacements[indx]) { count++; }    
            }
            
            if (count < bestCount) {
                bestCount = count;
                bestCol = col;
            }
        }
        
        return bestCol;
    }


    int cell_constraint(int row, int col) {
        return row * Board::SIZE + col;
    }

    int row_constraint(int row, int value) {
        return 81 + row * Board::SIZE + (value - 1);
    }

    int col_constraint(int col, int value) {
        return 162 + col * Board::SIZE + (value - 1);
    }

    int box_constraint(int row, int col, int value) {
        return 243 + ((row / Board::BOX) * Board::BOX + (col / Board::BOX)) * Board::SIZE + (value - 1);
    }

    bool check_constraints(const array<bool, TOTAL_CONSTRAINTS>& coveredCols) {
        for (const bool col : coveredCols) {
            if (!col) { return false; }
        }
        
        return true;
    }

    bool search(array<bool, TOTAL_CONSTRAINTS> coveredCols, vector<bool> blockedPlacements, vector<int>& solution, Result& result, const Options& options) {
        if (check_constraints(coveredCols)) { return true; }

        int col = choose_column(coveredCols, blockedPlacements);

        if (col == -1) { return true; }

        for (int indx : columnToPlacements[col]) {
            if (blockedPlacements[indx]) { continue; }

            Placement p = placements[indx];
            add_step(result, options, p.row, p.col, p.value, TRY_VALUE);

            array<bool, TOTAL_CONSTRAINTS> nextCoveredCols = coveredCols;
            vector<bool> nextBlockedPlacements = blockedPlacements;
            vector<int> nextSolution = solution;

            nextSolution.push_back(indx);
            add_step(result, options, p.row, p.col, p.value, PLACE_VALUE);

            for (int i = 0; i < COLUMNS_PER_PLACEMENT; i++) {
                int coveredCol = p.columns[i];
                nextCoveredCols[coveredCol] = true;
            }

            for (int i = 0; i < COLUMNS_PER_PLACEMENT; i++) {
                int coveredCol = p.columns[i];

                for (int blockedIndx : columnToPlacements[coveredCol]) {
                    nextBlockedPlacements[blockedIndx] = true;
                }
            }

            if (search(nextCoveredCols, nextBlockedPlacements, nextSolution, result, options)) {
                solution = nextSolution;
                return true;
            }

            add_step(result, options, p.row, p.col, p.value, BACKTRACK);
        }

        return false;
    }

    bool exactcover(Board& board, Result& result, const Options& options) {
        build_placements(board);

        array<bool, TOTAL_CONSTRAINTS> coveredCols = {false}; 
        vector<bool> blockedPlacements(placements.size(), false);
        vector<int> solution;

        if (!search(coveredCols, blockedPlacements, solution, result, options)) {
            return false;
        }

        for (int placementIndex : solution) {
            Placement placement = placements[placementIndex];
            board.set(placement.row, placement.col, placement.value);
        }

        return board.is_complete();
    }
};

Result solve_exactcover(Board puzzle, const Options& options) {
    ExactCoverSolver solver;
    return solver.solve(puzzle, options);
}