//
// Created by kieran on 16.05.20.
//
#include <Ogre.h>
#include <Bites/OgreApplicationContext.h>
#include <Terrain/OgreTerrain.h>
#include <Terrain/OgreTerrainGroup.h>
#include <OgreCameraMan.h>
#include "Grid.h"
#include "CameraControl.h"
#include "TerrainLoader.h"
#include "EntityManager.h"

#ifndef SCENE_MAIN_H
#define SCENE_MAIN_H

class scene :
        public OgreBites::ApplicationContext,
        public OgreBites::InputListener
{
public:
    scene();
    ~scene();

    void setup(void);

    /*
     * gets called in main loop from root before frameEnded and after frameStarted
     * usage for some class A derived from InputListener: frameStarted can give feedback if class A is 'ready'(?) for frame rendering by returning true or false.
     */
    bool frameRenderingQueued(const Ogre::FrameEvent& evt) override;
    bool keyPressed(const OgreBites::KeyboardEvent& evt) override;
    /*
     * not needed right now.
    bool mouseMoved(const OgreBites::MouseMotionEvent& evt) override;
    bool mousePressed(const OgreBites::MouseButtonEvent& evt) override;
    bool mouseReleased(const OgreBites::MouseButtonEvent& evt) override;
    bool mouseWheelRolled(const OgreBites::MouseWheelEvent& evt) override;
     */

private:
    Ogre::SceneManager* m_sceneManager;

    Ogre::Camera* m_camera;
    CameraControl* m_cameraControl;
    TerrainLoader* m_terrainLoader;
    EntityManager* m_entityManager;

    Grid* grid;
};

#endif //SCENE_MAIN_H
