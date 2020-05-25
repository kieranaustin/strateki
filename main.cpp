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
    m_sceneManager->setAmbientLight(ColourValue(1.0f,1.0f,1.0f));
    Ogre::Light* light = m_sceneManager->createLight("MainLight");
    Ogre::SceneNode* lightNode = m_sceneManager->getRootSceneNode()->createChildSceneNode();
    lightNode->setPosition(0, 0, 1500000);
    lightNode->attachObject(light);

    loadTerrain();

    // create the camera and attach to camera controller
    m_camera = m_sceneManager->createCamera("mainCamera");
    m_camera->setNearClipDistance(0.1); // specific to this sample
    m_camera->setFarClipDistance(1200000.0f);
    m_camera->setAutoAspectRatio(true);

    m_cameraControl = new CameraControl(m_camera, m_sceneManager, m_TerrainGroup);
    m_cameraControl->showCoordinateAxes(false);

    // and tell it to render into the main window
    getRenderWindow()->addViewport(m_camera);
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

    /*
    m_cameraNode->attachObject(l);
     */


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