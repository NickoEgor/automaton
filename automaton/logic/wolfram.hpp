#pragma once

#include <automaton/base/logic.hpp>
#include <automaton/grid/_1d.hpp>

namespace automaton {
namespace logic {

class wolfram : public base_logic {
public:
    wolfram(base_grid_ptr grid, int8_t code);

    void step() override;
    void reset() override;

private:
    grid_1d_ptr _grid;

    int8_t _code;
    uint32_t _current_step = 0;
};

} // namespace logic
} // namespace automaton
