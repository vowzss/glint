#pragma once

#include <cmath>

#include "vec.h"

template <typename T, std::size_t N> inline T vec<T, N>::dot(const vec<T, N>& v) const {
    T sum = T{};
    for (size_t i = 0; i < N; i++) {
        sum += data[i] * v.data[i];
    }
    return sum;
}

template <typename T, std::size_t N> inline T vec<T, N>::length() const {
    return std::sqrt(dot(*this));
}

template <typename T, std::size_t N> inline vec<T, N> vec<T, N>::normalized() const {
    T inv = T(1) / length();
    vec<T, N> result;
    for (size_t i = 0; i < N; i++) {
        result.data[i] = data[i] * inv;
    }
    return result;
}

template <typename T, std::size_t N> inline vec<T, N> vec<T, N>::operator+(const vec<T, N>& v) const {
    vec<T, N> result;
    for (size_t i = 0; i < N; i++) {
        result.data[i] = data[i] + v.data[i];
    }
    return result;
}

template <typename T, std::size_t N> inline vec<T, N> vec<T, N>::operator-(const vec<T, N>& v) const {
    vec<T, N> result;
    for (size_t i = 0; i < N; i++) {
        result.data[i] = data[i] - v.data[i];
    }
    return result;
}

template <typename T, std::size_t N> inline vec<T, N> vec<T, N>::operator*(T s) const {
    vec<T, N> result;
    for (size_t i = 0; i < N; i++) {
        result.data[i] = data[i] * s;
    }
    return result;
}
