#include <automaton/logic/fall.hpp>

namespace automaton {
namespace logic {

void fall_2d::step(grid_2d& grid) {
    std::set<base_cell> data_copy = {grid.get_data().begin(),
                                     grid.get_data().end()};

    for (auto it = data_copy.begin(); it != data_copy.end(); it++) {
        if (it->row == grid.get_rows() - 1) continue;

        if (!grid.has(it->row + 1, it->col)) {
            grid.move(*it, {it->row + 1, it->col});
        } else if (it->col > 0 && !grid.has(it->row + 1, it->col - 1)) {
            grid.move(*it, {it->row + 1, it->col - 1});
        } else if (it->col < grid.get_cols() - 1 &&
                   !grid.has(it->row + 1, it->col + 1)) {
            grid.move(*it, {it->row + 1, it->col + 1});
        }
    }
}

fall_3d::fall_3d() {
    std::random_device rd;
    _engine = std::default_random_engine(rd());
    _dist = std::uniform_int_distribution<int16_t>(0, 1);
}

/* TODO: use int16_t direction = _dist(_engine); */
void fall_3d::step(grid_3d& grid) {
    std::set<cell_3d> data_copy = {grid.get_data().begin(),
                                     grid.get_data().end()};

    for (auto it = data_copy.begin(); it != data_copy.end(); it++) {
        if (it->row == grid.get_rows() - 1) continue;

        if (!grid.has(it->row + 1, it->col, it->level)) {
            grid.move(*it, {it->row + 1, it->col});
        } else if (it->col > 0 &&
                   !grid.has(it->row + 1, it->col - 1, it->level)) {
            grid.move(*it, {it->row + 1, it->col - 1});
        } else if (it->col < grid.get_cols() - 1 &&
                   !grid.has(it->row + 1, it->col + 1, it->level)) {
            grid.move(*it, {it->row + 1, it->col + 1});
        } else if (!grid.has(it->row + 1, it->col, it->level - 1)) {
            grid.move(*it, {it->row + 1, it->col, it->level - 1});
        } else if (!grid.has(it->row + 1, it->col, it->level + 1)) {
            grid.move(*it, {it->row + 1, it->col, it->level + 1});
        }
    }
}

}  // namespace logic
}  // namespace automaton