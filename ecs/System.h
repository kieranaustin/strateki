//
// Created by kieran on 16.11.20.
//

#ifndef SCENE_SYSTEM_H
#define SCENE_SYSTEM_H

#include <Ogre.h>
#include "Definitions.h"

namespace ecs
{
    class System
    {
    public:
        virtual void init() {};
        virtual void update(const Ogre::FrameEvent& evt) = 0;
        void insertEntity(ecs::Entity entity)
        {
            m_entities.insert(entity);
        }
        void destroyEntity(ecs::Entity entity)
        {
            m_entities.erase(entity);
        }

    private:
        std::set<ecs::Entity> m_entities;
    };
}


#endif //SCENE_SYSTEM_H
