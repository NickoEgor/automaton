#pragma once

#include "automaton/base/cell.hpp"
#include "automaton/base/grid.hpp"

#include <memory>
#include <set>

namespace automaton {

struct cell_1d final : public base_cell {
    using base_cell::base_cell;

    bool operator<(const base_cell& other) const;
};

class grid_1d final : public grid<cell_1d> {
public:
    using grid<cell_1d>::grid;
};

using grid_1d_ptr = std::shared_ptr<grid_1d>;

}  // namespace automaton
