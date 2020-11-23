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

                // semi-implicit Euler integration
                // TODO: maybe use Verlet integration or other
                vel += acc*dt;
                aRegister.getComponent<ecs::Transform>(entity).position += vel * dt;
            }
        }
    };
}

#endif //SCENE_MOVEMENTSYSTEM_H
