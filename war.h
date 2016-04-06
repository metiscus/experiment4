#pragma once

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "territory.h"
#include "brigade.h"
#include "random.h"
#include "utility.h"
#include "weapon.h"

typedef struct CombatInfo
{
    Territory* territory;
    Brigade** attackers;
    uint32_t attacker_count;
    Brigade** defenders;
    uint32_t defender_count;
    RandomGen *generator;
    uint32_t hour;
} CombatInfo;

extern void combat(CombatInfo *info);
extern bool combat_round(CombatInfo *info);
