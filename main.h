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
    void loadTerrain(void);
    bool keyPressed(const OgreBites::KeyboardEvent& evt);
    bool mouseMoved(const OgreBites::MouseMotionEvent& evt);
    bool mousePressed(const OgreBites::MouseButtonEvent& evt);
    bool mouseReleased(const OgreBites::MouseButtonEvent& evt);
    bool mouseWheelRolled(const OgreBites::MouseWheelEvent& evt);

private:
    void configureTerrainDefaults(Ogre::Light* light);
    void defineTerrain(long x, long y, bool flat = false);
    void getTerrainImage(bool flipX, bool flipY, Ogre::Image& img);
    void initBlendMaps(Ogre::Terrain* terrain);

    bool m_TerrainsImported = false;
    Ogre::SceneManager* m_sceneManager;

    Ogre::Camera* m_camera;
    CameraControl* m_cameraControl;

    Ogre::TerrainGlobalOptions* m_TerrainGlobOpts;
    Ogre::TerrainGroup* m_TerrainGroup;

    Grid* grid;
};

#endif //SCENE_MAIN_H
