//
// Created by kieran on 09.11.20.
//

#include "TerrainLoader.h"

TerrainLoader::TerrainLoader(Ogre::SceneManager* sceneManager, int size, double worldSize)
    : m_sceneManager(sceneManager)
    , m_terrainSize(size)
    , m_terrainWorldSize(worldSize)
{

}

TerrainLoader::~TerrainLoader()
{
    delete m_TerrainGlobOpts;
    delete m_TerrainGroup;
}

void TerrainLoader::loadTerrain()
{
    Ogre::Vector3 lightdir(0, 0, -1);
    lightdir.normalise();
    Ogre::Light* l = m_sceneManager->createLight("tstLight");
    l->setType(Ogre::Light::LT_DIRECTIONAL);
    l->setDirection(lightdir);
    l->setDiffuseColour(Ogre::ColourValue::White);
    //l->setSpecularColour(ColourValue(0.4, 0.4, 0.4));
    l->setSpecularColour(Ogre::ColourValue::White);

    //m_cameraControl->attachLight(l);
    //m_camera->getParentSceneNode()->attachObject(l);

    m_TerrainGlobOpts = new Ogre::TerrainGlobalOptions();
    m_TerrainGroup = new Ogre::TerrainGroup(m_sceneManager, Ogre::Terrain::ALIGN_X_Y, m_terrainSize, m_terrainWorldSize);
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
        Ogre::TerrainGroup::TerrainIterator ti = m_TerrainGroup->getTerrainIterator();
        while(ti.hasMoreElements())
        {
            Ogre::Terrain* t = ti.getNext()->instance;
            initBlendMaps(t);
        }
    }

    m_TerrainGroup->freeTemporaryResources();
}


void TerrainLoader::configureTerrainDefaults(Ogre::Light *light)
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
    defaultimp.terrainSize = m_terrainSize;
    defaultimp.worldSize = m_terrainWorldSize;
    defaultimp.inputScale = 400;
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

void TerrainLoader::defineTerrain(long x, long y, bool flat)
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
    Ogre::String filename = m_TerrainGroup->generateFilename(x, y);
    if (Ogre::ResourceGroupManager::getSingleton().resourceExists(m_TerrainGroup->getResourceGroup(), filename))
    {
        m_TerrainGroup->defineTerrain(x, y);
    }
    else
    {
        Ogre::Image img;
        getTerrainImage(x % 2 != 0, y % 2 != 0, img);
        m_TerrainGroup->defineTerrain(x, y, &img);
        m_TerrainsImported = true;
    }
    //! [define]
}

void TerrainLoader::getTerrainImage(bool flipX, bool flipY, Ogre::Image& img)
{
    //! [heightmap]
    //img.load("terrain.png", m_TerrainGroup->getResourceGroup());
    img.load("terrain_cornerhill.png", "Map");
    if (flipX)
        img.flipAroundY();
    if (flipY)
        img.flipAroundX();
    //! [heightmap]
}

void TerrainLoader::initBlendMaps(Ogre::Terrain* terrain)
{
    //! [blendmap]
    Ogre::TerrainLayerBlendMap* blendMap0 = terrain->getLayerBlendMap(1);
    Ogre::TerrainLayerBlendMap* blendMap1 = terrain->getLayerBlendMap(2);
    float minHeight0 = 20;
    float fadeDist0 = 15;
    float minHeight1 = 70;
    float fadeDist1 = 15;
    float* pBlend0 = blendMap0->getBlendPointer();
    float* pBlend1 = blendMap1->getBlendPointer();
    for (Ogre::uint16 y = 0; y < terrain->getLayerBlendMapSize(); ++y)
    {
        for (Ogre::uint16 x = 0; x < terrain->getLayerBlendMapSize(); ++x)
        {
            Ogre::Real tx, ty;

            blendMap0->convertImageToTerrainSpace(x, y, &tx, &ty);
            float height = terrain->getHeightAtTerrainPosition(tx, ty);

            *pBlend0++ = Ogre::Math::saturate((height - minHeight0) / fadeDist0);
            *pBlend1++ = Ogre::Math::saturate((height - minHeight1) / fadeDist1);
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
      Ogre::Image colourMap;
      colourMap.load("testcolourmap.jpg", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
      terrain->getGlobalColourMap()->loadImage(colourMap);
      }
    */
}
