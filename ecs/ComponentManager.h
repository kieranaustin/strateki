//
// Created by kieran on 14.11.20.
//

#ifndef SCENE_COMPONENTMANAGER_H
#define SCENE_COMPONENTMANAGER_H

#include <unordered_map>
#include "Definitions.h"
#include "ComponentContainer.h"

namespace ecs
{
    class ComponentManager
    {
    public:
        template <typename T>
        void registerComponentType()
        {
            const char * typeName = typeid(T).name();

            assert(m_componentsTypes.find(typeName) == m_componentsTypes.end() && "Component Type already registered.");

            m_componentsTypes.insert({typeName, m_numComponentTypes});
            m_componentContainers.insert({typeName, std::make_shared<ComponentContainer<T>>()});

            ++m_numComponentTypes;
        }

        template <typename T>
        ComponentType getComponentType()
        {
            const char * typeName = typeid(T).name();

            assert(m_componentsTypes.find(typeName) != m_componentsTypes.end() && "Component Type not registered.");

            return m_componentsTypes[typeName];
        }

        template <typename T>
        void addComponent(ecs::Entity entity, T component)
        {
            getComponentContainerPtr<T>()->addComponent(entity, component);
        }

        template <typename T>
        void removeComponent(ecs::Entity entity)
        {
            getComponentContainerPtr<T>()->removeComponent(entity);
        }

        template <typename T>
        bool hasComponent(ecs::Entity entity)
        {
            return getComponentContainerPtr<T>()->hasComponent(entity);
        }

        template <typename T>
        T& getComponent(ecs::Entity entity)
        {
            return getComponentContainerPtr<T>()->getComponent(entity);
        }

        void destroyEntity(ecs::Entity entity)
        {
            for(const auto & container : m_componentContainers)
            {
                container.second->destroyEntity(entity);
            }
        }

    private:
        std::unordered_map<const char *, ecs::ComponentType> m_componentsTypes{};
        std::unordered_map<const char *, std::shared_ptr<ecs::IComponentContainer>> m_componentContainers{};
        ComponentType m_numComponentTypes{};

        template <typename T>
        std::shared_ptr<ComponentContainer<T>> getComponentContainerPtr()
        {
            const char * typeName = typeid(T).name();

            assert(m_componentsTypes.find(typeName) != m_componentsTypes.end() && "Component Type not registered.");

            return std::static_pointer_cast<ComponentContainer<T>>(m_componentContainers[typeName]);
        }
    };
}
#endif //SCENE_COMPONENTMANAGER_H
