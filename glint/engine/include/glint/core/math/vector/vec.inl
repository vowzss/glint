#pragma once

namespace glint::engine::core::math::vector {
    // --- methods ---
    template <typename T, std::size_t D, typename I>
    inline typename vec<T, D, I>::mag_t vec<T, D, I>::lengthSquared() const {
        mag_t result{};
        for (size_t i = 0; i < D; ++i) {
            result += static_cast<mag_t>(data[i]) * data[i];
        }
        return result;
    }

    template <typename T, std::size_t D, typename I>
    inline typename vec<T, D, I>::mag_t vec<T, D, I>::length() const {
        return std::sqrt(lengthSquared());
    }

    template <typename T, std::size_t D, typename I>
    inline typename vec<T, D, I>::mag_t vec<T, D, I>::distanceSquared(const vec& v) const {
        mag_t result{};
        for (size_t i = 0; i < D; ++i) {
            mag_t d = static_cast<mag_t>(data[i] - v[i]);
            result += d * d;
        }
        return result;
    }

    template <typename T, std::size_t D, typename I>
    inline typename vec<T, D, I>::mag_t vec<T, D, I>::distance(const vec& v) const {
        return std::sqrt(distanceSquared(v));
    }

    template <typename T, std::size_t D, typename I>
    inline typename vec<T, D, I>::mag_t vec<T, D, I>::dot(const vec& v) const {
        mag_t result{};
        for (size_t i = 0; i < D; ++i) {
            result += static_cast<mag_t>(data[i] * v[i]);
        }
        return result;
    }

    template <typename T, std::size_t D, typename I>
    inline I vec<T, D, I>::normalized() const {
        static_assert(!std::is_integral_v<T>, "'normalized()' is not available for integer vectors!");

        mag_t len = length();
        if (len == 0.0f) {
            return zero();
        }

        vec result{};
        for (size_t i = 0; i < D; ++i) {
            result[i] = data[i] / len;
        }
        return static_cast<I>(result);
    }

    template <typename T, std::size_t D, typename I>
    inline I& vec<T, D, I>::normalize() {
        static_assert(!std::is_integral_v<T>, "'normalize()' is not available for integer vectors!");

        mag_t len = length();
        if (len == 0.0f) {
            return static_cast<I&>(*this);
        }

        for (size_t i = 0; i < D; ++i) {
            data[i] /= len;
        }
        return static_cast<I&>(*this);
    }

    // --- utilities ---
    template <typename T, std::size_t D, typename I>
    constexpr T vec<T, D, I>::sum() const {
        T result{};
        for (size_t i = 0; i < D; ++i) {
            result += data[i];
        }
        return result;
    }

    template <typename T, std::size_t D, typename I>
    constexpr T vec<T, D, I>::min() const {
        T result = data[0];
        for (size_t i = 1; i < D; ++i) {
            if (data[i] < result) result = data[i];
        }
        return result;
    }

    template <typename T, std::size_t D, typename I>
    constexpr T vec<T, D, I>::max() const {
        T result = data[0];
        for (size_t i = 1; i < D; ++i) {
            if (data[i] > result) result = data[i];
        }
        return result;
    }

    template <typename T, std::size_t D, typename I>
    constexpr vec<T, D, I> vec<T, D, I>::abs() const {
        vec result{};
        for (size_t i = 0; i < D; ++i) {
            result[i] = data[i] < 0 ? -data[i] : data[i];
        }
        return result;
    }

    template <typename T, std::size_t D, typename I>
    constexpr vec<T, D, I> vec<T, D, I>::clamp(T min, T max) const {
        vec result{};
        for (size_t i = 0; i < D; ++i) {
            result[i] = data[i] < min ? min : (data[i] > max ? max : data[i]);
        }
        return result;
    }

    template <typename T, std::size_t D, typename I>
    constexpr vec<T, D, I> vec<T, D, I>::lerp(const vec& v, float t) const {
        vec result{};
        for (size_t i = 0; i < D; ++i) {
            result[i] = static_cast<T>(data[i] + (result[i] - data[i]) * t);
        }
        return result;
    }

    // --- operators ---
    template <typename T, std::size_t D, typename I>
    constexpr T& vec<T, D, I>::operator[](size_t i) {
        assert(i < D);
        return data[i];
    }

    template <typename T, std::size_t D, typename I>
    constexpr const T& vec<T, D, I>::operator[](size_t i) const {
        assert(i < D);
        return data[i];
    }

    // --- operators (unary) ---
    template <typename T, std::size_t D, typename I>
    constexpr I vec<T, D, I>::operator+() const noexcept {
        return static_cast<const I&>(*this);
    }

    template <typename T, std::size_t D, typename I>
    constexpr I vec<T, D, I>::operator-() const noexcept {
        I result{};
        for (size_t i = 0; i < D; ++i) {
            result[i] = -data[i];
        }
        return result;
    }

    // --- operators (vector-vector) ---
    template <typename T, std::size_t D, typename I>
    constexpr I vec<T, D, I>::operator+(const I& other) const noexcept {
        I result{};
        for (size_t i = 0; i < D; ++i) {
            result[i] = data[i] + other[i];
        }
        return result;
    }

    template <typename T, std::size_t D, typename I>
    constexpr I vec<T, D, I>::operator-(const I& other) const noexcept {
        I result{};
        for (size_t i = 0; i < D; ++i) {
            result[i] = data[i] - other[i];
        }
        return result;
    }

    template <typename T, std::size_t D, typename I>
    constexpr I vec<T, D, I>::operator*(const I& other) const noexcept {
        I result{};
        for (size_t i = 0; i < D; ++i) {
            result[i] = data[i] * other[i];
        }
        return result;
    }

    template <typename T, std::size_t D, typename I>
    constexpr I vec<T, D, I>::operator/(const I& other) const noexcept {
        I result{};
        for (size_t i = 0; i < D; ++i) {
            result[i] = data[i] / other[i];
        }
        return result;
    }

    // --- operators (vector-scalar) ---
    template <typename T, std::size_t D, typename I>
    constexpr I vec<T, D, I>::operator+(T s) const noexcept {
        I result{};
        for (size_t i = 0; i < D; ++i) {
            result[i] = data[i] + s;
        }
        return result;
    }

    template <typename T, std::size_t D, typename I>
    constexpr I vec<T, D, I>::operator-(T s) const noexcept {
        I result{};
        for (size_t i = 0; i < D; ++i) {
            result[i] = data[i] - s;
        }
        return result;
    }

    template <typename T, std::size_t D, typename I>
    constexpr I vec<T, D, I>::operator*(T s) const noexcept {
        I result{};
        for (size_t i = 0; i < D; ++i) {
            result[i] = data[i] * s;
        }
        return result;
    }

    template <typename T, std::size_t D, typename I>
    constexpr I vec<T, D, I>::operator/(T s) const noexcept {
        I result{};
        for (size_t i = 0; i < D; ++i) {
            result[i] = data[i] / s;
        }
        return result;
    }

    // --- operators (compound assignment) ---
    template <typename T, std::size_t D, typename I>
    constexpr I& vec<T, D, I>::operator+=(const I& other) noexcept {
        for (size_t i = 0; i < D; ++i) {
            data[i] += other[i];
        }
        return static_cast<I&>(*this);
    }

    template <typename T, std::size_t D, typename I>
    constexpr I& vec<T, D, I>::operator-=(const I& other) noexcept {
        for (size_t i = 0; i < D; ++i) {
            data[i] -= other[i];
        }
        return static_cast<I&>(*this);
    }

    template <typename T, std::size_t D, typename I>
    constexpr I& vec<T, D, I>::operator*=(const I& other) noexcept {
        for (size_t i = 0; i < D; ++i) {
            data[i] *= other[i];
        }
        return static_cast<I&>(*this);
    }

    template <typename T, std::size_t D, typename I>
    constexpr I& vec<T, D, I>::operator/=(const I& other) noexcept {
        for (size_t i = 0; i < D; ++i) {
            data[i] /= other[i];
        }
        return static_cast<I&>(*this);
    }

    // --- operators (scalar compound assignment) ---
    template <typename T, std::size_t D, typename I>
    constexpr I& vec<T, D, I>::operator+=(T s) noexcept {
        for (size_t i = 0; i < D; ++i) {
            data[i] += s;
        }
        return static_cast<I&>(*this);
    }

    template <typename T, std::size_t D, typename I>
    constexpr I& vec<T, D, I>::operator-=(T s) noexcept {
        for (size_t i = 0; i < D; ++i) {
            data[i] -= s;
        }
        return static_cast<I&>(*this);
    }

    template <typename T, std::size_t D, typename I>
    constexpr I& vec<T, D, I>::operator*=(T s) noexcept {
        for (size_t i = 0; i < D; ++i) {
            data[i] *= s;
        }
        return static_cast<I&>(*this);
    }

    template <typename T, std::size_t D, typename I>
    constexpr I& vec<T, D, I>::operator/=(T s) noexcept {
        for (size_t i = 0; i < D; ++i) {
            data[i] /= s;
        }
        return static_cast<I&>(*this);
    }

    // --- operators (comparison) ---
    template <typename T, std::size_t D, typename I>
    constexpr bool vec<T, D, I>::operator==(const I& other) const noexcept {
        for (size_t i = 0; i < D; ++i) {
            if (data[i] != other[i]) return false;
        }
        return true;
    }

    template <typename T, std::size_t D, typename I>
    constexpr bool vec<T, D, I>::operator!=(const I& other) const noexcept {
        return (!*this == other);
    }
}