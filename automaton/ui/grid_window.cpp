// vim: fdm=marker fdl=0
#include "automaton/ui/grid_window.hpp"
#include "automaton/grid/_1d.hpp"
#include "automaton/grid/_2d.hpp"
#include "automaton/grid/_3d.hpp"
#include "automaton/logic/fall.hpp"
#include "automaton/logic/life.hpp"
#include "automaton/logic/wolfram.hpp"

#include <glibmm/optioncontext.h>

#include <iostream>
#include <memory>

namespace automaton {

grid_window::grid_window()
    : _grid(nullptr) {
    set_title("Automaton");
    set_default_size(800, 600);
    add(_frame);

    _frame.property_margin() = 15;
    _frame.add(_area);
    _frame.property_visible() = true;

    _area.property_visible() = true;

    signal_key_press_event().connect(sigc::mem_fun(*this, &grid_window::on_key_press));
    signal_check_resize().connect(sigc::mem_fun(*this, &grid_window::on_resize));
}

bool grid_window::on_key_press(GdkEventKey* ev) {
    g_debug("grid_window::on_key_press(key='%s')", ev->string);
    if (ev->keyval == GDK_KEY_q) {
        close();
        return true;
    }
    return false;
}

static Glib::OptionGroup add_main_group(cmdline* options) {
    Glib::OptionGroup group("options", "Main options");

    Glib::OptionEntry entry;
    entry.set_short_name('c');
    entry.set_long_name("cols");
    entry.set_arg_description("NUMBER");
    entry.set_description(
        Glib::ustring::compose("Grid columns. Should be >= 0. '0' means fill window. Default: %1", options->cols));
    group.add_entry(entry, options->cols);

    entry = Glib::OptionEntry();
    entry.set_short_name('r');
    entry.set_long_name("rows");
    entry.set_arg_description("NUMBER");
    entry.set_description(
        Glib::ustring::compose("Grid rows. Should be >= 0. '0' means fill window. Default: %1", options->rows));
    group.add_entry(entry, options->rows);

    entry = Glib::OptionEntry();
    entry.set_short_name('b');
    entry.set_long_name("borders");
    entry.set_description("Draw borders between cells in grid");
    group.add_entry(entry, options->borders);

    entry = Glib::OptionEntry();
    entry.set_long_name("delay");
    entry.set_arg_description("NUMBER");
    entry.set_description(Glib::ustring::compose("Sets delay between grid updates in ms. Default: %1", options->delay));
    group.add_entry(entry, options->delay);

    entry = Glib::OptionEntry();
    entry.set_long_name("cell-width");
    entry.set_arg_description("NUMBER");
    entry.set_description(Glib::ustring::compose("Sets cell width in pixels. Default: %1", options->cell_width));
    group.add_entry(entry, options->cell_width);

    entry = Glib::OptionEntry();
    entry.set_short_name('l');
    entry.set_long_name("logic");
    entry.set_arg_description("STRING");
    entry.set_description(
        Glib::ustring::compose("Grid logic. Should be 'wolfram', 'fall' or 'life'. Default: '%1'", options->logic));
    group.add_entry(entry, options->logic);

    return group;
}

static Glib::OptionGroup add_wolfram_group(cmdline::wolfram_opts* options) {
    Glib::OptionGroup group("wolfram", "Wolfram logic options");

    auto entry = Glib::OptionEntry();
    entry.set_long_name("code");
    entry.set_arg_description("NUMBER");
    entry.set_description(Glib::ustring::compose("Wolfram code. Should be from 0 to 255. Default: %1", options->code));
    group.add_entry(entry, options->code);

    return group;
}

static Glib::OptionGroup add_fall_group(cmdline::fall_opts* options) {
    Glib::OptionGroup group("fall", "Fall logic options");

    auto entry = Glib::OptionEntry();
    entry.set_long_name("splices");
    entry.set_arg_description("NUMBER");
    entry.set_description(Glib::ustring::compose(
        "Grid splices amount. Should be >= 0. '0' means unlimited depth. Default: %1", options->splices));
    group.add_entry(entry, options->splices);

    return group;
}

int grid_window::on_cmdline(const app_cmdline_ptr& cmdline, app_ptr& app) {
    g_debug("grid_window::on_cmdline()");
    // NOTE:
    // https://gitlab.gnome.org/GNOME/glibmm/-/blob/glibmm-2-64/examples/options/main.cc

    Glib::OptionContext ctx;

    auto main_group = add_main_group(&_options);
    ctx.add_group(main_group);

    auto wolfram_group = add_wolfram_group(&_options.wolfram);
    ctx.add_group(wolfram_group);

    auto fall_group = add_fall_group(&_options.fall);
    ctx.add_group(fall_group);

    // add GTK options, --help-gtk, etc
    Glib::OptionGroup gtkgroup(gtk_get_option_group(true));
    ctx.add_group(gtkgroup);

    int argc;
    char** argv = cmdline->get_arguments(argc);

    try {
        ctx.parse(argc, argv);
    } catch (Glib::OptionError& e) {
        g_warning("%s", e.what().c_str());
        std::cout << e.what() << std::endl;
        return 1;
    }

    auto invalid_value = [](const Glib::ustring& param) {
        Glib::ustring msg = "Invalid value for param " + param;
        g_warning("%s", msg.c_str());
        std::cout << msg << std::endl;
    };

    auto validation = _options.validate();
    if (!std::get<0>(validation)) {
        invalid_value(std::get<1>(validation));
        return -1;
    }

    app->activate();
    initialize_grid();
    return 0;
}

void grid_window::initialize_grid() {
    uint32_t cols = _options.cols == 0 ? _area.get_width() / _area.get_cell_width() : _options.cols;
    uint32_t rows = _options.rows == 0 ? _area.get_height() / _area.get_cell_width() : _options.rows;

    // initialize grid
    if (_options.logic == "wolfram") {
        g_debug("grid_window::initialize_grid(logic='%s', code=%d)", _options.logic.c_str(), _options.wolfram.code);

        _grid = std::make_shared<automaton::grid_1d>(rows, cols);
        _logic = std::make_shared<logic::wolfram>(_grid, _options.wolfram.code);

    } else if (_options.logic == "fall" && _options.fall.splices != 1) {
        g_debug("grid_window::initialize_grid 3d(logic='%s', splices=%d)", _options.logic.c_str(),
                _options.fall.splices);

        _grid = std::make_shared<automaton::grid_3d>(rows, cols);
        _logic = std::make_shared<logic::fall_3d>(_grid, _options.fall.splices);

    } else {  // "life" or "fall" with splices == 1
        _grid = std::make_shared<automaton::grid_2d>(rows, cols);

        if (_options.logic == "fall") {
            g_debug("grid_window::initialize_grid 2d(logic='%s', splices=%d)", _options.logic.c_str(),
                    _options.fall.splices);
            _logic = std::make_shared<logic::fall_2d>(_grid);
        } else if (_options.logic == "life") {
            g_debug("grid_window::initialize_grid(logic='%s')", _options.logic.c_str());
            _logic = std::make_shared<logic::life_2d>(_grid);
        }
    }

    // setup grid drawing area
    _area.set_automaton(_logic, _grid);
    _area.set_grid_borders(_options.borders);
    _area.set_step_delay(_options.delay);
    _area.set_cell_width(_options.cell_width);
}

void grid_window::on_resize() {
    g_debug("grid_window::on_resize()");
    if (!_grid) return;

    if (_options.cols == 0) _grid->set_cols(_area.get_width() / _area.get_cell_width());
    if (_options.rows == 0) _grid->set_rows(_area.get_height() / _area.get_cell_width());
}

}  // namespace automaton
