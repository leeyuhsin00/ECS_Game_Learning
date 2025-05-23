#include "Animation.h"
#include <iostream>

Animation::Animation() 
    : m_sprite()
{}

Animation::Animation(const std::string& n, const sf::Texture& t, size_t fc, size_t sp)
    : m_name{n}, m_sprite{t}, m_frameCount{fc}, m_speed{sp}
    {
        m_size = Vec2(t.getSize().x, t.getSize().y);
        m_frameSize = Vec2(m_size.x / static_cast<float>(m_frameCount), m_size.y);
        
        // set origin to center of animation frame
        m_sprite->setOrigin(sf::Vector2f(m_frameSize.x/2.f, m_frameSize.y/2.f));
        m_sprite->setTextureRect(sf::IntRect({static_cast<int>(m_currentFrame * m_frameSize.x), 0}, {static_cast<int>(m_frameSize.x), static_cast<int>(m_frameSize.y)}));
    }

void Animation::update()
{
    m_currentFrame++;
    // modulo total number of frames to consistenly rotate between frames
    size_t animFrame = (m_currentFrame/m_speed) % m_frameCount;
    m_sprite->setTextureRect(sf::IntRect({static_cast<int>(animFrame * m_frameSize.x), 0}, {static_cast<int>(m_frameSize.x), static_cast<int>(m_frameSize.y)}));
}

const std::string& Animation::getName() const
{
    return m_name;
}

const Vec2& Animation::getFrameSize() const
{
    return m_frameSize;
}

const std::optional<sf::Sprite>& Animation::getSprite() const
{
    return m_sprite;
}
