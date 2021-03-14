//
// Created by kieran on 16.11.20.
//

#ifndef SCENE_REGISTER_H
#define SCENE_REGISTER_H

#include "Definitions.h"
#include "EntityManager.h"
#include "ComponentManager.h"
#include "SystemManager.h"

namespace ecs
{
    class Register
    {
    public:
        void init()
        {
            m_entityManager = std::make_unique<ecs::EntityManager>();
            m_componentManager = std::make_unique<ecs::ComponentManager>();
            m_systemManager = std::make_unique<ecs::SystemManager>();
        }

        Entity sizeEntities() const
        {
            return m_entityManager->size();
        }

        unsigned long sizeSystems() const
        {
            return m_systemManager->size();
        }

        // entity related functions
        ecs::Entity createEntity()
        {
            return m_entityManager->createEntity();
        }

        void destroyEntity(ecs::Entity entity)
        {
            m_entityManager->destroyEntity(entity);
            m_componentManager->destroyEntity(entity);
            m_systemManager->onEntityDestroyed(entity);
        }

        // component related functions
        template <typename T>
        void registerComponentType()
        {
            m_componentManager->registerComponentType<T>();
        }

        template <typename T>
        void addComponent(ecs::Entity entity, T & component)
        {
            // new signature
            ecs::Signature signature = m_entityManager->getSignature(entity);
            signature.set(m_componentManager->getComponentType<T>(), true);

            // save component
            m_componentManager->addComponent<T>(entity, component);

            // notify about new signature
            m_entityManager->setSignature(entity, signature);
            m_systemManager->onEntitySignatureChanged(entity, signature);
        }

        template <typename T>
        void removeComponent(ecs::Entity entity)
        {
            // new signature
            ecs::Signature signature = m_entityManager->getSignature(entity);
            signature.set(m_componentManager->getComponentType<T>(), false);

            // remove component type
            m_componentManager->removeComponent<T>(entity);

            // notify about new signature
            m_entityManager->setSignature(entity, signature);
            m_systemManager->onEntitySignatureChanged(entity, signature);
        }

        template <typename T>
        T & getComponent(ecs::Entity entity)
        {
            return m_componentManager->getComponent<T>(entity);
        }

        template <typename T>
        ecs::ComponentType getComponentType()
        {
            return m_componentManager->getComponentType<T>();
        }

        // system related functions
        template <typename TSystem>
        std::shared_ptr<TSystem> registerSystem()
        {
            return m_systemManager->registerSystem<TSystem>();
        }

        // system signature can only be set once, preferably directly after registering system, i.e., system signature can not change
        template <typename TSystem>
        void setSystemSignature(ecs::Signature signature)
        {
            m_systemManager->setSignature<TSystem>(signature);
        }

    private:
        std::unique_ptr<ecs::EntityManager> m_entityManager;
        std::unique_ptr<ecs::ComponentManager> m_componentManager;
        std::unique_ptr<ecs::SystemManager> m_systemManager;
    };
}

#endif //SCENE_REGISTER_H
