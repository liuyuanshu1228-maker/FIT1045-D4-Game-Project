#include "Health.hpp"

Health::Health(int max_value) : _max(max_value), _current(max_value)
{
}

void Health::damage(int amount)
{
    _current -= amount;
    if (_current < 0)
    {
        _current = 0;
    }
}

void Health::reset()
{
    _current = _max;
}
