#include <automaton/logic/life.hpp>

namespace automaton {
namespace logic {

life_2d::life_2d(base_grid_ptr grid) : _grid(std::static_pointer_cast<grid_2d>(grid)) {}

void life_2d::step() {
    size_t rows = _grid->get_rows(), cols = _grid->get_cols();
    std::set<base_cell> new_state;

    for (size_t row = 0; row < rows; row++) {
        for (size_t col = 0; col < cols; col++) {
            size_t prev_row = (row == 0 ? rows - 1 : row - 1);
            size_t next_row = (row == rows - 1 ? 0 : row + 1);

            size_t prev_col = (col == 0 ? cols - 1 : col - 1);
            size_t next_col = (col == cols - 1 ? 0 : col + 1);

            uint8_t neighbours = 0;
            if (_grid->has(prev_row, prev_col)) ++neighbours;
            if (_grid->has(prev_row, col)) ++neighbours;
            if (_grid->has(prev_row, next_col)) ++neighbours;
            if (_grid->has(row, prev_col)) ++neighbours;
            if (_grid->has(row, next_col)) ++neighbours;
            if (_grid->has(next_row, prev_col)) ++neighbours;
            if (_grid->has(next_row, col)) ++neighbours;
            if (_grid->has(next_row, next_col)) ++neighbours;

            if (!_grid->has(row, col) && neighbours == 3) new_state.emplace(row, col);

            if (_grid->has(row, col) && (neighbours == 2 || neighbours == 3)) new_state.emplace(row, col);
        }
    }

    _grid->clear();

    for (const auto& cell : new_state)
        _grid->add(cell.row, cell.col);
}

void life_2d::reset() {}

} // namespace logic
} // namespace automaton
