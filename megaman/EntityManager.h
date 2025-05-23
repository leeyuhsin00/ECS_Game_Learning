#pragma once

#include <vector>
#include <map>
#include <memory>
#include "Entity.h"

/*  EntityManager class to manage all entities
    member variables: m_entitiesToAdd - delayed push back of entity; entity_map for ease of retrieving entities
    member functions: getEntities() - retrieve ALL entities; getEntities(tag) - retrieve entity by tag
*/

typedef std::vector <std::shared_ptr<Entity>>   EntityVec;
typedef std::map    <std::string, EntityVec>    EntityMap;   

class EntityManager
{
private:
    EntityVec   m_entities;
    EntityVec   m_entitiesToAdd;
    EntityMap   m_entityMap;
    size_t      m_totalEntities{0};

public:
    EntityManager();
    void update();
    void removeDeadEntities(EntityVec& vec);
    std::shared_ptr<Entity> addEntity(const std::string& tag);
    const EntityVec& getEntities();
    const EntityVec& getEntities(const std::string& tag);
};
