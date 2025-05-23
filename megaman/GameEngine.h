#pragma once

#include "Entity.h"
#include "EntityManager.h"
#include "Scene.h"
#include "Scene_Play.h"
#include "Scene_Menu.h"
#include "Assets.h"
#include <SFML/Graphics.hpp>

/*  GameEngine to manage different game scenes, load assets from config, storing window, convert user
    input and run the game
*/

typedef std::map<std::string, std::shared_ptr<Scene>> SceneMap;

class GameEngine
{
private:
    sf::RenderWindow                m_window;           
    SceneMap                        m_sceneMap;
    Assets                          m_assets;
    std::string                     m_currentScene;
    bool                            m_running{true};   

    std::shared_ptr<Entity>     m_player;

    void init(const std::string& path);                
    void update();
    void sUserInput();
    std::shared_ptr<Scene> currentScene();

public:
    GameEngine(const std::string& path);        

    void run();
    void quit();

    void changeScene(const std::string& sceneName, std::shared_ptr<Scene> scene, bool endCurrentScene = false);
    const Assets& getAssets();
    sf::RenderWindow& window();

    bool isRunning();

};
