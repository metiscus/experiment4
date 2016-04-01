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
    uint32_t organization;
    uint32_t morale;
    uint32_t experience;
    uint32_t dug_in;
    uint32_t speed; // km/h
    uint32_t distance;
} Brigade;


extern void        brigade_create(Brigade* ptr, BrigadeType type);
extern void        brigade_debug(Brigade *ptr);
extern bool        brigade_is_frontline(const Brigade* ptr);
extern uint32_t    brigade_calculate_range(const Brigade* ptr);
extern const char* brigade_get_type_name(BrigadeType type);
