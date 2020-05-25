cmake_minimum_required(VERSION 3.14)
project(scene)


set(CMAKE_CXX_STANDARD 14)

## add extra include directories
#include_directories(/opt/OGRE/include)
#include_directories(/home/kieran/projects/Ogre/ogre/OgreMain)
## add extra lib directories
#link_directories(/opt/OGRE/lib)

#set(CMAKE_PREFIX_PATH "/opt/OGRE/")
#list(APPEND CMAKE_MODULE_PATH "/opt/OGRE/lib/OGRE/cmake/")
#find_package(OGRE 1.12.3 EXACT REQUIRED PATHS "/opt/OGRE/" COMPONENTS Bites RTShaderSystem)
find_package(OGRE 1.12.3 REQUIRED COMPONENTS Bites RTShaderSystem Terrain)
#set(OGRE_DIR "/opt/OGRE/include")
#set(CMAKE_SYSTEM_PREFIX_PATH  "/opt/OGRE;${CMAKE_SYSTEM_PREFIX_PATH}")
#set(CMAKE_SYSTEM_PREFIX_PATH  "/opt/OGRE")
message("cmake prefix path " "${CMAKE_PREFIX_PATH}")
message("cmake include path " "${CMAKE_INCLUDE_PATH}")
message("cmake system prefix path " "${CMAKE_SYSTEM_PREFIX_PATH}")
message("OGRE home " "${OGRE_HOME}")
message("OGRE path " "${OGRE_CONFIG}")
message("OGRE config dir " "${OGRE_CONFIG_DIR}")
message("OGRE lib dir " "${OGRE_DIR}")
message("OGRE include dir " "${OGRE_INCLUDE_DIRS}")
message("OGRE library dirs " "${OGRE_LIBRARY_DIRS}")
message("OGRE librareis " "${OGRE_LIBRARIES}")
message("OGRE library " "${OGRE_LIBRARY}")

add_library(Grid Grid.cpp Grid.h)
add_library(CameraControl CameraControl.cpp CameraControl.h)
#set_target_properties(CameraControl PROPERTIES LINKER_LANGUAGE CXX)
#set(SOURCES "/home/kieran/projects/ogre/OgreMain")
#add_library(ogre_for_ide EXCLUDE_FROM_ALL ${SOURCES})
#file(COPY ${OGRE_CONFIG_DIR}/resources.cfg DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

#include_directories(BEFORE SYSTEM ${OGRE_INCLUDE_DIRS})
add_executable(scene main.cpp main.h)

#target_include_directories(scene PUBLIC ${OGRE_INCLUDE_DIRS})
target_link_libraries(Grid OgreMain OgreBites)
target_link_libraries(CameraControl OgreBites OgreTerrain)
target_link_libraries(scene OgreBites OgreRTShaderSystem Grid CameraControl OgreTerrain)

#install( DIRECTORY media DESTINATION ${PROJECT_BINARY_DIR} )
#install( FILES resources.cfg DESTINATION ${PROJECT_BINARY_DIR} )

#file(COPY media DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY resources.cfg DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
