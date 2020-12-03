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

    ecs::Entity makeRobot(const Ogre::Vector3 &pos)
    {
        // set up Ogre Entity
        Ogre::Entity *robot = m_sceneManager->createEntity("robot.mesh");

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

