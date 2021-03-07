#pragma once

#include <automaton/base/cell.hpp>
#include <automaton/base/grid.hpp>
#include <memory>
#include <set>
#include <vector>

namespace automaton {

struct cell_3d : public base_cell {
    int level;

    cell_3d(uint32_t row, uint32_t col, int level = 0);
    bool operator<(const cell_3d& other) const;
};

class grid_3d : public base_grid {
public:
    grid_3d(uint32_t rows, uint32_t cols);
    ~grid_3d() override;

    void add(uint32_t row, uint32_t col) override;
    bool remove(uint32_t row, uint32_t col) override;
    bool has(uint32_t row, uint32_t col) override;
    void clear() override;

    std::set<base_cell> get_drawable_cells() const override;

    void set_rows(uint32_t rows) override;
    void set_cols(uint32_t cols) override;

    // custom methods
    bool has(uint32_t row, uint32_t col, int level) const;
    void move(cell_3d from, cell_3d to);
    const std::set<cell_3d>& get_data() const;

private:
    std::set<cell_3d> _data;

    void update_sizes(uint32_t rows, uint32_t cols);
};

using grid_3d_ptr = std::shared_ptr<grid_3d>;

} // namespace automaton
