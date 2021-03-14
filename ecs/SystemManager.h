//
// Created by kieran on 16.11.20.
//

#ifndef SCENE_SYSTEMMANAGER_H
#define SCENE_SYSTEMMANAGER_H

#include <unordered_map>
#include <cassert>
#include "Definitions.h"
#include "System.h"

namespace ecs
{
    class SystemManager
    {
    public:

        unsigned long size()
        {
            return m_systems.size();
        }

        template <typename TSystem>
        std::shared_ptr<TSystem> registerSystem()
        {
            const char * typeName = typeid(TSystem).name();

            assert(m_systems.find(typeName) == m_systems.end() && "System type already registered.");

            auto system = std::make_shared<TSystem>();
            m_systems.insert({typeName, system});

            return system;
        }

        template <typename TSystem>
        void setSignature(ecs::Signature signature)
        {
            const char * typeName = typeid(TSystem).name();

            assert(m_systems.find(typeName) != m_systems.end() && "System not yet registered.");

            // insert does not override signature, if typeName (i.e. system) already exists
            m_systemSignatures.insert({typeName, signature});
        }

        void onEntityDestroyed(ecs::Entity entity)
        {
            for(const auto & pairTypeSystem : m_systems)
            {
                pairTypeSystem.second->destroyEntity(entity);
            }
        }

        void onEntitySignatureChanged(ecs::Entity entity, ecs::Signature entitySignature)
        {
            for(auto const & pairTypeSystem : m_systems)
            {
                const auto & typeName = pairTypeSystem.first;
                const auto & system = pairTypeSystem.second;
                const ecs::Signature & systemSignature = m_systemSignatures[typeName];

                if ((systemSignature & entitySignature) == systemSignature)
                {
                    system->insertEntity(entity);
                }
                else
                {
                    system->destroyEntity(entity);
                }
            }
        }

    private:
        std::unordered_map<const char *, ecs::Signature> m_systemSignatures;
        std::unordered_map<const char *, std::shared_ptr<ecs::System> > m_systems;
    };
}

#endif //SCENE_SYSTEMMANAGER_H
