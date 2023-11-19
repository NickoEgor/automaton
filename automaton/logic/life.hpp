#pragma once

#include "automaton/base/logic.hpp"
#include "automaton/grid/_2d.hpp"

namespace automaton {
namespace logic {

class life_2d : public base_logic {
public:
    explicit life_2d(base_grid_ptr grid);

    bool step() override;
    void reset() override;

private:
    grid_2d_ptr _grid;
};

}  // namespace logic
}  // namespace automaton
