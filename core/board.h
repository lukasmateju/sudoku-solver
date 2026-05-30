#ifndef BOARD_H
#define BOARD_H

#pragma once

#include <array>
#include <vector>


class Board {
public:
    static const int SIZE = 9;
    static const int BOX = 3;
    static const int EMPTY = 0;

    using Grid = std::array<std::array<int, SIZE>, SIZE>;

    Board();
    Board(Grid boardIn);

    int  get(int rowIn, int colIn) const;
    void set(int rowIn, int colIn, int value);

    bool is_empty(int rowIn, int colIn) const;
    bool is_valid() const;
    bool is_complete() const;
    bool is_valid_placement(int rowIn, int colIn, int valueIn) const;

    std::vector<int> candidates(int rowIn, int colIn) const;
    bool find_empty(int& rowOut, int& colOut) const;

    Grid to_array() const;

private:
    Grid board;

    void in_bounds(int rowIn, int colIn) const;
    void valid_value(int valueIn) const;
    
};


#endif // DO NOT REMOVE