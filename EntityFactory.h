//
// Created by kieran on 19.11.20.
//

#include <Ogre.h>
#include "ecs/Definitions.h"
#include "ecs/components/Components.h"
#include "ecs/Register.h"

// code snippet: how to get the animation states strings
//Ogre::AnimationStateSet* animSet = wolf->getAllAnimationStates();
//std::cout << "wolf Animation States: " << std::endl;
//for (auto it = animSet->getAnimationStateIterator().begin(); it != animSet->getAnimationStateIterator().end(); it++)
//{
//    std::cout << it->first << std::endl;
//}

class EntityFactory
{
public:
    EntityFactory(ecs::Register * reg, Ogre::SceneManager * sceneManager)
        : m_register(reg)
        , m_sceneManager(sceneManager)
    {
    }

    ecs::Entity makeWolf(const Ogre::Vector3 &pos)
    {
        // set up Ogre Entity
        //Ogre::Entity *wolf = m_sceneManager->createEntity("Wolf-Blender-2.82a.blend"); // Does not work. Blender file does not work with Assimp plugin
        Ogre::Entity *wolf = m_sceneManager->createEntity("Wolf1_Material__wolf_col_tga_0.mesh");

        // possible animation state strings for wolf. Unfortunately, all but Idle are horribly broken... :/
        //01_Run_Armature_0
        //02_walk_Armature_0
        //03_creep_Armature_0
        //04_Idle_Armature_0
        //05_site_Armature_0
        Ogre::AnimationState *wolfAnimation = wolf->getAnimationState("04_Idle_Armature_0");
        wolfAnimation->setEnabled(true);
        wolfAnimation->setLoop(true);


        Ogre::SceneNode * wolfWorldNode = m_sceneManager->getRootSceneNode()->createChildSceneNode();
        wolfWorldNode->setPosition(pos.x, pos.y, pos.z);

        Ogre::SceneNode * wolfLocalNode = wolfWorldNode->createChildSceneNode();
        wolfLocalNode->setScale(75,75,75);
        // rotate entity to be upright in z-direction
        wolfLocalNode->rotate(Ogre::Vector3(0, 0, 1), Ogre::Degree(90), Ogre::Node::TS_LOCAL);
        wolfLocalNode->attachObject(wolf);

        // set up ecs Entity
        ecs::Entity ecsWolf = m_register->createEntity();

        ecs::Mesh ecsMesh;
        ecsMesh.ID = wolf->getName();
        ecsMesh.file = wolf->getMesh()->getName();
        ecsMesh.hasAnimation = true;
        ecsMesh.animationState = wolfAnimation->getAnimationName();
        m_register->addComponent<ecs::Mesh>(ecsWolf, ecsMesh);

        ecs::Transform transform{};
        transform.position = wolfWorldNode->getPosition();
        transform.scale = wolfLocalNode->getScale();
        m_register->addComponent<ecs::Transform>(ecsWolf, transform);

        ecs::Movement movement{};
        m_register->addComponent<ecs::Movement>(ecsWolf, movement);

        ecs::Destination destination{};
        m_register->addComponent<ecs::Destination>(ecsWolf, destination);

        ecs::Gravity gravity{};
        m_register->addComponent<ecs::Gravity>(ecsWolf, gravity);

        ecs::TerrainCollision terrainCollision{};
        m_register->addComponent<ecs::TerrainCollision>(ecsWolf, terrainCollision);

        return ecsWolf;
    }

    ecs::Entity makeRobot(const Ogre::Vector3 &pos)
    {
        // set up Ogre Entity
        Ogre::Entity *robot = m_sceneManager->createEntity("robot.mesh");

        // possible animation state strings for robot
        //Die
        //Idle
        //Shoot
        //Slump
        //Walk
        Ogre::AnimationState *robotAnimation = robot->getAnimationState("Walk");
        robotAnimation->setEnabled(true);
        robotAnimation->setLoop(true);

        Ogre::SceneNode * robotWorldNode = m_sceneManager->getRootSceneNode()->createChildSceneNode();
        robotWorldNode->setPosition(pos.x, pos.y, pos.z);

        Ogre::SceneNode * robotLocalNode = robotWorldNode->createChildSceneNode();
        // rotate entity to be upright in z-direction
        robotLocalNode->rotate(Ogre::Vector3(1, 0, 0), Ogre::Degree(90), Ogre::Node::TS_LOCAL);
        robotLocalNode->attachObject(robot);

        // set up ecs Entity
        ecs::Entity ecsRobot = m_register->createEntity();

        ecs::Mesh ecsMesh;
        ecsMesh.ID = robot->getName();
        ecsMesh.file = robot->getMesh()->getName();
        ecsMesh.hasAnimation = true;
        ecsMesh.animationState = robotAnimation->getAnimationName();
        m_register->addComponent<ecs::Mesh>(ecsRobot, ecsMesh);

        ecs::Transform transform{};
        transform.position = robotWorldNode->getPosition();
        m_register->addComponent<ecs::Transform>(ecsRobot, transform);

        ecs::Movement movement{};
        m_register->addComponent<ecs::Movement>(ecsRobot, movement);

        ecs::Destination destination{};
        m_register->addComponent<ecs::Destination>(ecsRobot, destination);

        ecs::Gravity gravity{};
        m_register->addComponent<ecs::Gravity>(ecsRobot, gravity);

        ecs::TerrainCollision terrainCollision{};
        m_register->addComponent<ecs::TerrainCollision>(ecsRobot, terrainCollision);

        return ecsRobot;
    }

private:
    ecs::Register * m_register = nullptr;
    Ogre::SceneManager * m_sceneManager = nullptr;
};

