#pragma once

#include "FunctionLibrary/HelperFunctionLibrary.h"

// =========================================================================
// CREATIONAL DESIGN PATTERNS: Factory Method
// =========================================================================
// "Define an interface for creating an object, but let subclasses 
// decide which class to instantiate."
//
// THE GOAL:
// The Factory Method decouples the "Client" (Game Level) from the "Details" 
// (Concrete Enemy Classes). Instead of hardcoding 'new Skeleton()', the 
// client asks a Factory to produce an 'Enemy'. This allows the system to 
// introduce new enemy types without modifying the core spawning logic.
//
// THE EXAMPLE:
// A dynamic Spawning System with two specialized production lines:
// 1. [MELEE FACTORY]: Produces Skeletons and Golems.
// 2. [RANGED FACTORY]: Produces Mages and Archers.
//
// THE BENEFIT:
// * Flexibility: Add a 'BossFactory' later without breaking the GameLevel.
// * Encapsulation: All complex construction logic (setting health, loading 
//   meshes) is hidden inside the factory, not scattered in the game loop.
// * Consistency: Ensures every enemy created by a specific factory follows 
//   the same initialization rules.
// =========================================================================

namespace FTM
{
    // =========================================================================
    // THE PRODUCT (Base Class)
    // ROLE: Defines the interface for the objects the factory creates.
    // =========================================================================
    class Enemy
    {
    public:
        Enemy(std::string Name) : Name(Name) {}
        virtual ~Enemy() = default;

        std::string GetName() const { return Name; }

        // ROLE: Virtual method so the GameLevel can trigger behavior without
        // knowing the specific subtype of the enemy.
        virtual void Attack() const { std::cout << "The " << Name << " attacks!\n"; }

    private:
        std::string Name;
    };

    // =========================================================================
    // CONCRETE PRODUCTS
    // ROLE: The actual objects being instantiated by the factories.
    // =========================================================================

    // MELEE UNITS
    class Skeleton : public Enemy { public: Skeleton() : Enemy("Skeleton") {} };
    class Golem : public Enemy { public: Golem() : Enemy("Golem") {} };

    // RANGED UNITS
    class SkeletonMage : public Enemy { public: SkeletonMage() : Enemy("Skeleton Mage") {} };
    class SkeletonArcher : public Enemy { public: SkeletonArcher() : Enemy("Skeleton Archer") {} };

    // =========================================================================
    // THE CREATOR (Factory Interface)
    // ROLE: Declares the Factory Method that returns an 'Enemy' pointer.
    // =========================================================================
    class IEnemyFactory
    {
    public:
        virtual ~IEnemyFactory() = default;

        // THE FACTORY METHOD:
        // Derived factories will override this to return specific concretions.
        // Returns 'std::unique_ptr<Enemy>' so ownership is transferred to the caller,
        // eliminating manual memory management and preventing leaks.
        virtual std::unique_ptr<Enemy> CreateEnemy(int TypeID) = 0;
    };

    // =========================================================================
    // CONCRETE CREATORS (The Factories)
    // ROLE: Overrides the factory method to return instances of products.
    // =========================================================================

    // Handles production of Melee units based on a TypeID.
    class MeleeFactory : public IEnemyFactory
    {
    public:
        std::unique_ptr<Enemy> CreateEnemy(int TypeID) override;
    };

    // Handles production of Ranged units based on a TypeID.
    class RangedFactory : public IEnemyFactory
    {
    public:
        std::unique_ptr<Enemy> CreateEnemy(int TypeID) override;
    };

    // =========================================================================
    // THE CLIENT
    // ROLE: Uses the factories to get products without knowing their classes.
    // =========================================================================
    class GameLevel
    {
    public:
        // Uses polymorphic creation: Using 'IEnemyFactory*' 
        // regardless of whether it is Melee or Ranged.
        void SpawnEnemies();

    private:
        IEnemyFactory* CurrentFactory;
    };

    // =========================================================================
    // DEMO IMPLEMENTATION
    // =========================================================================
    void RunDemo();
}