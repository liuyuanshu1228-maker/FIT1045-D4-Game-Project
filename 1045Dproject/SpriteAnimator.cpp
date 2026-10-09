#include "SpriteAnimator.hpp"

SpriteAnimator::SpriteAnimator()
    : _sheet(nullptr), _frame_width(0), _frame_height(0), _frame_count(1),
      _frame_duration(1), _frame(0), _timer(0)
{
}

void SpriteAnimator::load(const std::string &bitmap_id, const std::string &path,
                           int frame_width, int frame_height, int frame_count, int frame_duration)
{
    _sheet = load_bitmap(bitmap_id, path);
    _frame_width = frame_width;
    _frame_height = frame_height;
    _frame_count = frame_count;
    _frame_duration = frame_duration;
    _frame = 0;
    _timer = 0;

    if (_sheet == nullptr)
    {
        write_line("ERROR: Failed to load sprite sheet '" + path + "'");
    }
}

void SpriteAnimator::step_loop()
{
    _timer++;
    if (_timer >= _frame_duration)
    {
        _timer = 0;
        _frame = (_frame + 1) % _frame_count;
    }
}

bool SpriteAnimator::step_once_hold()
{
    if (_frame >= _frame_count - 1)
        return true;

    _timer++;
    if (_timer >= _frame_duration)
    {
        _timer = 0;
        _frame++;
        if (_frame >= _frame_count - 1)
        {
            _frame = _frame_count - 1;
            return true;
        }
    }
    return false;
}

bool SpriteAnimator::step_once_reset()
{
    _timer++;
    if (_timer >= _frame_duration)
    {
        _timer = 0;
        _frame++;
        if (_frame >= _frame_count)
        {
            _frame = 0;
            return true;
        }
    }
    return false;
}

void SpriteAnimator::reset()
{
    _frame = 0;
    _timer = 0;
}

void SpriteAnimator::draw(double x, double y) const
{
    if (_sheet == nullptr)
        return;

    rectangle src_rect = rectangle_from(_frame * _frame_width, 0, _frame_width, _frame_height);
    draw_bitmap(_sheet, x, y, option_part_bmp(src_rect));
}
