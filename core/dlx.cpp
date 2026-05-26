#include "solver.h"

using namespace std;

class DLXSolver : public Solver {
public:
    string name() const override {
        return "Dancing Links";
    }

    Result solve(Board puzzle, const Options& options) override {
        Result result;

        // setup result
        // convert the puzzle into an exact-cover representation
        // run the DLX search
        // fill result.finalBoard
        // return result

        return result;
    }

private:
    bool search(Board& board, Result& result, const Options& options) {
        // DLX search logic goes here
        return false;
    }
};

Result solve_dlx(Board puzzle, const Options& options) {
    DLXSolver solver;
    return solver.solve(puzzle, options);
}