//
// Created by kieran on 20.08.21.
//
#include <Ogre.h>
#include "AuxiliaryIdManager.h"
#include "ecs/Definitions.h"
#include <list>

extern AuxiliaryIdManager<Ogre::String> aAuxIdManager;

namespace Helper
{
    static std::list<Ogre::MovableObject*> getNeighbours(Ogre::SceneManager * sceneManager, Ogre::Entity * entity, Ogre::Real radius)
    {
        Ogre::Vector3 center = entity->getParentSceneNode()->_getDerivedPosition();
        Ogre::SphereSceneQuery * query = sceneManager->createSphereQuery(Ogre::Sphere(center, radius));
        Ogre::SceneQueryResult result = query->execute();
        result.movables.remove((Ogre::MovableObject*)entity);
        return result.movables;
    }

    static std::list<ecs::Entity> getNeighbours(Ogre::SceneManager * sceneManager, ecs::Entity entity, Ogre::Real radius)
    {
        Ogre::String oentity = aAuxIdManager.getAuxId(entity);
        Ogre::Vector3 center = sceneManager->getEntity(oentity)->getParentSceneNode()->_getDerivedPosition();
        Ogre::SphereSceneQuery * query = sceneManager->createSphereQuery(Ogre::Sphere(center, radius));
        Ogre::SceneQueryResult result = query->execute();
        std::list<ecs::Entity> neighbours;
        for (auto obj : result.movables)
        {
            Ogre::String name = obj->getName();
            if (aAuxIdManager.hasAuxId(name))
            {
                ecs::Entity neighbour = aAuxIdManager.getEntity(name);
                if (neighbour != entity)
                {
                    neighbours.push_back(neighbour);
                }
            }
        }
        sceneManager->destroyQuery(query);
        return neighbours;
    }
}