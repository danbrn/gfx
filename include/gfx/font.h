#pragma once

#include <SDL2/SDL_ttf.h>
#include <fmt/core.h>
#include <string>

namespace gfx {

class font {
    TTF_Font *m_font;

  public:
    font(font const &)                     = delete;
    font(font &&)                          = default;
    auto operator=(font const &) -> font & = delete;
    auto operator=(font &&) -> font      & = default;

    font(std::string const &file_name, int size)
        : m_font(TTF_OpenFont(file_name.c_str(), size)) {
        if(m_font == nullptr) {
            throw std::runtime_error{
                fmt::format("error opening font: {}", TTF_GetError())};
        }
    }

    auto get_ttf_font() -> TTF_Font * { return m_font; }

    ~font() { TTF_CloseFont(m_font); }
};

} // namespace gfx
