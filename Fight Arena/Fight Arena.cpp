#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <chrono>
#include <random>
#include <limits>

class Attack
{
public:
    Attack(const std::string& name, double damage, int upgradeLevel)
        : mName(name), mDamage(damage), mUpgradeLevel(upgradeLevel)
    {}

    std::string GetName() const
    {
        return mName;
    }

    double GetDamage() const
    {
        return mDamage;
    }

    int GetUpgradeLevel() const
    {
        return mUpgradeLevel;
    }

    int GetMaxUpgradeLevel() const
    {
        return mMaxUpgradeLevel;
    }

    double GetDamageBase() const
    {
        return mDamageBase;
    }

    double CalculateDamage(int characterLevel) const
    {
        return mDamage + mUpgradeLevel * (characterLevel / mDamageBase);
    }

    void IncreaseUpgradeLevel()
    {
        if (mUpgradeLevel < mMaxUpgradeLevel)
        {
            ++mUpgradeLevel;
        }
    }

private:
    std::string mName{ "" };
    double mDamage{ 0.0 };
    int mUpgradeLevel{ 1 };

    static inline const double mDamageBase{ 3.67 };
    static const int mMaxUpgradeLevel{ 10 };
};

class Character
{
public:
    Character(const std::string& name, double health, int level, int money)
        : mName(name), mHealth(health), mLevel(level), mMoney(money)
    {}

    void GiveHealth(double amount)
    {
        mHealth += amount;
        if (mHealth > mMaxHealth)
        {
            mHealth = mMaxHealth;
        }
    }

    void RemoveHealth(double amount)
    {
        mHealth -= amount;
        if (mHealth < mMinHealth)
        {
            mHealth = mMinHealth;
        }
    }

    void GiveExperience(double amount)
    {
        mExperience += amount;

        if (mLevel <= mMaxLevel)
        {
            while (mExperience >= GetLevelStep())
            {
                double carryoverSum{ mExperience -= GetLevelStep() };
                ++mLevel;
            }
        }
    }

    bool IsDead() const
    {
        if (mHealth == mMinHealth)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    void ChangeName(std::string name)
    {
        mName = name;
    }

    double GetHealth() const
    {
        return mHealth;
    }

    double GetMaxHealth() const
    {
        return mMaxHealth;
    }

    std::string GetName() const
    {
        return mName;
    }

    double GetExperience() const
    {
        return mExperience;
    }

    int GetLevel() const
    {
        return mLevel;
    }

    int GetMoney() const
    {
        return mMoney;
    }

    int GetLevelStep() const
    {
        return mLevelUpPercentage * mLevel;
    }

    std::vector<Attack> GetAttacks() const
    {
        return mAttackList;
    }

    void LearnAttack(const Attack& attackToLearn)
    {
        mAttackList.push_back(attackToLearn);
    }

private:
    std::string mName{ "" };
    double mHealth{ 100.0 };
    double mExperience{ 0.0 };
    int mLevel{ 1 };
    int mMoney{ 100 };
    std::vector<Attack> mAttackList{ };

    static inline const int mMaxLevel{ 100 };
    static inline const float mMaxHealth{ 100.0 };
    static inline const float mMinHealth{ 0.0 };
    static const int mLevelUpPercentage{ 500 };
};

void DisplayInfo(const Character& character);
void ChangePlayerName(Character& character);
void MainMenu(Character& character, std::vector<Attack>& attackList, std::vector<Character>& enemyList);

int main()
{
    Character hero("Hero", 100.0, 1, 1200);

    Character goblin("Goblin", 45.00, 1, 20);
    Character witch("Witch", 65.00, 1, 20);
    Character elf("Elf", 40.00, 1, 20);
    Character orc("Orc", 85.00, 1, 20);
    Character werewolf("Werewolf", 90.00, 1, 20);

    Attack scratch("Scratch", 2, 1);
    Attack slash("Slash", 4, 1);

    hero.LearnAttack(scratch);
    hero.LearnAttack(slash);

    std::vector<Attack> attackList{ scratch, slash };
    std::vector<Character> enemyList{ goblin, witch, elf, orc, werewolf };

    MainMenu(hero, attackList, enemyList);
}

void DisplayInfo(const Character& character)
{
    std::cout << character.GetName() << '\n';
    std::cout << "    -Hp:           " << character.GetHealth() << "/" << character.GetMaxHealth() << '\n';
    std::cout << "    -Lvl:          " << character.GetLevel() << "/100" << '\n';
    std::cout << "    -Exp:          " << character.GetExperience() << "/" << character.GetLevelStep() << "" << '\n';
    std::cout << "    -Dead?:        " << std::boolalpha << character.IsDead() << '\n';
    std::cout << "    -Attacks:    " << '\n';

    if (character.GetAttacks().size() == 0)
    {
        std::cout << "        -None";
    }
    else
    {
        for (const auto& i : character.GetAttacks())
        {
            std::cout << "        -Name:     " << i.GetName() << '\n';
            std::cout << "        -Dmg:      " << std::setprecision(3) << i.CalculateDamage(character.GetLevel()) << "\n";
            std::cout << "        -Lvl:      " << i.GetUpgradeLevel() << "/" << i.GetMaxUpgradeLevel() << "\n\n";
        }
    }
}

void ChangePlayerName(Character& character)
{
    bool IsSuccessfullyRenamed{ false };

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    while (IsSuccessfullyRenamed != true)
    {
        std::cout << "What would you like to change your name to?\n";
        std::cout << '>';

        std::string newName{ "" };
        std::string oldName{ "" };

        oldName = character.GetName();

        std::getline(std::cin, newName);

        if (newName.size() > 0)
        {
            std::cout << "Are  you sure you want to rename yourself from " << oldName << " to " << newName << "?\n";
            std::cout << "1) Yes\n";
            std::cout << "2) No\n";
            std::cout << "3) Main Menu\n";
            std::cout << '>';

            int choice{ 0 };

            std::cin >> choice;

            switch (choice)
            {
            case 1:
                character.ChangeName(newName);

                std::cout << "Your name has been changed from " << oldName << " to " << newName << "!\n\n";
                IsSuccessfullyRenamed = true;
                break;

            case 2:
                std::cout << "Give yourself a name.\n\n";
                break;

            case 3:
                return;

            default:
                std::cout << "Error\n";
            }
        }
        else
        {
            std::cout << "Please Choose a name\n";
        }
    }
}

std::mt19937 mt{ static_cast<unsigned int>(std::chrono::steady_clock::now().time_since_epoch().count()) };


//Needs  finishing
//
//Need to finish writing code for  battling creatures
void Battle(Character& player, std::vector<Character>& enemyList, std::vector<Attack>& attackList)
{
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::uniform_int_distribution<int> randomEnemy{};
    if (!enemyList.empty())
    {
        randomEnemy = std::uniform_int_distribution<int>(0, enemyList.size() - 1);
    }

    std::uniform_int_distribution<int> randomAttack{};
    if (!attackList.empty())
    {
        randomAttack = std::uniform_int_distribution<int>(0, attackList.size() - 1);
    }

    bool IsBattleOver{ false };

    int chooseRandomEnemy{ randomEnemy(mt) };

    std::cout << "A " << enemyList[chooseRandomEnemy].GetName() << " has appeared!\n";
    std::cout << "It has " << enemyList[chooseRandomEnemy].GetHealth() << " HP\n\n";

    while (IsBattleOver != true)
    {
        if (!enemyList.empty())
        {
            std::cout << "Do what?\n\n";

            std::cout << "1)Attack\n";
            std::cout << "2)Forfit\n";

            int choice{ 0 };
            std::cin >> choice;

            switch (choice)
            {
            case 1:
            {
                std::cout << "Use What Move?\n\n";

                for (int counter{ 1 }; const auto & i : attackList)
                {
                    std::cout << counter++ << ") " << i.GetName() << " -Dmg: " << i.CalculateDamage(player.GetLevel()) << '\n';
                }

                int choice{ 0 };

                std::cin >> choice;

                std::cout << player.GetName() << " used " << attackList[choice -1].GetName() << "!\n";
                std::cout << "The " << enemyList[chooseRandomEnemy].GetName() << " has " << enemyList[chooseRandomEnemy].GetHealth() << " HP left!\n";

                enemyList[chooseRandomEnemy].RemoveHealth(attackList[choice - 1].CalculateDamage(player.GetLevel()));

                if (enemyList[chooseRandomEnemy].IsDead())
                {
                    std::cout << player.GetName() << " defeated the " << enemyList[chooseRandomEnemy].GetName() << "!\n";
                }
            }
                break;

            case 2:
                return;

            default:
                std::cout << choice << " is not a valid selection\n";
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }

            std::cin.get();
        }
        else
        {
            std::cout << "There are no enemies to fight!\n";
            return;
        }
    }
}

void MainMenu(Character& character, std::vector<Attack>& attackList, std::vector<Character>& enemyList)
{
    bool IsGameRunning{ true };

    while (IsGameRunning)
    {
        std::cout << "Please Make a selection\n\n";

        std::cout << "1) Change Name\n";
        std::cout << "2) Fight\n";
        std::cout << "3) View Stats\n";
        std::cout << "4) Quit\n";
        std::cout << '>';

        int choice{ 0 };

        std::cin >> choice;

        switch (choice)
        {
        case  1:
            ChangePlayerName(character);
            break;

        case 2:
            Battle(character, enemyList, attackList);
            break;

        case 3:
            DisplayInfo(character);
            break;

        case 4:
            return;

        default:
            std::cout << "Error\n";
        }
    }
}