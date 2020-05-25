#include <iostream>
#include <Ogre.h>
#include <RTShaderSystem/OgreRTShaderSystem.h>
#include <Bites/OgreApplicationContext.h>
#include <OgreRenderWindow.h>
#include <OgreBitesConfigDialog.h>
#include "main.h"

//#define PAGING

#define TERRAIN_PAGE_MIN_X 0
#define TERRAIN_PAGE_MIN_Y 0
#define TERRAIN_PAGE_MAX_X 0
#define TERRAIN_PAGE_MAX_Y 0

#define TERRAIN_WORLD_SIZE 12000.0f
#define TERRAIN_SIZE 513

using namespace Ogre;

scene::scene() : OgreBites::ApplicationContext("first try")
{

}

scene::~scene()
{
    delete grid;
    delete m_TerrainGlobOpts;
    delete m_TerrainGroup;
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
        Vector3 curPos = m_cameraRigNode->getPosition();
        Vector3 camPos = m_cameraNode->_getDerivedPosition();
        Vector3 newPos = curPos;

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

        if ( newPos.x > TERRAIN_WORLD_SIZE/2.025f)
        {
            newPos.x = TERRAIN_WORLD_SIZE/2.025f;
        }
        if ( newPos.x < -TERRAIN_WORLD_SIZE/2.025f)
        {
            newPos.x = -TERRAIN_WORLD_SIZE/2.025f;
        }
        if ( newPos.y > TERRAIN_WORLD_SIZE/2.025f)
        {
            newPos.y = TERRAIN_WORLD_SIZE/2.025f;
        }
        if ( newPos.y < -TERRAIN_WORLD_SIZE/2.025f)
        {
            newPos.y = -TERRAIN_WORLD_SIZE/2.025f;
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

bool scene::mouseWheelRolled(const OgreBites::MouseWheelEvent &evt)
{
    float scaleFactor = 1.0 + evt.y /10.0f;
    Vector3 newPos = scaleFactor * m_cameraNode->getPosition();
    float distance =  -newPos.y;
    if (distance > 60.0f && distance < TERRAIN_WORLD_SIZE/1.33f)
    {
        m_cameraNode->setPosition(newPos);
        handleCollisionWithTerrain();
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

void scene::handleCollisionWithTerrain()
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
    int gridX = 15;
    int gridY = 20;
    grid = new Grid(gridX, gridY);
    grid->setup(m_sceneManager);

    // without light we would just get a black screen
    m_sceneManager->setAmbientLight(ColourValue(1.0f,1.0f,1.0f));
    Ogre::Light* light = m_sceneManager->createLight("MainLight");
    Ogre::SceneNode* lightNode = m_sceneManager->getRootSceneNode()->createChildSceneNode();
    lightNode->setPosition(0, 0, 1500000);
    lightNode->attachObject(light);

    // also need to tell where we are
    m_cameraRigNode = m_sceneManager->getRootSceneNode()->createChildSceneNode();
    m_cameraNode = m_cameraRigNode->createChildSceneNode();
    //m_TerrainGroup->getHeightAtWorldPosition(0)
    m_cameraRigNode->setPosition(gridX/2.0, gridY/2.0, 0);
    m_cameraRigNode->pitch(Ogre::Radian(-M_PI/4.0f));
    //m_cameraRigNode->setPosition(0.0, 0.0, 0);
    m_cameraNode->setPosition(0, -60, 0);
    m_cameraNode->lookAt(Ogre::Vector3(0, 0, 0), Ogre::Node::TS_PARENT);

    // create the camera
    m_camera = m_sceneManager->createCamera("mainCamera");
    m_camera->setNearClipDistance(0.1); // specific to this sample
    m_camera->setFarClipDistance(1200000.0f);
    m_camera->setAutoAspectRatio(true);
    m_cameraNode->attachObject(m_camera);

    // and tell it to render into the main window
    getRenderWindow()->addViewport(m_camera);

    // finally something to render
    Ogre::Entity* entSinbad = m_sceneManager->createEntity("Sinbad.mesh");
    //m_entityNode = m_sceneManager->getRootSceneNode()->createChildSceneNode();
    //m_entityNode->setScale(0.1, 0.1, 0.1);
    //m_entityNode->pitch(Ogre::Radian(M_PI/2.));
    //m_entityNode->setPosition(gridX/2., gridY/2.0, 0.5);
    //m_entityNode->attachObject(entSinbad);
    m_cameraRigNode->attachObject(entSinbad);

    loadTerrain();

    m_cameraRigNode->setPosition(0,0,m_TerrainGroup->getHeightAtWorldPosition(0,0,0));
}

void scene::loadTerrain()
{
    Vector3 lightdir(0, 1, 0);
    lightdir.normalise();
    Ogre::Light* l = m_sceneManager->createLight("tstLight");
    l->setType(Light::LT_DIRECTIONAL);
    l->setDirection(lightdir);
    l->setDiffuseColour(ColourValue::White);
    l->setSpecularColour(ColourValue(0.4, 0.4, 0.4));

    m_cameraNode->attachObject(l);


    m_TerrainGlobOpts = new Ogre::TerrainGlobalOptions();
    m_TerrainGroup = new Ogre::TerrainGroup(m_sceneManager, Ogre::Terrain::ALIGN_X_Y, TERRAIN_SIZE, TERRAIN_WORLD_SIZE);
    m_TerrainGroup->setFilenameConvention("testTerrain", "dat");
    m_TerrainGroup->setOrigin(Ogre::Vector3(0,0,0));

    configureTerrainDefaults(l);

    for (long x = TERRAIN_PAGE_MIN_X; x <= TERRAIN_PAGE_MAX_X; ++x)
        for (long y = TERRAIN_PAGE_MIN_Y; y <= TERRAIN_PAGE_MAX_Y; ++y)
            defineTerrain(x, y);
    // sync load since we want everything in place when we start
    m_TerrainGroup->loadAllTerrains(true);


    if (m_TerrainsImported)
    {
        TerrainGroup::TerrainIterator ti = m_TerrainGroup->getTerrainIterator();
        while(ti.hasMoreElements())
        {
            Terrain* t = ti.getNext()->instance;
            initBlendMaps(t);
        }
    }

    m_TerrainGroup->freeTemporaryResources();


}

void scene::configureTerrainDefaults(Ogre::Light *light)
{
    //! [configure_lod]
    m_TerrainGlobOpts->setMaxPixelError(8);
    m_TerrainGlobOpts->setCompositeMapDistance(3000);
    //! [configure_lod]

    //mTerrainGlobals->setUseRayBoxDistanceCalculation(true);
    //mTerrainGlobals->getDefaultMaterialGenerator()->setDebugLevel(1);
    //mTerrainGlobals->setLightMapSize(256);

    // Disable the lightmap for OpenGL ES 2.0. The minimum number of samplers allowed is 8(as opposed to 16 on desktop).
    // Otherwise we will run over the limit by just one. The minimum was raised to 16 in GL ES 3.0.
    /*
    if (Ogre::Root::getSingletonPtr()->getRenderSystem()->getCapabilities()->getNumTextureUnits() < 9)
    {
        TerrainMaterialGeneratorA::SM2Profile* matProfile =
                static_cast<TerrainMaterialGeneratorA::SM2Profile*>(mTerrainGlobals->getDefaultMaterialGenerator()->getActiveProfile());
        matProfile->setLightmapEnabled(false);
    }
     */

    //! [composite_lighting]
    // Important to set these so that the terrain knows what to use for baked (non-realtime) data
    m_TerrainGlobOpts->setLightMapDirection(light->getDerivedDirection());
    m_TerrainGlobOpts->setCompositeMapAmbient(m_sceneManager->getAmbientLight());
    m_TerrainGlobOpts->setCompositeMapDiffuse(light->getDiffuseColour());
    //! [composite_lighting]
    //mTerrainGlobals->setCompositeMapAmbient(ColourValue::Red);

    // Configure default import settings for if we use imported image
    //! [import_settings]
    Ogre::Terrain::ImportData& defaultimp = m_TerrainGroup->getDefaultImportSettings();
    defaultimp.terrainSize = TERRAIN_SIZE;
    defaultimp.worldSize = TERRAIN_WORLD_SIZE;
    defaultimp.inputScale = 600;
    defaultimp.minBatchSize = 33;
    defaultimp.maxBatchSize = 65;
    //! [import_settings]
    //! [textures]
    defaultimp.layerList.resize(3);
    defaultimp.layerList[0].worldSize = 100;
    defaultimp.layerList[0].textureNames.push_back("grass_green-01_diffusespecular.dds");
    defaultimp.layerList[0].textureNames.push_back("grass_green-01_normalheight.dds");
    defaultimp.layerList[1].worldSize = 100;
    defaultimp.layerList[1].textureNames.push_back("dirt_grayrocky_diffusespecular.dds");
    defaultimp.layerList[1].textureNames.push_back("dirt_grayrocky_normalheight.dds");
    defaultimp.layerList[2].worldSize = 200;
    defaultimp.layerList[2].textureNames.push_back("growth_weirdfungus-03_diffusespecular.dds");
    defaultimp.layerList[2].textureNames.push_back("growth_weirdfungus-03_normalheight.dds");
    //! [textures]

}

void scene::defineTerrain(long x, long y, bool flat)
{
    // if a file is available, use it
    // if not, generate file from import

    // Usually in a real project you'll know whether the compact terrain data is
    // available or not; I'm doing it this way to save distribution size

    if (flat)
    {
        m_TerrainGroup->defineTerrain(x, y, 0.0f);
        return;
    }

    //! [define]
    String filename = m_TerrainGroup->generateFilename(x, y);
    if (ResourceGroupManager::getSingleton().resourceExists(m_TerrainGroup->getResourceGroup(), filename))
    {
        m_TerrainGroup->defineTerrain(x, y);
    }
    else
    {
        Image img;
        getTerrainImage(x % 2 != 0, y % 2 != 0, img);
        m_TerrainGroup->defineTerrain(x, y, &img);
        m_TerrainsImported = true;
    }
    //! [define]
}

void scene::getTerrainImage(bool flipX, bool flipY, Ogre::Image& img)
{
    //! [heightmap]
    img.load("terrain.png", m_TerrainGroup->getResourceGroup());
    if (flipX)
        img.flipAroundY();
    if (flipY)
        img.flipAroundX();
    //! [heightmap]
}

void scene::initBlendMaps(Ogre::Terrain* terrain)
{
    //! [blendmap]
    using namespace Ogre;
    Ogre::TerrainLayerBlendMap* blendMap0 = terrain->getLayerBlendMap(1);
    Ogre::TerrainLayerBlendMap* blendMap1 = terrain->getLayerBlendMap(2);
    float minHeight0 = 20;
    float fadeDist0 = 15;
    float minHeight1 = 70;
    float fadeDist1 = 15;
    float* pBlend0 = blendMap0->getBlendPointer();
    float* pBlend1 = blendMap1->getBlendPointer();
    for (uint16 y = 0; y < terrain->getLayerBlendMapSize(); ++y)
    {
        for (uint16 x = 0; x < terrain->getLayerBlendMapSize(); ++x)
        {
            Real tx, ty;

            blendMap0->convertImageToTerrainSpace(x, y, &tx, &ty);
            float height = terrain->getHeightAtTerrainPosition(tx, ty);

            *pBlend0++ = Math::saturate((height - minHeight0) / fadeDist0);
            *pBlend1++ = Math::saturate((height - minHeight1) / fadeDist1);
        }
    }
    blendMap0->dirty();
    blendMap1->dirty();
    blendMap0->update();
    blendMap1->update();
    //! [blendmap]
    // set up a colour map
    /*
      if (!terrain->getGlobalColourMapEnabled())
      {
      terrain->setGlobalColourMapEnabled(true);
      Image colourMap;
      colourMap.load("testcolourmap.jpg", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
      terrain->getGlobalColourMap()->loadImage(colourMap);
      }
    */
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