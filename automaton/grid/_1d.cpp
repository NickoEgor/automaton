#include <automaton/grid/_1d.hpp>

namespace automaton {

cell_1d::cell_1d(size_t row, size_t col) : base_cell(row, col) {}

bool cell_1d::operator<(const base_cell& other) const {
    if (row == other.row) return col < other.col;
    return row < other.row;
}

grid_1d::grid_1d(size_t rows, size_t cols) : base_grid(rows, cols) {}

grid_1d::~grid_1d() {}

void grid_1d::add(size_t row, size_t col) { _data.emplace(row, col); }

bool grid_1d::remove(size_t row, size_t col) {
    auto it = _data.find({row, col});
    if (it == _data.end()) return false;
    _data.erase(it);
    return true;
}

bool grid_1d::has(size_t row, size_t col) { return _data.find({row, col}) != _data.end(); }

void grid_1d::clear() {
    _data.clear();
}

std::set<base_cell> grid_1d::get_drawable_cells() const { return {_data.begin(), _data.end()}; }

const std::set<cell_1d>& grid_1d::get_data() const { return _data; }

} // namespace automaton
