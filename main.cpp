#include <iostream>
#include <Ogre.h>
#include <RTShaderSystem/OgreRTShaderSystem.h>
#include <Bites/OgreApplicationContext.h>
#include <OgreRenderWindow.h>
#include <OgreBitesConfigDialog.h>
#include "main.h"

//#define PAGING

#define TERRAIN_WORLD_SIZE 8000.0f
#define TERRAIN_SIZE 513

using namespace Ogre;

scene::scene() : OgreBites::ApplicationContext("first try")
{

}

scene::~scene()
{
    delete grid;
    delete m_terrainLoader;
    delete m_cameraControl;
    delete m_camera;
    delete m_sceneManager;
}

bool scene::mousePressed(const OgreBites::MouseButtonEvent &evt)
{
    m_cameraControl->mousePressed(evt);
    return true;
}

bool scene::mouseReleased(const OgreBites::MouseButtonEvent &evt)
{
    m_cameraControl->mouseReleased(evt);
    return true;
}

bool scene::mouseMoved(const OgreBites::MouseMotionEvent &evt)
{
    m_cameraControl->mouseMoved(evt);
    return true;
}

bool scene::mouseWheelRolled(const OgreBites::MouseWheelEvent &evt)
{
    m_cameraControl->mouseWheelRolled(evt);
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

    m_sceneManager = root->createSceneManager();

    // register our scene with the RTSS
    Ogre::RTShader::ShaderGenerator* shadergen = Ogre::RTShader::ShaderGenerator::getSingletonPtr();
    shadergen->addSceneManager(m_sceneManager);

    // setup Grid
    /*
    int gridX = 15;
    int gridY = 20;
    grid = new Grid(gridX, gridY);
    grid->setup(m_sceneManager);
     */

    // without light we would just get a black screen
    //m_sceneManager->setAmbientLight(ColourValue(0.1f,0.1f,0.1f));
    Ogre::Light* light = m_sceneManager->createLight("MainLight");
    light->setType(Ogre::Light::LT_SPOTLIGHT);
    light->setSpotlightRange(Ogre::Degree(30), Ogre::Degree(50));
    light->setDirection(0,0,-1);
    light->setSpecularColour(ColourValue::Blue);
    light->setDiffuseColour(ColourValue::Blue);
    light->setPowerScale(10000);
    Ogre::SceneNode* lightNode = m_sceneManager->getRootSceneNode()->createChildSceneNode();
    lightNode->setPosition(0, 0, 500);
    lightNode->attachObject(light);

    // create the camera and attach to camera controller
    m_camera = m_sceneManager->createCamera("mainCamera");
    m_camera->setNearClipDistance(0.1); // specific to this sample
    m_camera->setFarClipDistance(12000000.0f);
    m_camera->setAutoAspectRatio(true);

    m_cameraControl = new CameraControl(m_camera, m_sceneManager);

    m_terrainLoader = new TerrainLoader(m_sceneManager, TERRAIN_SIZE, TERRAIN_WORLD_SIZE);
    m_terrainLoader->loadTerrain();

    m_cameraControl->attachTerrainGroup(m_terrainLoader->getTerrainGroup());
    m_cameraControl->showCoordinateAxes(true);

    // and tell it to render into the main window
    getRenderWindow()->addViewport(m_camera);

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