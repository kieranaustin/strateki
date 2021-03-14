//
// Created by kieran on 19.11.20.
//

#include <Ogre.h>
#include "ecs/Definitions.h"
#include "ecs/components/Components.h"
#include "ecs/Register.h"
#include "AuxiliaryIdManager.h"

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
    EntityFactory(ecs::Register * reg, Ogre::SceneManager * sceneManager, AuxiliaryIdManager<Ogre::String> * auxIdManager)
        : m_register(reg)
        , m_sceneManager(sceneManager)
        , m_auxIdManager(auxIdManager)
    {
    }

    ecs::Entity makeLighter(const Ogre::Vector3 &pos)
    {
        // set up Ogre Entity
        Ogre::Entity *lighter = m_sceneManager->createEntity("lighter.mesh");

        Ogre::SceneNode * lighterWorldNode = m_sceneManager->getRootSceneNode()->createChildSceneNode();
        lighterWorldNode->setPosition(pos.x, pos.y, pos.z);

        Ogre::SceneNode * lighterLocalNode = lighterWorldNode->createChildSceneNode();
        lighterLocalNode->setScale(2,2,2);
        lighterLocalNode->setPosition(0,0,6);
        lighterLocalNode->attachObject(lighter);

        // set up ecs Entity
        ecs::Entity ecsLighter = m_register->createEntity();

        ecs::Mesh ecsMesh;
        ecsMesh.ID = lighter->getName();
        ecsMesh.file = lighter->getMesh()->getName();
        ecsMesh.hasAnimation = false;
        ecsMesh.animationState = "";
        m_register->addComponent<ecs::Mesh>(ecsLighter, ecsMesh);

        ecs::Transform transform{};
        transform.position = lighterWorldNode->getPosition();
        transform.scale = lighterLocalNode->getScale();
        m_register->addComponent<ecs::Transform>(ecsLighter, transform);

        ecs::Movement movement{};
        m_register->addComponent<ecs::Movement>(ecsLighter, movement);

        ecs::Destination destination{};
        m_register->addComponent<ecs::Destination>(ecsLighter, destination);

        ecs::Gravity gravity{};
        m_register->addComponent<ecs::Gravity>(ecsLighter, gravity);

        ecs::TerrainCollision terrainCollision{};
        m_register->addComponent<ecs::TerrainCollision>(ecsLighter, terrainCollision);

        m_auxIdManager->add(ecsLighter, ecsMesh.ID);

        return ecsLighter;
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

        m_auxIdManager->add(ecsRobot, ecsMesh.ID);

        return ecsRobot;
    }

private:
    ecs::Register * m_register = nullptr;
    Ogre::SceneManager * m_sceneManager = nullptr;
    AuxiliaryIdManager<Ogre::String> * m_auxIdManager = nullptr;
};

