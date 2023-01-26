#pragma once

#include <cmath>
#include <fmt/format.h>
#include <utility>

template <typename T>
    requires std::integral<T> || std::floating_point<T>
struct vec2d_t {
    T x{};
    T y{};

    constexpr vec2d_t() noexcept = default;
    constexpr vec2d_t(T x, T y) noexcept : x(x), y(y) {}
    explicit constexpr vec2d_t<T>(std::pair<T, T> pair) noexcept
        : x(pair.first), y(pair.second) {}

    constexpr vec2d_t(vec2d_t<T> const &)     = default;
    constexpr vec2d_t(vec2d_t<T> &&) noexcept = default;
    ~vec2d_t()                                = default;

    auto operator==(vec2d_t<T> const &rhs) const -> bool {
        return std::is_eq(*this <=> rhs);
    }

    auto operator<=>(vec2d_t<T> const &rhs) const -> std::partial_ordering {
        return y == rhs.y ? x <=> rhs.x : y <=> rhs.y;
    }

    auto operator=(vec2d_t const &) -> vec2d_t     & = default;
    auto operator=(vec2d_t &&) noexcept -> vec2d_t & = default;

    auto operator+=(T const &rhs) -> vec2d_t<T> const & {
        x += rhs;
        y += rhs;
        return *this;
    }

    auto operator+=(vec2d_t<T> const &rhs) -> vec2d_t<T> const & {
        x += rhs.x;
        y += rhs.y;
        return *this;
    }

    auto operator-=(T const &rhs) -> vec2d_t<T> const & {
        x -= rhs;
        y -= rhs;
        return *this;
    }

    auto operator-=(vec2d_t<T> const &rhs) -> vec2d_t<T> const & {
        x -= rhs.x;
        y -= rhs.y;
        return *this;
    }

    auto operator*=(T const &rhs) -> vec2d_t<T> const & {
        x *= rhs;
        y *= rhs;
        return *this;
    }

    auto operator*=(vec2d_t<T> const &rhs) -> vec2d_t<T> const & {
        x *= rhs.x;
        y *= rhs.y;
        return *this;
    }

    auto operator/=(T const &rhs) -> vec2d_t<T> const & {
        x /= rhs;
        y /= rhs;
        return *this;
    }

    auto operator/=(vec2d_t<T> const &rhs) -> vec2d_t<T> const & {
        x /= rhs.x;
        y /= rhs.y;
        return *this;
    }

    friend auto operator+(vec2d_t<T> lhs, vec2d_t<T> const &rhs) -> vec2d_t<T> {
        lhs += rhs;
        return lhs;
    }

    friend auto operator+(vec2d_t<T> lhs, T const &rhs) -> vec2d_t<T> {
        lhs += rhs;
        return lhs;
    }

    friend auto operator-(vec2d_t<T> lhs, vec2d_t<T> const &rhs) -> vec2d_t<T> {
        lhs -= rhs;
        return lhs;
    }

    friend auto operator-(vec2d_t<T> lhs, T const &rhs) -> vec2d_t<T> {
        lhs -= rhs;
        return lhs;
    }

    friend auto operator*(vec2d_t<T> lhs, vec2d_t<T> const &rhs) -> vec2d_t<T> {
        lhs *= rhs;
        return lhs;
    }

    friend auto operator*(vec2d_t<T> lhs, T const &rhs) -> vec2d_t<T> {
        lhs *= rhs;
        return lhs;
    }

    friend auto operator/(vec2d_t<T> lhs, vec2d_t<T> const &rhs) -> vec2d_t<T> {
        lhs /= rhs;
        return lhs;
    }

    friend auto operator/(vec2d_t<T> lhs, T const &rhs) -> vec2d_t<T> {
        lhs /= rhs;
        return lhs;
    }

    auto dot(vec2d_t<T> const &vec) -> T { return x * vec.x + y * vec.y; }

    friend auto dot(vec2d_t<T> lhs, vec2d_t<T> const &rhs) -> T {
        lhs.dot(rhs);
        return lhs;
    }

    template <typename U> explicit operator vec2d_t<U>() {
        return vec2d_t<U>{static_cast<U>(x), static_cast<U>(y)};
    }

    template <typename U> explicit operator vec2d_t<U>() const {
        return vec2d_t<U>{static_cast<U>(x), static_cast<U>(y)};
    }

    auto is_zero() -> bool { return x == 0 && y == 0; }

    static auto unit_x() -> vec2d_t<T> { return {1, 0}; }
    static auto unit_y() -> vec2d_t<T> { return {0, 1}; }

    template <typename U = T>
        requires(!std::is_integral<U>::value)
    [[nodiscard]] auto mag() const -> T {
        return sqrt(mag_sq());
    }

    [[nodiscard]] auto mag_sq() const -> T { return x * x + y * y; }

    void set_mag(T new_mag) {
        normalize();
        *this *= new_mag;
    }

    auto with_mag(T new_mag) -> vec2d_t {
        vec2d_t new_vec{*this};
        new_vec.set_mag(new_mag);
        return new_vec;
    }

    [[nodiscard]] auto norm() const -> vec2d_t<T> {
        auto vec = *this;
        vec.normalize();
        return vec;
    }

    template <typename U = T>
        requires(!std::is_integral<U>::value)
    auto normalize() -> vec2d_t<T> const & {
        T length = mag();
        if(length != 0) {
            x /= length;
            y /= length;
        }
        return *this;
    }

    template <typename U = T>
        requires(!std::is_integral<U>::value)
    auto limit(T lim) -> vec2d_t<T> const & {
        if(mag_sq() <= lim * lim) {
            return *this;
        }
        normalize();
        *this *= lim;
        return *this;
    }

    auto theta() -> T { return atan2(y, x); }

    template <typename U = T>
        requires(!std::is_integral<U>::value)
    static auto from_angle(double angle) -> vec2d_t<T> {
        return {static_cast<T>(cos(angle)), static_cast<T>(sin(angle))};
    }
};

template <typename T>
struct fmt::formatter<vec2d_t<T>> : formatter<std::string_view> {
    char presentation = 'd';
    template <typename FormatContext>
    auto format(vec2d_t<T> const &pos, FormatContext &ctx) const
        -> decltype(ctx.out()) {
        return formatter<string_view>::format(
            fmt::format("({},{})", pos.x, pos.y), ctx);
    }
};

template <typename T>
    requires(!std::is_integral<T>::value)
constexpr auto rad_to_deg(T rad) noexcept -> T {
    return rad * 180 / M_PI;
}

template <typename T>
    requires(!std::is_integral<T>::value)
constexpr auto deg_to_rad(T deg) noexcept -> T {
    return deg / 180 * M_PI;
}

template <typename T> struct vec2d_hasher {
    auto operator()(vec2d_t<T> const &pos) const -> size_t {
        const T arbitrary_value = 2048;
        return pos.x * arbitrary_value + pos.y;
    }
};

template <typename T>
auto manhattan(vec2d_t<T> const &pos, vec2d_t<T> const &pos2) -> T {
    return abs(pos.x - pos2.x) + abs(pos.y - pos2.y);
}

template <typename T>
[[nodiscard]] auto index_to_vec2d(T index, T width) -> vec2d_t<T> {
    return {index % width, index / width};
}

template <typename T>
[[nodiscard]] auto vec2d_to_index(vec2d_t<T> pos, T width) -> T {
    return pos.x + pos.y * width;
}
