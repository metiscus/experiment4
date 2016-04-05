#pragma once

#include "weapon.h"
#include <stdbool.h>
#include <stdint.h>

typedef enum BrigadeType
{
    brigade_first,
    brigade_infantry = brigade_first,
    brigade_artillery,

    brigade_count
} BrigadeType;

typedef enum Tactic
{
    tactic_first,
    tactic_defend = tactic_first,
    tactic_defend_delay,      // defend giving ground
    tactic_defend_hold,       // defend with intent to keep
    tactic_defend_retreat,
    tactic_attack,
    tactic_attack_assault,    // attack with intent to take
    tactic_attack_scout,      // probing attack
    tactic_attack_retreat,

    tactic_count
} Tactic;

typedef struct Brigade
{
    uint32_t id;
    BrigadeType type;
    uint32_t armor;     // mm
    uint32_t troops;
    uint32_t supplies;
    WeaponType weapon_type;
    uint32_t weapon_count;
    WeaponType support_type;
    uint32_t support_count;
    float organization;
    uint32_t morale;
    float experience;
    uint32_t dug_in;
    uint32_t speed; // km/h
    int32_t  distance;
    Tactic   tactic;
    char *name;
} Brigade;


extern void        brigade_create(Brigade* ptr, BrigadeType type);
extern void        brigade_debug(Brigade *ptr);
extern bool        brigade_is_frontline(const Brigade* ptr);
extern uint32_t    brigade_calculate_range(const Brigade* ptr);
extern const char* brigade_get_type_name(BrigadeType type);
extern const char* brigade_get_name(Brigade* ptr);
extern uint32_t    brigade_get_troop_width(BrigadeType type);
