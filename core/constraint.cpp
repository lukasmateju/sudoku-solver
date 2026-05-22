#include "solver.h"

using namespace std;

class ConstraintSolver : public Solver {
public:
    string name() const override {
        return "Constraint Propagation";
    }

    Result solve(Board puzzle, const Options& options) override {
        Result result;

        // setup result
        // repeatedly fill cells with forced candidates
        // optionally fall back to search when propagation stalls
        // fill result.finalBoard
        // return result

        return result;
    }

private:
    bool propagate(Board& board, Result& result, const Options& options) {
        // constraint propagation logic goes here
        return false;
    }
};
