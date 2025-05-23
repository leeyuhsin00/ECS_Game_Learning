#include "Scene.h"
#include <iostream>

Scene::Scene(std::shared_ptr<GameEngine> game)
    : m_game{game}
    {}

// update entities - adding, removing, destroying
void Scene::update()
{
    m_entities.update();
}

// unique definitions for each scene type
void Scene::doAction(Action& act)
{

}

void Scene::sRender()
{

}

void Scene::simulate()
{

}

void Scene::sDoAction(sf::Keyboard::Key k, std::string actionType)
{
    Action act(m_keyActionMap[k], actionType);
    doAction(act);
}

void Scene::sDoAction(sf::Mouse::Button b, std::string actionType)
{
    Action act(m_mouseActionMap[b], actionType);
    doAction(act);
}

void Scene::registerAction(sf::Keyboard::Key k, std::string action)
{
    m_keyActionMap[k] = action;
}

void Scene::registerAction(sf::Mouse::Button b, std::string action)
{
    m_mouseActionMap[b] = action;
}
