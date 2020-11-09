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

    bool keyPressed(const OgreBites::KeyboardEvent& evt);
    bool mouseMoved(const OgreBites::MouseMotionEvent& evt);
    bool mousePressed(const OgreBites::MouseButtonEvent& evt);
    bool mouseReleased(const OgreBites::MouseButtonEvent& evt);
    bool mouseWheelRolled(const OgreBites::MouseWheelEvent& evt);

private:
    Ogre::SceneManager* m_sceneManager;

    Ogre::Camera* m_camera;
    CameraControl* m_cameraControl;
    TerrainLoader* m_terrainLoader;

    Grid* grid;
};

#endif //SCENE_MAIN_H
