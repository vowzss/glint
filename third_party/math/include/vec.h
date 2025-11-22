#pragma once

#include <cstddef>

template <typename T, std::size_t N> struct vec {
    T data[N];

    // --- ctor ---
    inline vec() {
        for (size_t i = 0; i < N; i++) {
            data[i] = T{};
        }
    }

    inline explicit vec(T v) {
        for (size_t i = 0; i < N; i++) {
            data[i] = v;
        }
    }

    // --- ----
    inline T length() const;
    inline T dot(const vec& v) const;
    inline vec normalized() const;

    // --- operators ---
    inline T& operator[](size_t i) { return data[i]; }
    inline const T& operator[](size_t i) const { return data[i]; }

    inline vec operator+(const vec& v) const;
    inline vec operator-(const vec& v) const;
    inline vec operator*(T s) const;
};

#include "vec.inl"