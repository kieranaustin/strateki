//
// Created by kieran on 09.03.21.
//

#ifndef STRATEKI_SELECTIONBOX_H
#define STRATEKI_SELECTIONBOX_H

#include <OgreManualObject.h>

/*
 * A ManualObject for drawing a selection box on the screen
 */
class SelectionBox : public Ogre::ManualObject
{
public:
    SelectionBox(const Ogre::String & name);
    virtual ~SelectionBox();

    /*
     * set the two corner points of the rectangle/box to draw in screen units, i.e., x,y in [0, 1]
     */
    void setCorners(float left, float top, float right, float bottom);

    /*
     * set the two corner points as Vector2
     * wrapper function that calls setCorners(float left, float top, float right, float bottom)
     * topLeft.x = left, topLeft.y = top, bottomRight.x = right, bottomRight.y = bottom
     */
    void setCorners(const Ogre::Vector2 & topLeft, const Ogre::Vector2 & bottomRight);
};

#endif //STRATEKI_SELECTIONBOX_H
