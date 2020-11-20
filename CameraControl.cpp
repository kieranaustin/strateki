//
// Created by kieran on 25.05.20.
//

#include "CameraControl.h"
#include <iostream>

CameraControl::CameraControl(Ogre::Camera* cam, Ogre::SceneManager* scnMgr)
        : m_camera(cam)
        , m_sceneManager(scnMgr) 
{
    // set camera
    m_camera->setNearClipDistance(0.1);
    m_camera->setFarClipDistance(12000000.0f);
    m_camera->setAutoAspectRatio(true);

    m_cameraRigNode = m_sceneManager->getRootSceneNode()->createChildSceneNode();
    m_cameraRigNode->setPosition(0, 0, 0);
    m_cameraRigRotateNode = m_cameraRigNode->createChildSceneNode();
    m_cameraRigRotateNode->pitch(Ogre::Radian(-M_PI / 4.0f));

    m_cameraNode = m_cameraRigRotateNode->createChildSceneNode(Ogre::Vector3(0, -60, 0));
    m_cameraNode->lookAt(Ogre::Vector3(0, 0, 0), Ogre::Node::TS_PARENT);
    //m_cameraNode->setPosition(0, -60, 0);
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
    lightNodeFront->attachObject(camLightFront);

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
    lightNodeBack->attachObject(camLightBack);
}

CameraControl::CameraControl(Ogre::Camera* cam, Ogre::SceneManager* scnMgr, Ogre::TerrainGroup* trnGrp)
    : m_camera(cam)
    , m_sceneManager(scnMgr)
    , m_TerrainGroup(trnGrp)
{
    m_cameraRigNode = m_sceneManager->getRootSceneNode()->createChildSceneNode();
    m_cameraNode = m_cameraRigNode->createChildSceneNode();
    m_cameraRigNode->setPosition(0, 0, 0);
    m_cameraRigNode->pitch(Ogre::Radian(-M_PI/4.0f));
    m_cameraNode->setPosition(0, -600, 0);
    m_cameraNode->lookAt(Ogre::Vector3(0, 0, 0), Ogre::Node::TS_PARENT);
    m_cameraNode->attachObject(m_camera);

    Ogre::Light* light = m_sceneManager->createLight("CameraLight");
    light->setType(Ogre::Light::LT_SPOTLIGHT);
    light->setVisible(true);
    light->setDiffuseColour(Ogre::ColourValue::Red);
    light->setSpecularColour(Ogre::ColourValue::Red);
    Ogre::SceneNode* lightNode = m_cameraNode->createChildSceneNode();
    lightNode->setPosition(0, 0, 0);
    lightNode->setDirection(Ogre::Vector3(0,1,0));
    lightNode->attachObject(light);

    m_TerrainWorldSize = m_TerrainGroup->getTerrainWorldSize();
    m_TerrainSize = m_TerrainGroup->getTerrainSize();

    m_cameraRigNode->setPosition(0,0,m_TerrainGroup->getHeightAtWorldPosition(0,0,0));
}

CameraControl::~CameraControl()
{
    delete m_cameraNode;
    delete m_cameraRigRotateNode;
    delete m_cameraRigNode;
    delete m_coordAxes;
    delete m_coordAxesRotateNode;
    delete m_coordAxesAnimState;
}

bool CameraControl::frameStarted(const Ogre::FrameEvent &evt)
{
    if (m_coordAxesAnimState == nullptr)
    {
        return true;
    }
    m_coordAxesAnimState->addTime(evt.timeSinceLastFrame);
    return true;
}

void CameraControl::attachTerrainGroup(Ogre::TerrainGroup *terrainGroup)
{
    m_TerrainGroup = terrainGroup;

    m_TerrainWorldSize = m_TerrainGroup->getTerrainWorldSize();
    m_TerrainSize = m_TerrainGroup->getTerrainSize();

    m_cameraRigNode->setPosition(0,0,m_TerrainGroup->getHeightAtWorldPosition(0,0,0));
}

void CameraControl::attachLight(Ogre::Light *light)
{
    m_cameraRigNode->attachObject(light);
}

void CameraControl::showCoordinateAxes(bool show)
{
    if(show)
    {
        if(m_coordAxes == nullptr)
        {
            /*
            m_coordAxesNode = m_cameraRigNode->createChildSceneNode();
            //m_coordAxes = m_sceneManager->createEntity("Sinbad.mesh");
            //m_coordAxes = m_sceneManager->createEntity("Cube.mesh");
            //m_coordAxes = m_sceneManager->createEntity("MyCubee.mesh");
            //m_coordAxes = m_sceneManager->createEntity("lighter.mesh");
            m_coordAxes = m_sceneManager->createEntity("Wolf1_Material__wolf_col_tga_0.mesh");
            Ogre::Entity* lighter = m_sceneManager->createEntity("lighter.mesh");
            Ogre::Entity* lighter2 = m_sceneManager->createEntity("lighter.mesh");
            std::cout << "wolf name = " << m_coordAxes->getName() << std::endl;
            std::cout << "lighter name = " << lighter->getName() << std::endl;
            std::cout << "lighter2 name = " << lighter2->getName() << std::endl;
            //m_coordAxes = m_sceneManager->createEntity("robot.mesh");
            m_coordAxesNode->attachObject(m_coordAxes);
            m_coordAxesNode->rotate(Ogre::Vector3(0,0,1), Ogre::Degree(90), Ogre::Node::TS_PARENT);
            //m_coordAxesNode->rotate(Ogre::Vector3(0,1,0), Ogre::Degree(90), Ogre::Node::TS_PARENT);
            //m_coordAxesNode->rotate(Ogre::Vector3(-1,0,0), Ogre::Degree(90), Ogre::Node::TS_PARENT);
            Ogre::AnimationStateSet* animSet = m_coordAxes->getAllAnimationStates();
            std::cout << "coordAxes Animation States: " << std::endl;
            for (auto it = animSet->getAnimationStateIterator().begin(); it != animSet->getAnimationStateIterator().end(); it++)
            {
                std::cout << it->first << std::endl;
            }

            //m_coordAxesAnimState = m_coordAxes->getAnimationState("Walk");
            //m_coordAxesAnimState = m_coordAxes->getAnimationState("01_Run_Armature_0");
            //m_coordAxesAnimState = m_coordAxes->getAnimationState("02_walk_Armature_0");
            //m_coordAxesAnimState = m_coordAxes->getAnimationState("03_creep_Armature_0");
            m_coordAxesAnimState = m_coordAxes->getAnimationState("04_Idle_Armature_0");
            //m_coordAxesAnimState = m_coordAxes->getAnimationState("05_site_Armature_0");
            m_coordAxesAnimState->setLoop(true);
            m_coordAxesAnimState->setEnabled(true);
            return;
             */

            //m_coordAxesNode = m_cameraRigNode->createChildSceneNode();
            m_coordAxes = m_sceneManager->createManualObject("coordAxes");
            // TODO: set colours red, green, blue for each axis
            m_coordAxes->begin("Tile/Red", Ogre::RenderOperation::OT_LINE_LIST, "Map");
                m_coordAxes->position(-50, 0, 0);
                m_coordAxes->position(+50, 0, 0);
                m_coordAxes->position(0, -50, 0);
                m_coordAxes->position(0, +50, 0);
                m_coordAxes->position(0, 0, -50);
                m_coordAxes->position(0, 0, +50);
            m_coordAxes->end();
            m_cameraRigNode->attachObject(m_coordAxes);

            m_coordAxesRotateNode = m_sceneManager->createManualObject("coordAxesRotateNode");
            m_coordAxesRotateNode->begin("Tile/Blue", Ogre::RenderOperation::OT_LINE_LIST, "Map");
                m_coordAxesRotateNode->position(-50, 0, 0);
                m_coordAxesRotateNode->position(+50, 0, 0);
                m_coordAxesRotateNode->position(0, -50, 0);
                m_coordAxesRotateNode->position(0, +50, 0);
                m_coordAxesRotateNode->position(0, 0, -50);
                m_coordAxesRotateNode->position(0, 0, +50);
            m_coordAxesRotateNode->end();
            // TODO: make this material translucent
            //Ogre::MaterialPtr _mat = static_cast<Ogre::MaterialPtr> (Ogre::MaterialManager::getSingleton().getByName("Tile/Blue"));
            //_mat->getTechnique(0)->getPass(0)->setDiffuse(0.616687,0.90461,0.5,0.5);
            //m_coordAxesRotateNode->setMaterial(0, _mat);
            m_cameraRigRotateNode->attachObject(m_coordAxesRotateNode);
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
        Ogre::Vector3 curPos = m_cameraRigNode->getPosition();
        Ogre::Vector3 camPos = m_cameraNode->_getDerivedPosition();
        Ogre::Vector3 newPos = curPos;

        float distance = -m_cameraNode->getPosition().y/240.0f;
        distance = distance<500.0f ? distance : 500.0f;
        newPos -= evt.xrel*distance * m_cameraRigRotateNode->getOrientation().xAxis();
        newPos += evt.yrel*distance * m_cameraRigRotateNode->getOrientation().xAxis().perpendicular();
        if (m_TerrainGroup != nullptr)
        {
            float heightRig = m_TerrainGroup->getHeightAtWorldPosition(newPos.x, newPos.y, 0.0);
            float heightCam = m_TerrainGroup->getHeightAtWorldPosition(camPos.x, camPos.y, 0.0) + 10.f;
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
        //handleCollisionWithTerrain();
    }
    else if (m_bRotateCamera)
    {
        m_cameraRigRotateNode->roll(Ogre::Radian(evt.xrel/180.0f), Ogre::Node::TS_PARENT);

        Ogre::Radian curTotalPitch = m_cameraRigRotateNode->getOrientation().getPitch(false);
        Ogre::Radian rotatePitch(-evt.yrel/360.0);
        Ogre::Radian newPitch = curTotalPitch + rotatePitch;
        if(newPitch < Ogre::Radian(0.0f) && newPitch > -Ogre::Radian(M_PI/2.1f))
        {
            m_cameraRigRotateNode->pitch(rotatePitch);
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
    if (distance > 1.0f && distance < m_TerrainWorldSize/1.33f)
    {
        m_cameraNode->setPosition(newPos);
        handleCollisionWithTerrain();
    }
    return true;
}


void CameraControl::handleCollisionWithTerrain()
{
    if (m_TerrainGroup == nullptr)
    {
        return;
    }

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
