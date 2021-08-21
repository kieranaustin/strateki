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
                ecs::Movement & mov = aRegister.getComponent<ecs::Movement>(entity);
                if (mov.hasArrived) continue;

                ecs::Transform & trans = aRegister.getComponent<ecs::Transform>(entity);

                Ogre::Real squaredDistance = mov.destination.squaredDistance(trans.position);
                bool stopMoving = (squaredDistance < 100.0 || mov.tries > 200);
                if (stopMoving)
                {
                    mov.hasArrived = true;
                    mov.tries = 0;
                    mov.velocity = Ogre::Vector3(0,0,0);
                    continue;
                }

                // semi-implicit Euler integration
                // TODO: maybe use Verlet integration or other
                Ogre::Vector3 dir = (mov.destination - trans.position).normalisedCopy();
                dir.z = 0.0f;
                Ogre::Real speed = mov.velocity.length() + mov.acceleration*dt;
                mov.velocity = speed*dir;
                mov.velocity.z = 0.0f;
                if(speed > mov.MAX_VELOCITY)
                {
                    mov.velocity = mov.velocity.normalisedCopy()*mov.MAX_VELOCITY;
                }
                trans.position += mov.velocity * dt;

                if (speed > 0.0f)
                {
                    trans.rotation = Ogre::Vector3::UNIT_X.getRotationTo(mov.velocity);
                }

            }
        }
    };
}

#endif //SCENE_MOVEMENTSYSTEM_H
