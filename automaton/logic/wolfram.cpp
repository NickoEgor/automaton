#include "automaton/logic/wolfram.hpp"

namespace automaton {
namespace logic {

wolfram::wolfram(base_grid_ptr grid, int8_t code)
    : _grid(std::static_pointer_cast<grid_1d>(grid)),
      _code(code) {}

void wolfram::step() {
    uint32_t rows = _grid->get_rows(), cols = _grid->get_cols();

    if (_current_step + 1 == rows) return;

    // when starting: fast forward to first row with cells (ignoring last row)
    if (_current_step == 0 && _grid->get_data().size() > 0 && _grid->get_data().begin()->row != rows - 1)
        _current_step = _grid->get_data().begin()->row;

    uint32_t row = _current_step;

    for (uint32_t col = 0; col < cols; col++) {
        uint32_t prev_col = (col == 0 ? cols - 1 : col - 1);
        uint32_t next_col = (col == cols - 1 ? 0 : col + 1);

        int8_t combination = 0;

        if (_grid->has(row, prev_col)) combination |= 0b001;
        if (_grid->has(row, col)) combination |= 0b010;
        if (_grid->has(row, next_col)) combination |= 0b100;

        /* TODO: maybe create override for add(uint32_t, uint32_t) */
        if ((_code >> combination) & 0b1) _grid->add({row + 1, col});
    }

    _current_step++;
}

void wolfram::reset() { _current_step = 0; }

}  // namespace logic
}  // namespace automaton
