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
        Ogre::Vector3 position{0,0,0};
        Ogre::Quaternion rotation{0,0,0,1};
        Ogre::Vector3 scale{1,1,1};
    };

    struct Movement
    {
        Ogre::Vector3 velocity{0,0,0};
        Ogre::Vector3 acceleration{0,0,0};
    };

    struct Destination
    {
        Ogre::Vector3 destination{0,0,0};
    };

    struct Gravity
    {
        Ogre::Vector3 acceleration{0.0, 0.0, -9.81};
        Ogre::Vector3 velocity{0,0,0};
    };

    struct TerrainCollision
    {

    };

    struct Mesh
    {
        Ogre::String ID{""};
        Ogre::String file{""};
        bool hasAnimation{false};
        Ogre::String animationState{""};
        /* TODO: maybe save pointer to worldNode of object, such that it is easier to get the node to change position and rotation in space
           and avoid having to sort out ...->getParentSceneNode()->getParentSceneNode()->... in RenderSystem
        */
    };
}

#endif //SCENE_COMPONENTS_H
