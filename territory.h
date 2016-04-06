#pragma once

#include <stdint.h>

typedef enum TerrainType
{
    terrain_plains,
    terrain_hills,
    terrain_mountains
} TerrainType;

typedef struct Territory
{
    uint32_t id;
    TerrainType type;
    uint32_t width;      // km
    char *name;
} Territory;

extern void territory_create(Territory* ptr, TerrainType terrain, const char* name);
extern void territory_destroy(Territory* ptr);
