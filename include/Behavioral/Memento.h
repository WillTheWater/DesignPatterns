#pragma once

#include "FunctionLibrary/HelperFunctionLibrary.h"

// =========================================================================
// BEHAVIORAL DESIGN PATTERN: Memento (Delta Memento Variant)
// =========================================================================
// "Without violating encapsulation, capture and externalize an object's 
// internal state so that the object can be restored to this state later."
//
// THE GOAL:
// Provide a "save/restore" mechanism that doesn't expose the object's 
// private variables or internal structure. The Delta variant optimizes 
// this by only storing values that have changed from a known baseline, 
// saving memory and processing time.
//
// THE BENEFIT:
// * Encapsulation: The Caretaker (storage) never sees the internal logic 
//   of the Originator (player).
// * Undo/Redo: Provides a clean way to revert state without complexity.
// * Efficiency: Delta Mementos only track "dirty" fields, making them 
//   lightweight for network synchronization or frequent autosaves.
//
// THE EXAMPLE:
// [PlayerState]: The Data Structure. Represents the full internal state.
// [PlayerDeltaMemento]: The Snapshot. Contains std::optional fields to 
//   store only the differences from the baseline state.
// [Player]: The Originator. The active object that creates and consumes
//   mementos to persist state across destruction boundaries.
// [PlayerInstance]: The Caretaker. A persistent manager that holds the 
//   memento while the main player object is destroyed or reloaded.
// =========================================================================

namespace MEM
{
    // =========================================================================
    // THE INTERNAL STATE
    // ROLE: A private data structure representing the full state of a player.
    // =========================================================================
    struct PlayerState
    {
        int Health = 100;
        int Mana = 50;
        int PosX = 0;
        int PosY = 0;
        std::string ActiveBuff = "[ None ]";
    };

    // =========================================================================
    // THE MEMENTO (The Snapshot)
    // ROLE: An opaque package containing state data. Using std::optional 
    // ensures it only stores and applies fields that have actually changed.
    // =========================================================================
    struct PlayerDeltaMemento
    {
        std::optional<int> Health;
        std::optional<int> Mana;
        std::optional<int> PosX;
        std::optional<int> PosY;
        std::optional<std::string> ActiveBuff;

        bool IsEmpty() const { return !Health && !Mana && !PosX && !PosY && !ActiveBuff; }
    };

    // =========================================================================
    // THE ORIGINATOR (The Object to Save)
    // ROLE: The object that does the work. It knows how to "compress" its
    // state into a memento and how to "reconstitute" itself from one.
    // =========================================================================
    class Player
    {
    public:
        Player(PlayerState Baseline);

        // Gameplay modifications
        void TakeDamage(int Amount);
        void UseMana(int Amount);
        void Move(int DistanceX, int DistanceY);
        void SetBuff(std::string Icon);

        // Memento Logic: The Originator is the only one who handles the Memento contents.
        std::unique_ptr<PlayerDeltaMemento> CaptureDelta(const PlayerState& Baseline);
        void ApplyDelta(const PlayerDeltaMemento& Delta);

        void DisplayStats() const;

    private:
        PlayerState CurrentState;
    };

    // =========================================================================
    // THE CARETAKER (The Manager)
    // ROLE: Responsible for the memento's safekeeping. It never operates on
    // or even looks inside the memento; it simply holds it.
    // =========================================================================
    class PlayerInstance
    {
    public:
        void StoreDelta(std::unique_ptr<PlayerDeltaMemento> Delta) { SavedDelta = std::move(Delta); }
        const PlayerDeltaMemento* GetDelta() const { return SavedDelta.get(); }
        void Clear() { SavedDelta.reset(); }

    private:
        std::unique_ptr<PlayerDeltaMemento> SavedDelta;
    };

    // =========================================================================
    // DEMO IMPLEMENTATION
    // =========================================================================
    void RunDemo();
}