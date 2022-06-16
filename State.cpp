#include "State.h"
#include <mutex>

static State current_state;
static std::mutex mtx;

void State::swap(State next)
{
    auto lock = std::scoped_lock(mtx);
    current_state = next;
}

State State::copy_current()
{
    auto lock = std::scoped_lock(mtx);
    return current_state;
}
