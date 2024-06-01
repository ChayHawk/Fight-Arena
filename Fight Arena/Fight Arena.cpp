#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <random>
#include <format>
#include <cassert>
#include <limits>
#include <algorithm>
#include <fstream>
#include <memory>

#include "Attack.h"
#include "Character.h"

std::mt19937 mt{ static_cast<unsigned int>(std::chrono::high_resolution_clock::now().time_since_epoch().count()) };
const int outOfMovesFlag{ 500 };

void GetCharacterInfo(const Character& player);
void Test(Character& player);
void PurchaseUpgradesMenu(Character& player);
void PurchaseAttackUpgrades(Character& player);
void Arena(Character& player, std::vector<Character>& enemyList);
bool ForfeitMatch(Character& player);
int ChooseAttack(Character& player);
bool Battle(Character& player, std::vector<Character>& enemyList, int randomlyChosenEnemy);
void Save(const Character& player);
void Load(Character& player);

int main()
{
    Character player("Hero", 100, 1, 0);

    Character goblin("Goblin", 30, 1, 0);
    Character orc("Orc", 42, 1, 0);
    Character dragon("Dragon", 500, 1, 0);

    std::vector<Character> enemyList{ goblin, orc, dragon };

    Attack slash("Slash", 4, 1, 1, 20);
    Attack scratch("Scratch", 1, 1, 25, 25);
    Attack fireBlast("Fire Blast", 12, 1, 15, 15);
    Attack stomp("Stomp", 7, 1, 25, 25);
    Attack orcSword("Orc Sword", 9, 1, 13, 13);
    Attack punch("Punch", 3, 1, 20, 20);
    Attack kick("Kick", 5, 1, 15,  15);
    Attack rifle("Assault Rifle", 15, 1, 30, 30);
    Attack handgun("Handgun", 10, 1, 8, 36);

    goblin.LearnAttack(stomp);
    goblin.LearnAttack(scratch);
    goblin.LearnAttack(punch);
    
    orc.LearnAttack(orcSword);
    orc.LearnAttack(punch);
    orc.LearnAttack(kick);

    dragon.LearnAttack(slash);
    dragon.LearnAttack(fireBlast);
    dragon.LearnAttack(stomp);

    player.LearnAttack(rifle);
    player.LearnAttack(handgun);

    bool isTesting{ false };

    if (isTesting == true)
    {
        Test(player);
    }
    else
    {
        bool isGameEnded{ false };

        while (!isGameEnded)
        {
            std::cout << "What would you like to do?\n\n";

            std::cout << "1) Purchase Upgrades\n";
            std::cout << "2) View Character Info\n";
            std::cout << "3) Fight in Arena\n";
            std::cout << "4) Save\n";
            std::cout << "5) Load\n";
            std::cout << "6) Exit\n\n";

            int choice{ 0 };

            //TODO
            // 
            //This code does not catch characters that are entered. Need to make a function 
            //that handles all input.
            std::cin >> choice;

            if (choice > 6)
            {
                std::cout << "Invalid Choice\n";
            }
            else
            {
                switch (choice)
                {
                case 1:
                    PurchaseUpgradesMenu(player);
                    break;

                case 2:
                    GetCharacterInfo(player);
                    break;

                case 3:
                {
                    if (player.GetAttacks().size() == 0)
                    {
                        std::cerr << "You dont have any attacks\n\n";
                        break;
                    }
                    else
                    {
                        Arena(player, enemyList);
                    }
                }
                    break;

                case 4:
                    Save(player);
                    std::cout << "Game Saved\n\n";
                    break;

                case 5:
                    Load(player);
                    std::cout << "Game Loaded\n\n";
                    break;

                case 6:
                    isGameEnded = true;
                    break;

                default:
                    std::cout << "Incorrect selection\n\n";
                }
            }
        }
    }
}

void Test(Character& player)
{
    std::cout << player.GetAttacks()[0].GetName() << " -Attack Uses: " << player.GetAttacks()[0].GetAttackUses() << "/" << player.GetAttacks()[0].GetMaxAttackUses() << '\n';

    for (int i = 0; i < 40; ++i)
    {
        player.GetAttacks()[0].SetAttackUses(100);
    }

    std::cout << player.GetAttacks()[0].GetName() << " -Attack Uses: " << player.GetAttacks()[0].GetAttackUses() << "/" << player.GetAttacks()[0].GetMaxAttackUses() << '\n';
}

void Arena(Character& player, std::vector<Character>& enemyList)
{
    int enemyIndex{ static_cast<int>(enemyList.size() - 1) };

    std::uniform_int_distribution<int> randomizeEnemy{ 0, enemyIndex };
    int randomlyChosenEnemy{ randomizeEnemy(mt) };

    std::cout << "Welcome to the arena! this is your opponent:\n";
    std::cout << "A level " << enemyList[randomlyChosenEnemy].GetLevel()  << " " << enemyList[randomlyChosenEnemy].GetName() << " with " << enemyList[randomlyChosenEnemy].GetHealth() << " HP!\n\n";

    int choice{ 0 };
    bool endMatch{ false };

    while(endMatch != true)
    {
        std::cout << "What would you like to do?\n\n";

        std::cout << "1) Attack\n";
        std::cout << "2) Forfeit Match\n";

        int choice{ 0 };

        std::cin >> choice;

        switch (choice)
        {
            case 1:
            {
                endMatch = Battle(player, enemyList, randomlyChosenEnemy);
            }
            break;

            case 2:
                endMatch = ForfeitMatch(player);
                break;

            default:
                std::cout << "Incorrect choice\n\n";
        }
    }
}

/**
* @version 1.0
* @date 5/22/2024 @ 12:37 PM
* @brief Choose an attack to use
*
* This function is where the player will choose their attack to use against the enemy.
*
* @param character The player's character performing the attack.
* @param attackList A list of possible attacks the player's character can use.
* @return Returns the players attack choice
* @warning This function does not have user input handling!
*/
int ChooseAttack(Character& player)
{
    bool hasChosenAttack{ false };

    while (hasChosenAttack != true)
    {
        auto lambda = [](const Attack& attack) { return attack.GetAttackUses() == 0; };

        if (std::all_of(player.GetAttacks().begin(), player.GetAttacks().end(), lambda))
        {
            std::cout << "You're all out of moves, this match is over!\n\n";
            
            for (auto& attack : player.GetAttacks())
            {
                attack.ResetAttackUses();
            }

            return ::outOfMovesFlag;
        }
        else
        {
            std::cout << "Choose  an attack to use:\n\n";

            for (int counter{ 1 }; const auto & attack : player.GetAttacks())
            {
                std::cout << counter++ << "). " << attack.GetName() << '\n';
                std::cout << std::format("{:>5}", "") << " -Power: " << attack.GetDamage(player.GetLevel()) << '\n';
                std::cout << std::format("{:>5}", "") << " -Level: " << attack.GetLevel() << '\n';
                std::cout << std::format("{:>5}", "") << " -Moves: " << attack.GetAttackUses() << '/' << attack.GetMaxAttackUses() << '\n';
            }

            int choice{ 0 };

            std::cin >> choice;

            if (choice > 0 && choice <= static_cast<int>(player.GetAttacks().size()))
            {
                if (player.GetAttacks()[choice - 1].GetAttackUses() == 0)
                {
                    std::cout << "That attack has no moves left! Choose something else!\n";
                }
                else
                {
                    return choice;
                }
            }
            else
            {
                hasChosenAttack = false;
            }
        }
    }

    return ::outOfMovesFlag;
}


/**
* @brief Battle enemies here
*
* This function is where the player battles enemies to earn money to buy upgrades with.
*
* @param character The player's character performing the attack.
* @param attackList A list of possible attacks the player's character can use.
* @param enemyList A list of enemies for the player to battle
* @param randomlyChosenEnemy A value containing a random number
* @details The 'randomlyChosenEnemy' variable is set each time the player is about to battle, 
* this ensures the player gets a random enemy each time.
* @return Returns a value that says if the player or enemy have died
* @warning This function does not have user input handling!
*/
bool Battle(Character& player, std::vector<Character>& enemyList, int randomlyChosenEnemy)
{
    if (enemyList[randomlyChosenEnemy].GetAttacks().empty())
    {
        std::cerr << "Enemy has no attacks.\n\n";
        return true;
    }
    std::uniform_int_distribution<int> randomizedAttack{ 0, static_cast<int>(enemyList[randomlyChosenEnemy].GetAttacks().size() - 1)};

    int randomlyChosenAttack{ randomizedAttack(mt) };

    std::uniform_int_distribution<int> randomPrizeMoney{ 10, 50 };
    std::uniform_int_distribution<int> randomExperience{ 20, 75 };

    int experienceReward{ randomExperience(mt) };

    int playerAttackChoice{ 0 };

    playerAttackChoice = ChooseAttack(player);

    if (playerAttackChoice == ::outOfMovesFlag)
    {
        return true;
    }
    else
    {
        std::cout << player.GetName() << "s HP: " << player.GetHealth() << '\n';
        std::cout << enemyList[randomlyChosenEnemy].GetName() << "s HP: " << enemyList[randomlyChosenEnemy].GetHealth() << "\n\n";

        if (player.IsAlive() == true && enemyList[randomlyChosenEnemy].IsAlive() == true)
        {
            auto lambda = [](const Attack& attack) { return attack.GetAttackUses() == 0; };

            std::uniform_int_distribution<int> missAttackChance{ 0, 5 };

            //Check if players attack missed
            if (missAttackChance(mt) == 5)
            {
                std::cout << "Your attack missed!\n";
            }
            else
            {
                std::cout << player.GetName() << " used " << player.GetAttacks()[playerAttackChoice - 1].GetName() << "!\n";
                std::cout << "It did " << player.GetAttacks()[playerAttackChoice - 1].GetDamage(player.GetLevel()) << " damage!\n\n";
                enemyList[randomlyChosenEnemy].SubtractHealth(player.GetAttacks()[playerAttackChoice - 1].GetDamage(player.GetLevel()));

                player.GetAttacks()[playerAttackChoice - 1].DecrementAttackUses();

                //Enemies turn
                std::cout << "The " << enemyList[randomlyChosenEnemy].GetName() << " used " << enemyList[randomlyChosenEnemy].GetAttacks()[randomlyChosenAttack].GetName() << "!\n";

                if (missAttackChance(mt) == 5)
                {
                    std::cout << enemyList[randomlyChosenEnemy].GetName() << "s attack missed!\n\n";
                }
                else
                {
                    std::cout << "It did " << enemyList[randomlyChosenEnemy].GetAttacks()[randomlyChosenAttack].GetDamage(player.GetLevel()) << " damage\n\n";
                    player.SubtractHealth(enemyList[randomlyChosenEnemy].GetAttacks()[randomlyChosenAttack].GetDamage(player.GetLevel()));
                }
            }
        }
        else
        {
            if (player.IsAlive() == false && enemyList[randomlyChosenEnemy].IsAlive() == true)
            {
                std::cout << "You died!\n\n";

                return true;
            }
            else if (enemyList[randomlyChosenEnemy].IsAlive() == false && player.IsAlive() == true)
            {
                int prizeMoney{ randomPrizeMoney(mt) };

                std::cout << "You defeated the " << enemyList[randomlyChosenEnemy].GetName() << "!\n";
                std::cout << "You won $" << prizeMoney << " as prize money.\n";

                player.AddMoney(prizeMoney);

                std::cout << "You gained " << experienceReward << " experience.\n\n";

                player.GiveExperience(experienceReward);

                for (auto& attack : player.GetAttacks())
                {
                    attack.ResetAttackUses();
                }

                return true;
            }
            else if (player.IsAlive() == false && enemyList[randomlyChosenEnemy].IsAlive() == false)
            {
                std::cout << "You both killed each other at the same time, it's a draw! what a show!\n\n";

                return true;
            }
        }
    }
}

/**
 * @brief Forfeit the match
 *
 * Causes the match to end at the players request
 *
 * @param character The player's character performing the attack.
 * 
 * @return Returns true if the player wants to end the match and false if they change their mind
 */
bool ForfeitMatch(Character& player)
{
    std::cout << "Are you sure? It will cost $100 to forfeit the match, and you have $" << player.GetMoney() << '\n';
    std::cout << "1) Yes\n";
    std::cout << "2) No\n";

    int choice{ 0 };

    std::cin >> choice;

    if (choice == 1)
    {
        std::cout << "You forfeited the match, you lost $";

        if (player.GetMoney() >= 100)
        {
            player.SubtractMoney(100);
            std::cout << "100\n\n";
        }
        else
        {
            std::cout << player.GetMoney() << "\n\n";
            player.SubtractMoney(player.GetMoney());
        }

        return true;
    }
    else if (choice == 2)
    {
        return false;
    }
}

/**
 * @brief Display character infomration
 *
 * Displays relevant character information like health, level, money etc.
 *
 * @param character The player's character performing the attack.
 * @param attackList A list of possible attacks the player's character can use.
 */
void GetCharacterInfo(const Character& player)
{
    std::cout << player.GetName() << '\n';
    std::cout << std::format("{:>4}", "") << "-HP: " << player.GetHealth() << '\n';
    std::cout << std::format("{:>4}", "") << "-Level: " << player.GetLevel() << '\n';
    std::cout << std::format("{:>4}", "") << "-$" << player.GetMoney() << '\n';

    std::cout << "\nAttacks\n";
    for (const auto& attack : player.GetAttacks())
    {
        std::cout << std::format("{:>4}", "") << attack.GetName() << '\n';
        std::cout << std::format("{:>8}", "") << "-Base Power: " << attack.GetBasePower() << '\n';
        std::cout << std::format("{:>8}", "") << "-Attack Power: " << attack.GetDamage(player.GetLevel()) << "\n";
        std::cout << std::format("{:>8}", "") << "-Level: " << attack.GetLevel() << "\n";
        std::cout << std::format("{:>8}", "") << "-Max Moves: " << attack.GetMaxAttackUses() << "\n\n";
    }
}

/**
 * @brief Select what you want to upgrade
 *
 * This function allows the player to choose what they want to upgrade.
 *
 * @param character The player's character performing the attack.
 * @param attackList A list of possible attacks the player's character can use.
 */
void PurchaseUpgradesMenu(Character& player)
{
    std::cout << "What would you like to purchase upgrades for?\n\n";

    std::cout << "1) Attacks\n";
    std::cout << "2) Return\n";

    int choice{ 0 };

    std::cin >> choice;

    switch (choice)
    {
        case 1:
            PurchaseAttackUpgrades(player);
            break;

        case 2:
            return;

        default:
            std::cout << "Incorrect choice\n";
    }
}

/**
 * @brief Purchase upgrades
 *
 * This function allows the player to purchase upgrades for their character and for their attacks by using
 * money earned from fighting in the arena.
 *
 * @param character The player's character performing the attack.
 * @param attackList A list of possible attacks the player's character can use.
 */
void PurchaseAttackUpgrades(Character& player)
{
    std::cout << "\nWhich attack would you like to upgrade?\n\n";
    std::cout << "Cash $" << player.GetMoney() << "\n\n";

    int attackListIndex{ 0 };

    if (player.GetAttacks().size() == 0)
    {
        std::cout << "You have no attacks to upgrade!\n\n";
        return;
    }

    for (int counter{ 1 }; const auto & attack : player.GetAttacks())
    {
        std::cout << counter++ << ") " << attack.GetName() << " -Level: " << attack.GetLevel() << " -Upgrade Cost $" << 500 * player.GetAttacks()[attackListIndex++].GetLevel() << '\n';
    }

    std::cout << '\n';

    int choice{ 0 };

    std::cin >> choice;
    
    int upgradeStep{ 0 };
    auto attackToUpgrade{ 0 };

    if (choice <= player.GetAttacks().size())
    {
        upgradeStep = 500 * player.GetAttacks()[choice - 1].GetLevel();
        attackToUpgrade = player.GetAttacks()[choice - 1].GetLevel();

        if (player.GetMoney() >= upgradeStep)
        {
            std::cout << player.GetAttacks()[choice - 1].GetName() << " will cost " << upgradeStep << " to upgrade to level " << player.GetAttacks()[choice - 1].GetLevel() + 1 << ".\n";
            std::cout << "You currently have $" << player.GetMoney() << " continue with upgrade?\n";

            int choice{ 0 };

            std::cout << "1) Yes\n";
            std::cout << "2) No\n";

            std::cin >> choice;

            if (choice == 1)
            {
                player.GetAttacks()[choice - 1].IncreaseLevel();
                player.SubtractMoney(upgradeStep);

                std::cout << "You currently have $" << player.GetMoney() << " and your " << player.GetAttacks()[choice - 1].GetName()
                    << " attack is at level " << player.GetAttacks()[choice - 1].GetLevel() << "!\n";
            }
            else if (choice == 2)
            {
                std::cout << "Come back anytime\n\n";
                return;
            }
            else
            {
                std::cout << "Huh?\n";
            }
        }
        else
        {
            std::cout << "It seems you do not posses enough funds to perform this upgrade.\n";
        }
    }
    else
    {
        std::cout << "It doesnt seem like that was a valid choice, please choose again.\n\n";
    }
}