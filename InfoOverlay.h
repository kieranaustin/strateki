//
// Created by kieran on 04.09.21.
//

#ifndef STRATEKI_INFOOVERLAY_H
#define STRATEKI_INFOOVERLAY_H

#include <Ogre.h>
#include <OgreOverlay.h>
#include <OgreOverlayManager.h>
#include <Overlay/OgreOverlayContainer.h>
#include <Overlay/OgreTextAreaOverlayElement.h>
#include <OgreMaterialManager.h>

class InfoOverlay
{
public:
    InfoOverlay() = default;
    InfoOverlay(Ogre::SceneManager * sceneManager);

    void setup();
    void show();
    void hide();
    bool isVisible();

    void setTextLeft(std::string text);
    void setTextRight(std::string text);

private:
    Ogre::SceneManager * m_sceneManager;
    Ogre::Overlay * m_overlay;
    Ogre::OverlayContainer * m_container;
    Ogre::OverlayContainer * m_infoPanelLeft;
    Ogre::OverlayContainer * m_infoPanelRight;
    Ogre::TextAreaOverlayElement * m_infoPanelLeftText;
    Ogre::TextAreaOverlayElement * m_infoPanelRightText;
};


#endif //STRATEKI_INFOOVERLAY_H
