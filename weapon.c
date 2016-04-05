#include "weapon.h"
#include "utility.h"
#include <assert.h>
#include <stdint.h>

static const char *WeaponStrings[]
= {
    "Unarmed",
    "Infantry Rifle",
    "Mortar",
    "Howitzer"
};

static const char *AmmoStrings[]
= {
    "Bullet",
    "HE Mortar Round",
    "AT Mortar Round",
    "HE Shell",
    "AT Shell",
};

void ammo_create(Ammo* ptr, AmmoType type)
{
    switch(type)
    {
        case ammo_none:
            ptr->range = 0;
            ptr->radius = 0;
            ptr->pierce = 0;
            break;
        case ammo_bullet:
            ptr->range  = 1000;
            ptr->radius = 1;
            ptr->pierce = 10;
            break;
        case ammo_mortar:
            ptr->range = 1814;
            ptr->radius = 200;
            ptr->pierce = 20;
            break;
        case ammo_howitzer_he:
            ptr->range = 7500;
            ptr->radius = 2000;
            ptr->pierce = 40;
            break;
        case ammo_howitzer_at:
            ptr->range = 7600;
            ptr->radius = 1000;
            ptr->pierce = 100;
            break;
        case ammo_count:
        default:
            assert(0);
            break;
    }
}

const char* ammo_get_type_name(AmmoType type)
{
    assert(type > ammo_first);
    assert(type < ammo_count);
    return AmmoStrings[(uint32_t)type];
}

void weapon_create(Weapon* ptr, WeaponType type)
{
    ammo_create(&ptr->ammo[0], ammo_none);
    ammo_create(&ptr->ammo[1], ammo_none);

    switch(type)
    {
        case weapon_none:
            ptr->rate_of_fire = 0;
            ptr->range = 0;
            break;
        case weapon_rifle:
            ptr->rate_of_fire = 1100;
            ptr->range = 300;
            ammo_create(&ptr->ammo[0], ammo_bullet);
            break;
        case weapon_mortar:
            ptr->rate_of_fire = 1080;
            ptr->range = 1900;
            ammo_create(&ptr->ammo[0], ammo_mortar);
            break;
        case weapon_howitzer:
            ptr->rate_of_fire = 120;
            ptr->range = 8000;
            ammo_create(&ptr->ammo[0], ammo_howitzer_he);
            ammo_create(&ptr->ammo[1], ammo_howitzer_at);
            break;

        case weapon_count:
        default:
            assert(0);
            break;
    }
}

uint32_t weapon_get_range(WeaponType type)
{
    Weapon wpn;
    weapon_create(&wpn, type);

    return umax(wpn.ammo[0].range, wpn.ammo[1].range);
}

const char* weapon_get_type_name(WeaponType type)
{
    assert(type > weapon_first);
    assert(type < weapon_count);
    return WeaponStrings[(uint32_t)type];
}
