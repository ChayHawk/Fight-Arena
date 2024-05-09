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

    std::string GetName() const
    {
        return mName;
    }

    int GetHealth() const
    {
        return mHealth;
    }

    int GetLevel() const
    {
        return mLevel;
    }

    int GetMoney() const
    {
        return mMoney;
    }

    int GetMaxLevel() const
    {
        return mMaxLevel;
    }

    int GetExperience() const
    {
        return mExperience;
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

    void GiveMoney(int amount)
    {
        if (amount > 0)
        {
            mMoney += amount;
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
        if (amount > 0)
        {
            mMoney = amount;
        }
    }

    int CalculateExperience() const
    {
        return 500 * mLevel;
    }

    int GetMaxExperience() const
    {
        return mMaxExperience;
    }

private:
    std::string mName{ "" };
    int mHealth{ 0 };
    int mLevel{ 1 };
    int mMoney{ 0 };
    int mExperience{ 0 };

    static const int mMaxLevel{ 100 };
    static const int mExperienceMultiplier{ 500 };
    static const int mMaxExperience{ mExperienceMultiplier * mMaxLevel };
};

void GetCharacterInfo(const Character& character, const std::vector<Attack>& attackList);
void Test(Character& character, std::vector<Attack>& attackList);
void PurchaseUpgradesMenu(Character& character, std::vector<Attack>& attackList);
void PurchaseUpgrades(Character& character, std::vector<Attack>& attackList);

int main()
{
    Character player("Hero", 100, 1, 25);

    Attack slash("Slash", 3, 1);
    Attack scratch("Scratch", 3, 1);
    Attack fireBlast("Fire Blast", 3, 1);
    Attack stomp("Stomp", 3, 1);

    std::vector<Attack> attackList{ slash, scratch, fireBlast, stomp };

    std::cout << "Players current Level: " << player.GetLevel() << "\n\n";

    //Test(player, attackList);

    //GetCharacterInfo(player, attackList);
    player.GiveMoney(30000);

    bool isGameEnded{ false };

    while (!isGameEnded)
    {
        PurchaseUpgradesMenu(player, attackList);
    }
}

void Test(Character& character, std::vector<Attack>& attackList)
{
    character.GiveMoney(30);

    std::cout << "Money: " << character.GetMoney() << '\n';
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
    std::cout << "What would you like to purchase upgrades for?\n\n";

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