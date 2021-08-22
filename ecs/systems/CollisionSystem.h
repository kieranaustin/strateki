//
// Created by kieran on 21.08.21.
//

#ifndef STRATEKI_COLLISIONSYSTEM_H
#define STRATEKI_COLLISIONSYSTEM_H

#include <Ogre.h>
#include "../Definitions.h"
#include "../components/Components.h"
#include "../System.h"
#include "../Register.h"
#include "../../Helper.cpp"

extern ecs::Register aRegister;

namespace ecs
{
    class CollisionSystem : public ecs::System
    {
    public:
        void init (Ogre::SceneManager* sceneManager)
        {
            m_sceneManager = sceneManager;
        }

        void update(const Ogre::Real & dt) override
        {
            for (auto &entity : m_entities)
            {
                ecs::Movement & mov = aRegister.getComponent<ecs::Movement>(entity);
                if (mov.hasArrived) continue;

                ecs::Transform & trans = aRegister.getComponent<ecs::Transform>(entity);

                std::list<ecs::Entity> neighbours = Helper::getNeighbours(m_sceneManager, entity, 10.0f);
                for(auto neighbour : neighbours)
                {
                    Ogre::Vector3 & neighPos = aRegister.getComponent<ecs::Transform>(neighbour).position;
                    Ogre::Vector3 & neighVel = aRegister.getComponent<ecs::Movement>(neighbour).velocity;
                    Ogre::Vector3 diffPos = neighPos - trans.position;
                    diffPos.z = 0.0f;
                    bool collided = (diffPos.squaredLength() < 1000.0f && mov.velocity.directionEquals(diffPos, Ogre::Radian(Ogre::Degree(10))));
                    if (collided)
                    {
                        trans.position -= 0.5*mov.velocity*dt;
                        trans.rotation = Ogre::Vector3::UNIT_X.getRotationTo(mov.velocity);
                        mov.velocity -= 15.0f*diffPos.perpendicular().normalisedCopy();
                        mov.velocity.z = 0.0f;
                        mov.tries++;
                    }
                    else
                    {
                        mov.tries = 0;
                    }
                }

            }
        }

    private:
        Ogre::SceneManager * m_sceneManager;
    };
}

#endif //STRATEKI_COLLISIONSYSTEM_H
