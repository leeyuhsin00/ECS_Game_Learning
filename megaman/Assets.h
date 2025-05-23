#pragma once

#include <map>
#include <string>
#include <SFML/Graphics.hpp>
#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include "Animation.h"

typedef std::map    <std::string, sf::Texture>      TextureMap;   
typedef std::map    <std::string, Animation>        AnimationMap;   
typedef std::map    <std::string, sf::SoundBuffer>  SoundBufferMap;  
typedef std::map    <std::string, sf::Font>         FontMap;   

/*  Assets class that contain maps from std::string names to textures, animations, sounds, and fonts
    loaded from GameEngine for ease of retrieving
*/

class Assets
{
private:
    TextureMap      m_textureMap;
    AnimationMap    m_animationMap;
    SoundBufferMap  m_soundBufferMap;
    FontMap         m_fontMap;

public:
    Assets();

    // add
    void addTexture(const std::string& name, const std::string& path);
    void addAnimation(const std::string& name, const Animation& an);
    void addSoundBuffer(const std::string& name, const std::string& path);
    void addFont(const std::string& name, const std::string& path);

    // get
    const sf::Texture& getTexture(const std::string& name) const;
    const Animation& getAnimation(const std::string& name) const;
    const sf::SoundBuffer& getSoundBuffer(const std::string& name) const;
    const sf::Font& getFont(const std::string& name) const;
};
