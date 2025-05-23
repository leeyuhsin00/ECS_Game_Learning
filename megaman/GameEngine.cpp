#include "GameEngine.h"
#include <iostream>
#include <fstream>

GameEngine::GameEngine(const std::string& path)
{
    init(path);
}

// initialize the GameState reading in config
void GameEngine::init(const std::string& path)
{
    std::ifstream fin(path);
    std::string word{};

    fin >> word;
    int W, H, FR, FS;
    if (word == "Window")
    {
        fin >> W >> H >> FR >> FS;
    }
    sf::Vector2u size(static_cast<unsigned int>(W), static_cast<unsigned int>(H));
    m_window.create(sf::VideoMode(size), "Game", FS ? sf::State::Fullscreen : sf::State::Windowed);
    m_window.setFramerateLimit(FR);

    fin >> word;
    std::string config;
    int S, R, G, B;
    std::string type;
    if (word == "Font")
    {
        fin >> type >> config >> S >> R >> G >> B;
        m_assets.addFont("Font_" + type, config);
    }

    while (fin >> word)
    {
        if (word == "Tile")
        {
            fin >> type >> config;
            m_assets.addTexture("Tile_"+ type, config);
        }

        else if (word == "Player")
        {
            fin >> type >> config;
            m_assets.addTexture("Player_" + type, config);
        }

        else if (word == "Enemy")
        {
            fin >> type >> config;
            m_assets.addTexture("Enemy_" + type, config);
        }

        else if (word == "Bullet")
        {
            fin >> type >> config;
            m_assets.addTexture("Bullet_" + type, config);
        }

        else if (word == "Background")
        {
            fin >> type >> config;
            m_assets.addTexture("Background_" + type, config);
        }
    }
}  

// system: user input
void GameEngine::sUserInput()
{   
    while (const std::optional event = m_window.pollEvent())
    {
        // stop running if window is closed
        if (event->is<sf::Event::Closed>())
        {
            m_running = false;
        }

        std::shared_ptr<Scene> currentS = currentScene();

        // when key is pressed or released
        if (currentS)
        {
            if (event->is<sf::Event::KeyPressed>())
            {
                sf::Keyboard::Key key = event->getIf<sf::Event::KeyPressed>()->code;
                currentS->sDoAction(key, "START");
            }
            else if (event->is<sf::Event::KeyReleased>())
            {
                sf::Keyboard::Key key = event->getIf<sf::Event::KeyReleased>()->code;
                currentS->sDoAction(key, "END");
            }
            else if (event->is<sf::Event::MouseButtonPressed>())
            {
                sf::Mouse::Button button = event->getIf<sf::Event::MouseButtonPressed>()->button;
                currentS->sDoAction(button, "START");
            }
            else if (event->is<sf::Event::MouseButtonReleased>())
            {
                sf::Mouse::Button button = event->getIf<sf::Event::MouseButtonReleased>()->button;
                currentS->sDoAction(button, "END");
            }
        }
    }
}   

// retrieve the current scene via name
std::shared_ptr<Scene> GameEngine::currentScene()
{
    return m_sceneMap[m_currentScene];
}

void GameEngine::quit()
{
    m_running = false;
}

// change to new scene using shared ptr
void GameEngine::changeScene(const std::string& sceneName, std::shared_ptr<Scene> scene, bool endCurrentScene)
{
    if (endCurrentScene && m_sceneMap.count(m_currentScene) > 0)
    {
        m_sceneMap.erase(m_currentScene); 
    }
    m_currentScene = sceneName;
    m_sceneMap[sceneName] = scene;
}

sf::RenderWindow& GameEngine::window()
{
    return m_window;
}

bool GameEngine::isRunning()
{
    return m_running;
}

const Assets& GameEngine::getAssets()
{
    return m_assets;
}

void GameEngine::update()
{
    if (std::shared_ptr<Scene> currentS = currentScene())
    {
        currentS->update();
    }
}

void GameEngine::run()
{
    while (isRunning())
    {
        // handle user input
        sUserInput();

        // update game state
        update();

        // render current scene
        std::shared_ptr<Scene> currentS = currentScene();
        currentS->sRender();
    }

    // quit the game
    quit();
}
