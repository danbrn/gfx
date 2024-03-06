#pragma once

#include <span>

#include <SDL.h>

#include "color.h"
#include "rect.h"
#include "texture.h"
#include "vec2d.h"

namespace gfx {

template <typename T> auto vec_to_point(vec2d_t<T> const &vec) -> SDL_Point {
    return {static_cast<int>(vec.x), static_cast<int>(vec.y)};
}

template <typename T> auto point_to_vec(SDL_Point *point) -> vec2d_t<T> {
    return {static_cast<T>(point->x), static_cast<T>(point->y)};
}

[[nodiscard]] auto world_to_window(vec2d_t<double> world_position,
                                   rect_t<double> view, double window_width)
    -> vec2d_t<double>;

[[nodiscard]] auto window_to_world(vec2d_t<double> window_position,
                                   rect_t<double> view, double window_width)
    -> vec2d_t<double>;

class renderer {
    SDL_Renderer *m_sdl_renderer{nullptr};
    vec2d_t<int>  m_window_size;

  public:
    explicit renderer(SDL_Window *win, bool vsync = false)
        : m_sdl_renderer{
              SDL_CreateRenderer(win, -1,
                                 SDL_RENDERER_ACCELERATED | // NOLINT
                                     (vsync ? SDL_RENDERER_PRESENTVSYNC : 0))} {
        if(m_sdl_renderer == nullptr) {
            throw std::runtime_error{
                fmt::format("couldn't create renderer: {}", SDL_GetError())};
        }
        if(SDL_SetRenderDrawBlendMode(m_sdl_renderer, SDL_BLENDMODE_BLEND) <
           0) {
            throw std::runtime_error{
                fmt::format("couldn't set blend mode: {}", SDL_GetError())};
        }
        SDL_GetWindowSize(win, &m_window_size.x, &m_window_size.y);
    }

    renderer(renderer const &)                     = delete;
    renderer(renderer &&)                          = default;
    auto operator=(renderer const &) -> renderer & = delete;
    auto operator=(renderer &&) -> renderer      & = default;
    ~renderer() { SDL_DestroyRenderer(m_sdl_renderer); }

    void clear() { clear(color_clear); }

    void clear(color c) {
        set_draw_color(c);
        SDL_RenderClear(m_sdl_renderer);
    }

    void present() { SDL_RenderPresent(m_sdl_renderer); }

    [[nodiscard]] auto get_draw_color() const -> color {
        color c;
        if(SDL_GetRenderDrawColor(m_sdl_renderer, &c.r, &c.g, &c.b, &c.a) < 0) {
            throw std::runtime_error{
                fmt::format("couldn't get color: {}", SDL_GetError())};
        }
        return c;
    }

    void set_draw_color(SDL_Color const &c) {
        set_draw_color(c.r, c.g, c.b, c.a);
    }

    void set_draw_color(color const &c) { set_draw_color(c.r, c.g, c.b, c.a); }

    void set_draw_color(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
        if(SDL_SetRenderDrawColor(m_sdl_renderer, r, g, b, a) < 0) {
            throw std::runtime_error{
                fmt::format("couldn't set color: {}", SDL_GetError())};
        }
    }

    void set_target(texture &t) {
        SDL_SetRenderTarget(m_sdl_renderer, t.get_sdl_texture());
    }

    void reset_target() { SDL_SetRenderTarget(m_sdl_renderer, nullptr); }

    template <typename T> void draw_point(vec2d_t<T> point) {
        if(SDL_RenderDrawPoint(m_sdl_renderer, point.x, point.y) < 0) {
            throw std::runtime_error{
                fmt::format("couldn't draw point: {}", SDL_GetError())};
        }
    }

    template <typename T> void draw_line(vec2d_t<T> from, vec2d_t<T> to) {
        if(SDL_RenderDrawLine(m_sdl_renderer, from.x, from.y, to.x, to.y) < 0) {
            throw std::runtime_error{
                fmt::format("couldn't draw line: {}", SDL_GetError())};
        }
    }

    void draw_lines(std::span<SDL_Point> points) {
        SDL_RenderDrawLines(m_sdl_renderer, points.data(),
                            static_cast<int>(points.size()));
    }

    template <typename T>
    void draw_circle(vec2d_t<T> center, T radius, size_t num_points) {
        std::vector<SDL_Point> points;
        points.reserve(num_points + 1);
        for(size_t i = 0; i < num_points + 1; ++i) {
            double angle = static_cast<double>(i) * 2 * M_PI /
                           static_cast<double>(num_points);
            points.push_back(vec_to_point(
                center + vec2d_t<T>{radius * cos(angle), radius * sin(angle)}));
        }
        draw_lines(points);
    }

    template <typename T>
    void draw_texture(texture const &texture, vec2d_t<T> position, rect_t<T> view,
                      bool resize = true) {
        draw_texture(texture, position, 0, {0, 0}, view, resize);
    }

    template <typename T>
    void draw_texture(texture const &texture, vec2d_t<T> position, double angle,
                      vec2d_t<T> center, rect_t<T> view, bool resize = true) {
        SDL_Rect rect;
        auto     zoom = m_window_size.x / view.size.x;
        SDL_QueryTexture(texture.get_sdl_texture(), nullptr, nullptr, &rect.w,
                         &rect.h);
        if(resize) {
            rect.w *= zoom;
            rect.h *= zoom;
            center *= zoom;
        }
        vec2d_t<T> p = world_to_window(position, view, m_window_size.x);
        p            -= center;

        rect.x = p.x;
        rect.y = p.y;

        SDL_Point point = vec_to_point(center);
        SDL_RenderCopyEx(m_sdl_renderer, texture.get_sdl_texture(), nullptr,
                         &rect, angle, &point, SDL_FLIP_NONE);
    }

    template <typename T>
    void draw_wrapped_text(font &font, char const *text, vec2d_t<T> position,
                           uint32_t width, color color) {
        surface  sur{TTF_RenderUTF8_Blended_Wrapped(
            font.get_ttf_font(), text, color.get_sdl_color(), width)};
        texture  texture{m_sdl_renderer, sur};
        SDL_Rect rect;
        SDL_QueryTexture(texture.get_sdl_texture(), nullptr, nullptr, &rect.w,
                         &rect.h);
        rect.x = position.x;
        rect.y = position.y;
        SDL_RenderCopy(m_sdl_renderer, texture.get_sdl_texture(), nullptr,
                       &rect);
    }

    template <typename T>
    auto text_to_texture(font &font, char const *text, color color)
        -> std::shared_ptr<texture> {
        surface sur{TTF_RenderUTF8_Blended(font.get_ttf_font(), text,
                                           color.get_sdl_color())};
        return std::make_shared<texture>(m_sdl_renderer, sur);
    }

    template <typename T>
    auto wrapped_text_to_texture(font &font, char const *text, color color,
                                 size_t width) {
        surface sur{TTF_RenderUTF8_Blended_Wrapped(
            font.get_ttf_font(), text, color.get_sdl_color(), width)};
        return std::make_shared<texture>(m_sdl_renderer, sur);
    }

    auto get_sdl_renderer() -> SDL_Renderer * { return m_sdl_renderer; }
};

} // namespace gfx
