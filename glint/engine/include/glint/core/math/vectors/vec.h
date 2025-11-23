#pragma once

#include <array>
#include <cassert>
#include <cstddef>
#include <ostream>

template <typename T, std::size_t D, typename I>
struct vec {
    static_assert(std::is_arithmetic_v<T>, "must be an arithmetic type!");
    using mag_t = std::conditional_t<std::is_integral_v<T>, double, T>;

  protected:
    std::array<T, D> data;

  protected:
    // --- constructors ---
    constexpr vec() : data{} {}
    constexpr vec(const std::array<T, D>& values) : data(values) {}
    constexpr explicit vec(T value) : data{} {
        for (size_t i = 0; i < D; ++i) {
            data[i] = value;
        }
    }

  public:
    // --- factories ---
    static constexpr vec<T, D, I> zero();
    static constexpr vec one();

    // --- methods ---
    inline mag_t lengthSquared() const;
    inline mag_t length() const;

    inline mag_t distanceSquared(const vec& v) const;
    inline mag_t distance(const vec& v) const;

    inline mag_t dot(const vec& v) const;

    inline I normalized() const;
    inline I& normalize();

    // --- utilities ---
    constexpr T sum() const;
    constexpr T min() const;
    constexpr T max() const;
    constexpr vec abs() const;
    constexpr vec clamp(T min, T max) const;
    constexpr vec lerp(const vec& v, float t) const;

    // --- operators ---
    constexpr T& operator[](size_t i);
    constexpr const T& operator[](size_t i) const;

    // --- operators (unary) ---
    constexpr I operator+() const noexcept;
    constexpr I operator-() const noexcept;

    // --- operators (vector-vector) ---
    constexpr I operator+(const I& other) const noexcept;
    constexpr I operator-(const I& other) const noexcept;
    constexpr I operator*(const I& other) const noexcept;
    constexpr I operator/(const I& other) const noexcept;

    // --- operators (vector-scalar) ---
    constexpr I operator+(T s) const noexcept;
    constexpr I operator-(T s) const noexcept;
    constexpr I operator*(T s) const noexcept;
    constexpr I operator/(T s) const noexcept;

    // --- operators (vector-vector compound assignment) ---
    constexpr I& operator+=(const I& other) noexcept;
    constexpr I& operator-=(const I& other) noexcept;
    constexpr I& operator*=(const I& other) noexcept;
    constexpr I& operator/=(const I& other) noexcept;

    // --- operators (vector-scalar compound assignment) ---
    constexpr I& operator+=(T s) noexcept;
    constexpr I& operator-=(T s) noexcept;
    constexpr I& operator*=(T s) noexcept;
    constexpr I& operator/=(T s) noexcept;

    // --- operators (comparison) ---
    constexpr bool operator==(const I& other) const noexcept;
    constexpr bool operator!=(const I& other) const noexcept;

    // --- operators (string) ---
    friend std::ostream& operator<<(std::ostream& os, const vec<T, D, I>& v) {
        os << "(";
        for (size_t i = 0; i < D; ++i) {
            os << v.data[i];
            if (i != D - 1) os << ", ";
        }
        os << ")";
        return os;
    }
};

#include "vec.inl"
