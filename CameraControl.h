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
    CameraControl(Ogre::Camera* cam, Ogre::SceneManager* scnMgr);
    CameraControl(Ogre::Camera* cam, Ogre::SceneManager* scnMgr, Ogre::TerrainGroup* trnGrp);
    ~CameraControl() override = default;

    void setup();

    void frameRendered(const Ogre::FrameEvent& evt) override;
    bool keyPressed(const OgreBites::KeyboardEvent& evt) override;
    bool keyReleased(const OgreBites::KeyboardEvent& evt) override;
    bool mouseMoved(const OgreBites::MouseMotionEvent& evt) override;
    bool mouseWheelRolled(const OgreBites::MouseWheelEvent& evt) override;
    bool mousePressed(const OgreBites::MouseButtonEvent& evt) override;
    bool mouseReleased(const OgreBites::MouseButtonEvent& evt) override;

    /*
     * move camera in 2 dimensions (x,y) along the current view point
     */
    void move(const Ogre::Vector2 & delta);

    /*
     * rotated the camera
     */
    void rotate(const Ogre::Radian & deltaYaw, const Ogre::Radian & deltaPitch);

    void makeCoordinateAxes();
    void makeCoordinateAxesRotate();
    void setVisibleCoordinateAxes(bool show);
    /*
     * switch visibility of coordinate axes
     */
    void switchVisibleCoordinateAxes();
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
    bool m_moveForward = false;
    bool m_moveLeft = false;
    bool m_moveBackward = false;
    bool m_moveRight = false;
    bool m_rotateRight = false;
    bool m_rotateLeft = false;

    float m_lastHeight = 0.0f;

    Ogre::Real m_TerrainWorldSize = 1/0.0f;
    uint16_t m_TerrainSize = 0;

    Ogre::ManualObject * m_coordAxes = nullptr;
    Ogre::ManualObject * m_coordAxesRotate = nullptr;

};

#endif //SCENE_CAMERACONTROL_H
