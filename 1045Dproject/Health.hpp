#ifndef HEALTH_HPP
#define HEALTH_HPP

// A current/max counter that depletes towards zero and never goes negative.
// Used by composition inside both Player (hearts) and Enemy (hit points) so
// the "take damage, clamp at zero" rule exists in exactly one place instead
// of being reimplemented slightly differently by each class.
class Health
{
public:
    explicit Health(int max_value);

    void damage(int amount);
    void reset();

    int current() const { return _current; }
    int max() const { return _max; }
    bool is_depleted() const { return _current <= 0; }

private:
    int _max;
    int _current;
};

#endif
