#pragma once

#include "automaton/base/cell.hpp"
#include "automaton/base/grid.hpp"

#include <memory>
#include <set>

namespace automaton {

struct cell_2d final : public base_cell {
    using base_cell::base_cell;

    bool operator<(const cell_2d& other) const;
};

class grid_2d final : public grid<cell_2d> {
public:
    using grid<cell_2d>::grid;

    void set_rows(uint32_t rows) override;
    void set_cols(uint32_t cols) override;

    // custom methods
    void move(cell_2d from, cell_2d to);

private:
    void update_sizes(uint32_t rows, uint32_t cols);
};

using grid_2d_ptr = std::shared_ptr<grid_2d>;

}  // namespace automaton
