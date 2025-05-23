#include "EntityManager.h"
#include <algorithm>
#include <iostream>

EntityManager::EntityManager()
{}

void EntityManager::update()
{
    // add entities from m_entitlesToAdd
    for (auto e: m_entitiesToAdd)
    {
        m_entities.push_back(e);
        m_entityMap[e->tag()].push_back(e);
    }

    // clear to prevent repeated additions
    m_entitiesToAdd.clear();

    // remove dead entities from the vector of all entities
    removeDeadEntities(m_entities);

    // remove dead entities from each vector in the entity map
    for (auto& [tag, entityVec] : m_entityMap)
    {
        removeDeadEntities(entityVec);
    }
}

void EntityManager::removeDeadEntities(EntityVec& vec)
{
    auto it = std::remove_if(vec.begin(), vec.end(), [](const std::shared_ptr<Entity>& e){ return !e->isActive(); });
    vec.erase(it, vec.end());
}

std::shared_ptr<Entity> EntityManager::addEntity(const std::string& tag)
{
    auto entity = std::shared_ptr<Entity>(new Entity(m_totalEntities++, tag));

    m_entitiesToAdd.push_back(entity);
    
    return entity;
}

const EntityVec& EntityManager::getEntities()
{
    return m_entities;
}

const EntityVec& EntityManager::getEntities(const std::string& tag)
{
    // return vector from the map
    return m_entityMap[tag];
}
