#include "solver.h"

using namespace std;

class AnnealingSolver : public Solver {
public:
    string name() const override {
        return "Simulated Annealing";
    }

    Result solve(Board puzzle, const Options& options) override {
        Result result;

        // setup result
        // create an initial filled board
        // repeatedly swap values and accept/reject moves by temperature
        // fill result.finalBoard
        // return result

        return result;
    }

private:
    bool anneal(Board& board, Result& result, const Options& options) {
        // simulated annealing logic goes here
        return false;
    }
};
