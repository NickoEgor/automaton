#pragma once

#include "automaton/base/cell.hpp"

#include <memory>
#include <set>
#include <utility>  // TODO: move to src

namespace automaton {

class base_grid {
public:
    base_grid(uint32_t rows, uint32_t cols)
        : _rows(rows),
          _cols(cols) {}
    virtual ~base_grid() = default;

    virtual void add(uint32_t row, uint32_t col) = 0;
    virtual bool remove(uint32_t row, uint32_t col) = 0;
    virtual bool has(uint32_t row, uint32_t col) = 0;

    virtual std::set<base_cell> get_drawable_cells() const = 0;

    /* TODO: do something with size updates in successors to get rid of virtual */
    virtual void set_rows(uint32_t rows) { _rows = rows; }
    virtual void set_cols(uint32_t cols) { _cols = cols; }

    uint32_t get_rows() const { return _rows; }
    uint32_t get_cols() const { return _cols; }

    virtual void clear() = 0;

protected:
    uint32_t _rows, _cols;
};

using base_grid_ptr = std::shared_ptr<base_grid>;

template<class Cell>
class grid : public base_grid {
public:
    using base_grid::base_grid;

    virtual void add(Cell cell);
    virtual bool remove(Cell cell);
    virtual bool has(Cell cell);

    void add(uint32_t row, uint32_t col) override;
    bool remove(uint32_t row, uint32_t col) override;
    bool has(uint32_t row, uint32_t col) override;
    void clear() override;

    std::set<base_cell> get_drawable_cells() const override;
    const std::set<Cell>& get_data() const;

protected:
    std::set<Cell> _data;
};

}  // namespace automaton

#include "automaton/base/grid.ipp"
