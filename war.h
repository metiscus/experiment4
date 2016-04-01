#pragma once

#include <stdio.h>
#include <stdint.h>

typedef enum WeaponType
{
    weapon_none,
    weapon_rifle,
    weapon_mortar,
    weapon_howitzer
} WeaponType;

extern const char *WeaponStrings[];

typedef enum AmmoType
{
    ammo_none,
    ammo_bullet,
    ammo_mortar,
    ammo_howitzer_he,
    ammo_howitzer_at
} AmmoType;

typedef struct Ammo
{
    uint32_t range;     // range in meters
    uint32_t radius;    // damage radius in cm
    uint32_t pierce;    // armor pierce in mm
} Ammo;

extern const char *AmmoStrings[];

#define MaxAmmo 2
typedef struct Weapon
{
    uint32_t rate_of_fire;  // shots per hour
    uint32_t range;         // meters
    Ammo ammo[MaxAmmo];
} Weapon;

typedef enum BrigadeType
{
    brigade_infantry,
    brigade_artillery
} BrigadeType;

extern const char *BrigadeStrings[];

typedef struct Brigade
{
    uint32_t id;
    BrigadeType type;
    uint32_t armor;
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
} Brigade;

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

extern void create_brigade(Brigade* ptr, BrigadeType type);
extern void describe_brigade(Brigade *ptr);

extern char* ordinal_number(uint32_t num);

extern uint32_t brigade_range(Brigade* ptr);
