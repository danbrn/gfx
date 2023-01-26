#include "gfx/gfx.h"
#include "vec2d.h"

void gfx::show_cursor(bool visible) {
    SDL_ShowCursor(visible ? SDL_ENABLE : SDL_DISABLE);
}

auto gfx::get_mouse_state(double &x, double &y) -> uint32_t {
    int      ix{};
    int      iy{};
    uint32_t ret = SDL_GetMouseState(&ix, &iy);
    x            = ix;
    y            = iy;
    return ret;
}

[[nodiscard]] auto gfx::create_window(std::string_view title, int width,
                                      int height, bool vsync, uint32_t flags)
    -> std::shared_ptr<window> {
    if(flags == 0) {
        flags = SDL_WINDOW_SHOWN;
    }
    return std::make_shared<window>(title, width, height, vsync, flags);
}

[[nodiscard]] auto gfx::create_surface_from_file(std::string const &file_name)
    -> std::shared_ptr<surface> {
    return std::make_shared<surface>(file_name);
}

[[nodiscard]] auto gfx::create_texture(renderer &r, int w, int h)
    -> std::shared_ptr<texture> {
    return std::make_shared<texture>(r.get_sdl_renderer(), w, h);
}

[[nodiscard]] auto gfx::open_font(std::string const &file_name, int size)
    -> std::shared_ptr<font> {
    return std::make_shared<font>(file_name, size);
}

[[nodiscard]] auto gfx::modifier_key_pressed(uint32_t key) -> bool {
    return (SDL_GetModState() & key) != 0;
}
