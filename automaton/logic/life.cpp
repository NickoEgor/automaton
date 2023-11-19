#include "automaton/logic/life.hpp"

#include <set>

namespace automaton {
namespace logic {

life_2d::life_2d(base_grid_ptr grid)
    : _grid(std::static_pointer_cast<grid_2d>(grid)) {}

bool life_2d::step() {
    uint32_t rows = _grid->get_rows(), cols = _grid->get_cols();
    std::set<cell_2d> new_state;

    for (uint32_t row = 0; row < rows; row++) {
        for (uint32_t col = 0; col < cols; col++) {
            uint32_t prev_row = (row == 0 ? rows - 1 : row - 1);
            uint32_t next_row = (row == rows - 1 ? 0 : row + 1);

            uint32_t prev_col = (col == 0 ? cols - 1 : col - 1);
            uint32_t next_col = (col == cols - 1 ? 0 : col + 1);

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

    /* TODO: probably worth replacing state completely, not one by one */
    for (const auto& cell : new_state)
        _grid->add(cell);

    return true;
}

void life_2d::reset() {}

}  // namespace logic
}  // namespace automaton
