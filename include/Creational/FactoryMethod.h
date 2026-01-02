#pragma once

#include "FunctionLibrary/HelperFunctionLibrary.h"

// =========================================================================
// PRINCIPLE: Factory Method Pattern (Creational)
// =========================================================================
// "Define an interface for creating an object, but let subclasses
// decide which class to instantiate."
//
// THE GOAL:
// Decouple object creation from usage. Instead of saying 'new Skeleton',
// user asks a Factory to 'CreateEnemy'.
//
// THE EXAMPLE:
// An Enemy Spawning System.
// 1. IEnemyFactory: Defines 'CreateEnemy()'.
// 2. MeleeFactory (Melee): Creates Skeletons and Golems.
// 3. RangedFactory (Ranged): Creates Mages and Archers.
//
// BENEFIT:
// The Game Level doesn't know how to construct specific enemies.
// It just calls 'Factory->CreateEnemy()'. The factory handles the 'new'.
// =========================================================================

namespace FTM
{
    // ------------------------------------------------------------------------
    // 1. PRODUCT TYPES
    // ------------------------------------------------------------------------
    // These represent the actual game objects.
    class Enemy
    {
    public:
        Enemy(std::string Name) : Name(Name) {}
        virtual ~Enemy() = default;

        std::string GetName() const { return Name; }

        // A generic attack method so the enemy does something
        virtual void Attack() const { std::cout << "The " << Name << " attacks!\n"; }

    private:
        std::string Name;
    };

    // ------------------------------------------------------------------------
    // SKELETON DERIVED CLASSES
    // ------------------------------------------------------------------------
    class Skeleton : public Enemy
    {
    public:
        // INLINE CONSTRUCTOR
        // : Enemy("Skeleton") : Calls the BASE CLASS CONSTRUCTOR.
        //    - "Skeleton" is passed to Enemy so Enemy can set its Name variable.
        Skeleton() : Enemy("Skeleton") {}
    };

    class Golem : public Enemy { public: Golem() : Enemy("Golem") {} };
    class SkeletonMage : public Enemy { public: SkeletonMage() : Enemy("Skeleton Mage") {} };
    class SkeletonArcher : public Enemy { public: SkeletonArcher() : Enemy("Skeleton Archer") {} };

    // ------------------------------------------------------------------------
    // 2. THE CREATOR INTERFACE
    // ------------------------------------------------------------------------
    // The Factory Method interface.
    class IEnemyFactory
    {
    public:
        virtual ~IEnemyFactory() = default;

        // The Factory Method. It returns a pointer to Base Class (Enemy).
        // The (Game Level) doesn't need to know if it's a Skeleton or Golem.
        virtual Enemy* CreateEnemy(int TypeID) = 0;
    };

    // ------------------------------------------------------------------------
    // 3. CONCRETE CREATORS (The Factories)
    // ------------------------------------------------------------------------

    // Handles melee combatants (Skeleton, Golem)
    class MeleeFactory : public IEnemyFactory
    {
    public:
        Enemy* CreateEnemy(int TypeID) override;
    };

    // Handles ranged combatants (Mage, Archer)
    class RangedFactory : public IEnemyFactory
    {
    public:
        Enemy* CreateEnemy(int TypeID) override;
    };

    // ------------------------------------------------------------------------
    // 4. THE GAME LEVEL
    // ------------------------------------------------------------------------
    class GameLevel
    {
    public:
        void SpawnEnemies();
    private:
        IEnemyFactory* Factory;
    };

    // ------------------------------------------------------------------------
    // DEMO
    // ------------------------------------------------------------------------
    void RunDemo();
}