#pragma once

#include <glibmm/ustring.h>

#include <string>
#include <tuple>

namespace automaton {

struct cmdline {
    // main
    int cols = 0;  // 0 - means fill widget width
    int rows = 0;  // 0 - means fill widget height
    bool borders = false;
    int delay = 100;
    double cell_width = 15;

    // wolfram, fall, life
    Glib::ustring logic = "fall";

    struct wolfram_opts {
        int code = 22;
    } wolfram;

    struct fall_opts {
        int splices = 1;  // 0 - means unlimited splices
    } fall;

    // returns false with failed options, otherwise true
    std::tuple<bool, std::string> validate() const;

private:
    bool validate_cols() const;
    bool validate_rows() const;
    bool validate_delay() const;
    bool validate_cell_width() const;
    bool validate_dimensions() const;

    bool validate_logic() const;
    bool validate_wolfram() const;
    bool validate_fall() const;
};

}  // namespace automaton
