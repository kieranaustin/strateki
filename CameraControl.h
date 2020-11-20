//
// Created by kieran on 25.05.20.
//
#include <Ogre.h>
#include <Bites/OgreInput.h>
#include <Terrain/OgreTerrainGroup.h>

#ifndef SCENE_CAMERACONTROL_H
#define SCENE_CAMERACONTROL_H

class CameraControl
        : public OgreBites::InputListener, public Ogre::FrameListener
{
public:
    CameraControl();
    CameraControl(Ogre::Camera* cam, Ogre::SceneManager* scnMgr);
    CameraControl(Ogre::Camera* cam, Ogre::SceneManager* scnMgr, Ogre::TerrainGroup* trnGrp);
    ~CameraControl();

    bool frameStarted(const Ogre::FrameEvent& evt);

    bool mouseMoved(const OgreBites::MouseMotionEvent& evt);
    bool mouseWheelRolled(const OgreBites::MouseWheelEvent& evt);
    bool mousePressed(const OgreBites::MouseButtonEvent& evt);
    bool mouseReleased(const OgreBites::MouseButtonEvent& evt);

    void showCoordinateAxes(bool show);
    void attachLight(Ogre::Light* light);
    void attachTerrainGroup(Ogre::TerrainGroup* terrainGroup);

    Ogre::Camera * getCamera() { return m_camera; }


private:
    void handleCollisionWithTerrain();

    Ogre::Camera* m_camera = nullptr;
    Ogre::SceneManager* m_sceneManager = nullptr;

    Ogre::SceneNode* m_cameraRigNode = nullptr;
    Ogre::SceneNode* m_cameraRigRotateNode = nullptr;
    Ogre::SceneNode* m_cameraNode = nullptr;

    Ogre::TerrainGroup* m_TerrainGroup = nullptr;

    bool m_bRotateCamera = false;
    bool m_bTranslateCamera = false;

    float m_lastHeight = 0.0f;

    Ogre::Real m_TerrainWorldSize = 1/0.0f;
    uint16_t m_TerrainSize = 0;

    Ogre::ManualObject * m_coordAxes = nullptr;
    Ogre::ManualObject * m_coordAxesRotateNode = nullptr;

    Ogre::AnimationState* m_coordAxesAnimState = nullptr;
};

#endif //SCENE_CAMERACONTROL_H
