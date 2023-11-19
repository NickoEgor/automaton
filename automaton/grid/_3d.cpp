#include "automaton/grid/_3d.hpp"

#include <algorithm>
#include <utility>

namespace automaton {

cell_3d::cell_3d(uint32_t row, uint32_t col, int slice)
    : base_cell(row, col),
      slice(slice) {}

bool cell_3d::operator<(const cell_3d& other) const {
    if (row == other.row) {
        if (col == other.col) return slice < other.slice;
        return col < other.col;
    }

    // return backwards to put upper point in std::set first
    return row > other.row;
}

bool grid_3d::has(cell_3d cell) {
    for (auto it = _cells.begin(); it != _cells.end(); it++) {
        if (it->row == cell.row && it->col == cell.col) return true;
    }
    return false;
}

void grid_3d::remove(cell_3d cell) {
    std::erase_if(_cells, [&](const cell_3d& c) { return c.row == cell.row && c.col == cell.col; });
}

void grid_3d::move(cell_3d from, cell_3d to) {
    _cells.erase(_cells.find(from));
    _cells.insert(to);
}

void grid_3d::set_rows(uint32_t rows) {
    base_grid::set_rows(rows);
    update_sizes(rows, _cols);
}

void grid_3d::set_cols(uint32_t cols) {
    base_grid::set_cols(cols);
    update_sizes(_rows, cols);
}

void grid_3d::update_sizes(uint32_t rows, uint32_t cols) {
    for (auto it = _cells.begin(); it != _cells.end();) {
        if (it->row > rows - 1 || it->col > cols - 1)
            it = _cells.erase(it);
        else
            it++;
    }
}

}  // namespace automaton
