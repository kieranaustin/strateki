//
// Created by kieran on 16.11.20.
//

#ifndef SCENE_COMPONENTS_H
#define SCENE_COMPONENTS_H

#include <Ogre.h>
#include <forward_list>

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
        Ogre::Real acceleration{0.0f};
        Ogre::Real MAX_VELOCITY{0.0f};

        bool hasArrived{true};
        uint tries{0};
        Ogre::Vector3 destination{0,0,0};
    };

    struct Gravity
    {
        Ogre::Vector3 acceleration{0.0, 0.0, -98.1};
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
        std::unordered_map<std::string, std::forward_list<Ogre::String> > animationStates{{"Idle", {}}
                                                                                        , {"Walk", {}}
                                                                                        , {"Run", {}}
                                                                                        , {"Fight", {}}
                                                                                        , {"Die", {}}
                                                                                        , {"Jump", {}}};
        std::unordered_set<std::string> activeAnimations{};
        /* TODO: maybe save pointer to worldNode of object, such that it is easier to get the node to change position and rotation in space
           and avoid having to sort out ...->getParentSceneNode()->getParentSceneNode()->... in RenderSystem
        */
    };
}

#endif //SCENE_COMPONENTS_H
