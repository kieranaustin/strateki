//
// Created by kieran on 16.05.20.
//
#include <Ogre.h>
#include <Bites/OgreApplicationContext.h>
#include <Terrain/OgreTerrain.h>
#include <Terrain/OgreTerrainGroup.h>
#include "CameraControl.h"
#include "TerrainLoader.h"
#include "ecs/EntityManager.h"
#include "ecs/systems/RenderSystem.h"
#include "ecs/systems/MovementSystem.h"
#include "ecs/systems/TerrainCollisionSystem.h"
#include "ecs/systems/GravitySystem.h"
#include "EntityFactory.h"

#ifndef SCENE_MAIN_H
#define SCENE_MAIN_H

enum MouseMode
{
    SELECTION,
    CAMERA
};

class Game :
        public OgreBites::ApplicationContext,
        public OgreBites::InputListener
{
public:
    Game();
    ~Game();

    void setup(void);

    /*
     * gets called in main loop from root before frameEnded and after frameStarted
     * usage for some class A derived from InputListener: frameStarted can give feedback if class A is 'ready'(?) for frame rendering by returning true or false.
     */
    bool frameRenderingQueued(const Ogre::FrameEvent& evt) override;
    bool keyPressed(const OgreBites::KeyboardEvent& evt) override;
    bool mousePressed(const OgreBites::MouseButtonEvent& evt) override;
    bool mouseMoved(const OgreBites::MouseMotionEvent& evt) override;
    bool mouseReleased(const OgreBites::MouseButtonEvent& evt) override;
    bool mouseWheelRolled(const OgreBites::MouseWheelEvent& evt) override;

    bool deformTerrain(const OgreBites::MouseButtonEvent& evt);

private:
    Ogre::SceneManager* m_sceneManager;

    MouseMode mouseMode = MouseMode::CAMERA;

    Ogre::Camera* m_camera;
    CameraControl* m_cameraControl;
    TerrainLoader* m_terrainLoader;
    std::shared_ptr<ecs::RenderSystem> renderSystem;
    std::shared_ptr<ecs::MovementSystem> movementSystem;
    std::shared_ptr<ecs::TerrainCollisionSystem> terrainCollisionSystem;
    std::shared_ptr<ecs::GravitySystem> gravitySystem;
    EntityFactory * m_entityFactory;
};

#endif //SCENE_MAIN_H
