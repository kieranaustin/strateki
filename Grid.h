//
// Created by kieran on 17.05.20.
//

#ifndef SCENE_GRID_H
#define SCENE_GRID_H

#include <Ogre.h>
#include <Bites/OgreApplicationContext.h>

struct Tile
{
    int type;
};

class Grid
{

public:
    Grid(int sizeX, int sizeY);
    ~Grid();
    void setup(Ogre::SceneManager* sceneManager);
    Tile getTileAt(int x, int y);
    Ogre::Vector2 minBound() {return Ogre::Vector2(0,0);};
    Ogre::Vector2 maxBound() {return Ogre::Vector2(m_nTilesX, m_nTilesY);};

private:
    void setTileAt(int x, int y);
    int getTileIndex(int x, int y);
    Ogre::Vector2 getTileCoordinates(int tileIndex);
    int m_nTilesX = 0;
    int m_nTilesY = 0;
    std::vector<Tile> m_vecGrid;
};

#endif //SCENE_GRID_H
