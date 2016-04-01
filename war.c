#include "war.h"

bool combat_round(CombatInfo *info)
{
    assert(info);
    
    uint32_t hour = 0;
    
    uint32_t e_attack_count = 0;
    uint32_t e_defend_count = 0;

    Brigade *e_attack[info->attacker_count];
    Brigade *e_defend[info->attacker_count];
   
    // pick out the front-line defending units
    for(uint32_t ii=0; ii<info->defender_count; ++ii)
    {
        if(brigade_is_frontline(info->defenders[ii]))
        {
            e_defend[e_defend_count++] = info->defenders[ii];
        }
    }

    // pick out the front-line attacking units
    for(uint32_t ii=0; ii<info->attacker_count; ++ii)
    {
        if(brigade_is_frontline(info->attackers[ii]))
        {
            e_attack[e_attack_count++] = info->attackers[ii];
        }
    }

    printf("Combat hour %u\n", hour);

    // Check for end of combat conditions
    // TODO: implement over-running a division / attacking support divisions
    if(e_attack_count == 0)
    {
        printf("The attacker has run out of brigades.\n");
        return false;
    }

    if(e_defend_count == 0)
    {
        printf("The defender has run out of brigades.\n");
        return false;
    }

    // the defender will attack first
    for(uint32_t ii=0; ii<info->defender_count; ++ii)
    {
        // disorganized brigades can not attack
        if(info->defenders[ii]->organization == 0)
        {
            printf("The defending %s %s brigade is too disorganized to attack.\n", 
                    ordinal_number(info->defenders[ii]->id),
                    brigade_get_type_name(info->defenders[ii]->type)
            );
        }
        else // the brigade can attack
        {
            Brigade *victim = NULL;
            // choose a victim
            // TODO: eventually this needs to try to match up target units to weapon types
            // e.g. AT brigades should prioritize tanks. Leaders and tactics should affect
            // this as well.
            if(brigade_is_frontline(info->defenders[ii]))
            {
                // front line divisions can only target other front line divisions
                uint32_t victim_id = randomgen_get_range_uint(info->generator, 0, e_attack_count);
                victim = e_attack[victim_id];
            }
            else
            {
                uint32_t victim_id = randomgen_get_range_uint(info->generator, 0, info->attacker_count);
                victim = info->attackers[victim_id];
            }
            
            printf("Defender %u is attacking attacker %u\n", ii, victim->id);

            // compute hits and apply them
            // compute linear density of victim unit
            uint32_t frontage = info->territory->width / e_attack_count;
            uint32_t density  = victim->troops / frontage; 
            printf("Attacker has a linear density of %u troops / km\n", density);

            // fire primary weapon
            uint32_t e_frontage = 1000 * frontage;
            uint32_t hits = 0;
            for(uint32_t shot=0; shot<info->defenders[ii]->troops;++shot)
            {
                
            }
        }
    }
    
    return true;
}

void combat(CombatInfo *info)
{
    uint32_t hour = 0;

    uint32_t attack_start_distance = 0;

    // compute the max distance of any defending unit
    for(uint32_t ii=0; ii<info->defender_count; ++ii)
    {
        attack_start_distance = umax(attack_start_distance, brigade_calculate_range(info->defenders[ii]));
    }
    printf("The attackers will start at a distance of %u meters\n", attack_start_distance);

    uint32_t attack_distance[info->attacker_count];
    for(uint32_t ii=0; ii<info->attacker_count; ++ii)
    {
        info->attackers[ii]->distance = attack_start_distance;
    }

    bool continue_combat = true;
    while(continue_combat)
    {
        combat_round(info);
        break;
    }
}
