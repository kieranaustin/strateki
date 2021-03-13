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
    m_sceneManager->destroyQuery(m_volumeQuery);
    m_sceneManager = nullptr;
    m_camera = nullptr;
    delete m_terrainLoader;
    delete m_cameraControl;
    delete m_entityFactory;
    delete m_selectionBox;
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

void Game::deselectAll()
{
    for(auto it = m_selection.begin(); it != m_selection.end(); it++)
    {
        (*it)->getParentSceneNode()->showBoundingBox(false);
    }
    m_selection.clear();
}

void Game::performBoxSelection(const Ogre::Vector2 &topLeft, const Ogre::Vector2 &bottomRight)
{
    float left = topLeft.x;
    float top = topLeft.y;
    float right = bottomRight.x;
    float bottom = bottomRight.y;

    // swap coordinates, such that always left<right and bottom<top
    if (right < left)
    {
        float temp = left;
        left = right;
        right = temp;
    }
    if (bottom < top)
    {
        float temp = top;
        top = bottom;
        bottom = temp;
    }

    //deselect all objects
    deselectAll();

    // check if selection box is too small
    if ((right - left) * (bottom - top) < 0.0001)
    {
        return;
        // TODO: select single entity via single ray maybe?
    }

    // get rays pointing from the four corners of our selection box to infinity
    Ogre::Ray rayTopLeft     = m_cameraControl->getCamera()->getCameraToViewportRay(left, top);
    Ogre::Ray rayTopRight    = m_cameraControl->getCamera()->getCameraToViewportRay(right, top);
    Ogre::Ray rayBottomLeft  = m_cameraControl->getCamera()->getCameraToViewportRay(left, bottom);
    Ogre::Ray rayBottomRight = m_cameraControl->getCamera()->getCameraToViewportRay(right, bottom);

    // define the planes that surround our selection volume, i.e., around our selection box and the rays pointing to infinity
    // planes normals point inside the volume
    Ogre::Plane planeFront, planeLeft, planeTop, planeRight, planeBottom;
    Ogre::Real p = 10; // arbitrary but identical point on rays
    planeFront  = Ogre::Plane(rayTopLeft.getOrigin(), rayTopRight.getOrigin(), rayBottomRight.getOrigin());
    planeLeft   = Ogre::Plane(rayTopLeft.getOrigin(), rayBottomLeft.getPoint(p), rayTopLeft.getPoint(p));
    planeTop    = Ogre::Plane(rayTopLeft.getOrigin(), rayTopLeft.getPoint(p), rayTopRight.getPoint(p));
    planeRight  = Ogre::Plane(rayTopRight.getOrigin(), rayTopRight.getPoint(p), rayBottomRight.getPoint(p));
    planeBottom = Ogre::Plane(rayBottomLeft.getOrigin(), rayBottomRight.getPoint(p), rayBottomLeft.getPoint(p));

    // set up the volume defined by our planes
    Ogre::PlaneBoundedVolume volume;
    volume.planes.push_back(planeFront);
    volume.planes.push_back(planeLeft);
    volume.planes.push_back(planeTop);
    volume.planes.push_back(planeRight);
    volume.planes.push_back(planeBottom);

    // create a volume list that is passed to m_volumeQuery
    Ogre::PlaneBoundedVolumeList volumeList;
    volumeList.push_back(volume);
    m_volumeQuery->setVolumes(volumeList);

    // obtain the query result
    Ogre::SceneQueryResult result = m_volumeQuery->execute();

    // select objects from query result
    // TODO: map Ogres MovableObjects to ecs entities and choose selection from there
    for(const auto obj : result.movables)
    {
        if (rejectFromSelection(obj))
            continue;

        m_selection.push_back(obj);
        obj->getParentSceneNode()->showBoundingBox(true);
        std::cout << obj->getName() << std::endl;
    }
}

void Game::performSphereSelection(const Ogre::Vector3 &center, const Ogre::Real &radius)
{
    //deselect all objects
    deselectAll();

    // perform query
    Ogre::Sphere sphere = Ogre::Sphere(center, radius);
    m_sphereQuery->setSphere(Ogre::Sphere(center, radius));
    Ogre::SceneQueryResult result = m_sphereQuery->execute();

    // select objects from query result
    for(const auto obj : result.movables)
    {
        if (rejectFromSelection(obj))
            continue;

        Ogre::Vector3 objPos = obj->getParentSceneNode()->_getDerivedPosition();
        Ogre::Real objDist = (center-objPos).length();
        if (objDist < radius)
        {
            m_selection.push_back(obj);
            obj->getParentSceneNode()->showBoundingBox(true);
            std::cout << obj->getName() << std::endl;
        }
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

        // selection
        Ogre::Real mousePosNormX = (float)evt.x / m_cameraControl->getCamera()->getViewport()->getActualWidth();
        Ogre::Real mousePosNormY = (float)evt.y / m_cameraControl->getCamera()->getViewport()->getActualHeight();
        m_isSelecting = true;
        m_selectionBoxEnd = {mousePosNormX, mousePosNormY};

        // selection box
        m_selectionBoxBegin = m_selectionBoxEnd;
        m_selectionBox->clear();
        m_selectionBox->setVisible(true);

        // selection sphere
        m_selectionSphereRadius = 0.0;
        Ogre::Ray mouseRay = m_cameraControl->getCamera()->getCameraToViewportRay(mousePosNormX, mousePosNormY);
        Ogre::TerrainGroup::RayResult result = m_terrainLoader->getTerrainGroup()->rayIntersects(mouseRay);
        if (result.hit)
        {
            // clicked on terrain
            m_selectionSphereBeginOnTerrain = true;
            m_selectionSphereCenter = result.position;
            m_selectionSphere->getParentSceneNode()->setPosition(m_selectionSphereCenter);
            m_selectionSphere->getParentSceneNode()->setScale(getSelectionSphereScale());
            m_selectionSphere->setVisible(true);
        }
        else
        {
            // clicked outside of terrain, i.e., on empty space
            m_selectionSphereBeginOnTerrain = false;
            m_selectionSphereCenter = {-1, -1, -1};
        }
    }
    return true;
}

bool Game::mouseMoved(const OgreBites::MouseMotionEvent& evt)
{
    if (m_isSelecting)
    {
        Ogre::Real mousePosNormX = (float)evt.x / m_cameraControl->getCamera()->getViewport()->getActualWidth();
        Ogre::Real mousePosNormY = (float)evt.y / m_cameraControl->getCamera()->getViewport()->getActualHeight();

        m_selectionBoxEnd = {mousePosNormX, mousePosNormY};

        // selection box
        m_selectionBox->setCorners(m_selectionBoxBegin.x, m_selectionBoxBegin.y, m_selectionBoxEnd.x, m_selectionBoxEnd.y);
        //performBoxSelection(m_selectionBoxBegin, m_selectionBoxEnd);

        // selection sphere
        Ogre::Ray mouseRay = m_cameraControl->getCamera()->getCameraToViewportRay(mousePosNormX, mousePosNormY);
        Ogre::TerrainGroup::RayResult result = m_terrainLoader->getTerrainGroup()->rayIntersects(mouseRay);
        bool selectionSphereEndOnTerrain = result.hit;
        if (m_selectionSphereBeginOnTerrain)
        {
            // mouse moved from terrain...
            Ogre::Vector3 mouseToWorldPosition;
            if (selectionSphereEndOnTerrain)
            {
                // ... onto terrain -> find world position of mouse on terrain
                mouseToWorldPosition = result.position;
            }
            else
            {
                // ... to outside of terrain -> find world position of mouse on plane,
                // where the plane is along xy-axes at the height (z) of the sphere-center
                Ogre::Real height = m_selectionSphereCenter.z;
                Ogre::Plane plane = Ogre::Plane(Ogre::Vector3(0,0, height),
                                                Ogre::Vector3(1,0, height),
                                                Ogre::Vector3(0,1, height));
                Ogre::RayTestResult mouseToPlaneResult = mouseRay.intersects(plane);
                mouseToWorldPosition = mouseRay.getPoint(mouseToPlaneResult.second);
            }
            // set and show selection sphere
            m_selectionSphereRadius = (mouseToWorldPosition - m_selectionSphereCenter).length();
            m_selectionSphere->getParentSceneNode()->setScale(getSelectionSphereScale());
            m_selectionSphere->setVisible(true);
        }
        else
        {
            // mouse moved from outside of terrain...
            if (selectionSphereEndOnTerrain)
            {
                // .. onto terrain -> set sphere-center on edge of terrain
                m_selectionSphereCenter = result.position;
                m_selectionSphere->getParentSceneNode()->setPosition(m_selectionSphereCenter);
                m_selectionSphereBeginOnTerrain = true;
            }
            // ... and is still outside of terrain -> do nothing = no selection sphere
        }
        performSphereSelection(m_selectionSphereCenter, m_selectionSphereRadius);
        m_selectionSphere->getParentSceneNode()->showBoundingBox(false);
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
        //performBoxSelection(m_selectionBoxBegin, m_selectionBoxEnd);
        m_selectionBox->clear();
        m_selectionBox->setVisible(false);
        m_isSelecting = false;

        performSphereSelection(m_selectionSphereCenter, m_selectionSphereRadius);
        m_selectionSphere->setVisible(false);
    }
    else
    {
        return m_cameraControl->mouseReleased(evt);
    }
    return true;
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

    m_selectionBox = new SelectionBox("selectionBox");
    m_sceneManager->getRootSceneNode()->createChildSceneNode()->attachObject(m_selectionBox);
    m_volumeQuery = m_sceneManager->createPlaneBoundedVolumeQuery(Ogre::PlaneBoundedVolumeList());

    m_selectionSphere = m_sceneManager->createEntity("selectionSphere", Ogre::SceneManager::PT_SPHERE);
    m_selectionSphere->setMaterialName("selectionSphereWide", "Map");
    m_selectionSphere->setVisible(false);
    m_sceneManager->getRootSceneNode()->createChildSceneNode()->attachObject(m_selectionSphere);
    m_selectionSphere->getParentSceneNode()->rotate(Ogre::Vector3(1,0,0), Ogre::Radian(M_PI/2.), Ogre::Node::TS_LOCAL);
    m_sphereQuery = m_sceneManager->createSphereQuery(Ogre::Sphere());
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
    if(m_isSelecting)
    {
        m_selectionSphere->getParentSceneNode()->rotate(Ogre::Vector3(0,0,1), Ogre::Radian(0.5*dt), Ogre::Node::TS_WORLD);
    }

    // mInputListeners comes from Base Class ApplicationContextBase
    for(InputListenerList::iterator it = mInputListeners.begin(); it != mInputListeners.end(); ++it)
    {
        it->second->frameRendered(evt);
    }

    return true;
}



