#pragma once

#include "FunctionLibrary/HelperFunctionLibrary.h"

// =========================================================================
// STRUCTURAL DESIGN PATTERN: Decorator
// =========================================================================
// "Attach additional responsibilities to an object dynamically."
//
// THE GOAL:
// Provide a flexible alternative to sub-classing for extending functionality. 
// Instead of creating a 'FireSword' class, we 'wrap' a BasicSword in a 
// FireDecorator at runtime.
//
// THE BENEFIT:
// * Flexibility: Responsibilities can be added or removed at runtime.
// * Composability: Multiple decorators can be stacked (Fire + Ice + Poison).
// * Single Responsibility: Complex behavior is broken into small, 
//   incremental classes rather than one "God Class" with every feature.
//
// THE EXAMPLE:
// [IWeapon]: The Component interface. Defines GetDamage() and GetName().
// [BasicSword]: The Concrete Component. The base object being decorated.
// [IEnchantment]: The Base Decorator. Maintains a reference to a weapon.
// [Fire/Ice/Poison]: Concrete Decorators. They add specific logic to the
//   wrapped weapon's calls.
// =========================================================================

namespace DEC
{
    // =========================================================================
    // THE COMPONENT (The Interface)
    // ROLE: Defines the common interface for both the core objects and 
    // the decorators that will wrap them.
    // =========================================================================
    class IWeapon
    {
    public:
        virtual ~IWeapon() = default;

        virtual int GetDamage() const = 0;
        virtual std::string GetName() const = 0;
    };

    // =========================================================================
    // THE CONCRETE COMPONENT (The Leaf)
    // ROLE: The core object that can have responsibilities added to it.
    // =========================================================================
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

    // =========================================================================
    // THE BASE DECORATOR (The Wrapper)
    // ROLE: Maintains a pointer to an IWeapon and conforms to the IWeapon 
    // interface, allowing it to be "transparent" to the client.
    // =========================================================================
    class IEnchantment : public IWeapon
    {
    public:
        // Decorator takes ownership of the item it wraps
        IEnchantment(std::unique_ptr<IWeapon> RawItem);

    protected:
        // Protected so concrete decorators can access the underlying item
        std::unique_ptr<IWeapon> WrappedItem;
    };

    // =========================================================================
    // CONCRETE DECORATORS (The Effects)
    // ROLE: Add state or behavior to the component. They call the wrapped 
    // object and then "decorate" the result.
    // =========================================================================

    // ======================== FIRE ENCHANTMENT (+20 Damage) ========================
    class FireEnchantment : public IEnchantment
    {
    public:
        FireEnchantment(std::unique_ptr<IWeapon> RawItem);

        int GetDamage() const override;
        std::string GetName() const override { return "Burning " + WrappedItem->GetName(); }
    };

    // ======================== ICE ENCHANTMENT (+10 Damage) ========================
    class IceEnchantment : public IEnchantment
    {
    public:
        IceEnchantment(std::unique_ptr<IWeapon> RawItem);

        int GetDamage() const override;
        std::string GetName() const override { return "Frozen " + WrappedItem->GetName(); }
    };

    // ======================== POISON ENCHANTMENT (+15 Damage) ========================
    class PoisonEnchantment : public IEnchantment
    {
    public:
        PoisonEnchantment(std::unique_ptr<IWeapon> RawItem);

        int GetDamage() const override;
        std::string GetName() const override { return "Poisoned " + WrappedItem->GetName(); }
    };

    // =========================================================================
    // DEMO IMPLEMENTATION
    // =========================================================================
    void RunDemo();
}