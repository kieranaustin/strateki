//
// Created by kieran on 16.11.20.
//

#ifndef SCENE_RENDERSYSTEM_H
#define SCENE_RENDERSYSTEM_H

#include <iostream>
#include <Ogre.h>
#include "../System.h"
#include "../Register.h"
#include "../components/Components.h"

extern ecs::Register aRegister;

namespace ecs
{
    class RenderSystem : public ecs::System
    {
    public:
        void init(Ogre::SceneManager* sceneManager)
        {
            m_sceneManager = sceneManager;

            // register our scene with the RTSS
            Ogre::RTShader::ShaderGenerator* shadergen = Ogre::RTShader::ShaderGenerator::getSingletonPtr();
            shadergen->addSceneManager(m_sceneManager);

            // set light
            m_sceneManager->setAmbientLight(Ogre::ColourValue(0.1f,0.1f,0.1f));
            Ogre::Light* light = m_sceneManager->createLight("MainLight");
            light->setType(Ogre::Light::LT_SPOTLIGHT);
            light->setSpotlightRange(Ogre::Degree(30), Ogre::Degree(50));
            light->setSpecularColour(Ogre::ColourValue::Blue);
            light->setDiffuseColour(Ogre::ColourValue::Blue);
            light->setPowerScale(10000);
            Ogre::SceneNode* lightNode = m_sceneManager->getRootSceneNode()->createChildSceneNode();
            lightNode->setPosition(0, 0, 500);
            lightNode->setDirection(0,0,-1);
            lightNode->attachObject(light);
        }

        void update(const Ogre::Real & dt) override
        {
            for(auto & entity : m_entities)
            {
                const ecs::Mesh & mesh = aRegister.getComponent<ecs::Mesh>(entity);
                const ecs::Transform & transform = aRegister.getComponent<ecs::Transform>(entity);

                m_sceneManager->getEntity(mesh.ID)->getParentSceneNode()->setPosition(transform.position);

                if(mesh.hasAnimation)
                {
                    m_sceneManager->getEntity(mesh.ID)->getAnimationState(mesh.animationState)->addTime(dt);
                }
            }
        }

    private:
        Ogre::SceneManager * m_sceneManager;
    };
}
#endif //SCENE_RENDERSYSTEM_H
