#ifndef SCREEN_HPP
#define SCREEN_HPP

// Single source of truth for the window's logical size, so it isn't
// redefined independently in every class that needs to clamp a position
// or check "off screen".
namespace Screen
{
    constexpr int WIDTH = 800;
    constexpr int HEIGHT = 600;
}

#endif
