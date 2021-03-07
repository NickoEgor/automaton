#include <automaton/grid/_3d.hpp>

namespace automaton {

cell_3d::cell_3d(uint32_t row, uint32_t col, int level) : base_cell(row, col), level(level) {}

bool cell_3d::operator<(const cell_3d& other) const {
    if (row == other.row) {
        if (col == other.col) return level < other.level;
        return col < other.col;
    }

    // return backwards to put upper point in std::set first
    return row > other.row;
}

grid_3d::grid_3d(uint32_t rows, uint32_t cols) : base_grid(rows, cols) {}

grid_3d::~grid_3d() {}

void grid_3d::add(uint32_t row, uint32_t col) { _data.emplace(row, col); }

bool grid_3d::has(uint32_t row, uint32_t col) {
    for (auto it = _data.begin(); it != _data.end(); it++) {
        if (it->row == row && it->col == col) return true;
    }
    return false;
}

bool grid_3d::remove(uint32_t row, uint32_t col) {
    bool removed = false;

    for (auto it = _data.begin(); it != _data.end(); it++) {
        if (it->row == row && it->col == col) {
            _data.erase(it);
            // don't return from cycle to remove
            // same row/col with diff levels
            removed = true;
        }
    }

    return removed;
}

void grid_3d::clear() { _data.clear(); }

std::set<base_cell> grid_3d::get_drawable_cells() const {
    std::set<base_cell> cells;
    for (auto& cell : _data)
        cells.emplace(cell.row, cell.col);
    return cells;
}

bool grid_3d::has(uint32_t row, uint32_t col, int level) const {
    return _data.find(cell_3d{row, col, level}) != _data.end();
}

void grid_3d::move(cell_3d from, cell_3d to) {
    _data.erase(_data.find(from));
    _data.insert(to);
}

const std::set<cell_3d>& grid_3d::get_data() const { return _data; }

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

} // namespace automaton
