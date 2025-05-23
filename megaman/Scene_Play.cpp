#include "Scene_Play.h"
#include "Scene_Menu.h"
#include <iostream>
#include <fstream>
#include <cmath>

const float EPSILON = 0.05f;

Scene_Play::Scene_Play(std::shared_ptr<GameEngine> game, const std::string& levelPath)
    : Scene(game), m_levelPath{levelPath}
    {
        init(m_levelPath);
    }

// load level through config file, register key and button maps
void Scene_Play::init(const std::string& levelPath)
{
    registerAction(sf::Keyboard::Key::P,        "PAUSE");
    registerAction(sf::Keyboard::Key::Escape,   "QUIT");
    registerAction(sf::Keyboard::Key::T,        "TOGGLE_TEXTURE");
    registerAction(sf::Keyboard::Key::C,        "TOGGLE_COLLISION");
    registerAction(sf::Keyboard::Key::G,        "TOGGLE_GRID");
    registerAction(sf::Keyboard::Key::W,        "JUMP");
    registerAction(sf::Keyboard::Key::A,        "LEFT");
    registerAction(sf::Keyboard::Key::D,        "RIGHT");
    registerAction(sf::Mouse::Button::Left,     "SHOOT");
    
    loadLevel(levelPath);
}

// load level: load background texture, read config file
void Scene_Play::loadLevel(const std::string& filename)
{
    const sf::Texture& background = m_game->getAssets().getTexture("Background_Background");
    sf::Sprite sp{background};
    m_background = sp;
    float windowWidth = static_cast<float>(m_game->window().getSize().x);
    float windowHeight = static_cast<float>(m_game->window().getSize().y);
    float textureWidth = static_cast<float>(background.getSize().x);
    float textureHeight = static_cast<float>(background.getSize().y);
    
    m_background->setScale(sf::Vector2f(windowWidth/textureWidth, windowHeight/textureHeight));
    m_background->setOrigin(sf::Vector2f(0.f, 0.f));
    m_background->setPosition(sf::Vector2f(0.f, 0.f));
    
    std::ifstream fin(filename);
    std::string word{};
    // load config
    for (int i{0}; i < 4; i++)
    {
        fin >> word;
        if (word == "Player")
        {
            fin >> m_playerConfig.speed >> m_playerConfig.gridScale_x
                >> m_playerConfig.gridScale_y >> m_playerConfig.lifeSpan;
        }
        else if (word == "Tile")
        {
            fin >> m_tileConfig.speed >> m_tileConfig.gridScale_x
                >> m_tileConfig.gridScale_y >> m_tileConfig.lifeSpan;
        }
        if (word == "Enemy")
        {
            fin >> m_enemyConfig.speed >> m_enemyConfig.gridScale_x
                >> m_enemyConfig.gridScale_y >> m_enemyConfig.lifeSpan;
        }
        if (word == "Bullet")
        {
            fin >> m_bulletConfig.type >> m_bulletConfig.speed >> m_bulletConfig.gridScale_x
                >> m_bulletConfig.gridScale_y >> m_bulletConfig.lifeSpan;
        }
    }

    // load level
    std::string type{};
    float x, y;
    while (fin >> word)
    {
        fin >> type >> x >> y;
        if (word == "Player")
        {
            m_playerConfig.origX = x;
            m_playerConfig.origY = y;
            spawnPlayer(m_playerConfig.origX, m_playerConfig.origY, type);
        }
        else if (word == "Enemy")
        {
            m_enemyConfig.origX = x;
            m_enemyConfig.origY = y;
            spawnEnemy(m_enemyConfig.origX, m_enemyConfig.origY, type);
        }
        else if (word == "Tile")
        {
            spawnTile(x, y, type);
        }
    }
}

// add animations whenever player satisifes a certain state: jump, idle and run animations
// and when the animation isn't already playing - otherwise the animation frame resets
void Scene_Play::sAnimation()
{
    std::string origAnim = m_player->getComponent<CAnimation>().animation.getName();
    if (m_player->getComponent<CState>().onGround == false && origAnim != "Player_Jump")
    {
        const sf::Texture& playerTexture = m_game->getAssets().getTexture("Player_Jump"); 
        Animation playerAnimation("Player_Jump", playerTexture, 1, 10);
        m_player->addComponent<CAnimation>(playerAnimation);
    }
    else if (m_player->getComponent<CState>().isIdle && origAnim != "Player_Idle")
    {
        const sf::Texture& playerTexture = m_game->getAssets().getTexture("Player_Idle"); 
        Animation playerAnimation("Player_Idle", playerTexture, 2, 10);
        m_player->addComponent<CAnimation>(playerAnimation);
    }
    else if (m_player->getComponent<CState>().isRunning && origAnim != "Player_Run")
    {
        const sf::Texture& playerTexture = m_game->getAssets().getTexture("Player_Run"); 
        Animation playerAnimation("Player_Run", playerTexture, 3, 10);
        m_player->addComponent<CAnimation>(playerAnimation);
    }
    
    // update animations    
    for (auto& e: m_entities.getEntities())
    {
        if (e->hasComponent<CAnimation>() && e->hasComponent<CState>())
            e->getComponent<CAnimation>().animation.update();
    }
}

// convert user input to movement
void Scene_Play::sMovement()
{
    // jump input - can only jump when on ground
    if (m_player->getComponent<CInput>().jump && m_player->getComponent<CState>().onGround 
        && !m_player->getComponent<CJumpTimer>().isJumping)
    {
        m_player->getComponent<CTransform>().velocity.y = -m_playerConfig.speed * m_player->getComponent<CJumpTimer>().speedMultiplier;
        m_player->getComponent<CState>().onGround = false;
        m_player->getComponent<CJumpTimer>().isJumping = true;
        m_player->getComponent<CJumpTimer>().currentFrame = 0;
    }
    // continuous jump so that it is not abrupt
    if (m_player->getComponent<CJumpTimer>().isJumping)
    {
        m_player->getComponent<CJumpTimer>().currentFrame++;
        if (m_player->getComponent<CJumpTimer>().currentFrame > m_player->getComponent<CJumpTimer>().duration)
        {
            m_player->getComponent<CJumpTimer>().isJumping = false; // End the jump
        }
        // keep applying upward velocity
        m_player->getComponent<CTransform>().velocity.y = -m_playerConfig.speed * m_player->getComponent<CJumpTimer>().speedMultiplier;
    }

    // left and right inputs - can only move horizontally on ground
    if (m_player->getComponent<CInput>().left && m_player->getComponent<CState>().onGround)
    {
        m_player->getComponent<CTransform>().velocity.x = -m_playerConfig.speed;
        m_player->getComponent<CState>().facingRight = false;
    }
    if (m_player->getComponent<CInput>().right && m_player->getComponent<CState>().onGround)
    {   
        m_player->getComponent<CTransform>().velocity.x = m_playerConfig.speed;
        m_player->getComponent<CState>().facingRight = true;
    }
    // add friction on ground
    if (m_player->getComponent<CState>().onGround)
    {
        m_player->getComponent<CTransform>().velocity.x *= m_player->getComponent<CFriction>().friction;
    }

    // shoot input
    if (m_player->getComponent<CInput>().shoot && !m_player->getComponent<CState>().isFiring)
    {
        spawnBullet(m_bulletConfig.type);
        m_player->getComponent<CState>().isFiring = true;
    }
    if (!m_player->getComponent<CInput>().shoot)
    {
        m_player->getComponent<CState>().isFiring = false;
    }

    for (auto& e: m_entities.getEntities())
    {   
        // set vertical velocity to zero if on ground
        if (e->hasComponent<CState>())
        {
            if (e->getComponent<CState>().onGround == true)
            {
                e->getComponent<CTransform>().velocity.y = 0.f;
            }

            // add window borders 
            if (e->getComponent<CTransform>().pos.x - e->getComponent<CBoundingBox>().size.x / 2.f < 0) 
            {
                // entity is past the left border
                if (e->tag() == "Bullet") e->destroy();
                e->getComponent<CTransform>().pos.x = e->getComponent<CBoundingBox>().size.x / 2.f; 
                if (e->tag() == "Enemy") 
                {
                    e->getComponent<CTransform>().velocity.x *= -1;
                    e->getComponent<CState>().facingRight = true;
                }
                else e->getComponent<CTransform>().velocity.x = 0.f; 
                
            }
            else if (e->getComponent<CTransform>().pos.x + e->getComponent<CBoundingBox>().size.x / 2.f > m_game->window().getSize().x) 
            {
                // entity is past the right border
                if (e->tag() == "Bullet") e->destroy();
                e->getComponent<CTransform>().pos.x = m_game->window().getSize().x - e->getComponent<CBoundingBox>().size.x / 2.f; 
                if (e->tag() == "Enemy") 
                {
                    e->getComponent<CTransform>().velocity.x *= -1;
                    e->getComponent<CState>().facingRight = false;
                }
                else e->getComponent<CTransform>().velocity.x = 0.f; 
            }

            if (e->getComponent<CTransform>().pos.y - e->getComponent<CBoundingBox>().size.y / 2.f < 0) 
            {
                // entity is past the top border
                if (e->tag() == "Bullet") e->destroy();
                e->getComponent<CTransform>().pos.y = e->getComponent<CBoundingBox>().size.y / 2.f; 
                e->getComponent<CTransform>().velocity.y = 0.f; 
            }
            else if (e->getComponent<CTransform>().pos.y + e->getComponent<CBoundingBox>().size.y / 2.f > m_game->window().getSize().y) 
            {
                // entity is past the bottom border
                e->destroy();
            }

            // add gravity
            if (e->hasComponent<CGravity>())
                e->getComponent<CTransform>().velocity.y += e->getComponent<CGravity>().gravity;
            
            // since with friciton velocity will not truly be zero - set states (used for animation) against epsilon instead
            if (std::fabs(e->getComponent<CTransform>().velocity.x) > EPSILON && e->getComponent<CState>().onGround)
            {
                e->getComponent<CState>().isRunning = true;
                e->getComponent<CState>().isIdle = false;
            }
            else if (std::fabs(e->getComponent<CTransform>().velocity.x < EPSILON) && e->getComponent<CState>().onGround)
            {
                e->getComponent<CState>().isRunning = false;
                e->getComponent<CState>().isIdle = true;
            }
        }
        
        // update previous position
        e->getComponent<CTransform>().prevPos = e->getComponent<CTransform>().pos;
        Vec2 newPos = e->getComponent<CTransform>().pos + e->getComponent<CTransform>().velocity;

        // move
        e->getComponent<CTransform>().pos = newPos;
    }
}

// collision interactions
void Scene_Play::sCollision()
{
    for (auto& e1: m_entities.getEntities())
    {
        for (auto& e2: m_entities.getEntities())
        {
        // Axis Aligned Bounding Boxes (AABB)
        if (e1->hasComponent<CBoundingBox>() && e2->hasComponent<CBoundingBox>() &&
                e1->hasComponent<CTransform>() && e2->hasComponent<CTransform>())
            {
                auto& bbox1 = e1->getComponent<CBoundingBox>();
                auto& bbox2 = e2->getComponent<CBoundingBox>();
                auto& transform1 = e1->getComponent<CTransform>();
                auto& transform2 = e2->getComponent<CTransform>();

                // current frame overlap
                Vec2 delta = {abs(transform1.pos.x - transform2.pos.x), abs(transform1.pos.y - transform2.pos.y)};
                float ox = bbox1.size.x/2.f + bbox2.size.x/2.f - delta.x;
                float oy = bbox1.size.y/2.f + bbox2.size.y/2.f - delta.y;

                if (ox > 0 && oy > 0)
                {
                    Vec2 overlap = {ox, oy};
                    // previous frame overlap to tell direction of approach
                    Vec2 delta2 = {abs(transform1.prevPos.x - transform2.prevPos.x), abs(transform1.prevPos.y - transform2.prevPos.y)};
                    float ox_prev = bbox1.size.x/2.f + bbox2.size.x/2.f - delta2.x;
                    float oy_prev = bbox1.size.y/2.f + bbox2.size.y/2.f - delta2.y;
                    Vec2 overlap_prev = {ox_prev, oy_prev};
                    resolveCollision(e1, e2, overlap, overlap_prev);
                }
            }   
        }
    }
}

// resolve collisions
void Scene_Play::resolveCollision(std::shared_ptr<Entity> e1, std::shared_ptr<Entity> e2, Vec2& overlap, Vec2& overlap_prev)
{
    std::string tag1 = e1->tag();
    std::string tag2 = e2->tag();

    auto& transform1 = e1->getComponent<CTransform>();
    auto& transform2 = e2->getComponent<CTransform>();

    if ((tag1 == "Tile" && tag2 != "Tile") || (tag2 == "Tile" && tag1 != "Tile"))
    {  
        if (tag2 != "Tile")
        {
            if (tag2 == "Bullet") e2->destroy();
            // previous position horizontal overlap - coming from top or bottom
            if (overlap_prev.x > 0)
            {
                // coming from the top
                if (transform2.prevPos.y < transform1.prevPos.y) 
                {
                    transform2.pos.y -= (overlap.y + 1);
                    e2->getComponent<CState>().onGround = true;

                }
                // coming from the bottom
                else transform2.pos.y += (overlap.y + 1);
            }
            // previous position vertical overlap - coming from the sides
            else if (overlap_prev.y > 0)
            {
                // coming from the right
                if (transform2.prevPos.x > transform1.prevPos.x)
                {
                    transform2.pos.x += (overlap.x + 1);
                    if (tag2 == "Enemy") 
                    {
                        transform2.velocity.x *= -1;
                        e2->getComponent<CState>().facingRight = true;
                    }
                }
                // coming from the left
                else 
                {
                    transform2.pos.x -= (overlap.x + 1);
                    if (tag2 == "Enemy") 
                    {
                        transform2.velocity.x *= -1;
                        e2->getComponent<CState>().facingRight = false;
                    }
                }
            }
        }
        else if (tag1 != "Tile")
        {
            if (tag1 == "Bullet") e1->destroy();
            // previous position horizontal overlap - coming from top or bottom
            if (overlap_prev.x > 0)
            {
                // coming from the top
                if (transform1.prevPos.y < transform2.prevPos.y) 
                {
                        transform1.pos.y -= (overlap.y + 1);
                        e1->getComponent<CState>().onGround = true;
                }
                // coming from the bottom
                else transform1.pos.y += (overlap.y + 1);
            }
            // previous position vertical overlap - coming from the sides
            else if (overlap_prev.y > 0)
            {
                // coming from the right
                if (transform1.prevPos.x > transform2.prevPos.x) 
                {
                    transform1.pos.x += (overlap.x + 1);
                    if (tag1 == "Enemy") 
                    {
                        transform1.velocity.x *= -1;
                        e1->getComponent<CState>().facingRight = true;
                    }
                }
                // coming from the left
                else 
                {
                    transform1.pos.x -= (overlap.x + 1);
                    if (tag1 == "Enemy") 
                    {
                        transform1.velocity.x *= -1;
                        e1->getComponent<CState>().facingRight = false;
                    }
                }
            }  
        }
        
    }
    if ((tag1 == "Player" && tag2 == "Enemy") || (tag2 == "Player" && tag1 == "Enemy"))
    {
        if (tag1 == "Player") e1->destroy();
        else if (tag2 == "Player") e2->destroy();
    }    

    if ((tag1 == "Bullet" && tag2 == "Enemy") || (tag2 == "Bullet" && tag1 == "Enemy"))
    {
        e1->destroy();
        e2->destroy();
    }
}

void Scene_Play::sRender()
{   
    m_game->window().clear();

    m_game->window().draw(*m_background);
    // debug for grid
    if (m_drawGrid)
    {
        drawGrid();
    }

    for (auto e : m_entities.getEntities())
    {
        // get the Transform component to set the position
        if (e->hasComponent<CTransform>())
        {
            auto& transform = e->getComponent<CTransform>();
            sf::Vector2f position(transform.pos.x, transform.pos.y);

            // if the entity has an Animation component, get its sprite and set the position
            if (e->hasComponent<CAnimation>())
            {
                auto& animation = e->getComponent<CAnimation>();
                if (auto spritePtr = animation.animation.getSprite())
                {   
                    // scale
                    float spriteSize_x = animation.animation.getFrameSize().x;
                    float spriteSize_y = animation.animation.getFrameSize().y;
                    transform.scale.x = m_gridSize.x/spriteSize_x;
                    transform.scale.y = m_gridSize.y/spriteSize_y;
                    if (e->hasComponent<CState>())
                    {
                        if (!e->getComponent<CState>().facingRight)
                        transform.scale.x = m_gridSize.x/spriteSize_x * -1.f;
                    }
                    spritePtr->setScale(sf::Vector2f(transform.scale.x, transform.scale.y));
                    spritePtr->setPosition(position);
                    m_game->window().draw(*spritePtr);
                }
            }
        }

        // debug for bounding boxes
        if (m_drawCollision)
        {
            if (e->hasComponent<CBoundingBox>())
            {
                auto& bbox = e->getComponent<CBoundingBox>();
                // draw debug bounding box 
                sf::RectangleShape box(sf::Vector2f(bbox.size.x, bbox.size.y));
                box.setFillColor(sf::Color(255,255,255,50));
                box.setOrigin(sf::Vector2f(bbox.size.x/2.f, bbox.size.y/2.f));
                box.setPosition(sf::Vector2f(e->getComponent<CTransform>().pos.x,e->getComponent<CTransform>().pos.y));
                m_game->window().draw(box);
            }
        }
    }
    m_game->window().display();
}

// unused
void Scene_Play::sDoAction()
{

}

// update the scene
void Scene_Play::update()
{
    if (!m_paused)
    {
        m_entities.update();
        // respawn player
        if (!m_player->isActive())
            spawnPlayer(m_playerConfig.origX, m_playerConfig.origY, "Idle");
        sMovement();
        sCollision();
        sAnimation();

        m_currentFrame++;
    }
    if (m_hasEnded)
    {
        std::shared_ptr<Scene> menuScene = std::make_shared<Scene_Menu>(m_game);
        m_game->changeScene("Menu", menuScene, false);
    }
}

// unused
void Scene_Play::simulate()
{

}

// do actions
void Scene_Play::doAction(Action& act)
{
    if (act.name() == "PAUSE")
    {   
        // Handle PAUSE action
        if (act.type() == "START") m_paused = true;
        else if (act.type() == "END") m_paused = false;
    }
    else if (act.name() == "QUIT")
    {
        // Handle QUIT action
        if (act.type() == "START") m_hasEnded = true;
        else if (act.type() == "END") m_hasEnded = false;
    }
    else if (act.name() == "TOGGLE_TEXTURE")
    {
        // Handle TOGGLE_TEXTURE action
        
    }
    else if (act.name() == "TOGGLE_COLLISION")
    {
        // Handle TOGGLE_COLLISION action
        if (act.type() == "START") m_drawCollision = true;
        else if (act.type() == "END") m_drawCollision = false;
    }
    else if (act.name() == "TOGGLE_GRID")
    {
        // Handle TOGGLE_GRID action
        if (act.type() == "START") m_drawGrid = true;
        else if (act.type() == "END") m_drawGrid = false;
    }
    else if (act.name() == "JUMP")
    {
        // Handle JUMP action
        if (act.type() == "START") m_player->getComponent<CInput>().jump = true;
        else if (act.type() == "END") m_player->getComponent<CInput>().jump = false;
    }
    else if (act.name() == "LEFT")
    {
        // Handle LEFT action
        if (act.type() == "START") m_player->getComponent<CInput>().left = true;
        else if (act.type() == "END") m_player->getComponent<CInput>().left = false;
    }
    else if (act.name() == "RIGHT")
    {
        // Handle RIGHT action
        if (act.type() == "START") m_player->getComponent<CInput>().right = true;
        else if (act.type() == "END") m_player->getComponent<CInput>().right = false;
    }
    else if (act.name() == "SHOOT")
    {
        // Handle SHOOT action
        if (act.type() == "START") m_player->getComponent<CInput>().shoot = true;
        else if (act.type() == "END") m_player->getComponent<CInput>().shoot = false;
    }
    // Add more else if conditions for other actions
    else
    {
        // Handle unknown or default action
    }
}

// calculate position with grid positions
Vec2 Scene_Play::gridToMidPixel(float gridX, float gridY, std::shared_ptr<Entity> entity)
{
    // (0,0) is bottom left of the window as opposed to SFML top left 
    // bottom left corner of Animation align with bottom left of grid cell
    auto& animation = entity->getComponent<CAnimation>();
    float xpos = gridX * m_gridSize.x + m_gridSize.x/2.f;
    float ypos = m_game->window().getSize().y - (gridY * m_gridSize.y + m_gridSize.y/2.f);
    return Vec2{xpos, ypos};
}

void Scene_Play::spawnEnemy(float x, float y, std::string type)
{
    std::string enemyName = "Enemy_" + type;
    // create enemy
    auto entity{m_entities.addEntity("Enemy")};
    
    // add animations 
    const sf::Texture& enemyTexture = m_game->getAssets().getTexture(enemyName); 
    Animation enemyAnimation(enemyName, enemyTexture, 2, 10);
    entity ->addComponent<CAnimation>(enemyAnimation);

    // add transform
    Vec2 pos = gridToMidPixel(x, y, entity);
    entity->addComponent<CTransform>(pos, Vec2(m_enemyConfig.speed,0.f), Vec2(1.f,1.f), 0.f);

    // add bounding box
    entity->addComponent<CBoundingBox>(Vec2(m_gridSize.x * m_enemyConfig.gridScale_x, 
        m_gridSize.y * m_enemyConfig.gridScale_y));

    // add lifespan
    entity->addComponent<CLifespan>(m_enemyConfig.lifeSpan);

    // add gravity
    entity->addComponent<CGravity>();

    // add state
    entity->addComponent<CState>();
}

void Scene_Play::spawnPlayer(float x, float y, std::string type)
{
    std::string playerName = "Player_" + type;
    // create player
    m_player = m_entities.addEntity("Player");

    // add animations - initialize player as idle
    const sf::Texture& playerTexture = m_game->getAssets().getTexture(playerName); 
    Animation playerAnimation(playerName, playerTexture, 2, 10);
    m_player->addComponent<CAnimation>(playerAnimation);

    // add transform
    Vec2 pos = gridToMidPixel(x, y, m_player);
    m_player->addComponent<CTransform>(pos, Vec2(0.f,0.f), Vec2(1.f,1.f), 0.f);

    // add bounding box
    m_player->addComponent<CBoundingBox>(Vec2(m_gridSize.x * m_playerConfig.gridScale_x, 
        m_gridSize.y * m_playerConfig.gridScale_y));

    // add lifespan
    m_player->addComponent<CLifespan>(m_playerConfig.lifeSpan);

    // add input
    m_player->addComponent<CInput>();

    // add gravity
    m_player->addComponent<CGravity>();

    // add friction
    m_player->addComponent<CFriction>();

    // add state
    m_player->addComponent<CState>();

    // add jump timer
    m_player->addComponent<CJumpTimer>();
}

void Scene_Play::spawnBullet(std::string type)
{
    std::string bulletName = "Bullet_" + type;

    auto entity{m_entities.addEntity("Bullet")};

    // add animation
    const sf::Texture& bulletTexture = m_game->getAssets().getTexture(bulletName);

    Animation bulletAnimation(bulletName, bulletTexture, 8, 10);
    entity->addComponent<CAnimation>(bulletAnimation);

    // add state
    entity->addComponent<CState>();
    
    // add transform
    if (m_player->getComponent<CState>().facingRight)
    {
        entity->addComponent<CTransform>(m_player->getComponent<CTransform>().pos, Vec2(m_bulletConfig.speed,0.f), 
        Vec2(1.f,1.f), 0.f);
        entity->getComponent<CState>().facingRight = true;
    }
    else
    {
        entity->addComponent<CTransform>(m_player->getComponent<CTransform>().pos, Vec2(-m_bulletConfig.speed,0.f), 
        Vec2(1.f,1.f), 0.f);
        entity->getComponent<CState>().facingRight = false;
    }
    
    // add bounding box
    entity->addComponent<CBoundingBox>(Vec2(m_gridSize.x * m_bulletConfig.gridScale_x, 
        m_gridSize.y * m_bulletConfig.gridScale_y));

    // add lifespan
    entity->addComponent<CLifespan>(m_bulletConfig.lifeSpan);
}

void Scene_Play::spawnTile(float x, float y, std::string type)
{
    std::string tileName = "Tile_" + type;

    auto entity{m_entities.addEntity("Tile")};
    // add animation
    const sf::Texture& tileTexture = m_game->getAssets().getTexture(tileName);
    Animation tileAnimation(tileName, tileTexture, 1, m_tileConfig.speed);
    entity->addComponent<CAnimation>(tileAnimation);

    // add transform
    Vec2 pos = gridToMidPixel(x, y, entity);
    entity->addComponent<CTransform>(pos, Vec2(0.f,0.f), Vec2(1.f,1.f), 0.f);

    // add bounding box
    entity->addComponent<CBoundingBox>(Vec2(m_gridSize.x * m_tileConfig.gridScale_x, 
        m_gridSize.y * m_tileConfig.gridScale_y));

    // add lifespan
    entity->addComponent<CLifespan>(m_tileConfig.lifeSpan);

}

// draw grid debug
void Scene_Play::drawGrid(sf::Color lineColor)
{
    unsigned int windowWidth = m_game->window().getSize().x;
    unsigned int windowHeight = m_game->window().getSize().y;
    sf::Vector2f gridOrigin = {0.f, static_cast<float>(windowHeight)}; 

    int gridWidth = static_cast<int>(windowWidth / m_gridSize.x);
    int gridHeight = static_cast<int>(windowHeight / m_gridSize.y);

    sf::VertexArray lines(sf::PrimitiveType::Lines);

    // draw vertical lines 
    for (int i = 0; i <= gridWidth; ++i)
    {
        float x = gridOrigin.x + static_cast<float>(i) * m_gridSize.x;
        lines.append(sf::Vertex{{x, gridOrigin.y}, lineColor});
        lines.append(sf::Vertex{{x, gridOrigin.y - static_cast<float>(gridHeight) * m_gridSize.y}, lineColor}); 
    }

    // draw horizontal lines 
    for (int j = 0; j <= gridHeight; ++j)
    {
        float y = gridOrigin.y - static_cast<float>(j) * m_gridSize.y; 
        lines.append(sf::Vertex{{gridOrigin.x, y}, lineColor});
        lines.append(sf::Vertex{{gridOrigin.x + static_cast<float>(gridWidth) * m_gridSize.x, y}, lineColor}); 
    }
    m_game->window().draw(lines);
}
