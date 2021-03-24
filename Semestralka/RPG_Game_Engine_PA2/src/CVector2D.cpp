//
// Created by jorge on 6/18/20.
//

#include "CVector2D.hpp"

CVector2D::CVector2D(): m_x(0.0f), m_y(0.0f){
}

CVector2D::CVector2D(float x, float y): m_x(x), m_y(y){
}

CVector2D& CVector2D::add(const CVector2D& vector2D){
    this->m_x += vector2D.m_x;
    this->m_y += vector2D.m_y;
    return *this;
}

CVector2D& CVector2D::subtract(const CVector2D& vector2D){
    this->m_x -= vector2D.m_x;
    this->m_y -= vector2D.m_y;
    return *this;
}

CVector2D& CVector2D::multiply(const CVector2D& vector2D){
    this->m_x *= vector2D.m_x;
    this->m_y *= vector2D.m_y;
    return *this;
}

CVector2D& CVector2D::divide(const CVector2D& vector2D){
    this->m_x /= vector2D.m_x;
    this->m_y /= vector2D.m_y;
    return *this;
}

CVector2D& operator+(CVector2D& vec1, const CVector2D& vec2){
    return vec1.add(vec2);
}

CVector2D& operator-(CVector2D& vec1, const CVector2D& vec2){
    return vec1.subtract(vec2);
}

CVector2D& operator*(CVector2D& vec1, const CVector2D& vec2){
    return vec1.multiply(vec2);
}

CVector2D& operator/(CVector2D& vec1, const CVector2D& vec2){
    return vec1.divide(vec2);
}

CVector2D& CVector2D::operator+=(const CVector2D& vector2D){
    return this->add(vector2D);
}

CVector2D& CVector2D::operator-=(const CVector2D& vector2D){
    return this->subtract(vector2D);
}

CVector2D& CVector2D::operator*=(const CVector2D& vector2D){
    return this->multiply(vector2D);
}

CVector2D& CVector2D::operator/=(const CVector2D& vector2D){
    return this->divide(vector2D);
}

CVector2D& CVector2D::operator*(const int& integer){
    this->m_x *= integer;this->m_y *= integer;
    return *this;
}

CVector2D& CVector2D::setZero(){
    this->m_x = 0;
    this->m_y = 0;
    return *this;
}

std::ostream& operator<<(std::ostream& stream, const CVector2D& vector2D){
    stream << "(" << vector2D.m_x << "," << vector2D.m_y << ")";
    return stream;
}