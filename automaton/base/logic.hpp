#pragma once

#include <memory>

namespace automaton {

class base_logic {
public:
    virtual ~base_logic() = default;

    virtual void step() = 0;
    virtual void reset() = 0;
};

using base_logic_ptr = std::shared_ptr<base_logic>;

}  // namespace automaton
