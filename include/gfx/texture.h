#pragma once

#include "gfx.h"

#include "surface.h"

namespace gfx {

class texture {
    SDL_Texture *m_sdl_texture{};

  public:
    texture(SDL_Renderer *renderer, int width, int height)
        : m_sdl_texture(SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32,
                                          SDL_TEXTUREACCESS_TARGET, width,
                                          height)) {
        if(m_sdl_texture == nullptr) {
            throw std::runtime_error{
                fmt::format("couldn't create texture: {}", SDL_GetError())};
        }
        SDL_SetTextureBlendMode(m_sdl_texture, SDL_BLENDMODE_BLEND);
    }

    texture(SDL_Renderer *renderer, surface const &surface)
        : m_sdl_texture{SDL_CreateTextureFromSurface(
              renderer, surface.get_sdl_surface())} {
        if(m_sdl_texture == nullptr) {
            throw std::runtime_error{
                fmt::format("couldn't create texture: {}", SDL_GetError())};
        }
    }

    texture()                = default;
    texture(texture const &) = delete;
    texture(texture &&rhs) noexcept {
        m_sdl_texture     = rhs.m_sdl_texture; // NOLINT
        rhs.m_sdl_texture = nullptr;
    }
    auto operator=(texture const &) -> texture & = delete;
    auto operator=(texture &&rhs) noexcept -> texture & {
        m_sdl_texture     = rhs.m_sdl_texture;
        rhs.m_sdl_texture = nullptr;
        return *this;
    }
    ~texture() { SDL_DestroyTexture(m_sdl_texture); }

    auto size() -> vec2d_t<int> {
        SDL_Rect sdl_rect;
        SDL_QueryTexture(m_sdl_texture, nullptr, nullptr, &sdl_rect.w,
                         &sdl_rect.h);
        return {sdl_rect.w, sdl_rect.h};
    }

    [[nodiscard]] auto get_sdl_texture() const -> SDL_Texture * {
        return m_sdl_texture;
    }
};

} // namespace gfx
