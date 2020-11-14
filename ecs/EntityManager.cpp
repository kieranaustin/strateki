//
// Created by kieran on 12.11.20.
//

#include "EntityManager.h"
#include "Definitions.h"

namespace ecs {

    template<> EntityManager *Ogre::Singleton<EntityManager>::msSingleton = 0;

    EntityManager *EntityManager::getSingletonPtr(void) {
        return msSingleton;
    }

    EntityManager &EntityManager::getSingleton(void) {
        assert(msSingleton);
        return (*msSingleton);
    }

    EntityManager::EntityManager(Ogre::SceneManager *sceneManager)
            : m_sceneManager(sceneManager) {
        for (Entity e = 0; e < MAX_ENTITIES; ++e) {
            m_entityPool.push(e);
        }
    }

    Entity EntityManager::createEntity() {
        assert(m_numLivingEntities < scene::MAX_ENTITIES && "Too many entities, can't create new entitiy.");

        Entity entity = m_entityPool.front();
        m_entityPool.pop();

        ++m_numLivingEntities;

        return entity;
    }

    Entity EntityManager::createEntity(Signature signature) {
        Entity entity = createEntity();

        setSignature(entity, signature);

        return entity;
    }

    void EntityManager::destroyEntity(Entity entity) {
        assert(entity < scene::MAX_ENTITIES && "Cannot destroy Entity. Entity out of range.");

        m_signatures[entity].reset();
        m_entityPool.push(entity);

        --m_numLivingEntities;
    }

    void EntityManager::setSignature(Entity entity, Signature signature) {
        assert(entity < scene::MAX_ENTITIES && "Cannot set Signature. Entity out of range.");

        m_signatures[entity] = signature;
    }

    Signature EntityManager::getSignature(Entity entity) {
        assert(entity < scene::MAX_ENTITIES && "Cannot get Signature. Entity out of range.");

        return m_signatures[entity];
    }

    void EntityManager::makeRobot(Ogre::Vector3 &pos) {
        Ogre::Entity *robot = m_sceneManager->createEntity("robot.mesh");
        Ogre::SceneNode *robotNode = m_sceneManager->getRootSceneNode()->createChildSceneNode();
        robotNode->rotate(Ogre::Vector3(1, 0, 0), Ogre::Degree(90));
        robotNode->setPosition(pos.x, pos.y, pos.z);
        robotNode->attachObject(robot);
        //m_entities.push_back(robot->getName());
        Ogre::AnimationState *robotAnimation = robot->getAnimationState("Walk");
        robotAnimation->setEnabled(true);
        robotAnimation->setLoop(true);
    }

}