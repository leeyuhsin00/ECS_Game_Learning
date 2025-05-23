#pragma once

#include "Vec2.h"
#include "Animation.h"
#include <SFML/Graphics.hpp>

/* Various component classes that each entity may contain
*/

// Transform class to record position, previous position, velocity, scale and angle for movement and collision
class CTransform
{
public:
    Vec2 pos        {0.0f,0.0f};
    Vec2 prevPos    {0.0f,0.0f};
    Vec2 velocity   {0.0f,0.0f};
    Vec2 scale      {1.0f,1.0f};
    float angle     {0.0f};

    CTransform() = default;
    CTransform(const Vec2& p, const Vec2& v, const Vec2& s, const float& a)
        : pos{p}, velocity{v}, scale{s}, angle{a}
    {}
};

// Bounding box class for collisions
class CBoundingBox
{
public:
    Vec2 size       {0.0f,0.0f};

    CBoundingBox() = default;
    CBoundingBox(const Vec2& s)
        : size{s}
    {}
};

// lifespan class
class CLifespan
{
public:
    int remaining {0};
    int total     {0};

    CLifespan() = default;
    CLifespan(int total)
        : remaining(total), total(total)
    {}
};

// input class after converting user input to actions
class CInput
{
public:
    bool jump   {false};
    bool left   {false};
    bool right  {false};
    bool shoot  {false};

    CInput() = default;
};

// animation class to add animations
class CAnimation
{
public:
    Animation   animation;

    CAnimation() = default;
    CAnimation(Animation a)
        : animation{a}
    {}
};

// gravity class for physics
class CGravity
{
public:
    float gravity  {5.f};

    CGravity() = default;
    CGravity(float g)
        : gravity{g}
    {}
};

// friction class for physics
class CFriction
{
public:
    float friction = 0.6f;

    CFriction() = default;
    CFriction(float coeff) : friction{coeff} {}
};

// state class for animations, collisions and movement
class CState
{
public:
    bool onGround {false};
    bool isJumping {false};
    bool isIdle {true};
    bool isRunning {false};
    bool isFiring {false};
    bool facingRight {true};
    
    CState() = default;
};

// jump timer class for smoother jumping mechanics
class CJumpTimer
{
public:
    int duration {10};
    float speedMultiplier {1.5f};
    int currentFrame {0};
    bool isJumping {false};

    CJumpTimer() = default;
};
