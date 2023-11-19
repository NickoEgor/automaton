#include "automaton/utils/cmdline.hpp"

namespace automaton {

std::tuple<bool, std::string> cmdline::validate() const {
    // main
    if (!validate_cols()) return {false, "cols"};
    if (!validate_rows()) return {false, "rows"};
    if (!validate_type()) return {false, "type"};
    if (!validate_delay()) return {false, "delay"};
    if (!validate_cell_width()) return {false, "cell-width"};
    // 1D
    if (!validate_code_1d()) return {false, "code"};
    // 2D
    if (!validate_logic_2d()) return {false, "logic"};
    // 3D
    if (!validate_levels_3d()) return {false, "levels"};

    return {true, ""};
}

// main
bool cmdline::validate_cols() const { return cols >= 0; }
bool cmdline::validate_rows() const { return rows >= 0; }
bool cmdline::validate_type() const { return type == "1D" || type == "2D" || type == "3D"; }
bool cmdline::validate_delay() const { return delay > 0; }
bool cmdline::validate_cell_width() const { return cell_width > 0; }

// 1D
bool cmdline::validate_code_1d() const { return code_1d >= 0 && code_1d <= 255; }

// 2D
bool cmdline::validate_logic_2d() const { return logic_2d == "fall" || logic_2d == "life"; }

// 3D
bool cmdline::validate_levels_3d() const { return levels_3d >= 0; }

}  // namespace automaton
