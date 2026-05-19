#pragma once

#include "FunctionLibrary/HelperFunctionLibrary.h"
#include <memory>
#include <vector>
#include <string>

// =========================================================================
// CREATIONAL DESIGN PATTERNS: PROTOTYPE PATTERN
// =========================================================================
// "Specify the kinds of objects to create using a prototype instance, 
// and create new objects by copying this prototype."
//
// THE GOAL:
// Avoid expensive initialization. In game development, spawning an actor 
// often involves heavy Disk I/O (loading meshes, textures, and sounds). 
// The Prototype pattern loads one "Master" instance into RAM and then 
// uses a 'Clone' method to stamp out new instances. This bypasses the 
// constructor's heavy lifting by performing a RAM-to-RAM deep copy.
//
// THE BENEFIT:
// * Performance: Instantly spawn massive hordes without frame-rate drops 
//   caused by repetitive file loading.
// * State Preservation: Clone a prototype that has already been modified 
//   (e.g., an 'Enraged' Zombie) to create pre-configured variations.
// * Reduced Coupling: The spawner doesn't need to know the concrete 
//   classes; it only needs the "Master" and the 'Clone()' interface.
//
// THE EXAMPLE:
// [Zombie]: The Abstraction. Defines the 'Clone()' and 'Attack()' contract.
// [Crawler/Walker/Bloater]: The Concretions. They implement Clone() using 
//         'Covariant Return Types' to return their specific type.
// [Demo]: The Client. Manages the "Master" list and executes clones 
//         based on user input without ever calling 'new Crawler()'.
// =========================================================================

namespace PRO
{
    // =========================================================================
    // 1. THE BASE CLASS PROTOTYPE
    // =========================================================================
    class Zombie
    {
    public:
        Zombie(std::string Name, int Health);
        virtual ~Zombie() = default;

        // THE CORE METHOD: Returns a unique_ptr-wrapped copy of itself.
        // Using std::unique_ptr ensures automatic memory management and
        // aligns with modern C++ ownership semantics across the codebase.
        virtual std::unique_ptr<Zombie> Clone() const = 0;

        virtual void Attack() const;
        std::string GetName() const { return Name; }

    protected:
        // Protected Constructor allows the Copy Constructor (used in Clone)
        // to pass data from the Prototype to the New Instance.
        Zombie() = default;

        std::string Name;
        int Health;
    };

    // =========================================================================
    // 2. CONCRETE PROTOTYPES
    // =========================================================================

    // Type 1: Fast, low health
    class Crawler : public Zombie
    {
    public:
        Crawler();
        std::unique_ptr<Zombie> Clone() const override;
        void Attack() const override;
    };

    // Type 2: Slow, high health
    class Walker : public Zombie
    {
    public:
        Walker();
        std::unique_ptr<Zombie> Clone() const override;
        void Attack() const override;
    };

    // Type 3: Tank, slow
    class Bloater : public Zombie
    {
    public:
        Bloater();
        std::unique_ptr<Zombie> Clone() const override;
        void Attack() const override;
    };

    // =========================================================================
    // 3. DEMO INTERFACE
    // =========================================================================
    void RunDemo();
}