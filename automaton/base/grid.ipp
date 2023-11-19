namespace automaton {

template<class Cell>
void grid<Cell>::add(Cell cell) {
    _cells.emplace(std::move(cell));
}

template<class Cell>
void grid<Cell>::remove(Cell cell) {
    _cells.erase(cell);
}

template<class Cell>
bool grid<Cell>::has(Cell cell) {
    return _cells.find(cell) != _cells.end();
}

template<class Cell>
void grid<Cell>::add(uint32_t row, uint32_t col) {
    add({row, col});
}

template<class Cell>
void grid<Cell>::remove(uint32_t row, uint32_t col) {
    remove({row, col});
}

template<class Cell>
bool grid<Cell>::has(uint32_t row, uint32_t col) {
    return has({row, col});
}

template<class Cell>
void grid<Cell>::clear() {
    _cells.clear();
}

template<class Cell>
std::set<base_cell> grid<Cell>::get_drawable_cells() const {
    return {_cells.begin(), _cells.end()};
}

template<class Cell>
const std::set<Cell>& grid<Cell>::get_cells() const {
    return _cells;
}

}  // namespace automaton
