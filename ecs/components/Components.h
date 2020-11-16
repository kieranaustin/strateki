//
// Created by kieran on 16.11.20.
//

#ifndef SCENE_COMPONENTS_H
#define SCENE_COMPONENTS_H

#include <Ogre.h>

namespace ecs {
    struct Transform {
        Ogre::Vector3 position;
        Ogre::Quaternion rotation;
        Ogre::Vector3 scale;
    };

    struct Movement {
        Ogre::Vector3 velocity;
        Ogre::Vector3 acceleration;
    };

    struct Gravity {
        Ogre::Vector3 acceleration{0.0, 0.0, -9.81};
    };

    struct Mesh {
        Ogre::String ID;
        Ogre::String file;
        bool hasAnimation;
        Ogre::String animationState;
    };
}

#endif //SCENE_COMPONENTS_H
