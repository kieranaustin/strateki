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
        void update(const Ogre::FrameEvent &evt) override
        {
            for (auto &entity : m_entities)
            {
                aRegister.getComponent<ecs::Transform>(entity).position.x += 0.5;
            }
        }
    };
}

#endif //SCENE_MOVEMENTSYSTEM_H
