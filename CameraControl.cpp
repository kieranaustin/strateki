//
// Created by kieran on 25.05.20.
//

#include "CameraControl.h"
#include <iostream>

CameraControl::CameraControl(Ogre::Camera* cam, Ogre::SceneManager* scnMgr)
        : m_camera(cam)
        , m_sceneManager(scnMgr) 
{
    setup();
}

CameraControl::CameraControl(Ogre::Camera* cam, Ogre::SceneManager* scnMgr, Ogre::TerrainGroup* trnGrp)
    : m_camera(cam)
    , m_sceneManager(scnMgr)
{
    setup();
    attachTerrainGroup(trnGrp);

    float max = m_TerrainWorldSize/2.025f;
    m_cameraRigNode->setPosition(max, max, m_TerrainGroup->getHeightAtWorldPosition(max,max,0));
    m_cameraRigNode->setPosition(0.0f, -3.0f*max/4.0f, m_TerrainGroup->getHeightAtWorldPosition(0.0f,-3.0f*max/4.0f,0));
}

CameraControl::~CameraControl()
{
    m_camera = nullptr;
    m_sceneManager = nullptr;
    m_cameraRigNode = nullptr;
    m_cameraRigRotateNode = nullptr;
    m_cameraNode = nullptr;
    m_TerrainGroup = nullptr;
    m_coordAxes = nullptr;
    m_coordAxesRotate = nullptr;
}

void CameraControl::setup()
{
    // set camera
    m_camera->setNearClipDistance(0.1);
    m_camera->setFarClipDistance(12000000.0f);
    m_camera->setAutoAspectRatio(true);

    m_cameraRigNode = m_sceneManager->getRootSceneNode()->createChildSceneNode();
    m_cameraRigNode->setPosition(0, 0, 0);
    m_cameraRigRotateNode = m_cameraRigNode->createChildSceneNode();
    //m_cameraRigRotateNode->roll(Ogre::Radian(3.*M_PI/4.));
    //m_cameraRigRotateNode->roll(Ogre::Radian(M_PI));
    m_cameraRigRotateNode->pitch(Ogre::Radian(-M_PI / 8.0f));

    m_cameraNode = m_cameraRigRotateNode->createChildSceneNode(Ogre::Vector3(0, -3000, 0));
    m_cameraNode->lookAt(Ogre::Vector3(0, 0, 0), Ogre::Node::TS_PARENT);
    m_cameraNode->attachObject(m_camera);

    Ogre::Light* camLightFront = m_sceneManager->createLight("CameraLightFront");
    camLightFront->setType(Ogre::Light::LT_SPOTLIGHT);
    Ogre::Vector3 lightdir(0, 0, -1);
    lightdir.normalise();
    camLightFront->setVisible(true);
    camLightFront->setDiffuseColour(Ogre::ColourValue::White);
    camLightFront->setSpecularColour(Ogre::ColourValue::White);
    Ogre::SceneNode *lightNodeFront = m_cameraNode->createChildSceneNode();
    lightNodeFront->setPosition(0, 0, 0);
    lightNodeFront->setDirection(lightdir);
    //lightNodeFront->attachObject(camLightFront);

    Ogre::Light* camLightBack = m_sceneManager->createLight("CameraLightBack");
    camLightBack->setType(Ogre::Light::LT_SPOTLIGHT);
    lightdir = Ogre::Vector3(0,-1,0);
    lightdir.normalise();
    camLightBack->setVisible(true);
    camLightBack->setDiffuseColour(Ogre::ColourValue::White);
    camLightBack->setSpecularColour(Ogre::ColourValue::White);
    camLightBack->setPowerScale(Ogre::Real(10000));
    Ogre::SceneNode *lightNodeBack = m_cameraRigNode->createChildSceneNode();
    lightNodeBack->setPosition(0, 100, 0);
    lightNodeBack->setDirection(lightdir);
    //lightNodeBack->attachObject(camLightBack);
}

void CameraControl::attachTerrainGroup(Ogre::TerrainGroup *terrainGroup)
{
    m_TerrainGroup = terrainGroup;

    m_TerrainWorldSize = m_TerrainGroup->getTerrainWorldSize();
    m_TerrainSize = m_TerrainGroup->getTerrainSize();
}

void CameraControl::attachLight(Ogre::Light *light)
{
    m_cameraRigNode->attachObject(light);
}

void CameraControl::setVisibleCoordinateAxes(bool show)
{
    if(m_coordAxes == nullptr)
    {
        makeCoordinateAxes();
    }
    if (m_coordAxesRotate == nullptr)
    {
        makeCoordinateAxesRotate();
    }

    m_coordAxes->setVisible(show);
    m_coordAxesRotate->setVisible(show);
}

void CameraControl::switchVisibleCoordinateAxes()
{
    bool show = !m_coordAxes->isVisible();
    setVisibleCoordinateAxes(show);
}

void CameraControl::makeCoordinateAxes()
{
    // draw coordinate axes
    m_coordAxes = m_sceneManager->createManualObject("coordAxes");
    float negative_half_width = 5.0;
    float positive_half_width = 10.0;
    float length = 150.0;
    float height = 3;
    m_coordAxes->begin("Tile/Red", Ogre::RenderOperation::OT_TRIANGLE_LIST, "Map");
    {
        // negative x axis
        m_coordAxes->position(-length, 0, height);
        m_coordAxes->position(0, -negative_half_width, height);
        m_coordAxes->position(0, +negative_half_width, height);
        // positive x axis
        m_coordAxes->position(0, +positive_half_width, height);
        m_coordAxes->position(0, -positive_half_width, height);
        m_coordAxes->position(+length, 0, height);
    }
    m_coordAxes->end();
    m_coordAxes->begin("Tile/Green", Ogre::RenderOperation::OT_TRIANGLE_LIST, "Map");
    {
        // negative y axis
        m_coordAxes->position(0, -length, height);
        m_coordAxes->position(+negative_half_width, 0, height);
        m_coordAxes->position(-negative_half_width, 0, height);
        // positive y axis
        m_coordAxes->position(-positive_half_width, 0, height);
        m_coordAxes->position(+positive_half_width, 0, height);
        m_coordAxes->position(0, +length, height);
    }
    m_coordAxes->end();
    m_coordAxes->begin("Tile/Blue", Ogre::RenderOperation::OT_TRIANGLE_LIST, "Map");
    {
        // normal at front
        // negative z axis
        m_coordAxes->position(0, 0, -length);
        m_coordAxes->position(+negative_half_width, 0, 0);
        m_coordAxes->position(-negative_half_width, 0, 0);
        // positive z axis
        m_coordAxes->position(-positive_half_width, 0, 0);
        m_coordAxes->position(+positive_half_width, 0, 0);
        m_coordAxes->position(0, 0, +length);
        //
        // normal at rear
        // negative z axis
        m_coordAxes->position(0, 0, -length);
        m_coordAxes->position(-negative_half_width, 0, 0);
        m_coordAxes->position(+negative_half_width, 0, 0);
        // positive z axis
        m_coordAxes->position(+positive_half_width, 0, 0);
        m_coordAxes->position(-positive_half_width, 0, 0);
        m_coordAxes->position(0, 0, +length);
        //
        // normal at side
        // negative z axis
        m_coordAxes->position(0, 0, -length);
        m_coordAxes->position(0, -negative_half_width, 0);
        m_coordAxes->position(0, +negative_half_width, 0);
        // positive z axis
        m_coordAxes->position(0, +positive_half_width, 0);
        m_coordAxes->position(0, -positive_half_width, 0);
        m_coordAxes->position(0, 0, +length);
        //
        // normal at opposite side
        // negative z axis
        m_coordAxes->position(0, 0, -length);
        m_coordAxes->position(0, +negative_half_width, 0);
        m_coordAxes->position(0, -negative_half_width, 0);
        // positive z axis
        m_coordAxes->position(0, -positive_half_width, 0);
        m_coordAxes->position(0, +positive_half_width, 0);
        m_coordAxes->position(0, 0, +length);
    }
    m_coordAxes->end();
    m_cameraRigNode->attachObject(m_coordAxes);
}

void CameraControl::makeCoordinateAxesRotate()
{
    // draw coordinate axes that rotate with the view of the camera
    m_coordAxesRotate = m_sceneManager->createManualObject("coordAxesRotate");
    float length_rotate = 100;
    m_coordAxesRotate->begin("Tile/Red", Ogre::RenderOperation::OT_LINE_LIST, "Map");
    {
        m_coordAxesRotate->position(-length_rotate, 0, 0);
        m_coordAxesRotate->position(+length_rotate, 0, 0);
    }
    m_coordAxesRotate->end();
    m_coordAxesRotate->begin("Tile/Green", Ogre::RenderOperation::OT_LINE_LIST, "Map");
    {
        m_coordAxesRotate->position(0, -length_rotate, 0);
        m_coordAxesRotate->position(0, +length_rotate, 0);
    }
    m_coordAxesRotate->end();
    m_coordAxesRotate->begin("Tile/Blue", Ogre::RenderOperation::OT_LINE_LIST, "Map");
    {
        m_coordAxesRotate->position(0, 0, -length_rotate);
        m_coordAxesRotate->position(0, 0, +length_rotate);
    }
    m_coordAxesRotate->end();
    m_cameraRigRotateNode->attachObject(m_coordAxesRotate);
}

void CameraControl::frameRendered(const Ogre::FrameEvent &evt)
{
    constexpr Ogre::Real stepSize = 5.0;
    if (m_moveForward)
        move(Ogre::Vector2(0,stepSize));
    if (m_moveLeft)
        move(Ogre::Vector2(-stepSize,0));
    if (m_moveBackward)
        move(Ogre::Vector2(0,-stepSize));
    if (m_moveRight)
        move(Ogre::Vector2(stepSize,0));
    if(m_rotateLeft)
        rotate(Ogre::Radian(-M_PI/180.0f), Ogre::Radian(0.0));
    if(m_rotateRight)
        rotate(Ogre::Radian(M_PI/180.0f), Ogre::Radian(0.0));
}

bool CameraControl::keyPressed(const OgreBites::KeyboardEvent& evt)
{
    if (evt.keysym.sym == 'w')
    {
        m_moveForward = true;
    }
    if (evt.keysym.sym == 'a')
    {
        m_moveLeft = true;
    }
    if (evt.keysym.sym == 's')
    {
        m_moveBackward = true;
    }
    if (evt.keysym.sym == 'd')
    {
        m_moveRight = true;
    }
    if (evt.keysym.sym == 'q')
    {
        m_rotateLeft = true;
    }
    if (evt.keysym.sym == 'e')
    {
        m_rotateRight = true;
    }
    return true;
}

bool CameraControl::keyReleased(const OgreBites::KeyboardEvent &evt)
{
    if (evt.keysym.sym == 'w')
    {
        m_moveForward = false;
    }
    if (evt.keysym.sym == 'a')
    {
        m_moveLeft = false;
    }
    if (evt.keysym.sym == 's')
    {
        m_moveBackward = false;
    }
    if (evt.keysym.sym == 'd')
    {
        m_moveRight = false;
    }
    if (evt.keysym.sym == 'q')
    {
        m_rotateLeft = false;
    }
    if (evt.keysym.sym == 'e')
    {
        m_rotateRight = false;
    }
    return true;
}

bool CameraControl::mousePressed(const OgreBites::MouseButtonEvent &evt)
{
    if(evt.button == OgreBites::BUTTON_LEFT)
    {
        m_bRotateCamera = true;
    }
    if(evt.button == OgreBites::BUTTON_RIGHT)
    {
        m_bTranslateCamera = true;
    }
    return true;
}

bool CameraControl::mouseReleased(const OgreBites::MouseButtonEvent &evt)
{
    if(evt.button == OgreBites::BUTTON_LEFT)
    {
        m_bRotateCamera = false;
    }
    if(evt.button == OgreBites::BUTTON_RIGHT)
    {
        m_bTranslateCamera = false;
    }
    return true;
}

bool CameraControl::mouseMoved(const OgreBites::MouseMotionEvent &evt)
{
    if(m_bTranslateCamera)
    {
        move(Ogre::Vector2(evt.xrel, evt.yrel));
    }
    if (m_bRotateCamera)
    {
        rotate(Ogre::Radian(evt.xrel/180.0f), Ogre::Radian(-evt.yrel/360.0));
    }
    return true;
}

bool CameraControl::mouseWheelRolled(const OgreBites::MouseWheelEvent &evt)
{
    float scaleFactor = 1.0 + evt.y /10.0f;
    Ogre::Vector3 newPos = scaleFactor * m_cameraNode->getPosition();
    float distance =  -newPos.y;
    if (distance > 1.0f && distance < m_TerrainWorldSize)
    {
        m_cameraNode->setPosition(newPos);
        handleCollisionWithTerrain();
    }
    return true;
}

void CameraControl::move(const Ogre::Vector2 &delta)
{
    Ogre::Vector3 curPos = m_cameraRigNode->getPosition();
    Ogre::Vector3 camPos = m_cameraNode->_getDerivedPosition();
    Ogre::Vector3 newPos = curPos;

    float distance = -m_cameraNode->getPosition().y/240.0f;
    distance = distance<500.0f ? distance : 500.0f;
    newPos += delta.x*distance * m_cameraRigRotateNode->getOrientation().xAxis();
    newPos += delta.y*distance * m_cameraRigRotateNode->getOrientation().xAxis().perpendicular();
    if (m_TerrainGroup != nullptr)
    {
        float heightRig = m_TerrainGroup->getHeightAtWorldPosition(newPos.x, newPos.y, 0.0);
        float heightCam = m_TerrainGroup->getHeightAtWorldPosition(camPos.x, camPos.y, 0.0) + 0.f;
        if (heightRig < heightCam)
        {
            newPos.z = heightCam;
        }
        else
        {
            newPos.z = heightRig;
        }

        if ( newPos.x > m_TerrainWorldSize/2.025f)
        {
            newPos.x = m_TerrainWorldSize/2.025f;
        }
        if ( newPos.x < -m_TerrainWorldSize/2.025f)
        {
            newPos.x = -m_TerrainWorldSize/2.025f;
        }
        if ( newPos.y > m_TerrainWorldSize/2.025f)
        {
            newPos.y = m_TerrainWorldSize/2.025f;
        }
        if ( newPos.y < -m_TerrainWorldSize/2.025f)
        {
            newPos.y = -m_TerrainWorldSize/2.025f;
        }
    }

    m_cameraRigNode->setPosition(newPos);
}

void CameraControl::rotate(const Ogre::Radian & deltaYaw, const Ogre::Radian & deltaPitch)
{
    m_cameraRigRotateNode->roll(deltaYaw, Ogre::Node::TS_PARENT);

    Ogre::Radian curTotalPitch = m_cameraRigRotateNode->getOrientation().getPitch(false);
    Ogre::Radian rotatePitch(deltaPitch);
    Ogre::Radian newPitch = curTotalPitch + rotatePitch;
    if(newPitch < Ogre::Radian(0.0f) && newPitch > -Ogre::Radian(M_PI/2.1f))
    {
        m_cameraRigRotateNode->pitch(rotatePitch);
    }

    handleCollisionWithTerrain();
}


void CameraControl::handleCollisionWithTerrain()
{
    if (m_TerrainGroup == nullptr)
    {
        return;
    }

    Ogre::Vector3 posCam = m_cameraNode->_getDerivedPosition();
    Ogre::Vector3 posRig = m_cameraRigNode->_getDerivedPosition();

    float minHeightCam = m_TerrainGroup->getHeightAtWorldPosition(posCam.x, posCam.y, 0)+0.0f;
    float minHeightRig = m_TerrainGroup->getHeightAtWorldPosition(posRig.x, posRig.y, 0);

    float diffCam = minHeightCam - posCam.z;
    float diffRig = minHeightRig - posRig.z;
    float diffCamRig = diffCam - diffRig;

    if (std::abs(m_lastHeight - minHeightCam) > 0.015f)
    {
        if(diffCam >= 0.0f)
        {
            m_cameraRigNode->translate(0,0,diffCam);
        }
        else
        {
            if (diffRig <= 0.0f && diffCamRig >= 0.0f)
            {
                m_cameraRigNode->translate(0,0,diffCam);
            }
            else
            {
                if (diffRig > 0.0f && diffCamRig >= 0.0f)
                {
                    m_cameraRigNode->translate(0,0,diffRig);
                }
            }
        }
    }

    m_lastHeight = minHeightCam;
}
