#pragma once

#include <automaton/base/cell.hpp>
#include <automaton/base/grid.hpp>
#include <memory>
#include <set>
#include <vector>

namespace automaton {

class grid_2d : public base_grid {
public:
    grid_2d(uint32_t rows, uint32_t cols);
    ~grid_2d() override;

    void add(uint32_t row, uint32_t col) override;
    bool remove(uint32_t row, uint32_t col) override;
    bool has(uint32_t row, uint32_t col) override;
    void clear() override;

    std::set<base_cell> get_drawable_cells() const override;

    void set_rows(uint32_t rows) override;
    void set_cols(uint32_t cols) override;

    // custom methods
    void move(base_cell from, base_cell to);
    const std::set<base_cell>& get_data() const;

private:
    std::set<base_cell> _data;

    void update_sizes(uint32_t rows, uint32_t cols);
};

using grid_2d_ptr = std::shared_ptr<grid_2d>;

} // namespace automaton
