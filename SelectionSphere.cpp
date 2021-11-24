//
// Created by kieran on 13.03.21.
//

#include "SelectionSphere.h"
#include <iostream>

SelectionSphere::SelectionSphere(Ogre::SceneManager * sceneManager, Ogre::Camera * camera, Ogre::TerrainGroup * terrainGroup)
    : SelectionView(sceneManager, camera)
    , m_terrainGroup(terrainGroup)
{
    Ogre::Entity * sphere = m_sceneManager->createEntity("selectionSphere", Ogre::SceneManager::PT_SPHERE);
    sphere->setMaterialName("selectionSphereWide", "Map");
    m_sceneManager->getRootSceneNode()->createChildSceneNode()->attachObject(sphere);
    sphere->getParentSceneNode()->rotate(Ogre::Vector3(1,0,0), Ogre::Radian(M_PI/2.), Ogre::Node::TS_LOCAL);
    m_view = sphere;
    m_view->setVisible(false);
    m_sphereQuery = m_sceneManager->createSphereQuery(Ogre::Sphere());
};

SelectionSphere::~SelectionSphere()
{
    m_sceneManager->destroyQuery(m_sphereQuery);
}

bool SelectionSphere::beginSelection(const int mouseX, const int mouseY)
{
    Ogre::Real mousePosNormX = (float)mouseX / m_camera->getViewport()->getActualWidth();
    Ogre::Real mousePosNormY = (float)mouseY / m_camera->getViewport()->getActualHeight();

    m_radius = 0.0;
    Ogre::Ray mouseRay = m_camera->getCameraToViewportRay(mousePosNormX, mousePosNormY);
    Ogre::TerrainGroup::RayResult result = m_terrainGroup->rayIntersects(mouseRay);
    if (result.hit)
    {
        // clicked on terrain
        m_beginOnTerrain = true;
        m_center = result.position;
        m_view->getParentSceneNode()->setPosition(m_center);
        m_view->getParentSceneNode()->setScale(getScale());
        m_view->setVisible(true);
    }
    else
    {
        // clicked outside of terrain, i.e., on empty space
        m_beginOnTerrain = false;
        m_center = {-1, -1, -1};
    }
    return true;
}

bool SelectionSphere::moveSelection(const int mouseX, const int mouseY)
{
    Ogre::Real mousePosNormX = (float)mouseX / m_camera->getViewport()->getActualWidth();
    Ogre::Real mousePosNormY = (float)mouseY / m_camera->getViewport()->getActualHeight();

    Ogre::Ray mouseRay = m_camera->getCameraToViewportRay(mousePosNormX, mousePosNormY);
    Ogre::TerrainGroup::RayResult result = m_terrainGroup->rayIntersects(mouseRay);
    bool endOnTerrain = result.hit;
    if (m_beginOnTerrain)
    {
        // mouse moved from terrain...
        Ogre::Vector3 mouseToWorldPosition;
        if (endOnTerrain)
        {
            // ... onto terrain -> find world position of mouse on terrain
            mouseToWorldPosition = result.position;
        }
        else
        {
            // ... to outside of terrain -> find world position of mouse on plane,
            // where the plane is along xy-axes at the height (z) of the sphere-center
            Ogre::Real height = m_center.z;
            Ogre::Plane plane = Ogre::Plane(Ogre::Vector3(0,0, height),
                                            Ogre::Vector3(1,0, height),
                                            Ogre::Vector3(0,1, height));
            Ogre::RayTestResult mouseToPlaneResult = mouseRay.intersects(plane);
            mouseToWorldPosition = mouseRay.getPoint(mouseToPlaneResult.second);
        }
        // set and show selection sphere
        m_radius = (mouseToWorldPosition - m_center).length();
        m_view->getParentSceneNode()->setScale(getScale());
        m_view->setVisible(true);
    }
    else
    {
        // mouse moved from outside of terrain...
        if (endOnTerrain)
        {
            // .. onto terrain -> set sphere-center on edge of terrain
            m_center = result.position;
            m_view->getParentSceneNode()->setPosition(m_center);
            m_beginOnTerrain = true;
        }
        // ... and is still outside of terrain -> do nothing = no selection sphere
    }
    m_view->getParentSceneNode()->showBoundingBox(false);
    return true;
}

bool SelectionSphere::endSelection()
{
    m_view->setVisible(false);
    return true;
}

void SelectionSphere::executeQuery()
{
    m_sphereQuery->setSphere(Ogre::Sphere(m_center, m_radius));
    m_queryResult = m_sphereQuery->execute();
}

bool SelectionSphere::inExactBounds(Ogre::MovableObject * object) const
{
    Ogre::Vector3 objPos = object->getParentSceneNode()->_getDerivedPosition();
    Ogre::Real objDist = (m_center-objPos).length();

    if (objDist < m_radius)
        return true;

    return false;
}
