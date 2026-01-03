#pragma once

#include "FunctionLibrary/HelperFunctionLibrary.h"

// =========================================================================
// PRINCIPLE: Prototype Pattern
// =========================================================================
// "Specify the kinds of objects to create using a prototype instance, 
// and create new objects by copying this prototype."
//
// THE GOAL:
// Avoid expensive initialization. Don't load assets/files 100 times.
// Load one Master, then Clone it.
//
// THE EXAMPLE:
// A Zombie Army Spawner.
// 1. Prototype Interface: Defines 'Clone()'.
// 2. Prototypes: Crawler, Walker, Bloater (The "Prototypes").
// 3. Spawner: Clones the Prototype n times.
//
// BENEFIT:
// Spawning 100 Zombies is instant because we don't reload their 
// assets (simulated) every time. We just copy the data in RAM.
// =========================================================================

namespace PRO
{
    // ------------------------------------------------------------------------
    // 1. THE BASE CLASS PROTOTYPE
    // ------------------------------------------------------------------------
    class Zombie
    {
    public:
        Zombie(std::string Name, int Health);
        virtual ~Zombie() = default;

        // THE CORE METHOD: Returns a copy of itself.
        // This is called 'Co-variant Return Types'.
        // Crawler returns Crawler*, Walker returns Walker* etc...
        virtual Zombie* Clone() const = 0;

        virtual void Attack() const;
        std::string GetName() const { return Name; }

    protected:
        // Protected Constructor allows children to copy data.
        Zombie() = default;

        std::string Name;
        int Health;
    };

    // ------------------------------------------------------------------------
    // 2. CONCRETE PROTOTYPES
    // ------------------------------------------------------------------------

    // Type 1: Fast, low health
    class Crawler : public Zombie
    {
    public:
        Crawler();

        // Covariance: Returns Crawler*, not just Zombie*
        Crawler* Clone() const override;
        void Attack() const override;
    };

    // Type 2: Slow, high health
    class Walker : public Zombie
    {
    public:
        Walker();

        Walker* Clone() const override;
        void Attack() const override;
    };

    // Type 3: Fat, slow
    class Bloater : public Zombie
    {
    public:
        Bloater();

        Bloater* Clone() const override;
        void Attack() const override;
    };

    // ------------------------------------------------------------------------
    // 3. DEMO
    // ------------------------------------------------------------------------
    void RunDemo();
}