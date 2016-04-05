#include "war.h"

int main()
{
    Brigade *usa[4];
    Brigade *ger[3];
    
    usa[0] = (Brigade*)malloc(sizeof(Brigade));
    usa[1] = (Brigade*)malloc(sizeof(Brigade));
    usa[2] = (Brigade*)malloc(sizeof(Brigade));
    usa[3] = (Brigade*)malloc(sizeof(Brigade));

    ger[0] = (Brigade*)malloc(sizeof(Brigade));
    ger[1] = (Brigade*)malloc(sizeof(Brigade));
    ger[2] = (Brigade*)malloc(sizeof(Brigade));

    brigade_create(usa[0], brigade_infantry);
    brigade_create(usa[1], brigade_infantry);
    brigade_create(usa[2], brigade_artillery);
    brigade_create(usa[3], brigade_artillery);
    
    brigade_create(ger[0], brigade_infantry);
    brigade_create(ger[1], brigade_infantry);
    brigade_create(ger[2], brigade_infantry);

    // Battle: Liberation of Nancy
    // the germans are defending near Atton
    // it is late in the war so the german units have lower morale
    ger[0]->troops *= 0.94;
    ger[1]->troops *= 0.94;
    ger[0]->morale = 50;
    ger[1]->morale = 60;
    
    ger[0]->tactic = tactic_defend_delay;
    ger[1]->tactic = tactic_defend_delay;
    ger[2]->tactic = tactic_defend_delay;
    
    usa[0]->tactic = tactic_attack_assault;
    usa[1]->tactic = tactic_attack_assault;
    usa[2]->tactic = tactic_attack_assault;
    usa[3]->tactic = tactic_attack_assault;

    printf("United States 1st Infantry Division:\n");
    brigade_debug(usa[0]);
    printf("\n");
    brigade_debug(usa[1]);
    printf("\n");
    brigade_debug(usa[2]);
    printf("\n");
    brigade_debug(usa[3]);
    printf("\n");

    printf("German 1st Infantry Division:\n");
    brigade_debug(ger[0]);
    printf("\n");
    brigade_debug(ger[1]);
    printf("\n");
    brigade_debug(ger[2]);
    printf("\n");

    Territory plains;
    plains.id = 0;
    plains.terrain = terrain_plains;
    plains.width = 5;
    plains.name = strdup("Atton");


    RandomGen random;
    randomgen_create(&random, 31415);

    CombatInfo info;
    info.territory      = &plains;
    info.attackers      = usa;
    info.attacker_count = 4;
    info.defenders      = ger;
    info.defender_count = 3;
    info.generator      = &random;
    info.hour           = 0;

    combat(&info);
    
    return 0;
}
