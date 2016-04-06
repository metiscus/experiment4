#include "territory.h"
#include <assert.h>
#include <string.h>
#include <stdlib.h>

void territory_create(Territory* ptr, TerrainType terrain, const char* name)
{
    assert(ptr);
    assert(name);
    
    static uint32_t id = 0;
    
    ptr->id = ++id;
    ptr->name = strdup(name);
    ptr->type = terrain;
    ptr->width = 5;
}

void territory_destroy(Territory* ptr)
{
    assert(ptr);
    free(ptr->name);
}
