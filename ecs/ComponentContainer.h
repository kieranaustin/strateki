//
// Created by kieran on 14.11.20.
//

#ifndef SCENE_COMPONENTCONTAINER_H
#define SCENE_COMPONENTCONTAINER_H

#include "Definitions.h"
#include <array>
#include <map>
#include <cassert>

namespace ecs
{
    template <typename T>
    class ComponentContainer
    {
        ComponentContainer() {};

        void addComponent(ecs::Entity entity, T component)
        {
            assert(entity < ecs::MAX_ENTITIES && "Cannot add Component. Entity id out of range.");
            assert(m_mapEntityToIndex.find(entity) == m_mapEntityToIndex.end() && "Cannot add Component. Component already exists for this entity.");

            size_t index = m_numComponents;
            m_mapIndexToEntity[index] = entity;
            m_mapEntityToIndex[entity] = index;
            m_components[index] = component;
            ++m_numComponents;
        }

        void removeComponent(ecs::Entity entity)
        {
            assert(m_mapEntityToIndex.find(entity) != m_mapEntityToIndex.end() && "Cannot remove Component. Entity does not exist.");

            // index to entity being removed, and is being reused for shifted entity
            size_t reuseIndex = m_mapEntityToIndex[entity];
            // index to entity that will be shifted, hence index will be obsolete
            size_t obsoleteIndex = m_numComponents - 1;
            // entity to be shifted
            ecs::Entity shiftEntity = m_mapIndexToEntity[obsoleteIndex];

            // shift entity to removed entity
            m_components[reuseIndex] = m_components[obsoleteIndex];
            --m_numComponents;

            // register new mapping
            m_mapEntityToIndex[shiftEntity] = reuseIndex;
            m_mapIndexToEntity[reuseIndex] = shiftEntity;

            // erase old mapping
            m_mapEntityToIndex.erase(entity);
            m_mapIndexToEntity.erase(obsoleteIndex);
        }

        T& getComponent(ecs::Entity entity)
        {
            assert(entity < ecs::MAX_ENTITIES && "Cannot get components. Entity out of range.");
            assert(m_mapEntityToIndex.find(entity) != m_mapEntityToIndex.end() && "Cannot get Component. Entity does not have Component.");

            size_t index = m_mapEntityToIndex[entity];

            return m_components[index];
        }


    private:
        // TODO: why not use std::map instead?
        std::array<T, ecs::MAX_ENTITIES> m_components{};
        std::map<ecs::Entity, std::size_t> m_mapEntityToIndex{};
        std::map<std::size_t, ecs::Entity> m_mapIndexToEntity{};
        size_t m_numComponents{};
    };
}

#endif //SCENE_COMPONENTCONTAINER_H
