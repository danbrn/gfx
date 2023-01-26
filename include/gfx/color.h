#pragma once

#include <SDL.h>
#include <cstdint>

namespace gfx {

struct color {
    uint8_t r{};
    uint8_t g{};
    uint8_t b{};
    uint8_t a{};

    constexpr static uint8_t max_value  = 0xFF;
    constexpr static uint8_t high_value = 0xBF;
    constexpr static uint8_t mid_value  = 0x7F;
    constexpr static uint8_t low_value  = 0x3F;
    constexpr static uint8_t min_value  = 0x00;

    constexpr static uint8_t opaque      = max_value;
    constexpr static uint8_t transparent = min_value;

    constexpr color() : color(min_value, min_value, min_value, transparent) {}
    constexpr color(uint8_t r, uint8_t g, uint8_t b, uint8_t a = opaque)
        : r{r}, g{g}, b{b}, a{a} {}

    constexpr void set_alpha(uint8_t alpha) { a = alpha; }

    [[nodiscard]] constexpr auto with_alpha(uint8_t alpha) const -> color {
        return {r, g, b, alpha};
    }

    [[nodiscard]] auto get_sdl_color() -> SDL_Color {
        return SDL_Color{r, g, b, a};
    }
};

} // namespace gfx
