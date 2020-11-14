//
// Created by kieran on 14.11.20.
//

#ifndef SCENE_DEFINITIONS_H
#define SCENE_DEFINITIONS_H

#include <cstdint>
#include <bitset>

using Entity = std::uint32_t;
const Entity MAX_ENTITIES = 100;

using ComponentType = std::uint8_t;
const ComponentType MAX_COMPONENTS = 32;

using Signature = std::bitset<MAX_COMPONENTS>;


#endif //SCENE_DEFINITIONS_H
