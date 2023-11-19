#include "automaton/base/grid.hpp"

namespace automaton {

template<class Cell>
void grid<Cell>::add(Cell cell) {
    _data.emplace(std::move(cell));
}

template<class Cell>
bool grid<Cell>::remove(Cell cell) {
    auto it = _data.find(cell);
    if (it == _data.end()) return false;
    _data.erase(it);
    return true;
}

template<class Cell>
bool grid<Cell>::has(Cell cell) {
    return _data.find(cell) != _data.end();
}

template<class Cell>
void grid<Cell>::add(uint32_t row, uint32_t col) {
    add({row, col});
}

template<class Cell>
bool grid<Cell>::remove(uint32_t row, uint32_t col) {
    return remove({row, col});
}

template<class Cell>
bool grid<Cell>::has(uint32_t row, uint32_t col) {
    return has({row, col});
}

template<class Cell>
void grid<Cell>::clear() {
    _data.clear();
}

template<class Cell>
std::set<base_cell> grid<Cell>::get_drawable_cells() const {
    return {_data.begin(), _data.end()};
}

template<class Cell>
const std::set<Cell>& grid<Cell>::get_data() const {
    return _data;
}

}  // namespace automaton
