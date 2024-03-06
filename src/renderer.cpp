#include "gfx/gfx.h"
#include "rect.h"

namespace gfx {

[[nodiscard]] auto world_to_window(vec2d_t<double> world_position,
                                   rect_t<double> view, double window_width)
    -> vec2d_t<double> {
    double zoom = window_width / view.size.x;
    return (world_position - view.position) * zoom;
}

[[nodiscard]] auto window_to_world(vec2d_t<double> window_position,
                                   rect_t<double> view, double window_width)
    -> vec2d_t<double> {
    double zoom = window_width / view.size.x;
    return window_position / zoom + view.position;
}

} // namespace gfx
