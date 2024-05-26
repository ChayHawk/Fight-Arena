#pragma once
#include <string>

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