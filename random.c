#include "random.h"

#include <assert.h>

void randomgen_create(RandomGen* ptr, uint64_t seed)
{
    assert(ptr);
    ptr->seed = seed;
}

static uint64_t xorshift64star(uint64_t *x);

uint64_t randomgen_get(RandomGen* ptr)
{
    assert(ptr);
    return xorshift64star(&ptr->seed);
}

uint64_t randomgen_get_range_uint(RandomGen* ptr, uint64_t min, uint64_t max)
{
    assert(ptr);
    assert(min < max);
    return xorshift64star(&ptr->seed) % (max-min) + min;
}

// from https://en.wikipedia.org/wiki/Xorshift
static uint64_t xorshift64star(uint64_t *x)
{
    (*x) ^= (*x) >> 12; // a
    (*x) ^= (*x) << 25; // b
    (*x) ^= (*x) >> 27; // c
    return (*x) * UINT64_C(2685821657736338717);
}
