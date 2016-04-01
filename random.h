#pragma once

#include <stdint.h>

typedef struct RandomGen
{
    uint64_t seed;
} RandomGen;

extern void randomgen_create(RandomGen* ptr, uint64_t seed);
extern uint64_t randomgen_get(RandomGen* ptr);
extern uint64_t randomgen_get_range_uint(RandomGen* ptr, uint64_t min, uint64_t max);
