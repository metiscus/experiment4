#pragma once

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "brigade.h"
#include "random.h"
#include "utility.h"
#include "weapon.h"

typedef enum TerrainType
{
    terrain_plains,
    terrain_hills,
    terrain_mountains
} TerrainType;

typedef struct Territory
{
    uint32_t id;
    TerrainType terrain;
    uint32_t width;      // km
    char *name;
} Territory;
