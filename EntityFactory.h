//
// Created by kieran on 19.11.20.
//

#ifndef STRATEKI_ENTITYFACTORY_H
#define STRATEKI_ENTITYFACTORY_H

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

    ecs::Entity makeTree(const Ogre::Vector3 &pos)
    {
        // set up Ogre Entity
        Ogre::Entity *tree = m_sceneManager->createEntity("tree_LOD.mesh");
        Ogre::SceneNode * treeWorldNode = m_sceneManager->getRootSceneNode()->createChildSceneNode();
        treeWorldNode->setPosition(pos.x, pos.y, pos.z);

        Ogre::SceneNode * treeLocalNode = treeWorldNode->createChildSceneNode();
        treeLocalNode->setScale(1,1,1);
        treeLocalNode->setPosition(0,0,0);
        treeLocalNode->attachObject(tree);

        // set up ecs Entity
        ecs::Entity ecsTree = m_register->createEntity();

        ecs::Mesh ecsMesh;
        ecsMesh.ID = tree->getName();
        ecsMesh.file = tree->getMesh()->getName();
        ecsMesh.hasAnimation = false;
        m_register->addComponent<ecs::Mesh>(ecsTree, ecsMesh);

        ecs::Transform transform{};
        transform.position = treeWorldNode->getPosition();
        transform.scale = treeLocalNode->getScale();
        m_register->addComponent<ecs::Transform>(ecsTree, transform);

        m_auxIdManager->add(ecsTree, ecsMesh.ID);

        return ecsTree;
    }

    ecs::Entity makeLighter(const Ogre::Vector3 &pos)
    {
        // set up Ogre Entity
        Ogre::Entity *lighter = m_sceneManager->createEntity("lighter.mesh");
        Ogre::SceneNode * lighterWorldNode = m_sceneManager->getRootSceneNode()->createChildSceneNode();
        lighterWorldNode->setPosition(pos.x, pos.y, pos.z);

        Ogre::SceneNode * lighterLocalNode = lighterWorldNode->createChildSceneNode();
        lighterLocalNode->setScale(2,2,2);
        //lighterLocalNode->setPosition(0,0,6);
        lighterLocalNode->attachObject(lighter);

        // set up ecs Entity
        ecs::Entity ecsLighter = m_register->createEntity();

        ecs::Mesh ecsMesh;
        ecsMesh.ID = lighter->getName();
        ecsMesh.file = lighter->getMesh()->getName();
        ecsMesh.hasAnimation = false;
        m_register->addComponent<ecs::Mesh>(ecsLighter, ecsMesh);

        ecs::Transform transform{};
        transform.position = lighterWorldNode->getPosition();
        transform.scale = lighterLocalNode->getScale();
        m_register->addComponent<ecs::Transform>(ecsLighter, transform);

        ecs::Movement movement{};
        movement.acceleration = 50.0f;
        movement.MAX_VELOCITY = 50.0f;
        m_register->addComponent<ecs::Movement>(ecsLighter, movement);

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
        // possible animation state strings for robot
        //Die
        //Idle
        //Shoot
        //Slump
        //Walk
        ecsMesh.hasAnimation = true;
        ecsMesh.animationStates["Idle"] = {"Idle"};
        ecsMesh.animationStates["Walk"] = {"Walk"};
        ecsMesh.animationStates["Run"] = {"Walk"};
        ecsMesh.animationStates["Fight"] = {"Shoot"};
        ecsMesh.animationStates["Die"] = {"Die"};
        ecsMesh.animationStates["Jump"] = {"Slump"};
        ecsMesh.activeAnimations.insert("Idle");
        m_register->addComponent<ecs::Mesh>(ecsRobot, ecsMesh);

        ecs::Transform transform{};
        transform.position = robotWorldNode->getPosition();
        m_register->addComponent<ecs::Transform>(ecsRobot, transform);

        ecs::Movement movement{};
        movement.acceleration = 100.0f;
        movement.MAX_VELOCITY = 200.0f;
        m_register->addComponent<ecs::Movement>(ecsRobot, movement);

        ecs::Gravity gravity{};
        m_register->addComponent<ecs::Gravity>(ecsRobot, gravity);

        ecs::TerrainCollision terrainCollision{};
        m_register->addComponent<ecs::TerrainCollision>(ecsRobot, terrainCollision);

        m_auxIdManager->add(ecsRobot, ecsMesh.ID);

        return ecsRobot;
    }

    ecs::Entity makeSinbad(const Ogre::Vector3 &pos)
    {
        // set up Ogre Entity
        Ogre::Entity *sinbad = m_sceneManager->createEntity("Sinbad.mesh");

        Ogre::SceneNode * sinbadWorldNode = m_sceneManager->getRootSceneNode()->createChildSceneNode();
        sinbadWorldNode->setPosition(pos.x, pos.y, pos.z);

        Ogre::SceneNode * sinbadLocalNode = sinbadWorldNode->createChildSceneNode();
        // rotate entity to be upright in z-direction
        sinbadLocalNode->setScale(6,6,6);
        sinbadLocalNode->setPosition(0,0,30);
        sinbadLocalNode->rotate(Ogre::Vector3(1, 0, 0), Ogre::Degree(90), Ogre::Node::TS_LOCAL);
        sinbadLocalNode->rotate(Ogre::Vector3(0, 1, 0), Ogre::Degree(90), Ogre::Node::TS_LOCAL);
        sinbadLocalNode->attachObject(sinbad);

        // set up ecs Entity
        ecs::Entity ecsSinbad = m_register->createEntity();

        ecs::Mesh ecsMesh;
        ecsMesh.ID = sinbad->getName();
        ecsMesh.file = sinbad->getMesh()->getName();
        // possible animation state strings for sinbad
        //Dance
        //DrawSwords
        //HandsClosed
        //HandsRelaxed
        //IdleBase
        //IdleTop
        //JumpEnd
        //JumpLoop
        //JumpStart
        //RunBase
        //RunTop
        //SliceHorizontal
        //SliceVertical
        ecsMesh.hasAnimation = true;
        ecsMesh.animationStates["Idle"] = {"IdleTop", "IdleBase"};
        ecsMesh.animationStates["Walk"] = {"RunBase"};
        ecsMesh.animationStates["Run"] = {"RunTop", "RunBase"};
        ecsMesh.animationStates["Fight"] = {"SliceHorizontal"};
        ecsMesh.animationStates["Die"] = {"Dance"};
        ecsMesh.animationStates["Jump"] = {"JumpLoop"};
        ecsMesh.activeAnimations.insert("Idle");
        m_register->addComponent<ecs::Mesh>(ecsSinbad, ecsMesh);

        ecs::Transform transform{};
        transform.position = sinbadWorldNode->getPosition();
        m_register->addComponent<ecs::Transform>(ecsSinbad, transform);

        ecs::Movement movement{};
        movement.acceleration = 100.0f;
        movement.MAX_VELOCITY = 150.0f;
        m_register->addComponent<ecs::Movement>(ecsSinbad, movement);

        ecs::Gravity gravity{};
        m_register->addComponent<ecs::Gravity>(ecsSinbad, gravity);

        ecs::TerrainCollision terrainCollision{};
        m_register->addComponent<ecs::TerrainCollision>(ecsSinbad, terrainCollision);

        m_auxIdManager->add(ecsSinbad, ecsMesh.ID);

        return ecsSinbad;
    }
private:
    ecs::Register * m_register = nullptr;
    Ogre::SceneManager * m_sceneManager = nullptr;
    AuxiliaryIdManager<Ogre::String> * m_auxIdManager = nullptr;
};

#endif //STRATEKI_ENTITYFACTORY_H
