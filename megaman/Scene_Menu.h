#pragma once

#include "Scene.h"
#include "GameEngine.h"
#include <map>

typedef std::map<std::string, std::unique_ptr<sf::Text>> TextMap;

/*  scene menu class inherited from scene class containing level selection texts, animations and user inputs
    member variables: levelSelect - level selected by user using keys; loadlevel - triggered when user selects
    a level
*/

class Scene_Menu : public Scene
{
protected:
    TextMap     m_text;
    sf::Font    m_font;
    size_t      levelSelect{0};
    size_t      maxLevel{2};
    bool        loadLevel{false};

    void init();
    virtual void sRender() override;
    virtual void sDoAction();

public:
    Scene_Menu(std::shared_ptr<GameEngine> g);

    virtual void update() override;
    virtual void simulate();
    virtual void doAction(Action& act);
};
