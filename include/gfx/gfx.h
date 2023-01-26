#pragma once

#include <SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <fmt/core.h>
#include <memory>
#include <random>
#include <vector>

#include "constants.h"
#include "font.h"
#include "renderer.h"
#include "surface.h"
#include "texture.h"
#include "window.h"

namespace gfx {

void show_cursor(bool visible);
auto get_mouse_state(double &x, double &y) -> uint32_t;
auto create_window(std::string_view title, int width = default_window_width,
                   int height = default_window_height, bool vsync = false,
                   uint32_t flags = 0) -> std::shared_ptr<window>;
auto create_surface_from_file(std::string const &file_name)
    -> std::shared_ptr<surface>;
auto create_texture(renderer &r, int w, int h) -> std::shared_ptr<texture>;
auto open_font(std::string const &file_name, int size) -> std::shared_ptr<font>;
auto modifier_key_pressed(uint32_t key) -> bool;

class gfx {
    bool m_initialized{false};

    std::random_device         m_rdev;
    std::default_random_engine m_reng{m_rdev()};

  public:
    gfx() : m_initialized(SDL_Init(SDL_INIT_VIDEO) >= 0) {
        if(!m_initialized) {
            throw std::runtime_error{
                fmt::format("error initializing graphics: {}", SDL_GetError())};
        }
        if(TTF_Init() < 0) {
            throw std::runtime_error{
                fmt::format("error initializing fonts: {}", TTF_GetError())};
        }
    }

    gfx(gfx const &)                     = delete;
    gfx(gfx &&)                          = delete;
    auto operator=(gfx const &) -> gfx & = delete;
    auto operator=(gfx &&) -> gfx      & = delete;
    ~gfx() {
        show_cursor(/*visible=*/true);
        TTF_Quit();
        SDL_Quit();
    }

    [[nodiscard]] auto is_initialized() const -> bool { return m_initialized; }

    auto uniform_random_between(double a, double b) -> double {
        std::uniform_real_distribution<double> dist{a, b};
        return dist(m_reng);
    }

    auto normal_random_between(double a, double b) -> double {
        std::normal_distribution<double> dist{a, b};
        return dist(m_reng);
    }
};

} // namespace gfx
