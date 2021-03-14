#include <iostream>
#include <random>
#include <Ogre.h>
#include <RTShaderSystem/OgreRTShaderSystem.h>
#include <Bites/OgreApplicationContext.h>
#include <OgreBitesConfigDialog.h>
#include "Game.h"
#include "ecs/Register.h"
#include "ecs/components/Components.h"
#include "ecs/systems/RenderSystem.h"

//#define PAGING
//#define DEBUG


ecs::Register aRegister;

Game::Game() : OgreBites::ApplicationContext("first try")
{

}

Game::~Game()
{
    m_sceneManager = nullptr;
    m_camera = nullptr;
    delete m_terrainLoader;
    delete m_cameraControl;
    delete m_entityFactory;
}

bool Game::deformTerrain(const OgreBites::MouseButtonEvent & evt)
{
    Ogre::Real mousePosNormX = (float)evt.x / m_cameraControl->getCamera()->getViewport()->getActualWidth();
    Ogre::Real mousePosNormY = (float)evt.y / m_cameraControl->getCamera()->getViewport()->getActualHeight();

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

void Game::simulateCommandSystem(const OgreBites::MouseButtonEvent &evt)
{
    // get destination on terrain from mouse click
    Ogre::Real mousePosNormX = (float)evt.x / m_cameraControl->getCamera()->getViewport()->getActualWidth();
    Ogre::Real mousePosNormY = (float)evt.y / m_cameraControl->getCamera()->getViewport()->getActualHeight();
    Ogre::Ray mouseRay = m_cameraControl->getCamera()->getCameraToViewportRay(mousePosNormX, mousePosNormY);
    Ogre::TerrainGroup::RayResult result = m_terrainLoader->getTerrainGroup()->rayIntersects(mouseRay);

    if (not result.hit)
        return;

    // choose entity: for now just saved in member variable

    for(auto & entity : m_tempEntities)
    {
        // set DestinationComponent of entity
        ecs::Destination & destination = aRegister.getComponent<ecs::Destination>(entity);
        destination.destination = result.position;

        // change MovementComponent: velocity to direction of destination
        ecs::Movement & movement = aRegister.getComponent<ecs::Movement>(entity);
        ecs::Transform & transform = aRegister.getComponent<ecs::Transform>(entity);
        movement.velocity = destination.destination - transform.position;
        movement.velocity.z = 0.0f;
        movement.velocity.normalise();
        movement.velocity *= 50.0f;
    }
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
            mouseMode = MouseMode::SELECTION;
        }
        else
        {
            mouseMode = MouseMode::CAMERA;
        }
    }
    if (evt.keysym.sym == OgreBites::SDLK_KP_1)
    {
        m_selectionController->switchTo("sphere");
    }
    if (evt.keysym.sym == OgreBites::SDLK_KP_2)
    {
        m_selectionController->switchTo("box");
    }
    return true;
}

bool Game::mousePressed(const OgreBites::MouseButtonEvent& evt)
{
    if (evt.button == OgreBites::BUTTON_MIDDLE)
    {
        mouseMode = mouseMode == MouseMode::CAMERA ? MouseMode::SELECTION : MouseMode::CAMERA;
    }
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
        //deformTerrain(evt);
        //simulateCommandSystem(evt);

        m_isSelecting = true;
        return m_selectionController->mousePressed(evt);
    }
    return true;
}

bool Game::mouseMoved(const OgreBites::MouseMotionEvent& evt)
{
    if (m_isSelecting)
    {
        return m_selectionController->mouseMoved(evt);
    }
    else
    {
        return m_cameraControl->mouseMoved(evt);
    }
    return true;
}

bool Game::mouseReleased(const OgreBites::MouseButtonEvent& evt)
{
    if(m_isSelecting)
    {
        m_selectionController->mouseReleased(evt);
        m_isSelecting = false;
        return true;
    }
    else
    {
        return m_cameraControl->mouseReleased(evt);
    }
}

bool Game::mouseWheelRolled(const OgreBites::MouseWheelEvent& evt)
{
    return m_cameraControl->mouseWheelRolled(evt);
}

void Game::setup(void)
{
    OgreBites::ApplicationContext::setup();

    Ogre::Root* root = getRoot();

    //ConfigDialog* configDialog = OgreBites::getNativeConfigDialog();
    //root->showConfigDialog(configDialog);

    m_sceneManager = root->createSceneManager();

    aRegister.init();

    aRegister.registerComponentType<ecs::Transform>();
    aRegister.registerComponentType<ecs::Movement>();
    aRegister.registerComponentType<ecs::Destination>();
    aRegister.registerComponentType<ecs::Gravity>();
    aRegister.registerComponentType<ecs::TerrainCollision>();
    aRegister.registerComponentType<ecs::Mesh>();

    m_renderSystem = aRegister.registerSystem<ecs::RenderSystem>();
    {
        ecs::Signature signature{};
        ecs::ComponentType componentType = aRegister.getComponentType<ecs::Mesh>();
        signature.set(componentType, true);
        aRegister.setSystemSignature<ecs::RenderSystem>(signature);
    }
    m_renderSystem->init(m_sceneManager);

    m_movementSystem = aRegister.registerSystem<ecs::MovementSystem>();
    {
        ecs::Signature signature{};
        ecs::ComponentType componentType = aRegister.getComponentType<ecs::Transform>();
        signature.set(componentType, true);
        componentType = aRegister.getComponentType<ecs::Movement>();
        signature.set(componentType, true);
        aRegister.setSystemSignature<ecs::MovementSystem>(signature);
    }
    m_movementSystem->init(); // does nothing so far

    m_terrainCollisionSystem = aRegister.registerSystem<ecs::TerrainCollisionSystem>();
    {
        ecs::Signature signature{};
        ecs::ComponentType componentType = aRegister.getComponentType<ecs::TerrainCollision>();
        signature.set(componentType, true);
        componentType = aRegister.getComponentType<ecs::Transform>();
        signature.set(componentType, true);
        aRegister.setSystemSignature<ecs::TerrainCollisionSystem>(signature);
    }

    m_gravitySystem = aRegister.registerSystem<ecs::GravitySystem>();
    {
        ecs::Signature signature{};
        ecs::ComponentType componentType = aRegister.getComponentType<ecs::Gravity>();
        signature.set(componentType, true);
        componentType = aRegister.getComponentType<ecs::Transform>();
        signature.set(componentType, true);
        aRegister.setSystemSignature<ecs::GravitySystem>(signature);
    }

    m_destinationSystem = aRegister.registerSystem<ecs::DestinationSystem>();
    {
        ecs::Signature signature{};
        ecs::ComponentType componentType = aRegister.getComponentType<ecs::Destination>();
        signature.set(componentType, true);
        componentType = aRegister.getComponentType<ecs::Transform>();
        signature.set(componentType, true);
        aRegister.setSystemSignature<ecs::DestinationSystem>(signature);
    }

    m_terrainLoader = new TerrainLoader(m_sceneManager, TERRAIN_SIZE, TERRAIN_WORLD_SIZE);
    m_terrainLoader->loadTerrain();

    m_terrainCollisionSystem->init(m_terrainLoader->getTerrainGroup());
    m_gravitySystem->init(m_terrainLoader->getTerrainGroup());

    m_camera = m_sceneManager->createCamera("mainCamera");
    m_cameraControl = new CameraControl(m_camera, m_sceneManager, m_terrainLoader->getTerrainGroup());
    m_cameraControl->showCoordinateAxes(true);

    m_entityFactory = new EntityFactory(&aRegister, m_sceneManager);

    getRenderWindow()->addViewport(m_camera);

    addInputListener(this);

    std::default_random_engine generator;
    std::uniform_real_distribution<float> distribution(-TERRAIN_WORLD_SIZE/2.0f, TERRAIN_WORLD_SIZE/2.0f);
    for(int i = 0; i<500; i++)
    {
        float posX = distribution(generator);
        float posY = distribution(generator);
        float height = m_terrainLoader->getTerrainGroup()->getHeightAtWorldPosition(posX,posY,0);
        Ogre::Vector3 pos{posX,posY,height*3.0f};
        ecs::Entity curEntity;
        if (i%2==0)
        {
            curEntity = m_entityFactory->makeRobot(pos);
        }
        else
        {
            curEntity = m_entityFactory->makeLighter(pos);
        }
        m_tempEntities.push_back(curEntity);
    }

    m_selectionController = new SelectionController(m_sceneManager, m_cameraControl->getCamera(), m_terrainLoader->getTerrainGroup());
    m_selectionController->switchTo("sphere");
}

bool Game::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
    const Ogre::Real dt = evt.timeSinceLastFrame;

    m_gravitySystem->update(dt);
    m_movementSystem->update(dt);
    m_destinationSystem->update(dt);
    m_terrainCollisionSystem->update(dt);
    m_renderSystem->update(dt);

    // TODO: implement the rotation of the selection sphere as ecs-system
    //if(m_isSelecting)
    //{
    //    m_selectionSphere->getParentSceneNode()->rotate(Ogre::Vector3(0,0,1), Ogre::Radian(0.5*dt), Ogre::Node::TS_WORLD);
    //}

    // mInputListeners comes from Base Class ApplicationContextBase
    for(InputListenerList::iterator it = mInputListeners.begin(); it != mInputListeners.end(); ++it)
    {
        it->second->frameRendered(evt);
    }

    return true;
}



