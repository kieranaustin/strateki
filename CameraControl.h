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
    CameraControl(Ogre::Camera* cam, Ogre::SceneManager* scnMgr, Ogre::TerrainGroup* trnGrp);
    ~CameraControl();

    bool mouseMoved(const OgreBites::MouseMotionEvent& evt);
    bool mouseWheelRolled(const OgreBites::MouseWheelEvent& evt);
    bool mousePressed(const OgreBites::MouseButtonEvent& evt);
    bool mouseReleased(const OgreBites::MouseButtonEvent& evt);

    void showCoordinateAxes(bool show);


private:
    void handleCollisionWithTerrain();

    Ogre::Camera* m_camera;
    Ogre::SceneManager* m_sceneManager;
    Ogre::TerrainGroup* m_TerrainGroup;


    Ogre::SceneNode* m_cameraRigNode;
    Ogre::SceneNode* m_cameraNode;

    bool m_bRotateCamera = false;
    bool m_bTranslateCamera = false;

    float m_lastHeight = 0.0f;

    Ogre::Real m_TerrainWorldSize;
    uint16_t m_TerrainSize;

    Ogre::Entity* m_coordAxes = nullptr;
};

#endif //SCENE_CAMERACONTROL_H
