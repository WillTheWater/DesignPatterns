#pragma once

#include "FunctionLibrary/HelperFunctionLibrary.h"

// =========================================================================
// PRINCIPLE: Liskov Substitution Principle (LSP)
// =========================================================================
// "Derived classes must be substitutable for their base classes."
//
// THE GOAL:
// Ensure that a subclass behaves exactly as you expect the base class to.
// If a function uses an 'Item' pointer, it should work perfectly
// for a 'Weapon' or a 'Potion' without checking the specific type.
//
// THE EXAMPLE:
// An Item Appraisal System.
// 1. Item (Base): Defines universal traits (Name, Value).
// 2. Weapon (Child): Has Damage. Calculates value based on Damage.
// 3. Potion (Child): Has Healing Power. Calculates value based on Rarity.
//
// THE RULE:
// We do NOT put 'GetDamage()' in the base 'Item' class,
// because not all items have damage (e.g., Gold, Rocks, Potions).
// =========================================================================

namespace LSP
{
    // ------------------------------------------------------------------------
    // 1. BASE CLASS (The Contract)
    // ------------------------------------------------------------------------

    class Item
    {
    public:
        Item(std::string Name, float Weight);

        // Virtual Destructor is CRITICAL for Polymorphism
        virtual ~Item() = default;

        // Pure virtual: Every item must be able to tell us its value,
        // but every item might calculate it differently.
        virtual int GetValue() const = 0;

        // Common getters
        std::string GetName() const { return Name; }
        float GetWeight() const { return Weight; }

    protected:
        std::string Name;
        float Weight;
    };

    // ------------------------------------------------------------------------
    // 2. DERIVED CLASSES (Substitutions)
    // ------------------------------------------------------------------------

    class Weapon : public Item
    {
    public:
        Weapon(std::string Name, float Weight, int Damage);

        // Weapon calculates value based on how much damage it deals.
        int GetValue() const override;

        // Specific to Weapon (NOT in Base Class - LSP Compliance)
        int GetDamage() const { return Damage; }

    private:
        int Damage;
    };

    class Potion : public Item
    {
    public:
        Potion(std::string Name, float Weight, int HealAmount);

        // Potion calculates value based on how much it heals.
        int GetValue() const override;

        // Specific to Potion (NOT in Base Class - LSP Compliance)
        int GetHealAmount() const { return HealAmount; }

    private:
        int HealAmount;
    };

    enum class ResourceType
    {
        Wood = 20,   // Easy way to assign unique values based on type
        Stone = 99,  // Makes GetValue calculation more interesting.
        Gem = 460,
        Ore = 145
    };

    class RawResource : public Item
    {
    public:
        RawResource(std::string Name, float Weight, ResourceType Material);

        int GetValue() const override;
        ResourceType GetMaterialType() const { return RawMaterialType; }

    private:
        ResourceType RawMaterialType;
    };

    // ------------------------------------------------------------------------
    // 3. THE SYSTEM (The Context)
    // This class accepts an 'Item'. It does not know about Weapons or Potions.
    // Because of LSP, it can appraise ANY item safely.
    // ------------------------------------------------------------------------

    class Trader
    {
    public:
        // Looks at the item
        void AppraiseItem(const Item* Target) const;

        // Sells the item (Removes it from the vector)
        // We pass the Index because pointers can't be used to erase items from a vector.
        void SellItem(int Index, std::vector<std::unique_ptr<Item>>& Inventory) const;
    };

    // ------------------------------------------------------------------------
    // DEMO
    // ------------------------------------------------------------------------
    void RunDemo();
}