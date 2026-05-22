#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <array>

#include "board.h"
#include "solver.h"

namespace py = pybind11;

PYBIND11_MODULE(sudoku_cpp, m) {
    m.doc() = "C++ Sudoku solver core";

    py::enum_<actionType>(m, "ActionType")
        .value("TRY_VALUE", TRY_VALUE)
        .value("PLACE_VALUE", PLACE_VALUE)
        .value("REMOVE_VALUE", REMOVE_VALUE)
        .value("BACKTRACK", BACKTRACK)
        .value("SOLVED", SOLVED)
        .export_values();

    py::class_<Board>(m, "Board")
        .def(py::init<>())
        .def(py::init<std::array<std::array<int, Board::SIZE>, Board::SIZE>>())
        .def("get", &Board::get)
        .def("set", &Board::set)
        .def("is_empty", &Board::is_empty)
        .def("is_valid", &Board::is_valid)
        .def("is_complete", &Board::is_complete)
        .def("is_valid_placement", &Board::is_valid_placement)
        .def("candidates", &Board::candidates)
        .def("find_empty", [](const Board& board) -> py::object {
            int row = 0;
            int col = 0;
            if (board.find_empty(row, col)) {
                return py::make_tuple(row, col);
            }
            return py::none();
        })
        .def("to_array", &Board::to_array);

    py::class_<Step>(m, "Step")
        .def(py::init([]() {
            Step step;
            step.action = TRY_VALUE;
            return step;
        }))
        .def_readwrite("row", &Step::row)
        .def_readwrite("col", &Step::col)
        .def_readwrite("value", &Step::value)
        .def_readwrite("action", &Step::action);

    py::class_<Result>(m, "Result")
        .def(py::init<>())
        .def_readwrite("totalSteps", &Result::totalSteps)
        .def_readwrite("solved", &Result::solved)
        .def_readwrite("board", &Result::board)
        .def_readwrite("stepPath", &Result::stepPath);

    py::class_<Options>(m, "Options")
        .def(py::init<>())
        .def_readwrite("collectSteps", &Options::collectSteps);
}
