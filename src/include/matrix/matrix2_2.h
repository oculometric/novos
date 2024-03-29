#pragma once

#include <vector/vector2.h>

namespace nov
{
namespace matrix
{

// forward declarations

template <typename T>
struct nov_matrix2_2;

template <typename T>
inline T det(const nov_matrix2_2<T>& a);
template <typename T>
inline nov_matrix2_2<T> cofact(const nov_matrix2_2<T>& a);
template <typename T>
inline nov_matrix2_2<T> adj(const nov_matrix2_2<T>& a);

/**
 * structure which represents a 2x2 matrix
 * with components:
 * [ x_0, y_0 ]
 * [ x_1, y_1 ]
 * 
 * elements are indexed left to right, top to bottom
 * 
 * the `-` operator can be applied to find the transpose of the matrix
 * the `~` operator can be applied to find the inverse of the matrix
 * **/
template <typename T>
struct nov_matrix2_2
{
    T x_0, y_0;
    T x_1, y_1;

    constexpr nov_matrix2_2<T>(const nov_matrix2_2<T>& o) : x_0(o.x_0), y_0(o.y_0), x_1(o.x_1), y_1(o.y_1) { }
    constexpr nov_matrix2_2<T>(const T _x_0, const T _y_0, const T _x_1, const T _y_1) : x_0(_x_0), y_0(_y_0), x_1(_x_1), y_1(_y_1) { }
    constexpr nov_matrix2_2<T>(): x_0(0), y_0(0), x_1(0), y_1(0) { }

    inline nov_vector2<T> col_0() const { return nov_vector2<T>{ x_0,x_1 }; }
    inline nov_vector2<T> col_1() const { return nov_vector2<T>{ y_0,y_1 }; }
    inline nov_vector2<T> row_0() const { return nov_vector2<T>{ x_0,y_0 }; }
    inline nov_vector2<T> row_1() const { return nov_vector2<T>{ x_1,y_1 }; }

    inline void operator=(const nov_matrix2_2<T>& a) { x_0 = a.x_0; y_0 = a.y_0; x_1 = a.x_1; y_1 = a.y_1; }
    inline void operator+=(const nov_matrix2_2<T>& a) { x_0 += a.x_0; y_0 += a.y_0; x_1 += a.x_1; y_1 += a.y_1; }
    inline void operator-=(const nov_matrix2_2<T>& a) { x_0 -= a.x_0; y_0 -= a.y_0; x_1 -= a.x_1; y_1 -= a.y_1; }
    inline void operator*=(const nov_matrix2_2<T>& a)
    {
        nov_vector2<T> r0 = row_0();
        nov_vector2<T> r1 = row_1();
        nov_vector2<T> c0 = a.col_0();
        nov_vector2<T> c1 = a.col_1();
        x_0 = r0 ^ c0; y_0 = r0 ^ c1;
        x_1 = r1 ^ c0; y_1 = r1 ^ c1;
    }
    inline void operator*=(const T a) { x_0 *= a; y_0 *= a; x_1 *= a; y_1 *= a; }
    inline void operator/=(const T a) { x_0 /= a; y_0 /= a; x_1 /= a; y_1 /= a; }
    inline nov_matrix2_2<T> operator-() const { return nov_matrix2_2<T>{ x_0, x_1, y_0, y_1 }; }
    inline nov_matrix2_2<T> operator~() const { return adj(*this) / det(*this); }
};

template <typename T>
inline nov_matrix2_2<T> operator+(const nov_matrix2_2<T>& a, const nov_matrix2_2<T>& b)
{
    return nov_matrix2_2<T>
    {
        a.x_0 + b.x_0, a.y_0 + b.y_0,
        a.x_1 + b.x_1, a.y_1 + b.y_1
    };
}

template <typename T>
inline nov_matrix2_2<T> operator-(const nov_matrix2_2<T>& a, const nov_matrix2_2<T>& b)
{
    return nov_matrix2_2<T>
    {
        a.x_0 - b.x_0, a.y_0 - b.y_0,
        a.x_1 - b.x_1, a.y_1 - b.y_1
    };
}

template <typename T>
inline nov_matrix2_2<T> operator*(const nov_matrix2_2<T>& a, const nov_matrix2_2<T>& b)
{
    nov_vector2<T> r0 = a.row_0();
    nov_vector2<T> r1 = a.row_1();
    nov_vector2<T> c0 = b.col_0();
    nov_vector2<T> c1 = b.col_1();
    return nov_matrix2_2<T>
    {
        r0 ^ c0, r0 ^ c1,
        r1 ^ c0, r1 ^ c1
    };
}

template <typename T, typename U>
inline nov_vector2<U> operator*(const nov_matrix2_2<T>& a, const nov_vector2<U>& b)
{
    return nov_vector2<U>{ a.row_0() ^ b, a.row_1() ^ b };
}

template <typename T>
inline nov_matrix2_2<T> operator*(const nov_matrix2_2<T>& a, const T b)
{
    return nov_matrix2_2<T>
    {
        a.x_0 * b, a.y_0 * b,
        a.x_1 * b, a.y_1 * b
    };
}

template <typename T>
inline nov_matrix2_2<T> operator/(const nov_matrix2_2<T>& a, const T b)
{
    return nov_matrix2_2<T>
    {
        a.x_0 / b, a.y_0 / b,
        a.x_1 / b, a.y_1 / b
    };
}

/**
 * calculate the minor of a matrix
 * 
 * - by taking the determinant of the matrix when the row and column
 * containing the specified element are discarded.
 * 
 * elements are ordered:
 * 
 * [ 0, 1 ]
 * [ 2, 3 ]
 * 
 * @param a matrix on which to operate
 * @param e index of the element for which to find the minor matrix. see above
 * 
 * @return determinant of the resulting minor matrix
 * **/
template <typename T>
inline T minor(const nov_matrix2_2<T>& a, const uint8_t e)
{
    switch (e)
    {
        case 0: return a.y_1;
        case 1: return a.x_1;
        case 2: return a.y_0;
        case 3: return a.x_0;
        default: return 0;
    }
}

/**
 * calculate determinant of a matrix
 * 
 * - by subtracting the product of the alternate diagonal from
 * the product of the leading diagonal
 * 
 * @param a matrix on which to operate
 * 
 * @return determinant of the matrix
 * **/
template <typename T>
inline T det(const nov_matrix2_2<T>& a) { return (a.x_0 * a.y_1) - (a.y_0 * a.x_1); }

/**
 * calculate matrix of cofactors from a matrix
 * 
 * - by finding the matrix of minors (each element replaced with
 * its minor), then multiplying by a checkered mask of alternating 1, -1
 * 
 * @param a matrix on which to operate
 * 
 * @return matrix of cofactors
 * **/
template <typename T>
inline nov_matrix2_2<T> cofact(const nov_matrix2_2<T>& a) { return nov_matrix2_2<T>{ a.y_1, -a.x_1, -a.y_0, a.x_0 }; }

/**
 * calculate adjoint of a matrix
 * 
 * - by finding the transpose of the matrix of cofactors of
 * the input matrix
 * 
 * @param a matrix on which to operate
 * 
 * @return adjoint matrix of the input
 * **/
template <typename T>
inline nov_matrix2_2<T> adj(const nov_matrix2_2<T>& a) { return -cofact(a); }

}
}
