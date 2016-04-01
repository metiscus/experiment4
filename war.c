#include "war.h"

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char *WeaponStrings[]
= {
    "Unarmed",
    "Infanry Rifle",
    "Mortar",
    "Howitzer"
};

const char *AmmoStrings[]
= {
    "Bullet",
    "HE Mortar Round",
    "AT Mortar Round",
    "HE Shell",
    "AT Shell",
};

const char *BrigadeStrings[]
= {
    "Infantry Brigade",
    "Artillery Brigade"
};

void create_ammo(Ammo* ptr, AmmoType type)
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
    }
}

void create_weapon(Weapon* ptr, WeaponType type)
{
    create_ammo(&ptr->ammo[0], ammo_none);
    create_ammo(&ptr->ammo[1], ammo_none);

    switch(type)
    {
        case weapon_none:
            ptr->rate_of_fire = 0;
            ptr->range = 0;
            break;
        case weapon_rifle:
            ptr->rate_of_fire = 1100;
            ptr->range = 300;
            create_ammo(&ptr->ammo[0], ammo_bullet);
            break;
        case weapon_mortar:
            ptr->rate_of_fire = 1080;
            ptr->range = 1900;
            create_ammo(&ptr->ammo[0], ammo_mortar);
            break;
        case weapon_howitzer:
            ptr->rate_of_fire = 120;
            ptr->range = 8000;
            create_ammo(&ptr->ammo[0], ammo_howitzer_he);
            create_ammo(&ptr->ammo[1], ammo_howitzer_at);
            break;
    }
}

void create_brigade(Brigade* ptr, BrigadeType type)
{
    assert(ptr);
    const uint32_t InfantryBrigade_DefaultStrength = 5000;
    const WeaponType   InfantryBrigade_DefaultWeapon = weapon_rifle;
    const WeaponType   InfantryBrigade_DefaultSupport = weapon_mortar;
    const uint32_t InfantryBrigade_DefaultSupportCount = 25;
    const uint32_t InfantryBrigade_DefaultSupplies = InfantryBrigade_DefaultStrength * 2;

    const uint32_t SupportBrigade_DefaultStrength = 1500;
    const WeaponType   SupportBrigade_DefaultWeapon = weapon_rifle;
    const uint32_t SupportBrigade_DefaultSupplies = InfantryBrigade_DefaultStrength * 2;
   
    static uint32_t brigade_id = 0;

    memset(&ptr, 0, sizeof(Brigade));

    ptr->id = ++brigade_id;
    ptr->type = type;
    ptr->dug_in = 0;
    ptr->armor = 0;

    switch(type)
    {
        case brigade_infantry:
            ptr->troops        = InfantryBrigade_DefaultStrength;
            ptr->supplies      = InfantryBrigade_DefaultSupplies;
            ptr->weapon_type   = InfantryBrigade_DefaultWeapon;
            ptr->weapon_count  = ptr->troops;
            ptr->support_type  = InfantryBrigade_DefaultSupport;
            ptr->support_count = InfantryBrigade_DefaultSupportCount;
            ptr->organization  = 100;
            ptr->morale        = 100;
            ptr->experience    = 0;
            break;

        case brigade_artillery:
            ptr->troops        = SupportBrigade_DefaultStrength;
            ptr->supplies      = SupportBrigade_DefaultSupplies;
            ptr->weapon_type   = SupportBrigade_DefaultWeapon;
            ptr->weapon_count  = ptr->troops;
            ptr->support_type  = weapon_howitzer;
            ptr->support_count = 100;
            ptr->organization  = 100;
            ptr->morale        = 100;
            ptr->experience    = 0;
            break;
    }

}

void describe_brigade(Brigade *ptr)
{
    printf("%s %s\n", ordinal_number(ptr->id), BrigadeStrings[ptr->type]);
    printf("Strength: %d\n", ptr->troops);
    printf("Supplies: %d\n", ptr->supplies); // each troop needs 1 supply per day to fight
    printf("Weapons:\n");
    // each troop needs a weapon to fight
    printf("\t%s : %d\n", WeaponStrings[ptr->weapon_type], ptr->weapon_count);
    if(ptr->support_type != weapon_none)
    {
        printf("\t%s : %d\n", WeaponStrings[ptr->support_type], ptr->support_count);
    }
    printf("Dug in: %s\n", (ptr->dug_in > 0) ? "Yes" : "No");
    printf("Organization: %d\n", ptr->organization);
    printf("Morale: %d\n", ptr->morale);
    printf("Experience: %d\n", ptr->experience);
}

uint32_t brigade_range(Brigade* ptr)
{
    Weapon weapons[2];
    create_weapon(&weapons[0], ptr->weapon_type);
    create_weapon(&weapons[1], ptr->support_type);
    if(weapons[0].range > weapons[1].range)
    {
        return weapons[0].range;
    }
    else
    {
        return weapons[1].range;
    }
}

char* ordinal_number(uint32_t num)
{
    static char ordinal[128];

    uint32_t last_digit = num % 9;
    switch(last_digit)
    {
        case 1:
            sprintf(ordinal, "%ust", num);
            break;
        case 2:
            sprintf(ordinal, "%und", num);
            break;
        case 3:
            sprintf(ordinal, "%urd", num);
            break;
        case 0: // intentional
        case 4: // intentional
        case 5: // intentional
        case 6: // intentional
        case 7: // intentional
        case 8: // intentional
        case 9: // intentional
            sprintf(ordinal, "%uth", num);
            break;
    }
    
    return ordinal;
}
