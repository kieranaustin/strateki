//
// Created by kieran on 04.09.21.
//

#include "InfoOverlay.h"

InfoOverlay::InfoOverlay(Ogre::SceneManager *sceneManager)
: m_sceneManager(sceneManager)
{
}

void InfoOverlay::setup()
{
    Ogre::OverlayManager * pOverlayManager = Ogre::OverlayManager::getSingletonPtr();
    m_overlay = pOverlayManager->create("infoOverlay");
    m_container = (Ogre::OverlayContainer*)pOverlayManager->createOverlayElement("Panel", "infoContainer");
    m_infoPanelLeft = (Ogre::OverlayContainer*)pOverlayManager->createOverlayElement("Panel", "infoPanelLeft");
    m_infoPanelRight = (Ogre::OverlayContainer*)pOverlayManager->createOverlayElement("Panel", "infoPanelRight");
    m_infoPanelLeftText = (Ogre::TextAreaOverlayElement *)pOverlayManager->createOverlayElement("TextArea", "infoPanelLeftText");
    m_infoPanelRightText = (Ogre::TextAreaOverlayElement *)pOverlayManager->createOverlayElement("TextArea", "infoPanelRightText");

    m_container->setHeight(0.33);
    m_container->setWidth(1.0);
    m_container->setPosition(0.0,0.67);

    Ogre::Real height = m_container->getHeight();
    Ogre::Real width = m_container->getWidth();

    m_infoPanelLeft->setMaterialName("Tile/Black80", "Map");
    m_infoPanelLeft->setPosition(0.0,0.0);
    m_infoPanelLeft->setWidth(0.295*width);
    m_infoPanelLeft->setHeight(height);

    m_infoPanelRight->setMaterialName("Tile/Black80", "Map");
    m_infoPanelRight->setPosition(0.305,0.0);
    m_infoPanelRight->setWidth((1.0-0.305)*width);
    m_infoPanelRight->setHeight(height);

    constexpr Ogre::Real offset = 0.005;

    m_infoPanelLeftText->setColour(Ogre::ColourValue(0.5,0.0,1.0,1.0));
    m_infoPanelLeftText->setFontName("DejaVuSans", "Fonts");
    m_infoPanelLeftText->setCaption(("Left Panel"));
    m_infoPanelLeftText->setPosition(offset,offset);
    width = m_infoPanelLeft->getHeight() - 2*offset;
    height = m_infoPanelLeft->getHeight() - 2*offset;
    m_infoPanelLeftText->setDimensions(width,height);
    m_infoPanelLeftText->setCharHeight(0.025);
    m_infoPanelLeftText->setSpaceWidth(0.0125);

    m_infoPanelRightText->setColour(Ogre::ColourValue(0.5,0.0,1.0,1.0));
    m_infoPanelRightText->setFontName("DejaVuSans", "Fonts");
    m_infoPanelRightText->setCaption(("Right Panel"));
    m_infoPanelRightText->setPosition(offset,offset);
    width = m_infoPanelRight->getHeight() - 2*offset;
    height = m_infoPanelRight->getHeight() - 2*offset;
    m_infoPanelRightText->setDimensions(width,height);
    m_infoPanelRightText->setCharHeight(0.025);
    m_infoPanelRightText->setSpaceWidth(0.0125);

    m_infoPanelLeft->addChild(m_infoPanelLeftText);
    m_infoPanelRight->addChild(m_infoPanelRightText);
    m_container->addChild(m_infoPanelLeft);
    m_container->addChild(m_infoPanelRight);

    m_overlay->add2D(m_container);
    m_overlay->setScale(1.0,1.0);
    m_overlay->show();
}

void InfoOverlay::show()
{
    m_overlay->show();
}

void InfoOverlay::hide()
{
    m_overlay->hide();
}

bool InfoOverlay::isVisible()
{
    return m_overlay->isVisible();
}

void InfoOverlay::setTextLeft(std::string text)
{
    m_infoPanelLeftText->setCaption(text);
}

void InfoOverlay::setTextRight(std::string text)
{
    m_infoPanelRightText->setCaption(text);
}
