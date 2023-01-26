#pragma once

#include <memory>

#include "gfx.h"

namespace gfx {

class window {
    std::string m_title;
    int         m_width;
    int         m_height;
    SDL_Window *m_sdl_window;
    renderer    m_renderer;

  public:
    window(std::string_view title, int width, int height, bool vsync = false,
           uint32_t flags = 0)
        : m_title(title), m_width(width), m_height(height),
          m_sdl_window(SDL_CreateWindow(
              m_title.c_str(), SDL_WINDOWPOS_UNDEFINED, // NOLINT
              SDL_WINDOWPOS_UNDEFINED, m_width,         // NOLINT
              m_height, flags)),
          m_renderer{renderer{m_sdl_window, vsync}} {
        if(m_sdl_window == nullptr) {
            throw std::runtime_error{
                fmt::format("error creating window: {}", SDL_GetError())};
        }
        if(m_renderer.get_sdl_renderer() == nullptr) {
            throw std::runtime_error{
                fmt::format("couldn't create renderer: {}", SDL_GetError())};
        }
    }

    window()                                   = delete;
    window(window const &)                     = delete;
    window(window &&)                          = default;
    auto operator=(window const &) -> window & = delete;
    auto operator=(window &&) -> window      & = default;

    ~window() {
        if(m_sdl_window != nullptr) {
            SDL_DestroyWindow(m_sdl_window);
        }
    }

    [[nodiscard]] auto get_renderer() -> renderer & { return m_renderer; }

    [[nodiscard]] auto get_sdl_window() const -> SDL_Window * {
        return m_sdl_window;
    }
};

} // namespace gfx
