#pragma once

#include <automaton/base/cell.hpp>
#include <automaton/base/grid.hpp>
#include <memory>
#include <set>

namespace automaton {

struct cell_1d : public base_cell {
    cell_1d(size_t row, size_t col);
    bool operator<(const base_cell& other) const override;
};

class grid_1d : public base_grid {
public:
    grid_1d(size_t rows, size_t cols);
    ~grid_1d() override;

    void add(size_t row, size_t col) override;
    bool remove(size_t row, size_t col) override;
    bool has(size_t row, size_t col) override;
    void clear() override;

    std::set<base_cell> get_drawable_cells() const override;

    // custom methods
    const std::set<cell_1d>& get_data() const;

private:
    std::set<cell_1d> _data;
};

using grid_1d_ptr = std::shared_ptr<grid_1d>;

} // namespace automaton
