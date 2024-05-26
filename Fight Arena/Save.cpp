#include <iostream>
#include <fstream>

#include "Character.h"

/**
 * @brief Save the players progress
 *
 * This function is responsible for saving all of the players progress.
 *
 * @param character The player's character performing the attack.
 * @param attackList A list of possible attacks the player's character can use.
 *
 * @todo Implement saving of player attack levels
 */
void Save(const Character& player)
{
    std::ofstream save("savegame.txt");

    if (!save)
    {
        std::cerr << "Unable to open save file!\n";
        return;
    }

    save << player.GetName() << '\n';
    save << player.GetHealth() << '\n';
    save << player.GetLevel() << '\n';
    save << player.GetMoney() << '\n';
    save << player.GetExperience() << '\n';

    for (const auto& attack : player.GetAttacks())
    {
        save << attack.GetLevel() << '\n';
    }
}