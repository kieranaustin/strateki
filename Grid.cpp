//
// Created by kieran on 17.05.20.
//

#include <cstdlib>
#include <iostream>
#include <cmath>
#include <Ogre.h>
#include "Grid.h"

Grid::Grid(int sizeX, int sizeY)
    : m_nTilesX(sizeX)
    , m_nTilesY(sizeY)
{
    m_vecGrid.resize(m_nTilesX*m_nTilesY);
}

Grid::~Grid() {}

void Grid::setup(Ogre::SceneManager* sceneManager)
{
    //for(int i=0; i<m_vecGrid.size())
    std::srand(0);
    Ogre::ManualObject* mapGrid = sceneManager->createManualObject("Map/Grid");
    for(int i = 0; i<m_vecGrid.size(); i++)
    {
        Tile tile;
        tile.type = std::rand() % 5;

        Ogre::Vector2 pos = getTileCoordinates(i);

        std::stringstream tileName;
        tileName << "Grid/tile_" << pos.x << "_" << pos.y;

        std::cout << "tile " << i << " of type " << tile.type << std::endl;
        std::cout << "and position (x, y) = (" << pos.x << ", " << pos.y << ")" << std::endl;
        std::cout << tileName.str() << std::endl;

        std::string strMaterial;
        switch (tile.type)
        {
            case 0:
                strMaterial = "Tile/Red";
                break;
            case 1:
                strMaterial = "Tile/Blue";
                break;
            case 2:
                strMaterial = "Tile/Green";
                break;
            case 3:
                strMaterial = "Tile/Yellow";
                break;
            case 4:
                strMaterial = "Tile/Purple";
                break;
        }
        mapGrid->begin(strMaterial, Ogre::RenderOperation::OT_TRIANGLE_LIST, "Map");

        mapGrid->position(pos.x - 0.5, pos.y + 0.5, 0.0);
        mapGrid->normal(0,0,1);
        mapGrid->textureCoord(0,0);

        mapGrid->position(pos.x - 0.5, pos.y - 0.5, 0.0);
        mapGrid->normal(0,0,1);
        mapGrid->textureCoord(0,1);

        mapGrid->position(pos.x + 0.5, pos.y - 0.5, 0.0);
        mapGrid->normal(0,0,1);
        mapGrid->textureCoord(1,1);

        mapGrid->position(pos.x + 0.5, pos.y + 0.5, 0.0);
        mapGrid->normal(0,0,1);
        mapGrid->textureCoord(1,0);

        mapGrid->quad(0, 1, 2, 3);

        mapGrid->end();

        /*
        tile.type = i;
        Ogre::Entity* entTile = sceneManager->createEntity("WoodPallet.mesh");
        Ogre::SceneNode* entNode = sceneManager->getRootSceneNode()->createChildSceneNode();
        entNode->setPosition(pos.x, pos.y, 0.0);
        //Ogre::Radian curRoll(0.5 * 3.1415926);
        Ogre::Radian curPitch(0.5 * 3.1415926);
        //entNode->roll(curRoll);
        entNode->setScale(0.13, 0.13, 0.13);
        entNode->pitch(curPitch);
        entNode->attachObject(entTile);
         */
    }
    Ogre::SceneNode* mapNode = sceneManager->getRootSceneNode()->createChildSceneNode();
    mapNode->attachObject(mapGrid);
}

int Grid::getTileIndex(int x, int y)
{
    int tileIndex = 0;

    tileIndex = (x-1)*m_nTilesY;
    tileIndex += y;

    assert(tileIndex < m_vecGrid.size() && "tileIndex out of bound");

    return tileIndex;
}

Ogre::Vector2 Grid::getTileCoordinates(int tileIndex)
{
    int x = tileIndex % m_nTilesX;
    int y = tileIndex / m_nTilesX;
    return Ogre::Vector2(x, y);
}
