//
// Created by kieran on 13.03.21.
//

#include "SelectionController.h"
#include <iostream>

SelectionController::SelectionController(Ogre::SceneManager *sceneManager, Ogre::Camera *camera)
    : m_sceneManager(sceneManager)
    , m_camera(camera)
{}

SelectionController::SelectionController(Ogre::SceneManager *sceneManager, Ogre::Camera *camera, Ogre::TerrainGroup *terrainGroup)
    : m_sceneManager(sceneManager)
    , m_camera(camera)
    , m_terrainGroup(terrainGroup)
{}

SelectionController::~SelectionController()
{
    m_selectionList.clear();
    delete m_view;
}

bool SelectionController::switchTo(const Ogre::String &viewType)
{
    auto search = m_views.find(viewType);
    if (search != m_views.end())
    {
        m_view = search->second;
    }
    else
    {
        if (viewType == "box")
        {
            m_view = createView(viewType);
        }
        if(viewType == "sphere")
        {
            if (m_terrainGroup == nullptr)
                return false;

            m_view = createView(viewType);
        }
    }
    return true;
}

SelectionView * SelectionController::createView(const Ogre::String &viewType)
{
    if (viewType == "box")
    {
        SelectionView * view = new SelectionBox(m_sceneManager, m_camera);
        m_views.insert({viewType, view});

        return view;
    }
    if(viewType == "sphere")
    {
        SelectionView * view = new SelectionSphere(m_sceneManager, m_camera, m_terrainGroup);
        m_views.insert({viewType, view});

        return view;
    }
    return nullptr;
}

bool SelectionController::mousePressed(const OgreBites::MouseButtonEvent &evt)
{
    if(m_view == nullptr)
        return false;
    return m_view->beginSelection(evt.x, evt.y);
}

bool SelectionController::mouseMoved(const OgreBites::MouseMotionEvent &evt)
{
    if(m_view == nullptr)
        return false;
    m_view->moveSelection(evt.x, evt.y);
    m_view->executeQuery();
    performSelection();
    return true;
}

bool SelectionController::mouseReleased(const OgreBites::MouseButtonEvent &evt)
{
    if(m_view == nullptr)
        return false;
    m_view->endSelection();
    performSelection();
    return true;
}

void SelectionController::deselectAll()
{
    for(const auto obj : m_selectionList)
    {
        obj->getParentSceneNode()->showBoundingBox(false);
    }
    m_selectionList.clear();
}

bool SelectionController::reject(Ogre::MovableObject * object)
{
    Ogre::String name = object->getName();
    if (name == "selectionSphere")
        return true;
    if (name == "coordAxes")
        return true;
    if (name == "coordAxesRotate")
        return true;

    return false;
}

void SelectionController::performSelection()
{
    deselectAll();

    Ogre::SceneQueryResult result = m_view->getLastQuery();

    for(const auto obj : result.movables)
    {
        // reject on ecs::Entity level via "Selectable" component or similar
        if (reject(obj))
            continue;

        if (m_view->inExactBounds(obj))
        {
            m_selectionList.push_back(obj);
            obj->getParentSceneNode()->showBoundingBox(true);
        }
    }
}

