#include "automaton/grid/_3d.hpp"

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
    for (auto it = _data.begin(); it != _data.end(); it++) {
        if (it->row == cell.row && it->col == cell.col) return true;
    }
    return false;
}

bool grid_3d::remove(cell_3d cell) {
    bool removed = false;

    for (auto it = _data.begin(); it != _data.end(); it++) {
        if (it->row == cell.row && it->col == cell.col) {
            _data.erase(it);
            // don't return from cycle to remove
            // same row/col with diff slices
            removed = true;
        }
    }

    return removed;
}

void grid_3d::move(cell_3d from, cell_3d to) {
    _data.erase(_data.find(from));
    _data.insert(to);
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
    for (auto it = _data.begin(); it != _data.end();) {
        if (it->row > rows - 1 || it->col > cols - 1)
            it = _data.erase(it);
        else
            it++;
    }
}

}  // namespace automaton
