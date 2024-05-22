#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <random>
#include <format>
#include <cassert>
#include <limits>

std::mt19937 mt{ static_cast<unsigned int>(std::chrono::high_resolution_clock::now().time_since_epoch().count()) };

class Attack
{
public:
    Attack(const std::string& name, int basePower, int level)
        : mName(name), mBasePower(basePower), mLevel(level)
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

private:
    std::string mName{ "" };
    int mBasePower{ 0 };
    int mLevel{ 1 };

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

        static const int mMaxLevel{ 100 };
        static const int mMaxHealth{ 100 };
        static const int mExperienceMultiplier{ 500 };
        static const int mMaxExperience{ mExperienceMultiplier * mMaxLevel };
        static const int mMaxMoney{ 99999999 };
};

void GetCharacterInfo(const Character& character, const std::vector<Attack>& attackList);
void Test(Character& character, std::vector<Attack>& attackList);
void PurchaseUpgradesMenu(Character& character, std::vector<Attack>& attackList);
void PurchaseAttackUpgrades(Character& character, std::vector<Attack>& attackList);
void Arena(Character& character, const std::vector<Attack>& attackList, std::vector<Character>& enemyList);
bool ForfeitMatch(Character& character);
int ChooseAttack(const Character& character, const std::vector<Attack>& attackList);
bool Battle(Character& character, const std::vector<Attack>& attackList, std::vector<Character>& enemyList, int randomlyChosenEnemy);

int main()
{
    Character player("Hero", 100, 1, 0);

    Character goblin("Goblin", 30, 1, 0);
    Character orc("Orc", 42, 1, 0);
    Character giant("Giant", 103, 1, 0);
    Character werewolf("Werewolf", 92, 1, 0);

    std::vector<Character> enemyList{ goblin, orc, giant, werewolf };

    Attack slash("Slash", 4, 1);
    Attack scratch("Scratch", 1, 1);
    Attack fireBlast("Fire Blast", 12, 1);
    Attack stomp("Stomp", 7, 1);

    std::vector<Attack> attackList{ slash, scratch, fireBlast, stomp };

    bool isTesting{ false };

    if (isTesting == true)
    {
        Test(player, attackList);
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
            std::cout << "4) Exit\n\n";

            int choice{ 0 };

            //TODO
            // 
            //This code does not catch characters that are entered. Need to make a function 
            //that handles all input.
            std::cin >> choice;

            if (choice > 4)
            {
                std::cout << "Invalid Choice\n";
            }
            else
            {
                switch (choice)
                {
                case 1:
                    PurchaseUpgradesMenu(player, attackList);
                    break;

                case 2:
                    GetCharacterInfo(player, attackList);
                    break;

                case 3:
                    Arena(player, attackList, enemyList);
                    break;

                case 4:
                    isGameEnded = true;
                    break;

                default:
                    std::cout << "Incorrect selection\n\n";
                }
            }
        }
    }
}

void Test(Character& character, std::vector<Attack>& attackList)
{
    std::cout << "Health: " << character.GetHealth() << std::boolalpha <<" - Is Alive?: " << character.IsAlive() << '\n';

    character.SubtractHealth(2);
    character.SubtractHealth(1);

    std::cout << "Health: " << character.GetHealth() << std::boolalpha << " - Is Alive?: " << character.IsAlive() << '\n';
}

void Arena(Character& character, const std::vector<Attack>& attackList, std::vector<Character>& enemyList)
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
                endMatch = Battle(character, attackList, enemyList, randomlyChosenEnemy);
            }
            break;

            case 2:
                endMatch = ForfeitMatch(character);
                break;

            default:
                std::cout << "Incorrect choice\n\n";
        }
    }
}

int ChooseAttack(const Character& character, const std::vector<Attack>& attackList)
{
    bool hasChosenAttack{ false };

    while (hasChosenAttack != true)
    {
        std::cout << "Choose  an attack to use:\n\n";

        for (int counter{ 1 }; const auto & attack : attackList)
        {
            std::cout << counter++ << "). " << attack.GetName() << '\n';
            std::cout << std::format("{:>5}", "") << " -PWR: " << attack.GetDamage(character.GetLevel()) << '\n';
            std::cout << std::format("{:>5}", "") << " -LVL: " << attack.GetLevel() << '\n';
        }

        int choice{ 0 };

        std::cin >> choice;

        //Add error handling for incorrect choices and make it so when the user selects a right choice the while loop ends
        //I believe the return statement will forcibly return a value and end the while loop so a true false to end the loop
        //May not be needed.

        if (choice <= attackList.size())
        {
            return choice;
        }
        else
        {
            hasChosenAttack = false;
        }
    }
}


/**
* @brief Battle enemies here
*
* This function is where the player battles enemies to earn money to buy upgrades with.
*
* @param character
* @param attackList
* @param enemyList
* @param randomlyChosenEnemy
* @return Returns a value that says if the player or enemy have died
*/
bool Battle(Character& character, const std::vector<Attack>& attackList, std::vector<Character>& enemyList, int randomlyChosenEnemy)
{
    std::uniform_int_distribution<int> missAttackChance{ 0, 5 };

    std::uniform_int_distribution<int> randomizedAttack{ 0, static_cast<int>(attackList.size() - 1) };
    int randomlyChosenAttack{ randomizedAttack(mt) };

    std::uniform_int_distribution<int> randomPrizeMoney{ 10, 50 };
    std::uniform_int_distribution<int> randomExperience{ 20, 75 };

    int experienceReward{ randomExperience(mt) };

    int attackChoice{ 0 };
    attackChoice = ChooseAttack(character, attackList);

    std::cout << character.GetName() << "s HP: " << character.GetHealth() << '\n';
    std::cout << enemyList[randomlyChosenEnemy].GetName() << "s HP: " << enemyList[randomlyChosenEnemy].GetHealth() << "\n\n";

    //TODO
    //
    //Give player experience for defeating enemies

    if(character.IsAlive() == true && enemyList[randomlyChosenEnemy].IsAlive() == true)
    {
        std::cout << character.GetName() << " used " << attackList[attackChoice - 1].GetName() << "!\n";

        //Check if players attack missed
        if (missAttackChance(mt) == 5)
        {
            std::cout << "Your attack missed!\n";
        }
        else
        {
            std::cout << "It did " << attackList[attackChoice - 1].GetDamage(character.GetLevel()) << " damage!\n\n";
            enemyList[randomlyChosenEnemy].SubtractHealth(attackList[attackChoice - 1].GetDamage(character.GetLevel()));

            //Enemies turn
            std::cout << "The " << enemyList[randomlyChosenEnemy].GetName() << " used " << attackList[randomlyChosenAttack].GetName() << "!\n";

            if (missAttackChance(mt) == 5)
            {
                std::cout << enemyList[randomlyChosenEnemy].GetName() << "s attack missed!\n\n";
            }
            else
            {
                std::cout << "It did " << attackList[randomlyChosenAttack].GetDamage(character.GetLevel()) << " damage\n\n";
                character.SubtractHealth(attackList[randomlyChosenAttack].GetDamage(character.GetLevel()));
            }
        }
    }
    else
    {
        if(character.IsAlive() == false && enemyList[randomlyChosenEnemy].IsAlive() == true)
        {
            std::cout << "You died!\n\n";

            return true;
        }
        else if(enemyList[randomlyChosenEnemy].IsAlive() == false && character.IsAlive() == true)
        {
            int prizeMoney{ randomPrizeMoney(mt) };

            std::cout << "You defeated the " << enemyList[randomlyChosenEnemy].GetName() << "!\n";
            std::cout << "You won $" << prizeMoney << " as prize money.\n";

            character.AddMoney(prizeMoney);

            std::cout << "You gained " << experienceReward << " experience.\n\n";

            character.GiveExperience(experienceReward);

            return true;
        }
        else if (character.IsAlive() == false && enemyList[randomlyChosenEnemy].IsAlive() == false)
        {
            std::cout << "You both killed each other at the same time, it's a draw! what a show!\n\n";

            return true;
        }
    }
}

bool ForfeitMatch(Character& character)
{
    std::cout << "Are you sure? It will cost $100 to forfeit the match, and you have $" << character.GetMoney() << '\n';
    std::cout << "1) Yes\n";
    std::cout << "2) No\n";

    int choice{ 0 };

    std::cin >> choice;

    if (choice == 1)
    {
        std::cout << "You forfeited the match, you lost $";

        if (character.GetMoney() >= 100)
        {
            character.SubtractMoney(100);
            std::cout << "100\n\n";
        }
        else
        {
            std::cout << character.GetMoney() << "\n\n";
            character.SubtractMoney(character.GetMoney());
        }

        return true;
    }
    else if (choice == 2)
    {
        return false;
    }
}

void GetCharacterInfo(const Character& character, const std::vector<Attack>& attackList)
{
    std::cout << character.GetName() << '\n';
    std::cout << std::format("{:>4}", "") << "-HP: " << character.GetHealth() << '\n';
    std::cout << std::format("{:>4}", "") << "-Level: " << character.GetLevel() << '\n';
    std::cout << std::format("{:>4}", "") << "-$" << character.GetMoney() << '\n';

    std::cout << "\nAttacks\n";
    for (const auto& attack : attackList)
    {
        std::cout << std::format("{:>4}", "") << attack.GetName() << '\n';
        std::cout << std::format("{:>8}", "") << "-Base Power: " << attack.GetBasePower() << '\n';
        std::cout << std::format("{:>8}", "") << "-Attack Power: " << attack.GetDamage(character.GetLevel()) << "\n\n";
    }
}

/**
 * @brief Select what you want to upgrade
 *
 * This function allows the player to choose what they want to upgrade.
 *
 * @param character The player's character performing the attack.
 * @param attackList A list of possible attacks the player's character can use.
 * @return None
 */
void PurchaseUpgradesMenu(Character& character, std::vector<Attack>& attackList)
{
    std::cout << "What would you like to purchase upgrades for?\n\n";

    std::cout << "1) Attacks\n";
    std::cout << "2) Return\n";

    int choice{ 0 };

    std::cin >> choice;

    switch (choice)
    {
        case 1:
            PurchaseAttackUpgrades(character, attackList);
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
 * @return None
 */
void PurchaseAttackUpgrades(Character& character, std::vector<Attack>& attackList)
{
    character.AddMoney(5000);
    std::cout << "\nWhich attack would you like to upgrade?\n\n";
    std::cout << "Cash $" << character.GetMoney() << "\n\n";

    int attackListIndex{ 0 };

    for (int counter{ 1 }; const auto & attack : attackList)
    {
        std::cout << counter++ << ") " << attack.GetName() << " -Level: " << attack.GetLevel() << " -Upgrade Cost $" << 500 * attackList[attackListIndex++].GetLevel() << '\n';
    }

    std::cout << '\n';

    int choice{ 0 };

    std::cin >> choice;
    
    int upgradeStep{ 0 };
    auto attackToUpgrade{ 0 };

    if (choice <= attackList.size())
    {
        upgradeStep = 500 * attackList[choice - 1].GetLevel();
        attackToUpgrade = attackList[choice - 1].GetLevel();

        if (character.GetMoney() >= upgradeStep)
        {
            std::cout << attackList[choice - 1].GetName() << " will cost " << upgradeStep << " to upgrade to level " << attackList[choice - 1].GetLevel() + 1 << ".\n";
            std::cout << "You currently have $" << character.GetMoney() << " continue with upgrade?\n";

            int choice{ 0 };

            std::cout << "1) Yes\n";
            std::cout << "2) No\n";

            std::cin >> choice;

            if (choice == 1)
            {
                attackList[choice - 1].IncreaseLevel();
                character.SubtractMoney(upgradeStep);

                std::cout << "You currently have $" << character.GetMoney() << " and your " << attackList[choice - 1].GetName()
                    << " attack is at level " << attackList[choice - 1].GetLevel() << "!\n";
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