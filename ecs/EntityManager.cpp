//
// Created by kieran on 12.11.20.
//

#include "EntityManager.h"
#include "Definitions.h"

namespace ecs {


    EntityManager::EntityManager()
    {
        Signature signature{};
        for (Entity e = 0; e < MAX_ENTITIES; ++e) {
            m_entityPool.push(e);
            m_signatures[e] = signature;
        }
    }

    Entity EntityManager::size() const
    {
        return m_numLivingEntities;
    }

    Entity EntityManager::createEntity()
    {
        assert(m_numLivingEntities < ecs::MAX_ENTITIES && "Too many entities, can't create new entity.");

        Entity entity = m_entityPool.front();
        m_entityPool.pop();

        ++m_numLivingEntities;

        return entity;
    }

    Entity EntityManager::createEntity(Signature signature)
    {
        Entity entity = createEntity();

        setSignature(entity, signature);

        return entity;
    }

    void EntityManager::destroyEntity(Entity entity)
    {
        assert(entity < ecs::MAX_ENTITIES && "Cannot destroy Entity. Entity out of range.");

        m_signatures[entity].reset();
        m_entityPool.push(entity);

        --m_numLivingEntities;
    }

    void EntityManager::setSignature(Entity entity, Signature signature)
    {
        assert(entity < ecs::MAX_ENTITIES && "Cannot set Signature. Entity out of range.");

        m_signatures[entity] = signature;
    }

    Signature EntityManager::getSignature(Entity entity)
    {
        assert(entity < ecs::MAX_ENTITIES && "Cannot get Signature. Entity out of range.");

        return m_signatures[entity];
    }
}