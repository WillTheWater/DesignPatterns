#pragma once

#include "FunctionLibrary/HelperFunctionLibrary.h"

// =========================================================================
// PRINCIPLE: Open-Closed Principle (OCP)
// =========================================================================
// "Entities (classes, modules, functions) should be open for 
// extension, but closed for modification."
//
// THE GOAL:
// Create a system where you can add new behaviors (new objects) without
// breaking or modifying existing classes.
//
// THE EXAMPLE:
// An Interaction System.
// 1. Player: Does not know what it is interacting with.
// 2. IInteractable: An interface that defines a single Interact() method.
// 3. Door, Chest, WaterSpring: Concrete implementations that perform 
//    unique actions when interacted with.
//
// BENEFIT:
// You can add any number of other objects to interact with later by creating a NEW class. 
// Without ever needing to edit Player (the interactor).
// =========================================================================

namespace OCP
{
    // Forward declaration of Player is needed so the interface knows the class exists.
    class Player;

    // =========================================================================
    // 1. THE INTERFACE (The "Open" part)
    // This is a pure virtual class, meaning its functions
    // must be overridden by the class that implements it.
    // =========================================================================
    class IInteractable
    {
    public:
        virtual ~IInteractable() = default;

        // The Player passes itself as a reference so objects can modify it.
        // (e.g., Chest gives gold, Spring heals HP).
        virtual void Interact(Player& PlayerRef) = 0;

        // Helper for displaying the interactable object in the menu
        virtual std::string GetName() const = 0;
    };

    // =========================================================================
    // 2. THE ACTOR (The "Closed" part)
    // The player only knows it can interact, but never defines with what it can
    // interact or what effect that interaction has.
    // =========================================================================
    class Player
    {
    public:
        Player();

        // This is the key of the Open-Closed Principle! 
        // Player does not define object types or control what they can interact with. 
        // It simply delegates to the interface.
        void InteractWith(IInteractable* Target);

        // Getters/Setters for the interaction objects
        // Even these elements could be wrapped in their own
        // version of OCP, but further abstraction would over complicate this demo.
        int GetHealth() const { return Health; }
        int GetMaxHealth() const { return MaxHealth; }
        int GetGold() const { return Gold; }
        void SetHealth(int Amount) { Health = Amount; }
        void AddGold(int Amount) { Gold += Amount; }

    private:
        // Think of these as player attributes.
        // Interacting with different objects determines how they are affected.
        int Health;
        int MaxHealth;
        int Gold;
    };

    // =========================================================================
    // 3. CONCRETE CLASSES (Extensions)
    // Here is the OPEN aspect of the OCP.
    // Infinite flexibility when adding objects that can be interacted with.
    // The Player does not need to be modified because it never defines the result 
    // of the interaction.
    // =========================================================================

    class Door : public IInteractable
    {
    public:
        Door();

        // What happens when the player interacts with a door
        // The door tells the player what happens.
        // It makes sense that the player does not get to tell the door
        // how it should work.
        void Interact(Player& PlayerRef) override;
        std::string GetName() const override { return "Wooden Door"; }

    private:
        bool IsOpen;
    };

    class Chest : public IInteractable
    {
    public:
        Chest(int GoldAmount);

        // What happens when the player interacts with a chest
        void Interact(Player& PlayerRef) override;
        std::string GetName() const override { return "Old Chest"; }

    private:
        bool IsLooted;
        int GoldAmount;
    };

    class WaterSpring : public IInteractable
    {
    public:
        // What happens when the player interacts with a water spring
        void Interact(Player& PlayerRef) override;
        std::string GetName() const override { return "Mystical Water Spring"; }
    };

    // =========================================================================
    // DEMO
    // =========================================================================
    void RunDemo();
}