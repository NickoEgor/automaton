#pragma once

#include "automaton/base/cell.hpp"
#include "automaton/base/grid.hpp"

#include <memory>
#include <set>

namespace automaton {

struct cell_3d final : public base_cell {
    int slice;

    cell_3d(uint32_t row, uint32_t col, int slice = 0);

    bool operator<(const cell_3d& other) const;
};

class grid_3d final : public grid<cell_3d> {
public:
    using grid<cell_3d>::grid;

    void remove(cell_3d cell) override;

    bool is_plain() const override;

    void set_rows(uint32_t rows) override;
    void set_cols(uint32_t cols) override;

    void move(cell_3d from, cell_3d to);

private:
    void update_sizes(uint32_t rows, uint32_t cols);
};

using grid_3d_ptr = std::shared_ptr<grid_3d>;

}  // namespace automaton
