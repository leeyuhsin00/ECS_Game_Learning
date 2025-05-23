#include "Vec2.h"
#include <cmath>

// constructor
Vec2::Vec2(){};
Vec2::Vec2(float xin, float yin)
    : x{xin}
    , y{yin}
{};

// operators
bool Vec2::operator == (const Vec2& rhs) const
{
    return (x == rhs.x && y == rhs.y); 
}
bool Vec2::operator != (const Vec2& rhs) const
{
    return !(x == rhs.x && y == rhs.y); 
}

Vec2 Vec2::operator + (const Vec2& rhs) const
{
    return Vec2(x + rhs.x, y + rhs.y);
}
Vec2 Vec2::operator - (const Vec2& rhs) const
{
    return Vec2(x - rhs.x, y - rhs.y);
}
Vec2 Vec2::operator * (const Vec2& rhs) const
{
    return Vec2(x * rhs.x, y * rhs.y);
}
Vec2 Vec2::operator / (const Vec2& rhs) const
{
    return Vec2(x / rhs.x, y / rhs.y);
}

void Vec2::operator += (const Vec2& rhs)
{
    x += rhs.x;
    y += rhs.y;
}
void Vec2::operator -= (const Vec2& rhs)
{
    x -= rhs.x;
    y -= rhs.y;
}
void Vec2::operator *= (const Vec2& rhs)
{
    x *= rhs.x;
    y *= rhs.y;
}
void Vec2::operator /= (const Vec2& rhs)
{
    if (rhs.x != 0 && rhs.y != 0)
    {
        x /= rhs.x;
        y /= rhs.y;
    }
}

// useful functions
void Vec2::normalize()
{
    float len = std::sqrt(x*x + y*y);
    if (len != 0.f) {
        x /= len;
        y /= len;
    }
}

float Vec2::length() const
{
    return std::sqrt(x*x + y*y);
}
