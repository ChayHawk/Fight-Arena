#include <iostream>
#include <fstream>

#include "Character.h"


void Load(Character& player)
{
    std::ifstream load("savegame.txt");

    if (!load)
    {
        std::cerr << "Unable to open load file!\n";
        return;
    }

    std::string name{ };
    int money{ };
    int health{ };
    int experience{ };
    int level{ };

    std::getline(load, name);
    load >> health;
    load >> level;
    load >> money;
    load >> experience;

    player = Character(name, health, level, money);
    player.GiveExperience(experience);

    //TODO - TEST
    //This needs to be tested
    for (int attackLevel{ 0 }; auto & attack : player.GetAttacks())
    {
        load >> attackLevel;
        attack.SetLevel(attackLevel);
        attackLevel = 0;
    }

}