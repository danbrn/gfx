#pragma once

#include "gfx.h"

namespace gfx {

class surface {
    SDL_Surface *m_sdl_surface{};
    bool         m_owned;

  public:
    surface(int width, int height)
        : m_sdl_surface(SDL_CreateRGBSurfaceWithFormat(
              0, width, height, bits_per_pixel, SDL_PIXELFORMAT_RGBA32)),
          m_owned{true} {
        if(m_sdl_surface == nullptr) {
            throw std::runtime_error{
                fmt::format("error creating surface: {}", SDL_GetError())};
        }
    }

    explicit surface(SDL_Surface *surface, bool owned = true)
        : m_sdl_surface{surface}, m_owned{owned} {}

    explicit surface(std::string const &file_name)
        : m_sdl_surface{IMG_Load(file_name.c_str())}, m_owned{true} {
        if(m_sdl_surface == nullptr) {
            throw std::runtime_error{
                fmt::format("error loading file: {}", SDL_GetError())};
        }
    }

    surface(surface const &)                     = delete;
    surface(surface &&)                          = default;
    auto operator=(surface const &) -> surface & = delete;
    auto operator=(surface &&) -> surface      & = default;

    ~surface() {
        if(m_owned) {
            SDL_FreeSurface(m_sdl_surface);
        }
    }

    [[nodiscard]] auto get_sdl_surface() const -> SDL_Surface * {
        return m_sdl_surface;
    }

    void blit_onto(std::shared_ptr<surface> const &to, int x = 0, int y = 0) {
        auto    *target = to->get_sdl_surface();
        SDL_Rect rect{x, y, target->w, target->h};
        SDL_BlitSurface(m_sdl_surface, nullptr, target, &rect);
    }
};

} // namespace gfx
