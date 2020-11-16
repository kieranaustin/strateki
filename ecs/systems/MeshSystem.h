//
// Created by kieran on 16.11.20.
//

#ifndef SCENE_MESHSYSTEM_H
#define SCENE_MESHSYSTEM_H

#include <Ogre.h>
#include "../System.h"
#include "../Register.h"
#include "../components/Components.h"

extern ecs::Register aRegister;

namespace ecs
{
    class MeshSystem : public ecs::System
    {
    public:
        void init(Ogre::SceneManager* sceneManager)
        {
            m_sceneManager = sceneManager;
            Ogre::Vector3 pos{0,0,83};
            makeRobot(pos);
        }

        void makeRobot(Ogre::Vector3 &pos) {
            ecs::Entity ecsRobot = aRegister.createEntity();
            Ogre::Entity *robot = m_sceneManager->createEntity("robot.mesh");

            ecs::Mesh ecsMesh;
            ecsMesh.ID = robot->getName();
            ecsMesh.file = "robot.mesh";
            ecsMesh.hasAnimation = true;
            ecsMesh.animationState = "Walk";
            aRegister.addComponent<ecs::Mesh>(ecsRobot, ecsMesh);

            ecs::Transform transform;
            transform.position = pos;
            aRegister.addComponent<ecs::Transform>(ecsRobot, transform);

            Ogre::SceneNode *robotNode = m_sceneManager->getRootSceneNode()->createChildSceneNode();
            robotNode->rotate(Ogre::Vector3(1, 0, 0), Ogre::Degree(90));
            robotNode->setPosition(pos.x, pos.y, pos.z);
            robotNode->attachObject(robot);
            Ogre::AnimationState *robotAnimation = robot->getAnimationState("Walk");
            robotAnimation->setEnabled(true);
            robotAnimation->setLoop(true);
        }

        void update(const Ogre::FrameEvent & evt) override
        {
            for(auto & entity : m_entities)
            {
                ecs::Mesh & mesh = aRegister.getComponent<ecs::Mesh>(entity);
                ecs::Transform & transform = aRegister.getComponent<ecs::Transform>(entity);

                m_sceneManager->getEntity(mesh.ID)->getParentSceneNode()->setPosition(transform.position);

                if(mesh.hasAnimation)
                {
                    m_sceneManager->getEntity(mesh.ID)->getAnimationState(mesh.animationState)->addTime(evt.timeSinceLastFrame);
                }
            }
        }

    private:
        Ogre::SceneManager * m_sceneManager;
    };
}
#endif //SCENE_MESHSYSTEM_H
