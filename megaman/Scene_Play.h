#pragma once

#include "Scene.h"
#include "GameEngine.h"

class Scene_Menu;

typedef std::map<std::string, Animation> AnimationMap;

/*  scene play class inherited from scene class containing game level movement, collision, animations
    user input, spawn entities etc.; draw level through grids
    different configs for different entities loaded in through another config file
    gridToMidPixel - since all textures origin set to center, calculate the XY position of the origin
    an entity should be set given the grid location
*/

class Scene_Play : public Scene
{   
private:
    struct PlayerConfig {int lifeSpan, origX, origY; float speed, gridScale_x, gridScale_y;};
    struct EnemyConfig {int lifeSpan, origX, origY; float speed, gridScale_x, gridScale_y;};
    struct TileConfig {int lifeSpan; float speed, gridScale_x, gridScale_y;};
    struct BulletConfig {int lifeSpan; float speed, gridScale_x, gridScale_y; std::string type;};

protected:
    std::string             m_levelPath;
    std::shared_ptr<Entity> m_player;
    PlayerConfig            m_playerConfig;
    EnemyConfig             m_enemyConfig;
    TileConfig              m_tileConfig;
    BulletConfig            m_bulletConfig;

    AnimationMap            m_playerAnimationMap;
    AnimationMap            m_tileAnimationMap;
    AnimationMap            m_enemyAnimationMap;

    bool                    m_drawtextures{false};
    bool                    m_drawGrid{false};
    bool                    m_drawCollision{false};
    const Vec2              m_gridSize = {64, 64};
    std::optional<sf::Text> m_gridText;
    std::optional<sf::Sprite> m_background;

    void init(const std::string& levelPath);
    void loadLevel(const std::string& filename);
    Vec2 gridToMidPixel(float gridX, float gridY, std::shared_ptr<Entity> entity);

    // system
    void sAnimation();
    void sMovement();
    void sCollision();
    void resolveCollision(std::shared_ptr<Entity> e1, std::shared_ptr<Entity> e2, Vec2& overlap, Vec2& overlap_prev);
    virtual void sRender() override;
    virtual void sDoAction();

    void spawnEnemy(float x, float y, std::string type);
    void spawnPlayer(float x, float y, std::string type);
    void spawnTile(float x, float y, std::string type);
    void spawnBullet(std::string type);

    void drawGrid(sf::Color lineColor = sf::Color(255, 255, 255));

public:
    Scene_Play(std::shared_ptr<GameEngine> g, const std::string& levelPath);

    virtual void update() override;
    virtual void simulate();
    virtual void doAction(Action& act);
};
