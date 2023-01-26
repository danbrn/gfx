#pragma once

#include "color.h"

namespace gfx {

constexpr int default_window_width  = 1024;
constexpr int default_window_height = 768;

constexpr int bits_per_pixel = 32;

constexpr static color color_clear{};
constexpr static color color_red{color::max_value, color::min_value,
                                 color::min_value, color::opaque};
constexpr static color color_green{color::min_value, color::max_value,
                                   color::min_value, color::opaque};
constexpr static color color_blue{color::min_value, color::min_value,
                                  color::max_value, color::opaque};
constexpr static color color_cyan{color::min_value, color::max_value,
                                  color::max_value, color::opaque};
constexpr static color color_magenta{color::max_value, color::min_value,
                                     color::max_value, color::opaque};
constexpr static color color_yellow{color::max_value, color::max_value,
                                    color::min_value, color::opaque};
constexpr static color color_white{color::max_value, color::max_value,
                                   color::max_value, color::opaque};
constexpr static color color_light_grey{color::high_value, color::high_value,
                                        color::high_value, color::opaque};
constexpr static color color_mid_grey{color::mid_value, color::mid_value,
                                      color::mid_value, color::opaque};
constexpr static color color_dark_grey{color::low_value, color::low_value,
                                       color::low_value, color::opaque};
constexpr static color color_black = color_clear.with_alpha(color::opaque);

} // namespace gfx
