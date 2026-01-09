#pragma once

#include "FunctionLibrary/HelperFunctionLibrary.h"

// =========================================================================
// STRUCTURAL DESIGN PATTERNS: Decorator
// =========================================================================
// "Attach additional responsibilities to an object dynamically."
//
// THE GOAL:
// Add features (Skins, Stats) to an object without changing its class.
//
// THE EXAMPLE:
// A "RPG Weapon" Enchantment System.
// 1. Base Component (IWeapon): Defines 'GetDamage()' and 'GetName()'.
// 2. Leaf (BasicSword): The raw item (The Component).
// 3. Decorators (Fire, Ice, Frost): Wrapper classes.
//    They "Wrap" a Weapon, add their own effect, and pass calls down.
//
// THE SCENARIO:
// We have a 'BasicSword' (Damage: 10).
// We apply 'Fire Enchantment'. It adds +10 Damage.
// We apply 'Ice Enchantment' (Stacking on Fire). It adds -5 Speed (Slow).
//
// BENEFIT:
// You can mix and match Enchantments (Fire + Ice) without changing 'BasicSword'.
// =========================================================================

namespace DEC
{
    // ------------------------------------------------------------------------
    // 1. THE COMPONENT (The Target Interface)
    // ------------------------------------------------------------------------
    // This is the "Contract" that both Items and Wrappers follow.
    // It defines what a Weapon DOES, not how it looks.
    class IWeapon
    {
    public:
        virtual ~IWeapon() = default;

        virtual int GetDamage() const = 0;
        virtual std::string GetName() const = 0;
    };

    // ------------------------------------------------------------------------
    // 2. THE CONCRETE COMPONENT (The Leaf)
    // ------------------------------------------------------------------------
    // This is the "Part". The raw Item.
    class BasicSword : public IWeapon
    {
    public:
        BasicSword(std::string Name, int BaseDamage);

        int GetDamage() const override { return BaseDamage; }
        std::string GetName() const override { return Name; }

    private:
        int BaseDamage;
        std::string Name;
    };

    // ------------------------------------------------------------------------
    // 3. THE ABSTRACT DECORATOR (The Wrapper Interface)
    // ------------------------------------------------------------------------
    // This is the "Label".
    // It inherits from IWeapon, so it *IS-A* Weapon.
    // It wraps another IWeapon to add features.
    class IEnchantment : public IWeapon
    {
    public:
        // The Decorator holds a pointer to the Item it decorates.
        // Passing 'RawItem' into the constructor use for ownership.
        IEnchantment(std::unique_ptr<IWeapon> RawItem);

    protected:
        // The wrapped object is 'protected' so Decorators can access it.
        std::unique_ptr<IWeapon> WrappedItem;
    };

    // ------------------------------------------------------------------------
    // 4. THE CONCRETE DECORATORS (The Stickers / Wrappers)
    // ------------------------------------------------------------------------

    // --- A. FIRE ENCHANTMENT ---
    // Wraps a weapon and adds Fire Damage (+20).
    class FireEnchantment : public IEnchantment
    {
    public:
        FireEnchantment(std::unique_ptr<IWeapon> RawItem);

        // THE DECORATOR LOGIC
        // Adds its own effect
        int GetDamage() const override;

        std::string GetName() const override { return "Burning " + WrappedItem->GetName(); }
    };

    // --- B. ICE ENCHANTMENT ---
    // Wraps a weapon and adds Cold Damage (+10).
    class IceEnchantment : public IEnchantment
    {
    public:
        IceEnchantment(std::unique_ptr<IWeapon> RawItem);

        int GetDamage() const override;

        std::string GetName() const override { return "Frozen " + WrappedItem->GetName(); }
    };

    // --- C. POISON ENCHANTMENT ---
    // Wraps a weapon and adds Poison (+15 Damage).
    class PoisonEnchantment : public IEnchantment
    {
    public:
        PoisonEnchantment(std::unique_ptr<IWeapon> RawItem);

        int GetDamage() const override;

        std::string GetName() const override { return "Poisoned " + WrappedItem->GetName(); }
    };

    // ------------------------------------------------------------------------
    // 5. DEMO
    // ------------------------------------------------------------------------
    void RunDemo();
}