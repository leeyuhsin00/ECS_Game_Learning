#pragma once

#include <memory>
#include "Entitymanager.h"
#include "Action.h"

class GameEngine;

typedef std::map<sf::Keyboard::Key, std::string> KeyActionMap;
typedef std::map<sf::Mouse::Button, std::string> MouseActionMap;

/*  base class Scene containing basic member variables and functions to be used in inherited classes
    shared ptr to GameEngine to call functions and variables whenever
    member variables: m_keyActionMap - map keys to Action(name, type);
    m_mouseActionMap - map mouse buttons to Action(name, type) different scenes will have different maps
    member functions: sDoAction - create Action using key maps and button maps then calls doAction 
    (unique for each scene type)
    registerAction - register key maps and button maps unique for each scene type
*/

class Scene
{
protected:
    std::shared_ptr<GameEngine> m_game;
    EntityManager               m_entities;
    size_t                      m_currentFrame{0};
    KeyActionMap                m_keyActionMap;
    MouseActionMap              m_mouseActionMap;
    bool                        m_paused{false};
    bool                        m_hasEnded{false};

public:
    Scene(std::shared_ptr<GameEngine> game);

    virtual void update();
    void sDoAction(sf::Keyboard::Key k, std::string actionType);
    void sDoAction(sf::Mouse::Button b, std::string actionType);
    virtual void sRender();

    virtual void simulate() = 0;
    virtual void doAction(Action& act) = 0;
    
    void registerAction(sf::Keyboard::Key k, std::string action);
    void registerAction(sf::Mouse::Button b, std::string action);
};
