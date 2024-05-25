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

std::mt19937 mt{ static_cast<unsigned int>(std::chrono::high_resolution_clock::now().time_since_epoch().count()) };
const int outOfMovesFlag{ 500 };

class Attack
{
public:
    Attack(const std::string& name, int basePower, int level, int attackUses, int  maxAttackUses)
        : mName(name), mBasePower(basePower), mLevel(level), mAttackUses(attackUses), mMaxAttackUses(maxAttackUses)
    {}
    Attack() = default;

    std::string GetName() const
    {
        return mName;
    }

    int GetBasePower() const
    {
        return mBasePower;
    }

    int GetLevel() const
    {
        return mLevel;
    }

    int GetMaxLevel() const
    {
        return mMaxLevel;
    }

    int GetDamage(int characterLevel) const
    {
        return (mLevel * characterLevel) * mBasePower;
    }

    void IncreaseLevel()
    {
        if (mLevel >= mMaxLevel)
        {
            mLevel = mMaxLevel;
        }
        else
        {
            ++mLevel;
        }
    }

    void SetLevel(int setLevelTo)
    {
        if (setLevelTo > 0)
        {
            mLevel = setLevelTo;

            if (mLevel > mMaxLevel)
            {
                mLevel = mMaxLevel;
            }
        }
    }

    bool IsMaxLevel() const
    {
        if (mLevel == mMaxLevel)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    int GetAttackUses() const
    {
        return mAttackUses;
    }

    int GetMaxAttackUses() const
    {
        return mMaxAttackUses;
    }

    void IncrementAttackUses()
    {
        if (mAttackUses >= mMaxAttackUses)
        {
            mAttackUses = mMaxAttackUses;
        }
        else
        {
            ++mAttackUses;
        }
    }

    void DecrementAttackUses()
    {
        if (mAttackUses > 0)
        {
            --mAttackUses;
        }
    }

    void SetAttackUses(int setAttackUses)
    {
        if (setAttackUses > 0)
        {
            if (setAttackUses > mMaxAttackUses)
            {
                mAttackUses = mMaxAttackUses;
            }
            else
            {
                mAttackUses = setAttackUses;
            }
        }
        else
        {
            mAttackUses = 0;
        }
    }

    void ResetAttackUses()
    {
        mAttackUses = mMaxAttackUses;
    }

private:
    std::string mName{ "" };
    int mBasePower{ 0 };
    int mLevel{ 1 };
    int mAttackUses{ 0 };
    int mMaxAttackUses{ 0 };

    static const int mMaxLevel{ 10 };
};

class Character
{
    public:
        Character(const std::string& name, int health, int level, int money)
            : mName(name), mHealth(health), mLevel(level), mMoney(money)
        {}

        //Done
        std::string GetName() const
        {
            return mName;
        }

        //Done
        int GetHealth() const
        {
            return mHealth;
        }

        //Done
        int GetLevel() const
        {
            return mLevel;
        }

        //Done
        int GetMoney() const
        {
            return mMoney;
        }

        //Done
        int GetMaxLevel() const
        {
            return mMaxLevel;
        }

        //Done
        int GetExperience() const
        {
            return mExperience;
        }

        //TODO
        //Add error handling for multiple attacks of the same name
        void LearnAttack(const Attack& attackToLearn)
        {
            mAttacks.push_back(attackToLearn);
        }

        std::vector<Attack>& GetAttacks() // Return by reference
        {
            return mAttacks;
        }

        const std::vector<Attack>& GetAttacks() const // Const version for const objects
        {
            return mAttacks;
        }

        //Done
        bool IsMaxLevel() const
        {
            if (mLevel == mMaxLevel)
            {
                return true;
            }
            else
            {
                return false;
            }
        }

        //Done
        void GiveExperience(int amount)
        {
            mExperience += amount;

            // Cap the experience to a maximum value
            if (mExperience > mMaxExperience)
            {
                mExperience = mMaxExperience;
            }

            // Level up as much as possible
            while (mExperience >= mExperienceMultiplier && mLevel < mMaxLevel)
            {
                ++mLevel;
                mExperience -= mExperienceMultiplier;
            }

            // Cap the level to a maximum value
            if (mLevel > mMaxLevel)
            {
                mLevel = mMaxLevel;
            }
        }

        //Done
        void AddMoney(int amount)
        {
            if (amount > 0)
            {
                mMoney += amount;

                if (mMoney >= mMaxMoney)
                {
                    mMoney = mMaxMoney;
                }
            }
        }

        void SubtractMoney(int amount)
        {
            if (amount > 0 && mMoney >= amount)
            {
                mMoney -= amount;
            }
        }

        
        void SetMoney(int amount)
        {
            mMoney = amount;

            if (amount >= 0 && amount <= mMaxMoney)
            {
                mMoney = amount;
            }
        }

        //Done
        int CalculateExperience() const
        {
            return 500 * mLevel;
        }

        //Done
        int GetMaxExperience() const
        {
            return mMaxExperience;
        }

        /**
         * @brief Checks if the character is alive.
         *
         * This function checks if the character is still alive based on the character's current health.
         *
         * @return true if the character's health is greater than 0, false otherwise.
         */
        bool IsAlive() const
        {
            if (mHealth > 0)
            {
                return true;
            }
            else
            {
                return false;
            }
        }

        /**
         * @brief Adds health to the character
         *
         * This function handles adding health to characters
         *
         * @param amount The amount of health to subtract
         */
        void AddHealth(int amount)
        {
            if (amount > 0)
            {
                mHealth += amount;

                if (mHealth >= mMaxHealth)
                {
                    mHealth = mMaxHealth;
                }
            } 
        }

        /**
         * @brief Subtracts health from the character
         *
         * This function handles subtracting health from the character. It ensures that the health 
         * will not go into negative numbers.
         *
         * @param amount The amount of health to subtract
         */
        void SubtractHealth(int amount)
        {
            if (amount > 0)
            {
                mHealth -= amount;

                if (mHealth < 0)
                {
                    mHealth = 0;
                }
            }
        }

    private:
        std::string mName{ "" };
        int mHealth{ 0 };
        int mLevel{ 1 };
        int mMoney{ 0 };
        int mExperience{ 0 };
        std::vector<Attack> mAttacks{};

        static const int mMaxLevel{ 100 };
        static const int mMaxHealth{ 100 };
        static const int mExperienceMultiplier{ 500 };
        static const int mMaxExperience{ mExperienceMultiplier * mMaxLevel };
        static const int mMaxMoney{ 99999999 };
};

void GetCharacterInfo(const Character& player);
void Test(Character& player);
void PurchaseUpgradesMenu(Character& player);
void PurchaseAttackUpgrades(Character& player);
void Arena(Character& player, std::vector<Character*>& enemyList);
bool ForfeitMatch(Character& player);
int ChooseAttack(Character& player);
bool Battle(Character& player, std::vector<Character*>& enemyList, int randomlyChosenEnemy);
void Save(const Character& player);
void Load(Character& player);

int main()
{
    Character player("Hero", 100, 1, 0);

    Character goblin("Goblin", 30, 1, 0);
    Character orc("Orc", 42, 1, 0);
    Character dragon("Dragon", 500, 1, 0);

    std::vector<Character*> enemyList{ &goblin, &orc, &dragon };

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


    //TEST
    player.AddMoney(5000);
    player.SubtractHealth(37);
    player.GiveExperience(17987);

    for (const auto& enemy : enemyList)
    {
        std::cout << enemy->GetName() << " has learned the following attacks:\n";
        for (const auto& attack : enemy->GetAttacks())
        {
            std::cout << "- " << attack.GetName() << '\n';
        }
        std::cout << '\n';
    }
    //END TEST

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

void Arena(Character& player, std::vector<Character*>& enemyList)
{
    int enemyIndex{ static_cast<int>(enemyList.size() - 1) };

    std::uniform_int_distribution<int> randomizeEnemy{ 0, enemyIndex };
    int randomlyChosenEnemy{ randomizeEnemy(mt) };

    std::cout << "Welcome to the arena! this is your opponent:\n";
    std::cout << "A level " << enemyList[randomlyChosenEnemy]->GetLevel()  << " " << enemyList[randomlyChosenEnemy]->GetName() << " with " << enemyList[randomlyChosenEnemy]->GetHealth() << " HP!\n\n";

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
bool Battle(Character& player, std::vector<Character*>& enemyList, int randomlyChosenEnemy)
{
    //Write an if statement to make sure this is valid
    //Due to the new way characters use attacks, this will need to be solved as to how an attack is  used.
    std::cout << "STOP ONE\n\n";

    if (enemyList[randomlyChosenEnemy]->GetAttacks().empty())
    {
        std::cerr << "Enemy has no attacks.\n\n";
        return true;
    }
    std::uniform_int_distribution<int> randomizedAttack{ 0, static_cast<int>(enemyList[randomlyChosenEnemy]->GetAttacks().size() - 1)};

    int randomlyChosenAttack{ randomizedAttack(mt) };

    std::uniform_int_distribution<int> randomPrizeMoney{ 10, 50 };
    std::uniform_int_distribution<int> randomExperience{ 20, 75 };

    int experienceReward{ randomExperience(mt) };

    int playerAttackChoice{ 0 };

    //CRASH
    std::cout << "STOP TWO\n\n";
    playerAttackChoice = ChooseAttack(player);
    std::cout << "STOP THREE\n\n";

    if (playerAttackChoice == ::outOfMovesFlag)
    {
        return true;
    }
    else
    {
        std::cout << player.GetName() << "s HP: " << player.GetHealth() << '\n';
        std::cout << enemyList[randomlyChosenEnemy]->GetName() << "s HP: " << enemyList[randomlyChosenEnemy]->GetHealth() << "\n\n";

        if (player.IsAlive() == true && enemyList[randomlyChosenEnemy]->IsAlive() == true)
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
                enemyList[randomlyChosenEnemy]->SubtractHealth(player.GetAttacks()[playerAttackChoice - 1].GetDamage(player.GetLevel()));

                player.GetAttacks()[playerAttackChoice - 1].DecrementAttackUses();

                //Enemies turn
                std::cout << "The " << enemyList[randomlyChosenEnemy]->GetName() << " used " << enemyList[randomlyChosenEnemy]->GetAttacks()[randomlyChosenAttack].GetName() << "!\n";

                if (missAttackChance(mt) == 5)
                {
                    std::cout << enemyList[randomlyChosenEnemy]->GetName() << "s attack missed!\n\n";
                }
                else
                {
                    std::cout << "It did " << player.GetAttacks()[randomlyChosenAttack].GetDamage(player.GetLevel()) << " damage\n\n";
                    player.SubtractHealth(player.GetAttacks()[randomlyChosenAttack].GetDamage(player.GetLevel()));
                }
            }
        }
        else
        {
            if (player.IsAlive() == false && enemyList[randomlyChosenEnemy]->IsAlive() == true)
            {
                std::cout << "You died!\n\n";

                return true;
            }
            else if (enemyList[randomlyChosenEnemy]->IsAlive() == false && player.IsAlive() == true)
            {
                int prizeMoney{ randomPrizeMoney(mt) };

                std::cout << "You defeated the " << enemyList[randomlyChosenEnemy]->GetName() << "!\n";
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
            else if (player.IsAlive() == false && enemyList[randomlyChosenEnemy]->IsAlive() == false)
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
    player.AddMoney(5000);
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

    for (int attackLevel{0}; auto& attack : player.GetAttacks())
    {
        load >> attackLevel;
        attack.SetLevel(attackLevel);
        attackLevel = 0;
    }

}