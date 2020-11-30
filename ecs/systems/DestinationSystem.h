//
// Created by kieran on 30.11.20.
//

#ifndef SCENE_DESTINATIONSYSTEM_H
#define SCENE_DESTINATIONSYSTEM_H

#include <Ogre.h>
#include "../Definitions.h"
#include "../components/Components.h"
#include "../System.h"
#include "../Register.h"

extern ecs::Register aRegister;

namespace ecs
{
    class DestinationSystem : public ecs::System
    {
    public:
        void init () {}

        void update(const Ogre::Real & dt) override
        {
            for (auto &entity : m_entities)
            {
                Ogre::Vector3 & pos = aRegister.getComponent<ecs::Transform>(entity).position;
                Ogre::Vector3 & dest = aRegister.getComponent<ecs::Destination>(entity).destination;
                ecs::Movement & mov = aRegister.getComponent<ecs::Movement>(entity);

                Ogre::Real squaredDistance = dest.squaredDistance(pos);
                if (squaredDistance < 10.0)
                {
                    mov.velocity = Ogre::Vector3(0,0,0);
                    mov.acceleration = Ogre::Vector3(0,0,0);
                }
            }
        }
    };
}
#endif //SCENE_DESTINATIONSYSTEM_H
