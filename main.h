//
// Created by kieran on 16.05.20.
//
#include <Ogre.h>
#include <Bites/OgreApplicationContext.h>
#include "Grid.h"

#ifndef SCENE_MAIN_H
#define SCENE_MAIN_H

class scene :
        public OgreBites::ApplicationContext,
        public OgreBites::InputListener
{
public:
    scene();
    void setup(void);
    bool keyPressed(const OgreBites::KeyboardEvent& evt);
    bool mouseMoved(const OgreBites::MouseMotionEvent& evt);
    bool mousePressed(const OgreBites::MouseButtonEvent& evt);
    bool mouseReleased(const OgreBites::MouseButtonEvent& evt);
    bool mouseWheelRolled(const OgreBites::MouseWheelEvent& evt);

private:
    Ogre::SceneNode* m_entityNode;
    Ogre::SceneNode* m_cameraNode;

    Grid* grid;

    bool m_bRotateCamera = false;
    bool m_bTranslateCamera = false;
};

#endif //SCENE_MAIN_H
