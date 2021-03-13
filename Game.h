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

#define TERRAIN_WORLD_SIZE 8000.0f
#define TERRAIN_SIZE 513
// TODO: why does aTerrainScale with values 8000.0f and 513 work exactly for scaling factor of selection sphere, even when changing TERRAIN_WORLD_SIZE and TERRAIN_SIZE
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

private:
    bool deformTerrain(const OgreBites::MouseButtonEvent& evt);
    void simulateCommandSystem(const OgreBites::MouseButtonEvent& evt);

    // Selection
    /*
     * deselect all objects
     */
    void deselectAll();

    /*
     * checks whether object is viable for selection
     */
    bool rejectFromSelection(Ogre::MovableObject * obj)
    {
        Ogre::String name = obj->getName();
        if (name == "selectionSphere")
            return true;
        if (name == "coordAxes")
            return true;
        if (name == "coordAxesRotate")
            return true;

        return false;
    }

    /*
     * select objects from inside a 2d box on screen
     */
    void performBoxSelection(const Ogre::Vector2 & topLeft, const Ogre::Vector2 & bottomRight);

    /*
     * select objects from inside a sphere on terrain
     */
    void performSphereSelection(const Ogre::Vector3 & center, const Ogre::Real & radius);

    /*
     * set set the correct scale from sphere radius - caution: magic number(!?)
     */
    Ogre::Vector3 getSelectionSphereScale() const
    {
        return Ogre::Vector3(m_selectionSphereRadius / aTerrainScale);
    }

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
    bool m_isSelecting = false;

    // Selection Box/Rectangle
    SelectionBox * m_selectionBox;
    Ogre::Vector2 m_selectionBoxBegin;
    Ogre::Vector2 m_selectionBoxEnd;
    Ogre::PlaneBoundedVolumeListSceneQuery * m_volumeQuery;

    // Selection Sphere
    Ogre::Entity * m_selectionSphere;
    bool m_selectionSphereBeginOnTerrain = false;
    Ogre::Vector3 m_selectionSphereCenter = {0.0, 0.0, 0.0};
    Ogre::Real m_selectionSphereRadius = 0.0;
    Ogre::SphereSceneQuery * m_sphereQuery;

    std::vector<ecs::Entity> m_tempEntities;
};

#endif //SCENE_MAIN_H
