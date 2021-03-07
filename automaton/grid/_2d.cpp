#include <automaton/grid/_2d.hpp>

namespace automaton {

grid_2d::grid_2d(size_t rows, size_t cols) : base_grid(rows, cols) {}

grid_2d::~grid_2d() {}

void grid_2d::add(size_t row, size_t col) { _data.emplace(row, col); }

bool grid_2d::has(size_t row, size_t col) { return _data.find({row, col}) != _data.end(); }

bool grid_2d::remove(size_t row, size_t col) {
    auto it = _data.find({row, col});
    if (it != _data.end()) {
        _data.erase(it);
        return true;
    }

    return false;
}

void grid_2d::clear() { _data.clear(); }

std::set<base_cell> grid_2d::get_drawable_cells() const { return {_data.begin(), _data.end()}; }

void grid_2d::move(base_cell from, base_cell to) {
    auto it = _data.find(from);
    if (it != _data.end()) _data.erase(it);

    _data.insert(to);
}

const std::set<base_cell>& grid_2d::get_data() const { return _data; }

void grid_2d::set_rows(size_t rows) {
    base_grid::set_rows(rows);
    update_sizes(rows, _cols);
}

void grid_2d::set_cols(size_t cols) {
    base_grid::set_cols(cols);
    update_sizes(_rows, cols);
}

void grid_2d::update_sizes(size_t rows, size_t cols) {
    for (auto it = _data.begin(); it != _data.end();) {
        if (it->row > rows - 1 || it->col > cols - 1)
            it = _data.erase(it);
        else
            it++;
    }
}

} // namespace automaton
