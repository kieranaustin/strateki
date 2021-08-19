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
AuxiliaryIdManager<Ogre::String> aAuxIdManager;

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

    for(auto & entity : m_selection)
    {
        bool good = aRegister.hasComponent<ecs::Destination>(entity) &&
                    aRegister.hasComponent<ecs::Movement>(entity) &&
                    aRegister.hasComponent<ecs::Transform>(entity);
        std::cout << "entity " << entity << ", good " << good << std::endl;
        if(good)
        {
            // set DestinationComponent of entity
            ecs::Destination &destination = aRegister.getComponent<ecs::Destination>(entity);
            destination.destination = result.position;

            // change MovementComponent: velocity to direction of destination
            ecs::Movement &movement = aRegister.getComponent<ecs::Movement>(entity);
            ecs::Transform &transform = aRegister.getComponent<ecs::Transform>(entity);
            movement.velocity = destination.destination - transform.position;
            movement.velocity.z = 0.0f;
            movement.velocity.normalise();
            movement.velocity *= 50.0f;
        }
    }
}

void Game::getSelection()
{
    std::list<Ogre::MovableObject*> selectMO = m_selectionController->getSelectionList();
    m_selection.clear();
    std::cout << "selection: ";
    for(auto obj : selectMO)
    {
        if (aAuxIdManager.hasAuxId(obj->getName()))
        {
            ecs::Entity entity = aAuxIdManager.getEntity(obj->getName());
            m_selection.push_back(entity);
            std::cout << "[" << entity << ", " << obj->getName() << "] ";
        }
    }
    std::cout << std::endl;
    std::cout << "(" << m_selection.size() << " selected)" << std::endl;
}

bool Game::keyPressed(const OgreBites::KeyboardEvent& evt)
{
    if (evt.keysym.sym == OgreBites::SDLK_ESCAPE)
    {
        getRoot()->queueEndRendering(true);
    }
    if (evt.keysym.sym == '.')
    {
        m_cameraControl->switchVisibleCoordinateAxes();
    }
    if (evt.keysym.sym == 'w')
    {
        m_cameraControl->keyPressed(evt);
    }
    if (evt.keysym.sym == 'a')
    {
        m_cameraControl->keyPressed(evt);
    }
    if (evt.keysym.sym == 's')
    {
        m_cameraControl->keyPressed(evt);
    }
    if (evt.keysym.sym == 'd')
    {
        m_cameraControl->keyPressed(evt);
    }
    if (evt.keysym.sym == 'q')
    {
        m_cameraControl->keyPressed(evt);
    }
    if (evt.keysym.sym == 'e')
    {
        m_cameraControl->keyPressed(evt);
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

bool Game::keyReleased(const OgreBites::KeyboardEvent &evt)
{
    if (evt.keysym.sym == 'w')
    {
        m_cameraControl->keyReleased(evt);
    }
    if (evt.keysym.sym == 'a')
    {
        m_cameraControl->keyReleased(evt);
    }
    if (evt.keysym.sym == 's')
    {
        m_cameraControl->keyReleased(evt);
    }
    if (evt.keysym.sym == 'd')
    {
        m_cameraControl->keyReleased(evt);
    }
    if (evt.keysym.sym == 'q')
    {
        m_cameraControl->keyReleased(evt);
    }
    if (evt.keysym.sym == 'e')
    {
        m_cameraControl->keyReleased(evt);
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
            simulateCommandSystem(evt);
            //m_cameraControl->mousePressed(evt);

        if (evt.button != OgreBites::BUTTON_LEFT)
            return false;

        // deform Terrain for now
        //deformTerrain(evt);
        //simulateCommandSystem(evt);

        m_isSelecting = true;
        m_selection.clear();
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
        getSelection();
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

    setupECS();

    m_terrainLoader = new TerrainLoader(m_sceneManager, TERRAIN_SIZE, TERRAIN_WORLD_SIZE);
    m_terrainLoader->loadTerrain();

    m_terrainCollisionSystem->init(m_terrainLoader->getTerrainGroup());
    m_gravitySystem->init(m_terrainLoader->getTerrainGroup());

    m_camera = m_sceneManager->createCamera("mainCamera");
    m_cameraControl = new CameraControl(m_camera, m_sceneManager, m_terrainLoader->getTerrainGroup());
    m_cameraControl->setVisibleCoordinateAxes(false);

    m_entityFactory = new EntityFactory(&aRegister, m_sceneManager, &aAuxIdManager);

    getRenderWindow()->addViewport(m_camera);

    addInputListener(this);

    setupEntities();

    m_selectionController = new SelectionController(m_sceneManager, m_cameraControl->getCamera(), m_terrainLoader->getTerrainGroup());
    m_selectionController->switchTo("sphere");

    mouseMode = MouseMode::CAMERA;
}

void Game::setupECS()
{
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
}

void Game::setupEntities()
{
    std::default_random_engine generator;
    std::uniform_real_distribution<float> distribution(-TERRAIN_WORLD_SIZE/2.0f, TERRAIN_WORLD_SIZE/2.0f);
    std::uniform_real_distribution<float> distribution_strip(-TERRAIN_WORLD_SIZE/2.0f, -TERRAIN_WORLD_SIZE/6.0f);
    std::uniform_real_distribution<float> distribution_height(300, 1000);
    std::stringstream loadEnts;
    loadEnts << "loading entities: ";
    int c = 0;
    for(int i = 0; i<500; i++)
    {
        float posX = distribution_strip(generator);
        float posY = distribution(generator);
        float posZ = m_terrainLoader->getTerrainGroup()->getHeightAtWorldPosition(posX,posY,0);
        float height = distribution_height(generator);
        Ogre::Vector3 pos;
        ecs::Entity curEntity;
        if (c==0)
        {
            posZ = m_terrainLoader->getTerrainGroup()->getHeightAtWorldPosition(-posX,posY,0);
            pos = {-posX, posY, posZ+height};
            curEntity = m_entityFactory->makeRobot(pos);
            c++;
        }
        else if (c==1)
        {
            pos = {posX, posY, posZ+height};
            curEntity = m_entityFactory->makeSinbad(pos);
            Ogre::Quaternion & rot = aRegister.getComponent<ecs::Transform>(curEntity).rotation;
            rot = Ogre::Vector3::UNIT_X.getRotationTo(Ogre::Vector3::UNIT_X);
            c++;
        }
        else
        {
            posX = distribution(generator);
            posZ = m_terrainLoader->getTerrainGroup()->getHeightAtWorldPosition(posX,posY,0);
            pos = {posX, posY, posZ};
            curEntity = m_entityFactory->makeTree(pos);
            c=0;
        }
        loadEnts << "[" << curEntity << ", " << aAuxIdManager.getAuxId(curEntity) << "] ";
    }
    loadEnts << std::endl << "(" << aRegister.sizeEntities() << " entities loaded)" << std::endl;
    std::cout << loadEnts.str() << std::flush;
    std::cout << "number of ecs systems registered: " << aRegister.sizeSystems() << std::endl;
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

    m_cameraControl->frameRendered(evt);

    // mInputListeners comes from Base Class ApplicationContextBase
    for(InputListenerList::iterator it = mInputListeners.begin(); it != mInputListeners.end(); ++it)
    {
        it->second->frameRendered(evt);
    }

    return true;
}



