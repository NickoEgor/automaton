#include "automaton/grid/_2d.hpp"

#include <utility>

namespace automaton {

bool cell_2d::operator<(const cell_2d& other) const {
    if (row == other.row) return col < other.col;
    // return backwards to put upper row in std::set first
    return row > other.row;
}

void grid_2d::move(cell_2d from, cell_2d to) {
    auto it = _data.find(from);
    if (it != _data.end()) _data.erase(it);

    _data.insert(to);
}

void grid_2d::set_rows(uint32_t rows) {
    base_grid::set_rows(rows);
    update_sizes(rows, _cols);
}

void grid_2d::set_cols(uint32_t cols) {
    base_grid::set_cols(cols);
    update_sizes(_rows, cols);
}

void grid_2d::update_sizes(uint32_t rows, uint32_t cols) {
    for (auto it = _data.begin(); it != _data.end();) {
        if (it->row > rows - 1 || it->col > cols - 1)
            it = _data.erase(it);
        else
            it++;
    }
}

}  // namespace automaton
