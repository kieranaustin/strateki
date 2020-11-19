#include <iostream>
#include <Ogre.h>
#include <RTShaderSystem/OgreRTShaderSystem.h>
#include <Bites/OgreApplicationContext.h>
#include <OgreRenderWindow.h>
#include <OgreBitesConfigDialog.h>
#include "Game.h"
#include "ecs/Register.h"
#include "ecs/components/Components.h"
#include "ecs/systems/MeshSystem.h"

//#define PAGING
//#define DEBUG

#define TERRAIN_WORLD_SIZE 8000.0f
#define TERRAIN_SIZE 513

using namespace Ogre;

ecs::Register aRegister;

Game::Game() : OgreBites::ApplicationContext("first try")
{

}

Game::~Game()
{
    delete grid;
    delete m_terrainLoader;
    delete m_cameraControl;
    delete m_camera;
    delete m_sceneManager;
}

bool Game::deformTerrain(const OgreBites::MouseButtonEvent & evt)
{
    Ogre::Real mousePosNormX = (float)evt.x / m_camera->getViewport()->getActualWidth();
    Ogre::Real mousePosNormY = (float)evt.y / m_camera->getViewport()->getActualHeight();

    Ogre::Ray mouseRay = m_cameraControl->getCamera()->getCameraToViewportRay(mousePosNormX, mousePosNormY);
    Ogre::TerrainGroup::RayResult result = m_terrainLoader->getTerrainGroup()->rayIntersects(mouseRay);

#ifdef DEBUG
    std::cout << "---- result.(x, y, z) = " << result.position.x << ", " << result.position.y << ", " << result.position.z << std::endl;
#endif

    if (not result.hit)
        return false;


    // convert result.position to normalized terrain world size (= 0...1.0)
    float xNormWorldPos = result.position.x / TERRAIN_WORLD_SIZE + 0.5;
    float yNormWorldPos = result.position.y / TERRAIN_WORLD_SIZE + 0.5;

    // convert to terrain size (0...TERRAIN_SIZE-1)
    // TODO: Ogre has some problem, if we give it {x,y}Terrain >= TERRAIN_SIZE-1 = 512, which I thought was valid
    long xTerrain{(long)(xNormWorldPos*(TERRAIN_SIZE-2)+1.0)};
    long yTerrain{(long)(yNormWorldPos*(TERRAIN_SIZE-2)+1.0)};

#ifdef DEBUG
    std::cout << "---- terrain size = " << m_terrainLoader->getTerrainGroup()->getTerrainSize() << std::endl;
    std::cout << "---- terrain world size = " << m_terrainLoader->getTerrainGroup()->getTerrainWorldSize() << std::endl;
    std::cout << "---- xNormWorldPos, yNormWorldPos = " << xNormWorldPos << ", " << yNormWorldPos << std::endl;
    std::cout << "---- xTerrainPos, yTerrainPos = " << xNormWorldPos*(TERRAIN_SIZE-2)+1.0 << ", " << yNormWorldPos*(TERRAIN_SIZE-2)+1.0 << std::endl;
    std::cout << "---- discrete terrain pos (x, y) = " << xTerrain << ", " << yTerrain << std::endl;
#endif

    float terrainHeight = result.terrain->getHeightAtTerrainPosition(xNormWorldPos, yNormWorldPos);
    result.terrain->setHeightAtPoint(xTerrain, yTerrain,terrainHeight+10.0);

    result.terrain->update(true);

    return true;
}

bool Game::keyPressed(const OgreBites::KeyboardEvent& evt)
{
    if (evt.keysym.sym == OgreBites::SDLK_ESCAPE)
    {
        getRoot()->queueEndRendering(true);
    }
    if (evt.keysym.sym == OgreBites::SDLK_SPACE)
    {
        if (mouseMode == MouseMode::CAMERA)
        {
            mouseMode = MouseMode ::SELECTION;
        }
        else
        {
            mouseMode = MouseMode::CAMERA;
        }
    }
    return true;
}

bool Game::mousePressed(const OgreBites::MouseButtonEvent& evt)
{
    if (mouseMode == MouseMode::CAMERA)
    {
        m_cameraControl->mousePressed(evt);
    }
    else
    {
        if (evt.button == OgreBites::BUTTON_RIGHT)
            m_cameraControl->mousePressed(evt);

        if (evt.button != OgreBites::BUTTON_LEFT)
            return false;

        // deform Terrain for now
        deformTerrain(evt);
    }
    return true;
}

bool Game::mouseMoved(const OgreBites::MouseMotionEvent& evt)
{
    return m_cameraControl->mouseMoved(evt);
}

bool Game::mouseReleased(const OgreBites::MouseButtonEvent& evt)
{
    return m_cameraControl->mouseReleased(evt);
}

bool Game::mouseWheelRolled(const OgreBites::MouseWheelEvent& evt)
{
    return m_cameraControl->mouseWheelRolled(evt);
}

bool Game::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
    // mInputListeners comes from Base Class ApplicationContextBase
    for(InputListenerList::iterator it = mInputListeners.begin(); it != mInputListeners.end(); ++it) 
    {
        it->second->frameRendered(evt);
    }

    meshSystem->update(evt);
    movementSystem->update(evt);

    return true;
}

void Game::setup(void)
{
    OgreBites::ApplicationContext::setup();

    Root* root = getRoot();

    aRegister.init();

    //ConfigDialog* configDialog = OgreBites::getNativeConfigDialog();
    //root->showConfigDialog(configDialog);

    m_sceneManager = root->createSceneManager();

    // register our scene with the RTSS
    Ogre::RTShader::ShaderGenerator* shadergen = Ogre::RTShader::ShaderGenerator::getSingletonPtr();
    shadergen->addSceneManager(m_sceneManager);

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

    aRegister.init();

    aRegister.registerComponentType<ecs::Transform>();
    aRegister.registerComponentType<ecs::Movement>();
    aRegister.registerComponentType<ecs::Gravity>();
    aRegister.registerComponentType<ecs::Mesh>();

    movementSystem = aRegister.registerSystem<ecs::MovementSystem>();
    {
        ecs::Signature signature{};
        ecs::ComponentType componentType = aRegister.getComponentType<ecs::Transform>();
        signature.set(componentType, true);
        aRegister.setSystemSignature<ecs::MovementSystem>(signature);
    }
    movementSystem->init(); // does nothing so far

    meshSystem = aRegister.registerSystem<ecs::MeshSystem>();
    {
        ecs::Signature signature{};
        ecs::ComponentType componentType = aRegister.getComponentType<ecs::Mesh>();
        signature.set(componentType, true);
        aRegister.setSystemSignature<ecs::MeshSystem>(signature);
    }
    meshSystem->init(m_sceneManager);

    // and tell it to render into the main window
    getRenderWindow()->addViewport(m_camera);

    addInputListener(this);
}



