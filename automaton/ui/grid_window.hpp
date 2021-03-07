#pragma once

#include <gtkmm/applicationwindow.h>
#include <gtkmm/frame.h>

#include <automaton/base/grid.hpp>
#include <automaton/ui/grid_area.hpp>
#include <automaton/utils/cmdline.hpp>
#include <memory>

namespace automaton {

class grid_window : public Gtk::ApplicationWindow {
public:
    using app_cmdline_ptr = Glib::RefPtr<Gio::ApplicationCommandLine>;
    using app_ptr = Glib::RefPtr<Gtk::Application>;

    grid_window();
    ~grid_window() override;

private:
    // GUI members
    Gtk::Frame _frame;
    grid_area _area;

    std::shared_ptr<base_grid> _grid;
    cmdline _options;

    // signals
    bool on_key_press(GdkEventKey* ev);
    void on_resize();

    // methods
    void initialize_grid();

public:
    int on_cmdline(const app_cmdline_ptr& cmdline, app_ptr& app);
};

} // namespace automaton
