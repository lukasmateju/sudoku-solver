cmake_minimum_required(VERSION 3.15)
project(sudoku_cpp LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(pybind11 REQUIRED)

pybind11_add_module(sudoku_cpp
    board.cpp
    backtracking.cpp
    constraint.cpp
    dlx.cpp
    annealing.cpp
    bind.cpp
)