// vim: fdm=marker fdl=0
#include <glibmm/optioncontext.h>

#include <automaton/grid/_1d.hpp>
#include <automaton/grid/_2d.hpp>
#include <automaton/grid/_3d.hpp>
#include <automaton/logic/fall.hpp>
#include <automaton/logic/life.hpp>
#include <automaton/logic/wolfram.hpp>
#include <automaton/ui/grid_window.hpp>
#include <iostream>
#include <memory>

namespace automaton {

grid_window::grid_window() : _grid(nullptr) {
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

grid_window::~grid_window() {}

bool grid_window::on_key_press(GdkEventKey* ev) {
    if (ev->keyval == GDK_KEY_q) {
        close();
        return true;
    }
    return false;
}

int grid_window::on_cmdline(const app_cmdline_ptr& cmdline, app_ptr& app) {
    // NOTE:
    // https://gitlab.gnome.org/GNOME/glibmm/-/blob/glibmm-2-64/examples/options/main.cc

    Glib::OptionContext ctx;

    // {{{ main
    Glib::OptionGroup group("options", "Main options");

    Glib::OptionEntry entry;
    entry.set_short_name('c');
    entry.set_long_name("cols");
    entry.set_arg_description("NUM");
    entry.set_description("Grid columns. Should be >= 0. Zero means fill window, sets by "
                          "default");
    group.add_entry(entry, _options.cols);

    entry = Glib::OptionEntry();
    entry.set_short_name('r');
    entry.set_long_name("rows");
    entry.set_arg_description("NUM");
    entry.set_description("Grid rows. Should be >= 0. Zero means fill window, sets by default");
    group.add_entry(entry, _options.rows);

    entry = Glib::OptionEntry();
    entry.set_short_name('t');
    entry.set_long_name("type");
    entry.set_arg_description("TYPE");
    entry.set_description("Grid type. Should be in '1D', '2D' or '3D'");
    group.add_entry(entry, _options.type);

    entry = Glib::OptionEntry();
    entry.set_short_name('b');
    entry.set_long_name("borders");
    /* entry.set_flags(Glib::OptionEntry::FLAG_NO_ARG); */
    entry.set_description("Draw borders between cells in grid");
    group.add_entry(entry, _options.borders);

    entry = Glib::OptionEntry();
    entry.set_short_name('d');
    entry.set_long_name("delay");
    entry.set_arg_description("NUM");
    entry.set_description("Sets delay between grid updates in ms");
    group.add_entry(entry, _options.delay);

    entry = Glib::OptionEntry();
    entry.set_long_name("cell-width");
    entry.set_arg_description("NUM");
    entry.set_description("Sets cell width in pixels");
    group.add_entry(entry, _options.cell_width);

    ctx.add_group(group);
    // }}}

    // {{{ 1D options
    Glib::OptionGroup group_1d("1D", "1D options");

    entry = Glib::OptionEntry();
    entry.set_long_name("code");
    entry.set_arg_description("NUM");
    entry.set_description("1D grid wolfram code. Should be from 0 to 255");
    group.add_entry(entry, _options.code_1d);

    ctx.add_group(group_1d);
    // }}}

    // {{{ 2D options
    Glib::OptionGroup group_2d("2D", "2D options");

    entry = Glib::OptionEntry();
    entry.set_long_name("logic");
    entry.set_arg_description("LOGIC");
    entry.set_description("2D grid logic. Should be in 'fall' or 'life'");
    group.add_entry(entry, _options.logic_2d);

    ctx.add_group(group_2d);
    // }}}

    // {{{ 3D options
    Glib::OptionGroup group_3d("3D", "3D options");

    entry = Glib::OptionEntry();
    entry.set_long_name("levels");
    entry.set_arg_description("NUM");
    entry.set_description("3D grid levels amount. Should be >= 0. Zero means unlimited depth "
                          "levels");
    group.add_entry(entry, _options.levels_3d);

    ctx.add_group(group_3d);
    // }}}

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
    size_t cols = _options.cols == 0 ? _area.get_width() / _area.get_cell_width() : _options.cols;
    size_t rows = _options.rows == 0 ? _area.get_height() / _area.get_cell_width() : _options.rows;

    // initialize grid
    if (_options.type == "1D") {
        _grid = std::make_shared<automaton::grid_1d>(rows, cols);
        // initialize logic
        _grid->set_logic(std::make_shared<logic::wolfram>(_options.code_1d));

    } else if (_options.type == "2D") {
        _grid = std::make_shared<automaton::grid_2d>(rows, cols);
        // initialize logic
        if (_options.logic_2d == "fall")
            _grid->set_logic(std::make_shared<logic::fall_2d>());
        else if (_options.logic_2d == "life")
            _grid->set_logic(std::make_shared<logic::life_2d>());

    } else if (_options.type == "3D") {
        _grid = std::make_shared<automaton::grid_3d>(rows, cols);
        // initialize logic
        _grid->set_logic(std::make_shared<logic::fall_3d>(_options.levels_3d));
    }

    // setup grid drawing area
    _area.set_grid(_grid);
    _area.set_grid_borders(_options.borders);
    _area.set_step_delay(_options.delay);
    _area.set_cell_width(_options.cell_width);
}

void grid_window::on_resize() {
    if (!_grid) return;

    if (_options.cols == 0) _grid->set_cols(_area.get_width() / _area.get_cell_width());

    if (_options.rows == 0) _grid->set_rows(_area.get_height() / _area.get_cell_width());
}

} // namespace automaton
