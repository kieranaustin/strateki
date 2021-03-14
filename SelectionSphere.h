//
// Created by kieran on 13.03.21.
//

#ifndef STRATEKI_SELECTIONSPHERE_H
#define STRATEKI_SELECTIONSPHERE_H

#include <Ogre.h>
#include <Terrain/OgreTerrainGroup.h>
#include "SelectionView.h"

class SelectionSphere : public SelectionView
{
public:
    SelectionSphere(Ogre::SceneManager * sceneManager, Ogre::Camera * camera, Ogre::TerrainGroup * terrainGroup);
    ~SelectionSphere();

    bool beginSelection(const int mouseX, const int mouseY) override;
    bool moveSelection(const int mouseX, const int mouseY) override;
    bool endSelection() override;
    void executeQuery() override;

    bool inExactBounds(Ogre::MovableObject * object) const override;

private:
    /*
     * set set the correct scale from sphere radius - caution: magic number(!?)
     */
    Ogre::Vector3 getScale() const
    {
        return Ogre::Vector3(m_radius / m_terrainScale);
    }


private:
    Ogre::Vector3 m_center = {0.0, 0.0, 0.0};
    Ogre::Real m_radius = 0.0;

    // TODO: why does terrainScale with values 8000.0f and 513 work exactly for scaling factor of selection sphere, even when changing TERRAIN_WORLD_SIZE and TERRAIN_SIZE
    const double m_terrainScale = M_PI * 8000.0f / (double)513; // TODO; magic number
    bool m_beginOnTerrain = false;
    Ogre::TerrainGroup * m_terrainGroup;

    Ogre::SphereSceneQuery * m_sphereQuery;
};
#endif //STRATEKI_SELECTIONSPHERE_H
