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
                auto & mov = aRegister.getComponent<ecs::Movement>(entity);
                if (mov.hasArrived) continue;

                auto & mesh = aRegister.getComponent<ecs::Mesh>(entity);
                auto & trans = aRegister.getComponent<ecs::Transform>(entity);
                Ogre::Real squaredDistance = mov.destination.xy().squaredDistance(trans.position.xy());
                if (squaredDistance < 100.0 || mov.tries > 200)
                {
                    //stop moving
                    mov.hasArrived = true;
                    mov.tries = 0;
                    mov.velocity = Ogre::Vector3::ZERO;
                    mesh.activeAnimations.erase("Run");
                    mesh.activeAnimations.insert("Idle");
                    continue;
                }

                // semi-implicit Euler integration
                // TODO: maybe use Verlet integration or other
                Ogre::Vector3 dir = (mov.destination - trans.position);
                dir.z = 0.0f;
                dir = dir.normalisedCopy();

                Ogre::Real speed = mov.velocity.length() + mov.acceleration*dt; // Euler
                speed = std::min(speed, mov.MAX_VELOCITY);
                mov.velocity = speed*dir;

                trans.position += mov.velocity * dt; // Euler

                Ogre::Radian angle = Ogre::Vector3::UNIT_X.xy().angleTo(mov.velocity.xy());
                trans.rotation = Ogre::Quaternion(angle, Ogre::Vector3::UNIT_Z);
            }
        }
    };
}

#endif //SCENE_MOVEMENTSYSTEM_H
