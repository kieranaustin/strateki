//
// Created by kieran on 23.11.20.
//

#ifndef SCENE_GRAVITYSYSTEM_H
#define SCENE_GRAVITYSYSTEM_H

#include <Ogre.h>
#include <Terrain/OgreTerrainGroup.h>
#include "../System.h"
#include "../Register.h"
#include "../components/Components.h"

extern ecs::Register aRegister;

namespace ecs
{
    class GravitySystem : public ecs::System
    {
    public:
        void init (Ogre::TerrainGroup * terrainGroup)
        {
            m_terrainGroup = terrainGroup;
        }

        void update(const Ogre::Real & dt) override
        {
            for (auto &entity : m_entities)
            {
                Ogre::Vector3 & vel = aRegister.getComponent<ecs::Gravity>(entity).velocity;
                Ogre::Vector3 & acc = aRegister.getComponent<ecs::Gravity>(entity).acceleration;
                Ogre::Vector3 & pos = aRegister.getComponent<ecs::Transform>(entity).position;

                // semi-implicit Euler integration
                // TODO: maybe use Verlet integration or other
                vel += acc*dt;
                pos += vel * dt;

                // handle collision with terrain here for now
                // TODO: maybe implement GravityTerrainCollisionSystem that takes care of the following steps
                float diffHeight = pos.z - m_terrainGroup->getHeightAtWorldPosition(pos);
                if (diffHeight < 0)
                {
                    vel.z = 0.0f;
                    pos.z -= diffHeight;
                }
            }
        }

    private:
        Ogre::TerrainGroup * m_terrainGroup;
    };
}

#endif //SCENE_GRAVITYSYSTEM_H
