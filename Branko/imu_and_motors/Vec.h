#ifndef VEC_H
#define VEC_H

#include <Zumo32U4.h>

template <typename T>
struct Vec : public Printable {
    T x, y, z;

    Vec() = default;

    Vec(T x, T y, T z)
        : x(x),
          y(y),
          z(z) {}

    template <typename U>
    Vec(const Zumo32U4IMU::vector<U>& v)
        : x(v.x),
          y(v.y),
          z(v.z) {}

    template <typename U>
    Vec<U> cast() const {
        return {static_cast<U>(x), static_cast<U>(y), static_cast<U>(z)};
    }

    template <typename U>
    Vec& operator+=(const Vec<U>& other) {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }

    template <typename U>
    Vec operator+(const Vec<U>& other) const {
        Vec<T> result = *this;
        result += other;
        return result;
    }

    Vec& operator-=(const Vec& other) {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        return *this;
    }

    Vec operator-(const Vec& other) const {
        Vec<T> result = *this;
        result -= other;
        return result;
    }

    Vec& operator/=(const T& scalar) {
        x /= scalar;
        y /= scalar;
        z /= scalar;
        return *this;
    }

    Vec operator/(const T& scalar) const {
        Vec<T> result = *this;
        result /= scalar;
        return result;
    }

    Vec& operator*=(const T& scalar) {
        x *= scalar;
        y *= scalar;
        z *= scalar;
        return *this;
    }

    Vec operator*(const T& scalar) const {
        Vec<T> result = *this;
        result *= scalar;
        return result;
    }

    size_t printTo(Print& p) const {
        size_t result = 0;
        result += p.print("{ ");
        result += p.print(x);
        result += p.print(", ");
        result += p.print(y);
        result += p.print(", ");
        result += p.print(z);
        result += p.print(" }");
        return result;
    }
};

#endif // VEC_H
