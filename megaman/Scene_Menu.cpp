#include "Scene_Menu.h"

Scene_Menu::Scene_Menu(std::shared_ptr<GameEngine> game)
    : Scene{game}
{
    init();
}

// initialize menu scene - set key and button maps, create sf::Text for level display
void Scene_Menu::init()
{
    registerAction(sf::Keyboard::Key::Escape,   "QUIT");
    registerAction(sf::Keyboard::Key::W,        "UP");
    registerAction(sf::Keyboard::Key::S,        "DOWN");
    registerAction(sf::Keyboard::Key::Enter,    "SELECT");

    sf::Font font = m_game->getAssets().getFont("Font_Arial");
    m_font = font; 
    std::unique_ptr<sf::Text> text = std::make_unique<sf::Text>(m_font);
    text->setCharacterSize(50);
    text->setFillColor(sf::Color::White);
    text->setString("MENU");

    m_text["menu"] = std::move(text);

    for (int i{0}; i<maxLevel; i++)
    {
        std::unique_ptr<sf::Text> text = std::make_unique<sf::Text>(m_font);
        text->setCharacterSize(50);
        text->setFillColor(sf::Color::White);
        int windowHeight = m_game->window().getSize().y;
        text->setPosition(sf::Vector2f(0.f, (i+1)*windowHeight/(maxLevel+1)));
        std::string levelName = "LEVEL " + std::to_string(i);
        text->setString(levelName);
        m_text[levelName] = std::move(text);
    }
}

// display texts
void Scene_Menu::sRender()
{
    m_game->window().clear();
    m_game->window().draw(*m_text["menu"]);

    for (int i{0}; i<maxLevel; i++)
    {
        std::string levelName = "LEVEL " + std::to_string(i);
        m_game->window().draw(*m_text[levelName]);
    }

    m_game->window().display();
}

void Scene_Menu::sDoAction()
{

}

// create shared ptrs to different scene play levels, change to those scenes when selected
void Scene_Menu::update()
{
    simulate();

    if (loadLevel)
    {
        std::string levelName = "LEVEL " + std::to_string(levelSelect);
        if (levelSelect == 0)
        {
            std::shared_ptr<Scene> playScene = std::make_shared<Scene_Play>(m_game, "config/levelPath.txt");
            m_game->changeScene(levelName, playScene, false);
        }
        else if (levelSelect == 1)
        {
            std::shared_ptr<Scene> playScene = std::make_shared<Scene_Play>(m_game, "config/levelPath2.txt");
            m_game->changeScene(levelName, playScene, false);
        }
        
        loadLevel = false;
    }
}

// change color when user selects level
void Scene_Menu::simulate()
{
    std::string levelName;
    for (int i{0}; i<maxLevel; i++)
    {
        levelName = "LEVEL " + std::to_string(i);
        if (i == levelSelect)
        { 
            m_text[levelName]->setFillColor(sf::Color::Blue);
        }
        else
        {
            m_text[levelName]->setFillColor(sf::Color::White);
        }
    }
}

// update base on actions
void Scene_Menu::doAction(Action& act)
{
    if (act.name() == "QUIT")
    {   
        // handle QUIT action
        if (act.type() == "START") m_hasEnded = true;
        else if (act.type() == "END") m_hasEnded = false;
    }
    else if (act.name() == "UP")
    {   
        // handle UP action
        if (act.type() == "START") levelSelect = (levelSelect - 1 + maxLevel) % maxLevel;
    }
    else if (act.name() == "DOWN")
    {   
        // handle DOWN action
        if (act.type() == "START") levelSelect = (levelSelect + 1 + maxLevel) % maxLevel;
    }
    else if (act.name() == "SELECT")
    {   
        // handle SELECT action
        if (act.type() == "START") loadLevel = true;
    }
}
