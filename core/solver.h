#ifndef SOLVER_H
#define SOLVER_H

#pragma once

#include <string>
#include <vector>

#include "board.h"

enum actionType {
    TRY_VALUE,
    PLACE_VALUE,
    REMOVE_VALUE,
    BACKTRACK,
    SOLVED
};

struct Step {
    int row = 0;
    int col = 0;
    int value = 0; // 0 is EMPTY
    actionType action;
};

struct Result {
    int totalSteps = 0;
    bool solved = false;
    Board board;
    std::vector<Step> stepPath;
};

struct Options {
    bool collectSteps = false;
};

inline void add_step(Result& result, const Options& options, int row, int col, int value, actionType action) {
    result.totalSteps++;

    if (options.collectSteps) {
        result.stepPath.push_back({row, col, value, action});
    }
}


class Solver {
public:
    virtual ~Solver() = default;

    virtual std::string name() const = 0;
    virtual Result solve(Board puzzle, const Options& options) = 0;
};

Result solve_backtracking(Board puzzle, const Options& options);
Result solve_constraint(Board puzzle, const Options& options);
Result solve_hybrid(Board puzzle, const Options& options);
Result solve_dlx(Board puzzle, const Options& options);
Result solve_annealing(Board puzzle, const Options& options);


#endif // DO NOT REMOVE