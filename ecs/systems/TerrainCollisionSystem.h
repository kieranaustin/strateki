//
// Created by kieran on 23.11.20.
//

#ifndef SCENE_TERRAINCOLLISIONSYSTEM_H
#define SCENE_TERRAINCOLLISIONSYSTEM_H

#include <Ogre.h>
#include <Terrain/OgreTerrainGroup.h>
#include "../System.h"
#include "../Register.h"
#include "../components/Components.h"

extern ecs::Register aRegister;

namespace ecs
{
    class TerrainCollisionSystem : public ecs::System
    {
    public:
        void init (Ogre::TerrainGroup * terrainGroup)
        {
            m_terrainGroup = terrainGroup;
        }

        void update (const Ogre::Real & dt) override
        {
            if (m_terrainGroup == nullptr)
                return;

            for(auto & entity : m_entities)
            {
                ecs::Transform & transform = aRegister.getComponent<ecs::Transform>(entity);
                float diffHeight = transform.position.z - m_terrainGroup->getHeightAtWorldPosition(transform.position);

                if (diffHeight < 0.0f)
                {
                    transform.position.z -= diffHeight;
                }
            }
        }

    private:
        Ogre::TerrainGroup * m_terrainGroup;
    };
}
#endif //SCENE_TERRAINCOLLISIONSYSTEM_H
