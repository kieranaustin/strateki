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
#include "ecs/systems/DestinationSystem.h"
#include "EntityFactory.h"
#include "SelectionBox.h"

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

    void setup(void) override;

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
    void simulateCommandSystem(const OgreBites::MouseButtonEvent& evt);
    void performSelection(const Ogre::Vector2 & topLeft, const Ogre::Vector2 & bottomRight);

private:
    Ogre::SceneManager* m_sceneManager;

    MouseMode mouseMode = MouseMode::CAMERA;

    Ogre::Camera* m_camera;
    CameraControl* m_cameraControl;
    TerrainLoader* m_terrainLoader;
    std::shared_ptr<ecs::RenderSystem> m_renderSystem;
    std::shared_ptr<ecs::MovementSystem> m_movementSystem;
    std::shared_ptr<ecs::TerrainCollisionSystem> m_terrainCollisionSystem;
    std::shared_ptr<ecs::GravitySystem> m_gravitySystem;
    std::shared_ptr<ecs::DestinationSystem> m_destinationSystem;
    EntityFactory * m_entityFactory;

    // Selection
    std::list<Ogre::MovableObject*> m_selection;
    SelectionBox * m_selectionBox;
    bool m_isSelecting = false;
    Ogre::Vector2 m_selectionStart;
    Ogre::Vector2 m_selectionEnd;
    Ogre::PlaneBoundedVolumeListSceneQuery * m_volumeQuery;

    std::vector<ecs::Entity> m_tempEntities;
};

#endif //SCENE_MAIN_H
