//
// Created by kieran on 09.03.21.
//

#include <OgreRenderQueue.h>
#include <iostream>
#include "SelectionBox.h"

SelectionBox::SelectionBox(Ogre::SceneManager * sceneManager, Ogre::Camera * camera)
    : SelectionView(sceneManager, camera)
{
    Ogre::ManualObject * box = new Ogre::ManualObject("selectionBox");
    box->setRenderQueueGroup(Ogre::RENDER_QUEUE_OVERLAY);
    box->setUseIdentityProjection(true);
    box->setUseIdentityView(true);
    box->setQueryFlags(0);
    m_sceneManager->getRootSceneNode()->createChildSceneNode()->attachObject(box);
    m_view = box;
    m_volumeQuery = m_sceneManager->createPlaneBoundedVolumeQuery(Ogre::PlaneBoundedVolumeList());
}

SelectionBox::~SelectionBox()
{}

bool SelectionBox::beginSelection(const int mouseX, const int mouseY)
{
    m_queryResult.movables.clear();
    m_begin.x = (float)mouseX / m_camera->getViewport()->getActualWidth();
    m_begin.y = (float)mouseY / m_camera->getViewport()->getActualHeight();
    m_end = m_begin;
    static_cast<Ogre::ManualObject*>(m_view)->clear();
    m_view->setVisible(true);
    return true;
}

bool SelectionBox::moveSelection(const int mouseX, const int mouseY)
{
    m_end.x = (float)mouseX / m_camera->getViewport()->getActualWidth();
    m_end.y = (float)mouseY / m_camera->getViewport()->getActualHeight();
    setCorners(m_begin, m_end);
    return true;
}

bool SelectionBox::endSelection()
{
    static_cast<Ogre::ManualObject*>(m_view)->clear();
    static_cast<Ogre::ManualObject*>(m_view)->setVisible(false);
    return true;
}

void SelectionBox::executeQuery()
{
    float left = m_begin.x;
    float top = m_begin.y;
    float right = m_end.x;
    float bottom = m_end.y;

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

    // check if selection box is too small
    if ((right - left) * (bottom - top) < 0.0001)
    {
        return;
        // TODO: select single entity via single ray maybe?
    }

    // get rays pointing from the four corners of our selection box to infinity
    Ogre::Ray rayTopLeft     = m_camera->getCameraToViewportRay(left, top);
    Ogre::Ray rayTopRight    = m_camera->getCameraToViewportRay(right, top);
    Ogre::Ray rayBottomLeft  = m_camera->getCameraToViewportRay(left, bottom);
    Ogre::Ray rayBottomRight = m_camera->getCameraToViewportRay(right, bottom);

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
    m_queryResult = m_volumeQuery->execute();
}

void SelectionBox::setCorners(float left, float top, float right, float bottom)
{
    // shift to Ogre units, i.e., x,y in [-1, 1]
    left = 2 * left - 1;
    right = 2 * right - 1;
    top = 1 - 2 * top;
    bottom = 1 - 2 * bottom;

    // draw rectangle/box
    Ogre::ManualObject * mo = static_cast<Ogre::ManualObject*>(m_view);
    mo->clear();
    mo->begin("Tile/Yellow", Ogre::RenderOperation::OT_LINE_STRIP, "Map");
        mo->position(left, top, -1);
        mo->position(right, top, -1);
        mo->position(right, bottom, -1);
        mo->position(left, bottom, -1);
        mo->position(left, top, -1);
    mo->end();
    mo->setBoundingBox(Ogre::AxisAlignedBox::BOX_INFINITE);
}

void SelectionBox::setCorners(const Ogre::Vector2 &topLeft, const Ogre::Vector2 &bottomRight)
{
    setCorners(topLeft.x, topLeft.y, bottomRight.x, bottomRight.y);
}
