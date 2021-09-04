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
#include "ecs/systems/CollisionSystem.h"
#include "EntityFactory.h"
#include "SelectionController.h"
#include "SelectionBox.h"
#include "AuxiliaryIdManager.h"
#include "InfoOverlay.h"

#ifndef SCENE_MAIN_H
#define SCENE_MAIN_H

#define TERRAIN_WORLD_SIZE 6000.0f
#define TERRAIN_SIZE 513
//static double aTerrainScale = M_PI * TERRAIN_WORLD_SIZE / (double)TERRAIN_SIZE;
static double aTerrainScale = M_PI * 8000.0f / (double)513;

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
    void setupECS(void);
    void setupEntities(void);

    /*
     * gets called in main loop from root before frameEnded and after frameStarted
     * usage for some class A derived from InputListener: frameStarted can give feedback if class A is 'ready'(?) for frame rendering by returning true or false.
     */
    bool frameRenderingQueued(const Ogre::FrameEvent& evt) override;
    bool frameEnded(const Ogre::FrameEvent& evt) override;
    bool keyPressed(const OgreBites::KeyboardEvent& evt) override;
    bool keyReleased(const OgreBites::KeyboardEvent& evt) override;
    bool mousePressed(const OgreBites::MouseButtonEvent& evt) override;
    bool mouseMoved(const OgreBites::MouseMotionEvent& evt) override;
    bool mouseReleased(const OgreBites::MouseButtonEvent& evt) override;
    bool mouseWheelRolled(const OgreBites::MouseWheelEvent& evt) override;

private:
    bool deformTerrain(const OgreBites::MouseButtonEvent& evt);
    void simulateCommandSystem(const OgreBites::MouseButtonEvent& evt);
    void getSelection();

private:
    Ogre::SceneManager* m_sceneManager;

    MouseMode mouseMode = MouseMode::CAMERA;

    Ogre::Camera* m_camera;
    InfoOverlay * m_infoOverlay;
    CameraControl* m_cameraControl;
    TerrainLoader* m_terrainLoader;
    std::shared_ptr<ecs::RenderSystem> m_renderSystem;
    std::shared_ptr<ecs::MovementSystem> m_movementSystem;
    std::shared_ptr<ecs::TerrainCollisionSystem> m_terrainCollisionSystem;
    std::shared_ptr<ecs::GravitySystem> m_gravitySystem;
    std::shared_ptr<ecs::CollisionSystem> m_collisionSystem;
    EntityFactory * m_entityFactory;

    // Auxiliary ID
    //AuxiliaryIdManager<Ogre::String> m_auxIdManager;

    // Selection
    bool m_isSelecting = false;
    SelectionController * m_selectionController;
    std::list<ecs::Entity> m_selection;

    std::vector<ecs::Entity> m_tempEntities;

    uint8_t m_numFrames{0};
    Ogre::Timer m_timer;

};

#endif //SCENE_MAIN_H
