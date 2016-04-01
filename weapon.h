#pragma once

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


extern void ammo_create(Ammo* ptr, AmmoType type);
extern void weapon_create(Weapon* ptr, WeaponType type);
extern uint32_t weapon_get_range(WeaponType type);
