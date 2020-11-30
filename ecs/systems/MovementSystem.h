//
// Created by kieran on 16.11.20.
//

#ifndef SCENE_MOVEMENTSYSTEM_H
#define SCENE_MOVEMENTSYSTEM_H

#include <Ogre.h>
#include "../System.h"
#include "../Register.h"
#include "../components/Components.h"

extern ecs::Register aRegister;

namespace ecs
{
    class MovementSystem : public ecs::System
    {
    public:
        void update(const Ogre::Real & dt) override
        {
            for (auto &entity : m_entities)
            {
                Ogre::Vector3 & vel = aRegister.getComponent<ecs::Movement>(entity).velocity;
                Ogre::Vector3 & acc = aRegister.getComponent<ecs::Movement>(entity).acceleration;
                Ogre::Quaternion & rot = aRegister.getComponent<ecs::Transform>(entity).rotation;
                Ogre::Vector3 & pos = aRegister.getComponent<ecs::Transform>(entity).position;

                // semi-implicit Euler integration
                // TODO: maybe use Verlet integration or other
                vel += acc*dt;
                pos += vel * dt;

                if (vel.length() > 0.0f)
                {
                    rot = Ogre::Vector3::UNIT_X.getRotationTo(vel);
                }
            }
        }
    };
}

#endif //SCENE_MOVEMENTSYSTEM_H
