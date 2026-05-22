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
    Board finalBoard;
    std::vector<Step> stepPath;
};

struct Options {
    bool collectSteps = false;
    int maxSteps = 1000000;
};


class Solver {
public:
    virtual ~Solver() = default;

    virtual std::string name() const = 0;
    virtual Result solve(Board puzzle, const Options& options) = 0;
};



#endif // DO NOT REMOVE
