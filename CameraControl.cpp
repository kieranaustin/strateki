//
// Created by kieran on 25.05.20.
//

#include "CameraControl.h"
#include <iostream>

CameraControl::CameraControl(Ogre::Camera* cam, Ogre::SceneManager* scnMgr, Ogre::TerrainGroup* trnGrp)
    : m_camera(cam)
    , m_sceneManager(scnMgr)
    , m_TerrainGroup(trnGrp)
{

    m_cameraRigNode = m_sceneManager->getRootSceneNode()->createChildSceneNode();
    m_cameraNode = m_cameraRigNode->createChildSceneNode();
    m_cameraRigNode->setPosition(0, 0, 0);
    m_cameraRigNode->pitch(Ogre::Radian(-M_PI/4.0f));
    m_cameraNode->setPosition(0, -60, 0);
    m_cameraNode->lookAt(Ogre::Vector3(0, 0, 0), Ogre::Node::TS_PARENT);
    m_cameraNode->attachObject(m_camera);
    m_TerrainWorldSize = m_TerrainGroup->getTerrainWorldSize();
    m_TerrainSize = m_TerrainGroup->getTerrainSize();

    m_cameraRigNode->setPosition(0,0,m_TerrainGroup->getHeightAtWorldPosition(0,0,0));
}

CameraControl::~CameraControl()
{
    delete m_cameraNode;
    delete m_cameraRigNode;
}

void CameraControl::showCoordinateAxes(bool show)
{
    if(show)
    {
        if(m_coordAxes == nullptr)
        {
            m_coordAxes = m_sceneManager->createEntity("Sinbad.mesh");
            m_cameraRigNode->attachObject(m_coordAxes);
            return;
        }
    }
    else
    {
        if(m_coordAxes == nullptr)
        {
            return;
        }
    }
    m_coordAxes->setVisible(show);
}

bool CameraControl::mousePressed(const OgreBites::MouseButtonEvent &evt) {
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
        Ogre::Vector3 curPos = m_cameraRigNode->getPosition();
        Ogre::Vector3 camPos = m_cameraNode->_getDerivedPosition();
        Ogre::Vector3 newPos = curPos;

        float distance = -m_cameraNode->getPosition().y/240.0f;
        distance = distance<500.0f ? distance : 500.0f;
        newPos -= evt.xrel*distance * m_cameraRigNode->getOrientation().xAxis();
        newPos += evt.yrel*distance * m_cameraRigNode->getOrientation().xAxis().perpendicular();
        float heightRig = m_TerrainGroup->getHeightAtWorldPosition(newPos.x, newPos.y, 0.0);
        float heightCam = m_TerrainGroup->getHeightAtWorldPosition(camPos.x, camPos.y, 0.0)+10.f;
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

        m_cameraRigNode->setPosition(newPos);
        //handleCollisionWithTerrain();
    }
    else if (m_bRotateCamera)
    {
        m_cameraRigNode->roll(Ogre::Radian(evt.xrel/180.0f), Ogre::Node::TS_PARENT);

        Ogre::Radian curTotalPitch = m_cameraRigNode->getOrientation().getPitch(false);
        Ogre::Radian rotatePitch(-evt.yrel/360.0);
        Ogre::Radian newPitch = curTotalPitch + rotatePitch;
        if(newPitch < Ogre::Radian(0.0f) && newPitch > -Ogre::Radian(M_PI/2.1f))
        {
            m_cameraRigNode->pitch(rotatePitch);
        }

        handleCollisionWithTerrain();
    }
    return true;
}

bool CameraControl::mouseWheelRolled(const OgreBites::MouseWheelEvent &evt)
{
    float scaleFactor = 1.0 + evt.y /10.0f;
    Ogre::Vector3 newPos = scaleFactor * m_cameraNode->getPosition();
    float distance =  -newPos.y;
    if (distance > 60.0f && distance < m_TerrainWorldSize/1.33f)
    {
        m_cameraNode->setPosition(newPos);
        handleCollisionWithTerrain();
    }
    return true;
}


void CameraControl::handleCollisionWithTerrain()
{
    Ogre::Vector3 posCam = m_cameraNode->_getDerivedPosition();
    Ogre::Vector3 posRig = m_cameraRigNode->_getDerivedPosition();

    float minHeightCam = m_TerrainGroup->getHeightAtWorldPosition(posCam.x, posCam.y, 0)+10.0f;
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
