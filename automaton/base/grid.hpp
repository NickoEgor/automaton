#pragma once

#include <automaton/base/cell.hpp>
#include <memory>
#include <set>

namespace automaton {

class base_grid {
public:
    base_grid(uint32_t rows, uint32_t cols) : _rows(rows), _cols(cols) {}
    virtual ~base_grid() {}

    virtual void add(uint32_t row, uint32_t col) = 0;
    virtual bool remove(uint32_t row, uint32_t col) = 0;
    virtual bool has(uint32_t row, uint32_t col) = 0;
    virtual void clear() = 0;

    virtual std::set<base_cell> get_drawable_cells() const = 0;

    virtual void set_rows(uint32_t rows) { _rows = rows; }
    virtual void set_cols(uint32_t cols) { _cols = cols; }

    uint32_t get_rows() const { return _rows; }
    uint32_t get_cols() const { return _cols; }

protected:
    uint32_t _rows, _cols;
};

using base_grid_ptr = std::shared_ptr<base_grid>;

} // namespace automaton
