#pragma once

/*  2D vector class that describes the X-Y cartesian coordinates and their operators
    together with a few useful functions like normalize and length
*/

class Vec2
{
    public:

    // coordinates
    float x{0.f};
    float y{0.f};

    // constructors
    Vec2();
    Vec2(float xin, float yin);

    // operators
    bool operator == (const Vec2& rhs) const;
    bool operator != (const Vec2& rhs) const;

    Vec2 operator + (const Vec2& rhs) const;
    Vec2 operator - (const Vec2& rhs) const;
    Vec2 operator * (const Vec2& rhs) const;
    Vec2 operator / (const Vec2& rhs) const;

    void operator += (const Vec2& rhs);
    void operator -= (const Vec2& rhs);
    void operator *= (const Vec2& rhs);
    void operator /= (const Vec2& rhs);

    // useful functions
    void normalize();
    float length() const;
};
