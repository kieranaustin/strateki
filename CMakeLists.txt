cmake_minimum_required(VERSION 3.14)
project(scene)


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

add_library(Grid Grid.cpp Grid.h)
add_library(CameraControl CameraControl.cpp CameraControl.h)
add_library(TerrainLoader TerrainLoader.cpp TerrainLoader.h)
add_library(ecs
    ecs/Definitions.h
    ecs/EntityManager.cpp ecs/EntityManager.h
    ecs/ComponentContainer.h ecs/ComponentManager.h
    ecs/System.h ecs/SystemManager.h
    ecs/Register.h
    ecs/components/Components.h
)
add_library(ecsSystems
    ecs/systems/MovementSystem.h
    ecs/systems/RenderSystem.h
    ecs/systems/TerrainCollisionSystem.h
    ecs/systems/GravitySystem.h
)


add_executable(scene main.cpp Game.cpp Game.h EntityFactory.h)


target_link_libraries(Grid OgreMain OgreBites)
target_link_libraries(CameraControl OgreBites OgreTerrain)
target_link_libraries(TerrainLoader OgreMain OgreTerrain)
target_link_libraries(ecs OgreMain)
target_link_libraries(ecsSystems ecs)
target_link_libraries(scene 
    OgreBites OgreRTShaderSystem OgreTerrain
    Grid CameraControl TerrainLoader
    ecs ecsSystems
)


file(COPY media DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY resources.cfg DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
