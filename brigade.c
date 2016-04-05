#include "brigade.h"
#include "utility.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

static const char *BrigadeStrings[]
= {
    "Infantry Brigade",
    "Artillery Brigade"
};

void brigade_create(Brigade* ptr, BrigadeType type)
{
    assert(ptr);
    const uint32_t InfantryBrigade_DefaultStrength = 5000;
    const WeaponType   InfantryBrigade_DefaultWeapon = weapon_rifle;
    const WeaponType   InfantryBrigade_DefaultSupport = weapon_mortar;
    const uint32_t InfantryBrigade_DefaultSupportCount = 25;
    const uint32_t InfantryBrigade_DefaultSupplies = InfantryBrigade_DefaultStrength * 8;

    const uint32_t SupportBrigade_DefaultStrength = 1500;
    const WeaponType   SupportBrigade_DefaultWeapon = weapon_rifle;
    const uint32_t SupportBrigade_DefaultSupplies = InfantryBrigade_DefaultStrength * 8;
   
    static uint32_t brigade_id = 0;

    memset(ptr, 0, sizeof(Brigade));

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
            ptr->speed         = 5;
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
            ptr->speed         = 2;
            break;
        case brigade_count:
        default:
            assert(0);
            break;
    }
}

void brigade_debug(Brigade *ptr)
{
    assert(ptr);
    printf("%s %s\n", ordinal_number(ptr->id), BrigadeStrings[ptr->type]);
    printf("Strength: %d\n", ptr->troops);
    printf("Supplies: %d\n", ptr->supplies); // each troop needs 1 supply per day to fight
    printf("Weapons:\n");
    // each troop needs a weapon to fight
    printf("\t%s : %d\n", weapon_get_type_name(ptr->weapon_type), ptr->weapon_count);
    if(ptr->support_type != weapon_none)
    {
        printf("\t%s : %d\n", weapon_get_type_name(ptr->support_type), ptr->support_count);
    }
    printf("Dug in: %s\n", (ptr->dug_in > 0) ? "Yes" : "No");
    printf("Organization: %d\n", ptr->organization);
    printf("Morale: %d\n", ptr->morale);
    printf("Experience: %f\n", ptr->experience);
}

bool brigade_is_frontline(const Brigade* ptr)
{
    assert(ptr);
    // brigades that are too disorganized or
    // are support brigades will not hold a front
    if(ptr->type == brigade_infantry)
    {
        if(ptr->organization > 0)
            return true;
    }
    return false;
}

uint32_t brigade_calculate_range(const Brigade* ptr)
{
    assert(ptr);
    Weapon weapons[2];
    weapon_create(&weapons[0], ptr->weapon_type);
    weapon_create(&weapons[1], ptr->support_type);
    if(weapons[0].range > weapons[1].range)
    {
        return weapons[0].range;
    }
    else
    {
        return weapons[1].range;
    }
}

const char* brigade_get_type_name(BrigadeType type)
{
    assert(type >= brigade_first);
    assert(type < brigade_count);
    return BrigadeStrings[(uint32_t)type];
}

const char* brigade_get_name(Brigade* ptr)
{
    assert(ptr);
    if(!ptr->name)
    {
        char buffer[1000];
        sprintf(buffer, "%s %s brigade",
            ordinal_number(ptr->id),
            brigade_get_type_name(ptr->type)
        );
        ptr->name = strdup(buffer);
    }
    return ptr->name;
}

uint32_t brigade_get_troop_width(BrigadeType type)
{
    assert(type >= brigade_first);
    assert(type < brigade_count);
    switch(type)
    {
        case brigade_infantry:
            return 1;
            break;
        case brigade_artillery:
            return 3;
            break;
        case brigade_count:
        default:
            assert(0);
            return 0;
            break;
    }
}
