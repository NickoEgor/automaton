#include <gdkmm/general.h>
#include <glibmm/main.h>

#include <automaton/ui/grid_area.hpp>

namespace automaton {

grid_area::grid_area() {
    // catch mouse press/release events
    add_events(Gdk::BUTTON_PRESS_MASK | Gdk::BUTTON_RELEASE_MASK);
    signal_button_press_event().connect(sigc::mem_fun(*this, &grid_area::on_mouse_press));
    signal_button_release_event().connect(sigc::mem_fun(*this, &grid_area::on_mouse_release));
    // catch mouse motion event
    add_events(Gdk::POINTER_MOTION_MASK);
    signal_motion_notify_event().connect(sigc::mem_fun(*this, &grid_area::on_mouse_motion));
    // catch mouse leave/enter events
    add_events(Gdk::ENTER_NOTIFY_MASK | Gdk::LEAVE_NOTIFY_MASK);
    signal_enter_notify_event().connect(sigc::mem_fun(*this, &grid_area::on_mouse_enter));
    signal_leave_notify_event().connect(sigc::mem_fun(*this, &grid_area::on_mouse_leave));

    // catch keyboard events
    add_events(Gdk::KEY_PRESS_MASK);
    property_can_focus() = true;
    signal_key_press_event().connect(sigc::mem_fun(*this, &grid_area::on_key_press));

    // add draw function
    signal_draw().connect(sigc::mem_fun(*this, &grid_area::on_draw_cells));
}

grid_area::~grid_area() {}

void grid_area::set_automaton(base_logic_ptr logic, base_grid_ptr grid) {
    _logic = logic;
    _grid = grid;
}

void grid_area::set_grid_borders(bool borders) { _grid_borders = borders; }

void grid_area::set_step_delay(size_t delay) { _delay = delay; }

void grid_area::set_editable(bool is_editable) { _is_editable = is_editable; }

void grid_area::set_cell_width(double cell_width) { _cell_width = cell_width; }

bool grid_area::on_draw_cells(const cairo_context& cr) {
    draw_background(cr);
    draw_frame(cr);

    if (_grid) {
        if (_grid_borders) draw_grid_borders(cr);
        draw_grid_cells(cr);
    }

    return false;
}

bool grid_area::on_key_press(GdkEventKey* ev) {
    if (!_grid) return false;

    if (ev->keyval == GDK_KEY_s) {
        _logic->step();
        queue_draw();
        return true;
    } else if (ev->keyval == GDK_KEY_space) {
        toggle_ongoing();
        return true;
    } else if (ev->keyval == GDK_KEY_c) {
        _grid->clear();
        queue_draw();
        return true;
    }

    return false;
}

bool grid_area::on_mouse_press(GdkEventButton* ev) {
    g_debug("on_mouse_press()");
    if (!_grid) return false;
    if (!_is_editable) return false;

    if (ev->type == GDK_BUTTON_PRESS && (ev->button == 1 || ev->button == 3)) {
        size_t col = ev->x / _cell_width;
        size_t row = ev->y / _cell_width;
        if (row >= _grid->get_rows() || col >= _grid->get_cols()) return false;

        if (ev->button == 1) {
            _grid->add(row, col);
            _is_drawing = true;
        } else if (ev->button == 3) {
            _grid->remove(row, col);
            _is_clearing = true;
        }

        queue_draw();
        return true;
    }

    return false;
}

bool grid_area::on_mouse_release(GdkEventButton* ev) {
    g_debug("on_mouse_release()");
    if (!_grid) return false;
    if (!_is_editable) return false;

    if (ev->button == 1 && _is_drawing) {
        _is_drawing = false;
        return true;
    } else if (ev->button == 3 && _is_clearing) {
        _is_clearing = false;
        return true;
    }

    return false;
}

bool grid_area::on_mouse_motion(GdkEventMotion* ev) {
    if (!_grid) return false;
    if (!_is_editable) return false;
    if (!_is_drawing && !_is_clearing) return false;

    if (_is_drawing || _is_clearing) {
        size_t col = ev->x / _cell_width;
        size_t row = ev->y / _cell_width;
        if (row >= _grid->get_rows() || col >= _grid->get_cols()) return false;

        if (_is_drawing)
            _grid->add(row, col);
        else if (_is_clearing)
            _grid->remove(row, col);

        queue_draw();
        return true;
    }

    return false;
}

bool grid_area::on_mouse_enter(GdkEventCrossing* ev) {
    g_debug("on_mouse_enter()");
    (void)ev;
    return false;
}

bool grid_area::on_mouse_leave(GdkEventCrossing* ev) {
    g_debug("on_mouse_leave()");
    (void)ev;
    return false;
}

bool grid_area::on_timeout() {
    g_debug("on_timeout(): _is_drawing %d, _is_clearing %d", _is_drawing, _is_clearing);

    if (!_grid) return false;

    if (_is_drawing || _is_clearing) {
        int x, y;
        get_pointer(x, y);

        size_t col = x / _cell_width;
        size_t row = y / _cell_width;

        // NOTE: draw/clear only when mouse on grid
        if (row < _grid->get_rows() && col < _grid->get_cols()) {
            if (_is_drawing)
                _grid->add(row, col);
            else if (_is_clearing)
                _grid->remove(row, col);
        }
    }

    _logic->step();
    queue_draw();
    return true;
}

void grid_area::toggle_ongoing() {
    g_debug("toggle_ongoing(): _is_ongoing %d", _is_ongoing);
    if (!_is_ongoing) {
        // ongoing timeout
        sigc::slot<bool()> _ongoing_slot = sigc::mem_fun(*this, &grid_area::on_timeout);

        // ongoing connection
        _ongoing_connection = Glib::signal_timeout().connect(_ongoing_slot, _delay);
        _is_ongoing = true;
    } else {
        _ongoing_connection.disconnect();
        _is_ongoing = false;
    }
    g_debug("toggle_ongoing(): new _is_ongoing %d", _is_ongoing);
}

void grid_area::draw_background(const cairo_context& cr) {
    cr->save();
    Gdk::Cairo::set_source_color(cr, _bg_color);
    cr->paint();
    cr->restore();
}

void grid_area::draw_frame(const cairo_context& cr) {
    cr->save();
    Gdk::Cairo::set_source_color(cr, _border_color);
    cr->set_line_width(_line_width);

    cr->rectangle(0, 0, get_grid_width(), get_grid_height());
    cr->stroke();

    cr->restore();
}

void grid_area::draw_grid_borders(const cairo_context& cr) {
    cr->save();
    Gdk::Cairo::set_source_color(cr, _border_color);
    cr->set_line_width(_line_width);

    double x = _cell_width, y = _cell_width;
    size_t width = get_grid_width(), height = get_grid_height();

    for (size_t col = 0; col < _grid->get_cols() - 1; col++) {
        cr->move_to(x, 0);
        cr->line_to(x, height);
        x += _cell_width;
    }

    for (size_t row = 0; row < _grid->get_rows() - 1; row++) {
        cr->move_to(0, y);
        cr->line_to(width, y);
        y += _cell_width;
    }

    cr->stroke();
    cr->restore();
}

void grid_area::draw_grid_cells(const cairo_context& cr) {
    cr->save();
    Gdk::Cairo::set_source_color(cr, _cell_color);

    for (auto& cell : _grid->get_drawable_cells()) {
        auto xy = get_cell_xy(cell.row, cell.col);
        cr->rectangle(xy.first, xy.second, _cell_width, _cell_width);
        cr->fill();
    }

    cr->restore();
}

double grid_area::get_cell_width() const { return _cell_width; }

double grid_area::get_grid_width() const {
    if (!_grid) return 0;

    return _cell_width * _grid->get_cols();
}

double grid_area::get_grid_height() const {
    if (!_grid) return 0;

    return _cell_width * _grid->get_rows();
}

std::pair<double, double> grid_area::get_cell_xy(size_t row, size_t col) const {
    if (!_grid) return {0, 0};

    return std::make_pair(_cell_width * col, _cell_width * row);
}

} // namespace automaton
