#pragma once

#include <algorithm>

#include "vec2d.h"

template <typename T> struct rect_t {
    vec2d_t<T> position;
    vec2d_t<T> size;

    constexpr rect_t() = default;
    constexpr rect_t(T x, T y, T w, T h) noexcept : position{x, y}, size{w, h} {}
    constexpr rect_t(vec2d_t<T> p, vec2d_t<T> s) noexcept
        : position{p}, size{s} {}

    auto contains(rect_t const &other) const -> bool {
        return position.x <= other.position.x &&
               position.x + size.x >= other.position.x + other.size.x &&
               position.y <= other.position.y &&
               position.y + size.y >= other.position.y + other.size.y;
    }

    auto overlaps(rect_t const &other) const -> bool {
        return position.x < other.position.x + other.size.x &&
               position.x + size.x > other.position.x &&
               position.y < other.position.y + other.size.y &&
               position.y + size.y > other.position.y;
    }

    void clamp(rect_t const &other) {
        if(!other.contains(*this)) {
            position.x = std::clamp(position.x, other.position.x,
                                    other.position.x + other.size.x - size.x);
            position.y = std::clamp(position.y, other.position.y,
                                    other.position.y + other.size.y - size.y);
        }
    }
};
