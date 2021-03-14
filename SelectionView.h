//
// Created by kieran on 13.03.21.
//

#include <Ogre.h>
#include <OgreInput.h>

#ifndef STRATEKI_SELECTIONVIEW_H
#define STRATEKI_SELECTIONVIEW_H

class SelectionView
{
public:
    SelectionView(Ogre::SceneManager * sceneManager, Ogre::Camera * camera)
        : m_sceneManager(sceneManager)
        , m_camera(camera)
    {}

    virtual ~SelectionView() {}

    virtual bool beginSelection(const int mouseX, const int mouseY) = 0;
    virtual bool moveSelection(const int mouseX, const int mouseY) = 0;
    virtual bool endSelection() = 0;

    virtual void executeQuery() = 0;

    virtual const Ogre::SceneQueryResult & executeAndGetQuery()
    {
        executeQuery();
        return m_queryResult;
    }
    virtual const Ogre::SceneQueryResult & getLastQuery()
    {
        return m_queryResult;
    }

    virtual bool inExactBounds(Ogre::MovableObject * object) const { return true; }

protected:
    Ogre::MovableObject * m_view;
    Ogre::SceneManager * m_sceneManager;
    Ogre::Camera * m_camera;
    Ogre::SceneQueryResult m_queryResult;
};
#endif //STRATEKI_SELECTIONVIEW_H
