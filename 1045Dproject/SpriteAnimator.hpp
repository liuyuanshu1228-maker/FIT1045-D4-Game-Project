#ifndef SPRITE_ANIMATOR_HPP
#define SPRITE_ANIMATOR_HPP

#include "splashkit.h"
#include <string>

// Encapsulates a frame-strip sprite sheet and its frame-stepping state.
// Replaces the hand-rolled "timer++ ; if timer >= duration ..." bookkeeping
// that used to be duplicated for every walk/attack/death animation.
class SpriteAnimator
{
public:
    SpriteAnimator();

    void load(const std::string &bitmap_id, const std::string &path,
              int frame_width, int frame_height, int frame_count, int frame_duration);

    // Looping animation (e.g. walking): advances and wraps back to frame 0.
    void step_loop();

    // Plays once and freezes on the last frame (e.g. death). Returns true once finished.
    bool step_once_hold();

    // Plays once and wraps back to frame 0 (e.g. a single sword swing). Returns true on completion.
    bool step_once_reset();

    void reset();
    void draw(double x, double y) const;

    bool is_loaded() const { return _sheet != nullptr; }
    int current_frame() const { return _frame; }
    int frame_count() const { return _frame_count; }

private:
    bitmap _sheet;
    int _frame_width;
    int _frame_height;
    int _frame_count;
    int _frame_duration;
    int _frame;
    int _timer;
};

#endif
