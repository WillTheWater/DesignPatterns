#pragma once

#include "FunctionLibrary/HelperFunctionLibrary.h"

// =========================================================================
// SOLID DESIGN PRINCIPLE: Liskov Substitution Principle
// =========================================================================
// "Derived classes must be substitutable for their base classes."
//
// THE GOAL:
// To ensure that a subclass can stand in for its parent without the calling 
// code needing to know the difference. This principle enforces that a derived 
// class must honor the "contract" of the base class without breaking the 
// program's logic or requiring type-checking (like casting).
//
// THE BENEFIT:
// * Reliability: Functions that use base class pointers remain bug-free 
//   regardless of which specific child class is passed to them.
// * Clean Logic: Removes the need for "if (type == Weapon)" checks.
// * Interface Integrity: Ensures the base class remains a clean, 
//   universal representation of the shared logic.
//
// THE EXAMPLE:
// [Item]: Base class defining universal traits (Name, Value).
// [Weapon/Potion/Resource]: Subclasses that fulfill the 'GetValue' contract.
// [Trader]: A system that processes 'Items' without knowing their subtypes.
// =========================================================================

namespace LSP
{
    // =========================================================================
    // BASE CLASS: ITEM
    // Defines the contract. All items MUST have a weight and a value.
    // =========================================================================
    class Item
    {
    public:
        Item(std::string Name, float Weight);
        virtual ~Item() = default;

        // The Contract: Every item must calculate a value, but logic is 
        // deferred to the specific implementation.
        virtual int GetValue() const = 0;

        std::string GetName() const { return Name; }
        float GetWeight() const { return Weight; }

    protected:
        std::string Name;
        float Weight;
    };

    // =========================================================================
    // DERIVED CLASSES
    // These satisfy the substitution principle by providing valid logic 
    // for the virtual 'GetValue' function.
    // =========================================================================

    class Weapon : public Item
    {
    public:
        Weapon(std::string Name, float Weight, int Damage);
        int GetValue() const override; // Value = Damage-based
        int GetDamage() const { return Damage; }

    private:
        int Damage;
    };

    class Potion : public Item
    {
    public:
        Potion(std::string Name, float Weight, int HealAmount);
        int GetValue() const override; // Value = Potency-based
        int GetHealAmount() const { return HealAmount; }

    private:
        int HealAmount;
    };

    enum class ResourceType { Wood = 20, Stone = 99, Gem = 460, Ore = 145 };

    class RawResource : public Item
    {
    public:
        RawResource(std::string Name, float Weight, ResourceType Material);
        int GetValue() const override; // Value = Material-based
        ResourceType GetMaterialType() const { return RawMaterialType; }

    private:
        ResourceType RawMaterialType;
    };

    // =========================================================================
    // THE TRADER SYSTEM
    // This system demonstrates LSP by interacting only with the 'Item' interface.
    // =========================================================================
    class Trader
    {
    public:
        void AppraiseItem(const Item* Target) const;
        void SellItem(int Index, std::vector<std::unique_ptr<Item>>& Inventory) const;
    };

    void RunDemo();
}