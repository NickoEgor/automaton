#pragma once

#include <cstdint>

namespace automaton {

struct base_cell {
    uint32_t row;
    uint32_t col;

    base_cell(uint32_t row, uint32_t col)
        : row(row),
          col(col) {}
    virtual ~base_cell() = default;

    bool operator<(const base_cell& other) const {
        if (row == other.row) return col < other.col;
        // return backwards to put upper row in std::set first
        return row > other.row;
    }
};

}  // namespace automaton
