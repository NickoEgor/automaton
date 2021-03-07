#include <automaton/logic/fall.hpp>

namespace automaton {
namespace logic {

fall_2d::fall_2d(base_grid_ptr grid) : _grid(std::static_pointer_cast<grid_2d>(grid)) {
    std::random_device rd;
    _engine = std::default_random_engine(rd());
    _dist = std::uniform_int_distribution<int16_t>(1, 2);
}

void fall_2d::step() {
    std::set<base_cell> data_copy = _grid->get_data();

    for (auto it = data_copy.begin(); it != data_copy.end(); it++) {
        if (it->row == _grid->get_rows() - 1) continue;

        if (!_grid->has(it->row + 1, it->col)) {
            _grid->move(*it, {it->row + 1, it->col});
            continue;
        }

        std::vector<base_cell> positions;

        if (it->col > 0 && !_grid->has(it->row + 1, it->col - 1)) {
            positions.emplace_back(it->row + 1, it->col - 1);
        }

        if (it->col < _grid->get_cols() - 1 && !_grid->has(it->row + 1, it->col + 1)) {
            positions.emplace_back(it->row + 1, it->col + 1);
        }

        if (positions.size() == 0) continue;

        int8_t direction = _dist(_engine) % positions.size();
        auto position = positions.at(direction);
        _grid->move(*it, position);
    }
}

void fall_2d::reset() {}

fall_3d::fall_3d(base_grid_ptr grid, uint16_t levels)
    : _grid(std::static_pointer_cast<grid_3d>(grid)), _levels(levels) {
    std::random_device rd;
    _engine = std::default_random_engine(rd());
    _dist = std::uniform_int_distribution<int16_t>(1, 12);
}

void fall_3d::step() {
    std::set<cell_3d> data_copy = _grid->get_data();

    for (auto it = data_copy.begin(); it != data_copy.end(); it++) {
        if (it->row == _grid->get_rows() - 1) continue;

        if (!_grid->has(it->row + 1, it->col, it->level)) {
            _grid->move(*it, {it->row + 1, it->col, it->level});
            continue;
        }

        std::vector<cell_3d> positions;

        if (it->col > 0 && !_grid->has(it->row + 1, it->col - 1, it->level)) {
            positions.emplace_back(it->row + 1, it->col - 1, it->level);
        }

        if (it->col < _grid->get_cols() - 1 && !_grid->has(it->row + 1, it->col + 1, it->level)) {
            positions.emplace_back(it->row + 1, it->col + 1, it->level);
        }

        if (_levels == 0) {
            if (!_grid->has(it->row + 1, it->col, it->level - 1)) {
                positions.emplace_back(it->row + 1, it->col, it->level - 1);
            }

            if (!_grid->has(it->row + 1, it->col, it->level + 1)) {
                positions.emplace_back(it->row + 1, it->col, it->level + 1);
            }
        } else {
            if (it->level < _levels && !_grid->has(it->row + 1, it->col, it->level + 1)) {
                positions.emplace_back(it->row + 1, it->col, it->level + 1);
            }

            if (it->level > -_levels && !_grid->has(it->row + 1, it->col, it->level)) {
                positions.emplace_back(it->row + 1, it->col, it->level - 1);
            }
        }

        if (positions.size() == 0) continue;

        int8_t direction = _dist(_engine) % positions.size();
        auto position = positions.at(direction);
        _grid->move(*it, position);
    }
}

void fall_3d::reset() {}

} // namespace logic
} // namespace automaton
