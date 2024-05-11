#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <random>
#include <format>
#include <cassert>

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
        return (mBasePower / 2 * ((mLevel * characterLevel) / 2));
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
        void GiveMoney(int amount)
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

        void TakeMoney(int amount)
        {
            if (amount > 0 && mMoney >= amount)
            {
                mMoney -= amount;
            }
        }

        
        void SetMoney(int amount)
        {
            if (amount > 0 && amount <= mMaxMoney)
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

        //Done
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

        //Done
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
void PurchaseUpgrades(Character& character, std::vector<Attack>& attackList);
void Arena(Character& character, const std::vector<Attack>& attackList, std::vector<Character>& enemyList);

int main()
{
    Character player("Hero", 3, 1, 25);

    Character goblin("Goblin", 30, 1, 0);
    Character orc("Orc", 42, 1, 0);
    Character giant("Giant", 103, 1, 0);

    Attack slash("Slash", 3, 1);
    Attack scratch("Scratch", 3, 1);
    Attack fireBlast("Fire Blast", 3, 1);
    Attack stomp("Stomp", 3, 1);

    std::vector<Attack> attackList{ slash, scratch, fireBlast, stomp };
    std::vector<Character> enemyList{ goblin, orc, giant };

    bool isTesting{ true };

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

            std::cin >> choice;

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

void Test(Character& character, std::vector<Attack>& attackList)
{
    std::cout << "Health: " << character.GetHealth() << std::boolalpha <<" - Is Alive?: " << character.IsAlive() << '\n';

    character.SubtractHealth(2);
    character.SubtractHealth(1);

    std::cout << "Health: " << character.GetHealth() << std::boolalpha << " - Is Alive?: " << character.IsAlive() << '\n';
}

void Arena(Character& character, const std::vector<Attack>& attackList, std::vector<Character>& enemyList)
{

    if (enemyList.size() > 0)
    {
        std::uniform_int_distribution<int> randomizedEnemy{ 0, static_cast<int>(enemyList.size() - 1) };
    }

    std::uniform_int_distribution<int> randomizedRewardMoneyLow{ 3, 13 };
    std::uniform_int_distribution<int> randomizedRewardMoneyMedium{ 14, 37 };
    std::uniform_int_distribution<int> randomizedRewardMoneyHigh{ 38, 219 };
    std::uniform_int_distribution<int> randomizedRewardMoneyBoss{ 1000, 3000 };
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

void PurchaseUpgradesMenu(Character& character, std::vector<Attack>& attackList)
{
    std::cout << "What would you like to purchase upgrades for?\n";
    std::cout << "Cash $" << character.GetMoney() << "\n\n";

    std::cout << "1) Attacks\n";
    std::cout << "2) Return\n";

    int choice{ 0 };

    std::cin >> choice;

    switch (choice)
    {
        case 1:
            PurchaseUpgrades(character, attackList);
            break;

        case 2:
            return;

        default:
            std::cout << "Incorrect choice\n";
    }
}

void PurchaseUpgrades(Character& character, std::vector<Attack>& attackList)
{
    std::cout << "\nWhich attack would you like to upgrade?\n\n";

    for (int counter{ 1 }; const auto & attack : attackList)
    {
        std::cout << counter++ << ") " << attack.GetName() << " -Level: " << attack.GetLevel() << '\n';
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

        if (character.GetMoney() > upgradeStep)
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
                character.TakeMoney(upgradeStep);

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
            std::cout << "It seems you do not posses enough funds to perform any upgrades.\n";
        }
    }
    else
    {
        std::cout << "It doesnt seem like that was a valid choice, please choose again.\n\n";
    }
}