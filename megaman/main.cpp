#include <SFML/Graphics.hpp>
#include "GameEngine.h"
#include <iostream>

int main()
{
    // initalize game engine reading config file
    std::shared_ptr<GameEngine> game = std::make_shared<GameEngine>("config/config.txt");

    // create menu scene ptr
    std::shared_ptr<Scene> menuScene = std::make_shared<Scene_Menu>(game);

    // change to the menu scene
    game->changeScene("MENU", menuScene, false);  
    
    // run and render
    game->run();

    
    return 0;
}
