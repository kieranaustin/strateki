//
// Created by kieran on 16.11.20.
//

#ifndef SCENE_COMPONENTS_H
#define SCENE_COMPONENTS_H

#include <Ogre.h>

namespace ecs
{
    struct Transform
     {
        Ogre::Vector3 position{};
        Ogre::Quaternion rotation{};
        Ogre::Vector3 scale{};
    };

    struct Movement
    {
        Ogre::Vector3 velocity{};
        Ogre::Vector3 acceleration{};
    };

    struct Gravity
    {
        Ogre::Vector3 acceleration{0.0, 0.0, -9.81};
        Ogre::Vector3 velocity{};
    };

    struct TerrainCollision
    {

    };

    struct Mesh
    {
        Ogre::String ID{};
        Ogre::String file{};
        bool hasAnimation{};
        Ogre::String animationState{};
        /* TODO: maybe save pointer to worldNode of object, such that it is easier to get the node to change position and rotation in space
           and avoid having to sort out ...->getParentSceneNode()->getParentSceneNode()->... in RenderSystem
        */
    };
}

#endif //SCENE_COMPONENTS_H
