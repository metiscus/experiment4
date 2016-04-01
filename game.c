#include "war.h"

void combat(Territory* territory, Brigade* attackers, uint32_t attacker_count, Brigade* defenders, uint32_t defender_count);

int main()
{
    Brigade usa[3];
    Brigade ger[2];
   
    brigade_create(&usa[0], brigade_infantry);
    brigade_create(&usa[1], brigade_infantry);
    brigade_create(&usa[2], brigade_artillery);
    
    brigade_create(&ger[0], brigade_infantry);
    brigade_create(&ger[1], brigade_infantry);

    printf("United States 1st Infantry Division:\n");
    brigade_debug(&usa[0]);
    printf("\n");
    brigade_debug(&usa[1]);
    printf("\n");
    brigade_debug(&usa[2]);
    printf("\n");

    Territory plains;
    plains.id = 0;
    plains.terrain = terrain_plains;
    plains.width = 5;
    plains.name = strdup("Atton");


    // Battle: Liberation of Nancy
    // the germans are defending near Atton
    // it is late in the war so the german units have lower morale
    ger[0].troops *= 0.94;
    ger[1].troops *= 0.94;
    ger[0].morale = 50;
    ger[1].morale = 60;

    combat(&plains, usa, 3, ger, 2);
    
    return 0;
}

void combat(Territory* territory, Brigade* attackers, uint32_t attacker_count, Brigade* defenders, uint32_t defender_count)
{
    uint32_t hour = 0;

    uint32_t attack_start_distance = 0;
    
    RandomGen random;
    randomgen_create(&random, 31415);

    // compute the max distance of any defending unit
    for(uint32_t ii=0; ii<defender_count; ++ii)
    {
        attack_start_distance = umax(attack_start_distance, brigade_calculate_range(&defenders[ii]));
    }
    printf("The attackers will start at a distance of %u meters\n", attack_start_distance);

    uint32_t attack_distance[attacker_count];
    for(uint32_t ii=0; ii<attacker_count; ++ii)
    {
        attack_distance[ii] = attack_start_distance;
    }

    while(true)
    {
        uint32_t e_attack_count = 0;
        uint32_t e_defend_count = 0;

        Brigade *e_attack[attacker_count];
        Brigade *e_defend[attacker_count];
   
        // pick out the front-line defending units
        for(uint32_t ii=0; ii<defender_count; ++ii)
        {
            if(brigade_is_frontline(&defenders[ii]))
            {
                e_defend[e_defend_count++] = &defenders[ii];
            }
        }

        // pick out the front-line attacking units
        for(uint32_t ii=0; ii<attacker_count; ++ii)
        {
            if(brigade_is_frontline(&attackers[ii]))
            {
                e_attack[e_attack_count++] = &attackers[ii];
            }
        }

        printf("Combat hour %u\n", hour);

        // Check for end of combat conditions
        // TODO: implement over-running a division / attacking support divisions
        if(e_attack_count == 0)
        {
            printf("The attacker has run out of brigades.\n");
            break;
        }

        if(e_defend_count == 0)
        {
            printf("The defender has run out of brigades.\n");
            break;
        }

        // the defender will attack first
        for(uint32_t ii=0; ii<defender_count; ++ii)
        {
            // disorganized brigades can not attack
            if(defenders[ii].organization == 0)
            {
                printf("The defending %s %s brigade is too disorganized to attack.\n", 
                        ordinal_number(defenders[ii].id),
                        brigade_get_type_name(defenders[ii].type)
                );
            }
            else // the brigade can attack
            {
                Brigade *victim = NULL;
                // choose a victim
                // TODO: eventually this needs to try to match up target units to weapon types
                // e.g. AT brigades should prioritize tanks. Leaders and tactics should affect
                // this as well.
                if(brigade_is_frontline(&defenders[ii]))
                {
                    // front line divisions can only target other front line divisions
                    uint32_t victim_id = randomgen_get_range_uint(&random, 0, e_attack_count);
                    victim = e_attack[victim_id];
                }
                else
                {
                    uint32_t victim_id = randomgen_get_range_uint(&random, 0, attacker_count);
                    victim = &attackers[victim_id];
                }
                
                printf("Defender %u is attacking attacker %u\n", ii, victim->id);

                // compute hits and apply them
                // compute linear density of victim unit
                uint32_t frontage = territory->width / e_attack_count;
                uint32_t density  = victim->troops / frontage; 
                printf("Attacker has a linear density of %u troops / km\n", density);

                // fire primary weapon
                uint32_t e_frontage = 1000 * frontage;
                uint32_t hits = 0;
                for(uint32_t shot=0; shot<defenders[ii].troops;++shot)
                {
                    
                }
            }
        }
        
        break;
    }
}
