#pragma once

#include "automaton/base/grid.hpp"
#include "automaton/base/logic.hpp"

#include <gtkmm/drawingarea.h>

#include <memory>
#include <utility>
#include <vector>

namespace automaton {

class grid_area : public Gtk::DrawingArea {
public:
    using cairo_context = Cairo::RefPtr<Cairo::Context>;

    grid_area();
    virtual ~grid_area();

    void set_automaton(base_logic_ptr logic, base_grid_ptr grid);
    void set_grid_borders(bool borders);
    void set_step_delay(uint32_t delay);
    void set_editable(bool is_editable);
    void set_cell_width(double cell_width);

    double get_cell_width() const;

private:
    // drawing
    bool on_draw_cells(const cairo_context& cr);
    // keyboard
    bool on_key_press(GdkEventKey* ev);
    // mouse
    bool on_mouse_press(GdkEventButton* ev);
    bool on_mouse_release(GdkEventButton* ev);
    bool on_mouse_motion(GdkEventMotion* ev);
    bool on_mouse_enter(GdkEventCrossing* ev);
    bool on_mouse_leave(GdkEventCrossing* ev);
    // other
    bool on_timeout();

    void enable_motion();
    void disable_motion();
    void toggle_motion();

    void draw_background(const cairo_context& cr);
    void draw_frame(const cairo_context& cr);
    void draw_grid_borders(const cairo_context& cr);
    void draw_grid_cells(const cairo_context& cr);

    double get_grid_width() const;
    double get_grid_height() const;

    base_logic_ptr _logic;
    base_grid_ptr _grid;

    bool _is_editable = true;
    bool _is_drawing = false;
    bool _is_clearing = false;

    bool _is_motion = false;
    uint32_t _delay = 100;
    sigc::connection _motion_connection;

    bool _grid_borders = false;

    const Gdk::Color _bg_color{"#fbfbfb"};
    const Gdk::Color _border_color{"#000000"};
    const Gdk::Color _cell_color{"#008080"};

    double _cell_width = 15;
    const double _line_width = 0.5;
};

}  // namespace automaton
