#pragma once

#include <string>
#include <SFML/Graphics.hpp>
#include "Vec2.h"

/*  Animation class that contains member variables:
    m_sprite - sf::Sprite to be drawn; m_frameCount - number of animation frames in loaded texture;
    m_speed - speed of animation; m_size - size of entire texture; m_frameSize - size of one animation frame;
    m_name - name of the animation
    
    Member functions:
    update() - update the animation;
*/

class Animation
{
private:
    std::optional<sf::Sprite>   m_sprite;
    size_t                      m_frameCount;
    size_t                      m_currentFrame{0};
    size_t                      m_speed;
    Vec2                        m_size;
    Vec2                        m_frameSize;
    std::string                 m_name;

public:
    Animation();
    Animation(const std::string& n, const sf::Texture&t, size_t fc, size_t sp);

    void  update();
    const std::string& getName() const;
    const Vec2& getFrameSize() const;
    const std::optional<sf::Sprite>& getSprite() const;
};
