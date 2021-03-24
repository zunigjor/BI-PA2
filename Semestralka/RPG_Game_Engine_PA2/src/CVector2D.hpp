//
// Created by jorge on 6/18/20.
//

#ifndef RPG_GAME_ENGINE_PA2_CVECTOR2D_HPP
#define RPG_GAME_ENGINE_PA2_CVECTOR2D_HPP

#include <iostream>

/**
 * CVector class is basically just a struct with two coordinates.
 * It is used to easily work in a 2D system.
 */
class CVector2D {
public:
    CVector2D();
    /// Overloaded constructor with x and y positions.
    CVector2D(float x, float y);
    /// Adds a vector to the current vector.
    CVector2D& add(const CVector2D& vector2D);
    /// Subtracts a vector from the current vector.
    CVector2D& subtract(const CVector2D& vector2D);
    /// Multiplies this vector by a provided vector.
    CVector2D& multiply(const CVector2D& vector2D);
    /// Divides this vector by provided vector.
    CVector2D& divide(const CVector2D& vector2D);
public:
    /// Adds two vectors.
    friend CVector2D& operator+(CVector2D& vec1, const CVector2D& vec2);
    /// Subtracts two vectors.
    friend CVector2D& operator-(CVector2D& vec1, const CVector2D& vec2);
    /// Multiplies vector by the right vector.
    friend CVector2D& operator*(CVector2D& vec1, const CVector2D& vec2);
    /// Divides vector by the right vector.
    friend CVector2D& operator/(CVector2D& vec1, const CVector2D& vec2);
public:
    /// Adds a vector to this vector.
    CVector2D& operator+=(const CVector2D& vector2D);
    /// Subtracts a vector from this vector.
    CVector2D& operator-=(const CVector2D& vector2D);
    /// Multiplies this vector by a vector.
    CVector2D& operator*=(const CVector2D& vector2D);
    /// Divides this vector by a vector.
    CVector2D& operator/=(const CVector2D& vector2D);
    /// Multiples a vector by an integer.
    CVector2D& operator*(const int& integer);
public:
    /// Sets the vectors coordinates to zero.
    CVector2D& setZero();
    /// Operator << allows to print the vector coordinates. Prints in format (x, y). Note: x and y are floats.
    friend std::ostream& operator<<(std::ostream& stream, const CVector2D& vec);
public:
    float m_x;
    float m_y;
};


#endif //RPG_GAME_ENGINE_PA2_CVECTOR2D_HPP
