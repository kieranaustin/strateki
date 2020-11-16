//
// Created by kieran on 12.11.20.
//

#ifndef SCENE_ENTITYMANAGER_H
#define SCENE_ENTITYMANAGER_H

#include "Definitions.h"
#include <Ogre.h>

namespace ecs {

    class EntityManager
    {
    public:
        EntityManager();

        Entity createEntity();

        Entity createEntity(Signature signature);

        void destroyEntity(Entity entity);

        void setSignature(Entity entity, Signature signature);

        Signature getSignature(Entity entity);

        //std::queue<scene::Entity>* getEntities() {return &m_entityPool;}; TODO: return only currently living entities

    private:
        Ogre::SceneManager *m_sceneManager;

        // A pool of still available entity ids, i.e., of not-living entities
        std::queue<Entity> m_entityPool{};
        // array of the entities signatures; index of array = entity id
        std::array<Signature, MAX_ENTITIES> m_signatures{};
        // counter for currently living entities
        Entity m_numLivingEntities{};


    };

}

#endif //SCENE_ENTITYMANAGER_H
