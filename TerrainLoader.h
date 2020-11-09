//
// Created by kieran on 09.11.20.
//

#ifndef SCENE_TERRAINLOADER_H
#define SCENE_TERRAINLOADER_H


#include <Ogre.h>
#include <Terrain/OgreTerrain.h>
#include <Terrain/OgreTerrainGroup.h>

// the following can be used to load one terrain multiple times in a grid-like pattern
#define TERRAIN_PAGE_MIN_X 0
#define TERRAIN_PAGE_MIN_Y 0
#define TERRAIN_PAGE_MAX_X 0
#define TERRAIN_PAGE_MAX_Y 0

class TerrainLoader {

public:
    TerrainLoader(Ogre::SceneManager* sceneManager, int size, double worldSize);
    ~TerrainLoader();

    void loadTerrain(void);

    Ogre::TerrainGroup* getTerrainGroup() {return m_TerrainGroup;};

private:
    Ogre::SceneManager* m_sceneManager;

    int m_terrainSize = 0; // Must be of type 2^n + 1
    double m_terrainWorldSize = 0.0f;

    bool m_TerrainsImported = false;

    Ogre::TerrainGlobalOptions* m_TerrainGlobOpts;
    Ogre::TerrainGroup* m_TerrainGroup;

    void configureTerrainDefaults(Ogre::Light* light);
    void defineTerrain(long x, long y, bool flat = false);
    void getTerrainImage(bool flipX, bool flipY, Ogre::Image& img);
    void initBlendMaps(Ogre::Terrain* terrain);
};


#endif //SCENE_TERRAINLOADER_H
