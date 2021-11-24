cmake_minimum_required(VERSION 3.14)

project(strateki)

set(CMAKE_CXX_STANDARD 14)

message("")
message("-- CMAKE")
message("--   version            : ${CMAKE_VERSION}")
message("--   prefix path        : ${CMAKE_PREFIX_PATH}")
message("--   include path       : ${CMAKE_INCLUDE_PATH}")
message("--   system prefix path : ${CMAKE_SYSTEM_PREFIX_PATH}")
message("")

###
### Find package dependencies
find_package(OGRE 1.12.12 REQUIRED COMPONENTS Bites RTShaderSystem Terrain)
if(${OGRE_FOUND})
    message("--   version    : ${OGRE_VERSION} ${OGRE_VERSION_NAME}")
    message("--   dir        : ${OGRE_DIR}")
    message("--   config dir : ${OGRE_CONFIG_DIR}")
    message("--   config     : ${OGRE_CONFIG}")
    message("--   include dir: ${OGRE_INCLUDE_DIRS}")
    message("--   plugin dir : ${OGRE_PLUGIN_DIR}")
    message("--   plugins    : ${OGRE_PLUGINS}")
    message("--   library dir: ${OGRE_LIBRARY_DIRS}")
    message("--   libraries  : ${OGRE_LIBRARIES}")
    message("--   media      : ${OGRE_MEDIA_DIR}")
    message("--   components : ${OGRE_COMPONENTS}")
else()
    message(FATAL_ERROR "-- OGRE not found!")
endif()
message("")

find_package(Qt5 REQUIRED COMPONENTS Core Gui Qml Widgets)
if(${Qt5_FOUND})
    set(CMAKE_AUTOMOC ON)
    set(CMAKE_AUTOUIC ON)
    set(CMAKE_AUTORCC ON)
    message("-- Found Qt")
    message("--   version    : ${Qt5_VERSION}")
    message("--   libraries  : ${Qt5Core_LIBRARIES};${Qt5Gui_LIBRARIES};${Qt5Qml_LIBRARIES};${Qt5Widgets_LIBRARIES}")
else()
    message(FATAL_ERROR "-- Qt not found")
endif()
message("")

add_subdirectory(test)

###
### Declare Libraries
add_library(Grid Grid.cpp Grid.h)
add_library(GameCore
    Game.h Game.cpp
    CameraControl.h CameraControl.cpp
    TerrainLoader.h TerrainLoader.cpp
    EntityFactory.h
    SelectionController.h SelectionController.cpp
    SelectionView.h
    SelectionBox.h SelectionBox.cpp
    SelectionSphere.h SelectionSphere.cpp
    AuxiliaryIdManager.h
    Helper.cpp InfoOverlay.cpp InfoOverlay.h
    Screen.h GameScreen.cpp GameScreen.h StartScreen.cpp StartScreen.h
)
add_library(ecs
    ecs/Definitions.h
    ecs/EntityManager.cpp ecs/EntityManager.h
    ecs/ComponentContainer.h ecs/ComponentManager.h
    ecs/System.h ecs/SystemManager.h
    ecs/Register.h
)
add_library(ecsSystems
    ecs/components/Components.h
    ecs/systems/MovementSystem.h
    ecs/systems/RenderSystem.h
    ecs/systems/TerrainCollisionSystem.h
    ecs/systems/GravitySystem.h
    ecs/systems/CollisionSystem.h
)


###
### Declare Executables
add_executable(${CMAKE_PROJECT_NAME} main.cpp)


###
### Link Libraries
target_link_libraries(Grid OgreMain OgreBites)
target_link_libraries(GameCore
    Qt::Core Qt::Gui Qt::Widgets
    OgreMain OgreBites OgreBitesQt OgreTerrain
    ecs ecsSystems
)
target_link_libraries(ecs OgreMain)
target_link_libraries(ecsSystems ecs)
target_link_libraries(${CMAKE_PROJECT_NAME}
    Qt::Core Qt::Gui Qt::Widgets
    OgreBites OgreBitesQt OgreRTShaderSystem OgreTerrain
    GameCore
)


###
### Copy Assets and Configuration to build directory
message("-- Copying assets and configurations to build directory")
message("")
file(COPY media DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY resources.cfg DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
