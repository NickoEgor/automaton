#include "automaton/logic/fall.hpp"

#include <set>
#include <vector>

namespace automaton {
namespace logic {

fall_2d::fall_2d(base_grid_ptr grid)
    : _grid(std::static_pointer_cast<grid_2d>(grid)),
      _engine(std::default_random_engine(std::random_device{}())),
      _dist(std::uniform_int_distribution<int16_t>(1, 2)) {}

void fall_2d::step() {
    std::set<cell_2d> data_copy = _grid->get_data();

    for (auto it = data_copy.begin(); it != data_copy.end(); it++) {
        if (it->row == _grid->get_rows() - 1) continue;

        if (!_grid->has(it->row + 1, it->col)) {
            _grid->move(*it, {it->row + 1, it->col});
            continue;
        }

        std::vector<cell_2d> positions;

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

fall_3d::fall_3d(base_grid_ptr grid, uint16_t slices)
    : _grid(std::static_pointer_cast<grid_3d>(grid)),
      _slices(slices),
      _engine(std::default_random_engine(std::random_device{}())),
      _dist(std::uniform_int_distribution<int16_t>(1, 12)) {}

void fall_3d::step() {
    std::set<cell_3d> data_copy = _grid->get_data();

    for (auto it = data_copy.begin(); it != data_copy.end(); it++) {
        if (it->row == _grid->get_rows() - 1) continue;

        cell_3d cell_below = {it->row + 1, it->col, it->slice};
        if (!_grid->has(cell_below)) {
            _grid->move(*it, cell_below);
            continue;
        }

        std::vector<cell_3d> positions;

        cell_3d cell_left{it->row + 1, it->col - 1, it->slice};
        if (it->col > 0 && !_grid->has(cell_left)) {
            positions.emplace_back(cell_left);
        }

        cell_3d cell_right{it->row + 1, it->col + 1, it->slice};
        if (it->col < _grid->get_cols() - 1 && !_grid->has(cell_right)) {
            positions.emplace_back(cell_right);
        }

        if (_slices == 0) {
            cell_3d cell_back{it->row + 1, it->col, it->slice - 1};
            if (!_grid->has(cell_back)) {
                positions.emplace_back(cell_back);
            }

            cell_3d cell_front{it->row + 1, it->col, it->slice + 1};
            if (!_grid->has(cell_front)) {
                positions.emplace_back(cell_front);
            }
        } else {
            cell_3d cell_front{it->row + 1, it->col, it->slice + 1};
            if (it->slice < _slices && !_grid->has(cell_front)) {
                positions.emplace_back(cell_front);
            }

            if (it->slice > -_slices && !_grid->has({it->row + 1, it->col, it->slice})) {
                positions.emplace_back(it->row + 1, it->col, it->slice - 1);
            }
        }

        if (positions.size() == 0) continue;

        int8_t direction = _dist(_engine) % positions.size();
        auto position = positions.at(direction);
        _grid->move(*it, position);
    }
}

void fall_3d::reset() {}

}  // namespace logic
}  // namespace automaton
