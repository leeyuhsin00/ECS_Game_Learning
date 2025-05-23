#include "Assets.h"

Assets::Assets()
{}

// add
void Assets::addTexture(const std::string& name, const std::string& path)
{
    sf::Texture texture(path);
    m_textureMap[name] = texture;
}

void Assets::addAnimation(const std::string& name, const Animation& an)
{
    m_animationMap[name] = an;
}

void Assets::addSoundBuffer(const std::string& name, const std::string& path)
{
    sf::SoundBuffer buffer(path);
    m_soundBufferMap[name] = buffer;
}
void Assets::addFont(const std::string& name, const std::string& path)
{
    sf::Font font(path);
    m_fontMap[name] = font;
}

// get
const sf::Texture& Assets::getTexture(const std::string& name) const
{
    return m_textureMap.at(name);
}

const Animation& Assets::getAnimation(const std::string& name) const
{
    return m_animationMap.at(name);
}

const sf::SoundBuffer& Assets::getSoundBuffer(const std::string& name) const
{
    return m_soundBufferMap.at(name);
}

const sf::Font& Assets::getFont(const std::string& name) const
{
    return m_fontMap.at(name);
}
