#include "board.h"

#include <stdexcept>

using namespace std;

Board::Board() {
    board = {0};
}

Board::Board(Grid boardIn) {
    for (int row = 0; row < SIZE; row++) {
        for (int col = 0; col < SIZE; col++) {
            valid_value(boardIn[row][col]);
            board[row][col] = boardIn[row][col];
        }
    }
    
}


int  Board::get(int rowIn, int colIn) const {
    in_bounds(rowIn, colIn);
    return board[rowIn][colIn];
}

void Board::set(int rowIn, int colIn, int valueIn) {
    in_bounds(rowIn, colIn);
    valid_value(valueIn);

    board[rowIn][colIn] = valueIn;
}

bool Board::is_empty(int rowIn, int colIn) const {
    in_bounds(rowIn, colIn);

    return board[rowIn][colIn] == EMPTY;
}

bool Board::is_valid() const {
    for (int row = 0; row < SIZE; row++) {
        for (int col = 0; col < SIZE; col++) {
            int currValue = board[row][col];
            if (currValue == EMPTY) { continue; }

            if (!is_valid_placement(row, col, currValue)) {
                return false;
            }
        }
    }

    return true;
}

bool Board::is_complete() const {
    for (int row = 0; row < SIZE; row++) {
        for (int col = 0; col < SIZE; col++) {
            if (is_empty(row, col)) { return false; }
        }
    }

    return is_valid();
}

bool Board::is_valid_placement(int rowIn, int colIn, int valueIn) const {
    in_bounds(rowIn, colIn);
    valid_value(valueIn);

    // Empty Value Check
    if (valueIn == EMPTY) {
        return false;
    }

    // Column Check
    for (int row = 0; row < SIZE; row++) {
        if ((row != rowIn) && (board[row][colIn] == valueIn)) { return false; }
    }

    // Row Check
    for (int col = 0; col < SIZE; col++) {
        if ((col != colIn) && (board[rowIn][col] == valueIn)) { return false; }
    }

    // 3x3 Box Check
    int boxRowStart = (rowIn / BOX) * BOX;
    int boxColStart = (colIn / BOX) * BOX;

    for (int row = boxRowStart; row < boxRowStart + BOX; row++) {
        for (int col = boxColStart; col < boxColStart + BOX; col++) {
            if (!((row == rowIn) && (col == colIn))) {
                if (board[row][col] == valueIn) { return false; }
            }
        }
    }

    return true;
}

vector<int> Board::candidates(int rowIn, int colIn) const {
    in_bounds(rowIn, colIn);
    
    vector<int> result;
    if (!is_empty(rowIn, colIn)) { return result; }

    for (int value = 1; value <= SIZE; value++) {
        if (is_valid_placement(rowIn, colIn, value)) {
            result.push_back(value);
        }
    }

    return result;
}

bool Board::find_empty(int& rowOut, int& colOut) const {
    for (int row = 0; row < SIZE; row++) {
        for (int col = 0; col < SIZE; col++) {
            if (is_empty(row, col)) {
                rowOut = row;
                colOut = col;
                return true;
            }
        }
    }

    return false;
}

Board::Grid Board::to_array() const {
    return board;
}

void Board::in_bounds(int rowIn, int colIn) const {
    if (rowIn < 0 || rowIn >= SIZE || colIn < 0 || colIn >= SIZE) {
        throw std::out_of_range("Board index out of range");
    }
}

void Board::valid_value(int valueIn) const {
    if (valueIn < EMPTY || valueIn > SIZE) {
        throw std::invalid_argument("Board value must be between 0 and 9");
    }
}
