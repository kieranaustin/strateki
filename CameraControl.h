//
// Created by kieran on 25.05.20.
//
#include <Ogre.h>
#include <Bites/OgreInput.h>
#include <Terrain/OgreTerrainGroup.h>

#ifndef SCENE_CAMERACONTROL_H
#define SCENE_CAMERACONTROL_H

class CameraControl
        : OgreBites::InputListener
{
public:
    CameraControl();
    CameraControl(Ogre::Camera* cam, Ogre::SceneManager* scnMgr);
    CameraControl(Ogre::Camera* cam, Ogre::SceneManager* scnMgr, Ogre::TerrainGroup* trnGrp);
    ~CameraControl();

    bool mouseMoved(const OgreBites::MouseMotionEvent& evt);
    bool mouseWheelRolled(const OgreBites::MouseWheelEvent& evt);
    bool mousePressed(const OgreBites::MouseButtonEvent& evt);
    bool mouseReleased(const OgreBites::MouseButtonEvent& evt);

    void showCoordinateAxes(bool show);
    void attachLight(Ogre::Light* light);
    void attachTerrainGroup(Ogre::TerrainGroup* terrainGroup);


private:
    void handleCollisionWithTerrain();

    Ogre::Camera* m_camera = nullptr;
    Ogre::SceneManager* m_sceneManager = nullptr;

    Ogre::SceneNode* m_cameraRigNode = nullptr;
    Ogre::SceneNode* m_cameraNode = nullptr;

    Ogre::TerrainGroup* m_TerrainGroup = nullptr;

    bool m_bRotateCamera = false;
    bool m_bTranslateCamera = false;

    float m_lastHeight = 0.0f;

    Ogre::Real m_TerrainWorldSize = 1/0.0f;
    uint16_t m_TerrainSize = 0;

    Ogre::Entity* m_coordAxes = nullptr;
};

#endif //SCENE_CAMERACONTROL_H
