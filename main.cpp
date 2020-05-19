#include <iostream>
#include <Ogre.h>
#include <RTShaderSystem/OgreRTShaderSystem.h>
#include <Bites/OgreApplicationContext.h>
#include <OgreRenderWindow.h>
#include <OgreBitesConfigDialog.h>
#include "main.h"

using namespace Ogre;

scene::scene() : OgreBites::ApplicationContext("first try")
{

}

bool scene::mousePressed(const OgreBites::MouseButtonEvent &evt) {
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

bool scene::mouseReleased(const OgreBites::MouseButtonEvent &evt)
{
    std::cout << evt.type << std::endl;
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

bool scene::mouseMoved(const OgreBites::MouseMotionEvent &evt)
{
    if(m_bTranslateCamera)
    {
        Vector3 curPos = m_cameraNode->getPosition();
        Vector3 newPos = curPos;
        newPos.x -= evt.xrel/100.0;
        newPos.y += evt.yrel/100.0;
        if (   newPos.x > grid->minBound().x - 0.5
            && newPos.x < grid->maxBound().x - 0.5
            && newPos.y > grid->minBound().y - 0.5
            && newPos.y < grid->maxBound().y - 0.5)
        {
            m_cameraNode->setPosition(newPos);
        }
    }
    else if (m_bRotateCamera)
    {
        Ogre::Radian curTotalPitch = m_cameraNode->getOrientation().getPitch(0);
        //Ogre::Radian curYaw(evt.xrel/180.0);
        Ogre::Radian rotatePitch(-evt.yrel/180.0);
        Ogre::Radian newPitch = curTotalPitch + rotatePitch;
        if(newPitch > Ogre::Radian(0.0)
            && newPitch < Ogre::Radian(M_PI/2.0))
        {
            m_cameraNode->pitch(rotatePitch);
        }
    }
    return true;
}

bool scene::mouseWheelRolled(const OgreBites::MouseWheelEvent &evt)
{
    float scaleFactor = 1.0;
    if (evt.y < 0)
    {
        scaleFactor = 0.9;
    }
    if (evt.y > 0)
    {
        scaleFactor = 1.1;
    }
    Vector3 newPos = m_cameraNode->getPosition();
    newPos.z *= scaleFactor;
    if (newPos.z > 0.5)
    {
        m_cameraNode->setPosition(newPos);
    }
    return true;
}

bool scene::keyPressed(const OgreBites::KeyboardEvent& evt)
{
    if (evt.keysym.sym == OgreBites::SDLK_ESCAPE)
    {
        getRoot()->queueEndRendering();
    }
    return true;
}

void scene::setup(void)
{
    OgreBites::ApplicationContext::setup();

    addInputListener(this);

    Root* root = getRoot();

    //ConfigDialog* configDialog = OgreBites::getNativeConfigDialog();
    //root->showConfigDialog(configDialog);


    SceneManager* scnMgr = root->createSceneManager();

    // register our scene with the RTSS
    Ogre::RTShader::ShaderGenerator* shadergen = Ogre::RTShader::ShaderGenerator::getSingletonPtr();
    shadergen->addSceneManager(scnMgr);


    // setup Grid
    int gridX = 15;
    int gridY = 20;
    grid = new Grid(gridX, gridY);
    grid->setup(scnMgr);

    // without light we would just get a black screen
    //scnMgr->setAmbientLight(ColourValue(0.5,0.5,0.5));
    Ogre::Light* light = scnMgr->createLight("MainLight");
    Ogre::SceneNode* lightNode = scnMgr->getRootSceneNode()->createChildSceneNode();
    lightNode->setPosition(0, 0, 15);
    lightNode->attachObject(light);

    // also need to tell where we are
    m_cameraNode = scnMgr->getRootSceneNode()->createChildSceneNode();
    m_cameraNode->setPosition(gridX/2.0, gridY/2.0, 15);
    m_cameraNode->lookAt(Ogre::Vector3(gridX/2.0, gridY/2.0, 0), Ogre::Node::TS_PARENT);

    // create the camera
    Ogre::Camera* cam = scnMgr->createCamera("mainCamera");
    cam->setNearClipDistance(0.1); // specific to this sample
    cam->setAutoAspectRatio(true);
    m_cameraNode->attachObject(cam);

    // and tell it to render into the main window
    getRenderWindow()->addViewport(cam);

    // finally something to render
    Ogre::Entity* entSinbad = scnMgr->createEntity("Sinbad.mesh");
    m_entityNode = scnMgr->getRootSceneNode()->createChildSceneNode();
    m_entityNode->setScale(0.1, 0.1, 0.1);
    m_entityNode->pitch(Ogre::Radian(M_PI/2.));
    m_entityNode->setPosition(gridX/2., gridY/2.0, 0.5);
    m_entityNode->attachObject(entSinbad);
}


int main() {

    std::cout << "Hello, World!" << std::endl;

    scene app;
    app.initApp();
    app.getRoot()->startRendering();
    app.closeApp();


    //std::this_thread::sleep_for(std::chrono::milliseconds(5000));

    return 0;
}