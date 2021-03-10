//
// Created by kieran on 09.03.21.
//

#include <OgreRenderQueue.h>
#include <iostream>
#include "SelectionBox.h"

SelectionBox::SelectionBox(const Ogre::String &name)
    : ManualObject(name)
{
    setRenderQueueGroup(Ogre::RENDER_QUEUE_OVERLAY);
    setUseIdentityProjection(true);
    setUseIdentityView(true);
    setQueryFlags(0);
}

SelectionBox::~SelectionBox()
{}

void SelectionBox::setCorners(float left, float top, float right, float bottom)
{
    // shift to Ogre units, i.e., x,y in [-1, 1]
    left = 2 * left - 1;
    right = 2 * right - 1;
    top = 1 - 2 * top;
    bottom = 1 - 2 * bottom;

    // draw rectangle/box
    clear();
    begin("Tile/Yellow", Ogre::RenderOperation::OT_LINE_STRIP, "Map");
        position(left, top, -1);
        position(right, top, -1);
        position(right, bottom, -1);
        position(left, bottom, -1);
        position(left, top, -1);
    end();
    setBoundingBox(Ogre::AxisAlignedBox::BOX_INFINITE);
}

void SelectionBox::setCorners(const Ogre::Vector2 &topLeft, const Ogre::Vector2 &bottomRight)
{
    setCorners(topLeft.x, topLeft.y, bottomRight.x, bottomRight.y);
}
