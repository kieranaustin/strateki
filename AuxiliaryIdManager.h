//
// Created by kieran on 14.03.21.
//

#ifndef STRATEKI_AUXILIARYIDMANAGER_H
#define STRATEKI_AUXILIARYIDMANAGER_H

#include <unordered_map>
#include "ecs/Definitions.h"

template <typename T>
class AuxiliaryIdManager
{
public:
    enum Error
    {
        ENTITY_NOT_FOUND = ecs::MAX_ENTITIES + 1
    };

    AuxiliaryIdManager()
    {}

    AuxiliaryIdManager(T null)
    {
        m_nullElement = null;
    }

    bool add(ecs::Entity entity, T auxId)
    {
        auto searchAuxId = m_AuxIdToEntity.find(auxId);
        auto searchEntity = m_EntityToAuxId.find(entity);
        if (searchAuxId == m_AuxIdToEntity.end() && searchEntity == m_EntityToAuxId.end())
        {
            m_EntityToAuxId.insert(std::pair<ecs::Entity, T>(entity, auxId));
            m_AuxIdToEntity.insert(std::pair<T, ecs::Entity>(auxId, entity));
            return true;
        }
        return false;
    }

    void removeByEntity(ecs::Entity entity)
    {
        auto searchEntity = m_EntityToAuxId.find(entity);
        T auxId;
        if (searchEntity != m_EntityToAuxId.end())
        {
            auxId = m_EntityToAuxId[entity];
            m_EntityToAuxId.erase(entity);
        }
        auto searchAuxId = m_AuxIdToEntity.find(auxId);
        if (searchAuxId != m_AuxIdToEntity.end())
        {
            m_AuxIdToEntity.erase(auxId);
        }
    }

    void removeByAuxId(T auxId)
    {
        auto searchAuxId = m_AuxIdToEntity.find(auxId);
        ecs::Entity entity;
        if (searchAuxId != m_AuxIdToEntity.end())
        {
            entity = m_AuxIdToEntity[auxId];
            m_AuxIdToEntity.erase(auxId);
        }
        auto searchEntity = m_EntityToAuxId.find(entity);
        if (searchEntity != m_EntityToAuxId.end())
        {
            m_EntityToAuxId.erase(entity);
        }
    }

    bool hasAuxId(T auxId)
    {
        auto search = m_AuxIdToEntity.find(auxId);
        if (search == m_AuxIdToEntity.end())
        {
            return false;
        }
        return true;
    }

    bool hasEntity(ecs::Entity entity)
    {
        auto search = m_EntityToAuxId.find(entity);
        if (search == m_EntityToAuxId.end())
        {
            return false;
        }
        return true;
    }

    ecs::Entity getEntity(T auxId)
    {
        auto search = m_AuxIdToEntity.find(auxId);
        if (search == m_AuxIdToEntity.end())
        {
            return ENTITY_NOT_FOUND;
        }
        return m_AuxIdToEntity[auxId];
    }

    T getAuxId(ecs::Entity entity)
    {
        auto search = m_EntityToAuxId.find(entity);
        if (search == m_EntityToAuxId.end())
        {
            return m_nullElement;
        }
        return m_EntityToAuxId[entity];
    }
private:
    std::unordered_map<T, ecs::Entity> m_AuxIdToEntity;
    std::unordered_map<ecs::Entity, T> m_EntityToAuxId;

    T m_nullElement{};

};
#endif //STRATEKI_AUXILIARYIDMANAGER_H
