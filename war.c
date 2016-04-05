#include "war.h"
#include <math.h>

static void implement_tactic(CombatInfo *info, Brigade* ptr, Brigade* victim);

bool combat_round(CombatInfo *info)
{
    assert(info);

    uint32_t e_attack_count = 0;
    uint32_t e_defend_count = 0;

    Brigade *e_attack[info->attacker_count];
    Brigade *e_defend[info->defender_count];
   
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
                //printf("DDD victim id: %u\n", victim_id);
            }
            else
            {
                uint32_t victim_id = randomgen_get_range_uint(info->generator, 0, info->attacker_count);
                victim = info->attackers[victim_id];
                //printf("DDD victim id: %u\n", victim_id);
            }
            
            implement_tactic(info, info->defenders[ii], victim);
        }
    }
    
    for(uint32_t ii=0; ii<info->attacker_count; ++ii)
    {
        // disorganized brigades can not attack
        if(info->attackers[ii]->organization == 0)
        {
            printf("The attackers %s %s brigade is too disorganized to attack.\n", 
                    ordinal_number(info->attackers[ii]->id),
                    brigade_get_type_name(info->attackers[ii]->type)
            );
        }
        else // the brigade can attack
        {
            Brigade *victim = NULL;
            // choose a victim
            // TODO: eventually this needs to try to match up target units to weapon types
            // e.g. AT brigades should prioritize tanks. Leaders and tactics should affect
            // this as well.
            if(brigade_is_frontline(info->attackers[ii]))
            {
                // front line divisions can only target other front line divisions
                uint32_t victim_id = randomgen_get_range_uint(info->generator, 0, e_defend_count);
                victim = e_defend[victim_id];
                //printf("AAA victim id: %u\n", victim_id);
            }
            else
            {
                uint32_t victim_id = randomgen_get_range_uint(info->generator, 0, info->defender_count);
                victim = info->defenders[victim_id];
                //printf("AAA victim id: %u\n", victim_id);
            }
            
            implement_tactic(info, info->attackers[ii], victim);
        }
    }
    
    return true;
}

static uint32_t calculate_troop_hits(CombatInfo *info, Brigade* ptr, Brigade* victim, float power_ratio)
{
    // ensure there are enough supplies for that power ratio
    float supply_ratio = (float)ptr->supplies / (float)ptr->troops;
    power_ratio = fmin(power_ratio, supply_ratio);
    
    //printf("supply ratio: %f\n", supply_ratio);
    //printf("power ratio: %f\n", power_ratio);

    uint32_t max_hits = power_ratio * ptr->troops;
    //printf("max hits: %u\n", max_hits);
    ptr->supplies = umin(ptr->supplies - max_hits, ptr->supplies);

    uint32_t min_hits = umax((ptr->experience / 100.0f) * max_hits, 0);

    if(max_hits == 0)
        return 0;

    return randomgen_get_range_uint(info->generator, min_hits, max_hits);
}

static uint32_t calculate_support_hits(CombatInfo* info, Brigade* ptr, Brigade* victim, float power_ratio)
{
    // ensure there are enough supplies for that power ratio
    float supply_ratio = (float)ptr->supplies / (float)ptr->support_count;
    power_ratio = fmin(power_ratio, supply_ratio);
    
    //printf("supply ratio: %f\n", supply_ratio);
    //printf("power ratio: %f\n", power_ratio);

    uint32_t max_hits = power_ratio * ptr->support_count;
    //printf("max hits: %u\n", max_hits);
    ptr->supplies = umin(ptr->supplies - max_hits, ptr->supplies);

    uint32_t min_hits = umax((ptr->experience / 100.0f) * max_hits, 0);

    if(max_hits == 0)
        return 0;

    return randomgen_get_range_uint(info->generator, min_hits, max_hits);
    
}

static float get_victim_power_ratio(Tactic tactic)
{
    switch(tactic)
    {
        case tactic_attack:
        case tactic_attack_assault:
            return 1.0;
            break;
        case tactic_attack_scout:
            return 0.25f;
            break;
        case tactic_attack_retreat:
            return 1.0f;
            break;
        case tactic_defend:
            return 0.85f;
            break;
        case tactic_defend_delay:
            return 0.24f;
            break;
        case tactic_defend_hold:
            return 1.0f;
            break;
        case tactic_defend_retreat:
            return 1.0f;
        default:
            assert(0);
            break;
    }
}

static void implement_tactic(CombatInfo *info, Brigade* ptr, Brigade* victim)
{
    assert(info);
    assert(ptr);
    
    bool is_night = false;
    if(info->hour < 8 || info->hour > 20)
    {
        is_night = true;
    }

    float power_ratio = 1.0f;
    float org_ratio = 1.0f;
    
    if(is_night)
    {
        //printf("Night time penalizes all combat\n");
        power_ratio = 0.1f;
        org_ratio   = 0.2f;
    }

    int32_t motion_dir = 0;
    switch(ptr->tactic)
    {
        case tactic_attack:
            org_ratio *= 0.75f;
            motion_dir = 1;
            break;
        case tactic_attack_assault:
            power_ratio *= 1.0f;
            org_ratio *= 0.6f;
            motion_dir = 1;
            break;
        case tactic_attack_scout:
            power_ratio *= 0.25f;
            org_ratio *= 0.2f;
            motion_dir = 1;
            break;
        case tactic_attack_retreat:
            ptr->distance = umax(0, ptr->distance - 1000 * ptr->speed);
            return;
            break;
        case tactic_defend:
            org_ratio *= 0.7f;
            power_ratio *= 0.85f;
            motion_dir = 0;
            break;
        case tactic_defend_delay:
            org_ratio *= 0.18f;
            power_ratio *= 0.24f;
            motion_dir = 0;
            break;
        case tactic_defend_hold:
            org_ratio *= 0.95f;
            power_ratio *= 1.0f;
            motion_dir = 0;
            break;
        case tactic_defend_retreat:
            ptr->distance += 1000 * ptr->speed;
            return;
            break;
        default:
            assert(0);
            break;
    }

    //printf("power ratio2: %f\n", power_ratio);
    int32_t distance = victim->distance - ptr->distance;
    if(fabs(distance) <= weapon_get_range(ptr->weapon_type))
    {
        uint32_t troop_hits = calculate_troop_hits(info, ptr, victim, power_ratio);

        troop_hits *= get_victim_power_ratio(victim->tactic);

        printf("%s hit %s for %u casualties by %s.\n",
            brigade_get_name(ptr),
            brigade_get_name(victim),
            troop_hits,
            weapon_get_type_name(ptr->weapon_type)
        );
        
        victim->troops       = umin(victim->troops - troop_hits, victim->troops);
        victim->organization = umin( victim->organization - (org_ratio * troop_hits / 100.0f), victim->organization);
        ptr->experience     += sqrtf(troop_hits / 10000.0f);
    }
    else
    {
        int32_t distance_moved = fmax(distance, motion_dir * 1000 * ptr->speed);
        ptr->distance += distance_moved;
        if(motion_dir != 0)
        {
            printf("%s moves %d to %d\n", brigade_get_name(ptr), distance_moved, ptr->distance);
        }
        return;
    }
    
    if(fabs(distance) <= weapon_get_range(ptr->support_type))
    {
        uint32_t troop_hits = calculate_support_hits(info, ptr, victim, power_ratio);
        troop_hits *= get_victim_power_ratio(victim->tactic);
        printf("%s hit %s for %u casualties by %s.\n",
            brigade_get_name(ptr),
            brigade_get_name(victim),
            troop_hits,
            weapon_get_type_name(ptr->support_type)
        );
        
        victim->troops       = umin(victim->troops - troop_hits, victim->troops);
        victim->organization = umin( victim->organization - (org_ratio * troop_hits / 100.0f), victim->organization);
        ptr->experience     += sqrtf(troop_hits / 10000.0f);
    }

}

void combat(CombatInfo *info)
{
    int32_t attack_start_distance = 0;

    // compute the max distance of any defending unit
    for(uint32_t ii=0; ii<info->defender_count; ++ii)
    {
        attack_start_distance = umax(attack_start_distance, brigade_calculate_range(info->defenders[ii]));
    }
    printf("The attackers will start at a distance of %u meters\n", attack_start_distance);

    int32_t attack_distance[info->attacker_count];
    for(uint32_t ii=0; ii<info->attacker_count; ++ii)
    {
        info->attackers[ii]->distance = -attack_start_distance + 10;
    }

    bool continue_combat = true;
    while(continue_combat)
    {
        uint32_t defense_strength = 0;
        uint32_t attack_strength  = 0;
        // determine total attacking strength
        for(uint32_t ii=0; ii<info->attacker_count; ++ii)
        {
            attack_strength += sqrt(info->attackers[ii]->troops * info->attackers[ii]->organization);
        }

        // determine total defending strength
        for(uint32_t ii=0; ii<info->defender_count; ++ii)
        {
            defense_strength += sqrt(info->defenders[ii]->troops * info->defenders[ii]->organization);
        }

        // hack to supply a little supply to each brigade every day
        if(info->hour % 24 == 0)
        {
            for(uint32_t ii=0; ii<info->attacker_count; ++ii)
            {
                info->attackers[ii]->supplies += 10000;
            }
            for(uint32_t ii=0; ii<info->defender_count; ++ii)
            {
                info->defenders[ii]->supplies += 10000;
            }
            
            info->hour = 0;
        }

        printf("Attacker effective strength: %u\n", attack_strength);
        printf("Defender effective strength: %u\n", defense_strength);

        // attacker: pick tactics
        for(uint32_t ii=0; ii<info->attacker_count; ++ii)
        {
            if(info->attackers[ii]->organization == 0)
            {
                printf("%s will retreat.\n", brigade_get_name(info->attackers[ii]));
                info->attackers[ii]->tactic = tactic_attack_retreat;
            }
            else if(attack_strength > 2 * defense_strength)
            {
//                printf("%s will assault.\n", brigade_get_name(info->attackers[ii]));
//                info->attackers[ii]->tactic = tactic_attack_assault;
            }
            else if(attack_strength < defense_strength)
            {
//                printf("%s will scout.\n", brigade_get_name(info->attackers[ii]));
//                info->attackers[ii]->tactic = tactic_attack_scout;
            }
            else
            {
//                printf("%s will attack.\n", brigade_get_name(info->attackers[ii]));
//                info->attackers[ii]->tactic = tactic_attack;
            }
        }

        // defender: pick tactics
        for(uint32_t ii=0; ii<info->defender_count; ++ii)
        {
            if(info->defenders[ii]->organization == 0)
            {
                printf("%s will retreat.\n", brigade_get_name(info->defenders[ii]));
                info->defenders[ii]->tactic = tactic_defend_retreat;
            }
            else if(defense_strength > 2 * attack_strength)
            {
                printf("%s will assault.\n", brigade_get_name(info->defenders[ii]));
                info->defenders[ii]->tactic = tactic_attack_assault;
            }
            else if(defense_strength * 2 < attack_strength)
            {
                printf("%s will delay.\n", brigade_get_name(info->defenders[ii]));
                info->defenders[ii]->tactic = tactic_defend_delay;
            }
            else
            {
//                printf("%s will defend.\n", brigade_get_name(info->defenders[ii]));
//                info->defenders[ii]->tactic = tactic_defend;
            }
        }
    
        printf("Combat hour: %d\n", ++info->hour);
        continue_combat &= combat_round(info);
        
        static int x = 0;
        if(++x > 50)
            break;
    }
    
    printf("attackers:\n");
    for(uint32_t ii=0; ii<info->attacker_count; ++ii)
    {
        brigade_debug(info->attackers[ii]);
    }

    printf("defenders:\n");
    for(uint32_t ii=0; ii<info->defender_count; ++ii)
    {
        brigade_debug(info->defenders[ii]);
    }
}
