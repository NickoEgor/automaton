#pragma once

#include <glibmm/ustring.h>

#include <string>
#include <tuple>

namespace automaton {

struct cmdline {
    // main
    int cols = 0;  // 0 - means fill widget width
    int rows = 0;  // 0 - means fill widget height
    Glib::ustring type = "2D";
    bool borders = false;
    int delay = 100;
    double cell_width = 15;
    // 1D
    int code_1d = 22;
    // 2D
    Glib::ustring logic_2d = "fall";
    // 3D
    int levels_3d = 0;  // 0 - means unlimited levels

    std::tuple<bool, std::string> validate() const;

private:
    // main
    bool validate_cols() const;
    bool validate_rows() const;
    bool validate_type() const;
    bool validate_delay() const;
    bool validate_cell_width() const;
    // 1D
    bool validate_code_1d() const;
    // 2D
    bool validate_logic_2d() const;
    // 3D
    bool validate_levels_3d() const;
};

}  // namespace automaton
