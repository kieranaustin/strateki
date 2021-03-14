//
// Created by kieran on 13.03.21.
//

#include <Ogre.h>
#include <OgreInput.h>
#include <Terrain/OgreTerrainGroup.h>
#include "SelectionView.h"
#include "SelectionBox.h"
#include "SelectionSphere.h"

#ifndef STRATEKI_SELECTIONMANAGER_H
#define STRATEKI_SELECTIONMANAGER_H

class SelectionController : public OgreBites::InputListener
{
public:
    SelectionController(Ogre::SceneManager * sceneManager, Ogre::Camera * camera);
    SelectionController(Ogre::SceneManager * sceneManager, Ogre::Camera * camera, Ogre::TerrainGroup * terrainGroup);
    ~SelectionController();

    bool mousePressed(const OgreBites::MouseButtonEvent& evt) override;
    bool mouseMoved(const OgreBites::MouseMotionEvent& evt) override;
    bool mouseReleased(const OgreBites::MouseButtonEvent& evt) override;

    bool switchTo(const Ogre::String & viewType);

    const std::list<Ogre::MovableObject*> & getSelectionList()
    {
        return m_selectionList;
    }

    /*
     * attach a Ogre::TerrainGroup to this controller
     * needed for spherical selection
     */
    void setTerrainGroup(Ogre::TerrainGroup * terrainGroup)
    {
        if (terrainGroup == nullptr)
            return;

        m_terrainGroup = terrainGroup;
    }

private:
    /*
     * create a new selection view
     * /arg viewType "box" or "sphere"
     * @return true if creating the viewType was successful, else false
     */
    SelectionView * createView(const Ogre::String & viewType);
    /*
     * deselect all objects
     */
    void deselectAll();
    /*
     * checks whether object is viable for selection
     * @return true if object should be omitted, false if object can be selected
     */
    bool reject(Ogre::MovableObject * object);

    void performSelection();

private:
    std::list<Ogre::MovableObject*> m_selectionList;

    Ogre::SceneManager * m_sceneManager;
    Ogre::Camera * m_camera;

    SelectionView * m_view;
    std::map<Ogre::String, SelectionView *> m_views;

    Ogre::TerrainGroup * m_terrainGroup;
};
#endif //STRATEKI_SELECTIONMANAGER_H
