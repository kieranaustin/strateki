//
// Created by kieran on 19.11.20.
//

#include <Ogre.h>
#include "ecs/Definitions.h"
#include "ecs/components/Components.h"
#include "ecs/Register.h"

class EntityFactory
{
public:
    EntityFactory(ecs::Register * reg, Ogre::SceneManager * sceneManager)
        : m_register(reg)
        , m_sceneManager(sceneManager)
    {
    }

    //TODO: implement function for wolf
    //m_coordAxes = m_sceneManager->createEntity("Wolf1_Material__wolf_col_tga_0.mesh");

    void makeRobot(Ogre::Vector3 &pos)
    {
        ecs::Entity ecsRobot = m_register->createEntity();
        Ogre::Entity *robot = m_sceneManager->createEntity("robot.mesh");

        ecs::Mesh ecsMesh;
        ecsMesh.ID = robot->getName();
        ecsMesh.file = "robot.mesh";
        ecsMesh.hasAnimation = true;
        ecsMesh.animationState = "Walk";
        m_register->addComponent<ecs::Mesh>(ecsRobot, ecsMesh);
        m_register->getComponent<ecs::Mesh>(ecsRobot).ID;

        ecs::Transform transform;
        transform.position = pos;
        m_register->addComponent<ecs::Transform>(ecsRobot, transform);

        ecs::Movement movement;
        movement.velocity.x = 0.0;
        movement.acceleration.x = -1.0;
        movement.acceleration.y = 1.0;
        m_register->addComponent<ecs::Movement>(ecsRobot, movement);

        ecs::Gravity gravity;
        m_register->addComponent<ecs::Gravity>(ecsRobot, gravity);

        ecs::TerrainCollision terrainCollision;
        m_register->addComponent<ecs::TerrainCollision>(ecsRobot, terrainCollision);

        Ogre::SceneNode * robotWorldNode = m_sceneManager->getRootSceneNode()->createChildSceneNode();
        // set entity to look at direction (1, 0, 0) at spawn time
        //robotWorldNode->roll(Ogre::Radian(Ogre::Degree(180)));
        robotWorldNode->setPosition(pos.x, pos.y, pos.z);

        Ogre::SceneNode * robotLocalNode = robotWorldNode->createChildSceneNode();
        robotLocalNode->rotate(Ogre::Vector3(1, 0, 0), Ogre::Degree(90), Ogre::Node::TS_LOCAL);
        robotLocalNode->attachObject(robot);

        Ogre::AnimationState *robotAnimation = robot->getAnimationState("Walk");
        robotAnimation->setEnabled(true);
        robotAnimation->setLoop(true);
    }

private:
    ecs::Register * m_register;
    Ogre::SceneManager * m_sceneManager;
};

