#pragma once

#include "FunctionLibrary/HelperFunctionLibrary.h"

// =========================================================================
// SOLID DESIGN PRINCIPLE: Open-Closed Principle
// =========================================================================
// "Software entities should be open for extension, but closed for modification."
//
// THE GOAL:
// To ensure system stability during expansion. By utilizing abstraction, new 
// functionality is introduced by adding new code (extension) rather than 
// altering existing, tested code (modification). This prevents regressive 
// bugs in the core logic when the system grows.
//
// THE BENEFIT:
// * Stability: Core classes remain untouched, reducing the risk of 
//   breaking existing features.
// * Scalability: New object types can be integrated indefinitely without 
//   increasing the complexity of the calling class.
// * Maintainability: Logic for specific behaviors is encapsulated within 
//   the objects themselves, not in a massive central switch statement.
//
// THE EXAMPLE:
// [IInteractable]: An interface defining the "Open" contract for interaction.
// [Player]: The "Closed" actor that triggers interactions without knowing 
//           the specific types.
// [Door/Chest/Spring]: Extensions that implement unique interaction logic.
// =========================================================================

namespace OCP
{
    // Forward declaration to allow the interface to reference the Player class.
    class Player;

    // =========================================================================
    // THE INTERFACE
    //
    // Defines the contract for all interactable objects. This is the "Open" 
    // part of the principle, allowing for infinite behavioral extensions.
    // =========================================================================
    class IInteractable
    {
    public:
        virtual ~IInteractable() = default;

        // Triggers the unique behavior of the object.
        // The Player instance is passed to allow the object to modify player state.
        virtual void Interact(Player& PlayerRef) = 0;

        // Returns the identifier for display within the user interface.
        virtual std::string GetName() const = 0;
    };

    // =========================================================================
    // THE ACTOR
    //
    // The Player class is "Closed" for modification. It facilitates interaction 
    // through the interface without requiring knowledge of concrete types.
    // =========================================================================
    class Player
    {
    public:
        Player();

        // Execution of the Open-Closed Principle.
        // Interaction is delegated entirely to the interface, ensuring this 
        // function never needs to change when new interactable types are added.
        void InteractWith(IInteractable* Target);

        // Attribute Accessors
        int GetHealth() const { return Health; }
        int GetMaxHealth() const { return MaxHealth; }
        int GetGold() const { return Gold; }

        // Attribute Mutators
        void SetHealth(int Amount) { Health = Amount; }
        void AddGold(int Amount) { Gold += Amount; }

    private:
        // Attributes.
        // Interacting with different objects determines how they are affected.
        int Health;
        int MaxHealth;
        int Gold;
    };

    // =========================================================================
    // CONCRETE EXTENSIONS
    //
    // These classes extend the system functionality. Adding or removing these 
    // modules requires zero changes to the Player class logic.
    // =========================================================================

    class Door : public IInteractable
    {
    public:
        Door();

        void Interact(Player& PlayerRef) override;
        std::string GetName() const override { return "Wooden Door"; }

    private:
        bool IsOpen;
    };

    class Chest : public IInteractable
    {
    public:
        Chest(int GoldAmount);

        void Interact(Player& PlayerRef) override;
        std::string GetName() const override { return "Old Chest"; }

    private:
        bool IsLooted;
        int GoldAmount;
    };

    class WaterSpring : public IInteractable
    {
    public:
        void Interact(Player& PlayerRef) override;
        std::string GetName() const override { return "Mystical Water Spring"; }
    };

    // =========================================================================
    // DEMO
    // =========================================================================
    void RunDemo();
}