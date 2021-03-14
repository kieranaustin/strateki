cmake_minimum_required(VERSION 3.14)

project(strateki)

set(CMAKE_CXX_STANDARD 14)

message("")
message("CMAKE variables")
message("cmake prefix path " "${CMAKE_PREFIX_PATH}")
message("cmake include path " "${CMAKE_INCLUDE_PATH}")
message("cmake system prefix path " "${CMAKE_SYSTEM_PREFIX_PATH}")
message("")

find_package(OGRE 1.12.9 REQUIRED COMPONENTS Bites RTShaderSystem Terrain)
message("-- OGRE_CONFIG = " "${OGRE_CONFIG}")
message("-- OGRE_CONFIG_DIR = " "${OGRE_CONFIG_DIR}")
message("-- OGRE_DIR = " "${OGRE_DIR}")
message("-- OGRE_INCLUDE_DIRS = " "${OGRE_INCLUDE_DIRS}")
message("-- OGRE_LIBRARY_DIRS = " "${OGRE_LIBRARY_DIRS}")
message("-- OGRE_LIBRARIES = " "${OGRE_LIBRARIES}")
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
    AuxiliaryIdManager.h)
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
    ecs/systems/DestinationSystem.h
)


###
### Declare Executables
add_executable(${CMAKE_PROJECT_NAME} main.cpp)


###
### Link Libraries
target_link_libraries(Grid OgreMain OgreBites)
target_link_libraries(GameCore
    OgreMain OgreBites OgreTerrain
    ecs ecsSystems
)
target_link_libraries(ecs OgreMain)
target_link_libraries(ecsSystems ecs)
target_link_libraries(${CMAKE_PROJECT_NAME}
    OgreBites OgreRTShaderSystem OgreTerrain
    GameCore
)


###
### Copy Assets and Configuration to build directory
message("Copying assets and configurations to build directory")
file(COPY media DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY resources.cfg DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
