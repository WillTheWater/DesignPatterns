#pragma once

#include "FunctionLibrary/HelperFunctionLibrary.h"

// =========================================================================
// BEHAVIORAL DESIGN PATTERNS: Memento (Delta Memento)
// =========================================================================
// "Without violating encapsulation, capture and externalize an object's 
// internal state so that the object can be restored to this state later."
//
// THE GOAL:
// Provide a "save/restore" mechanism that doesn't expose the object's 
// private variables. The Delta variant optimizes this by only storing
// values that have changed from a known baseline.
//
// THE EXAMPLE:
// A Level Transition System.
// 1. The Originator (Player): Has private stats like Health and Buffs.
// 2. The Memento (PlayerDelta): A package containing only changed stats.
// 3. The Caretaker (PlayerInstance): Persistent object that carries the 
//    memento across level loads while the Player object is destroyed.
//
// THE SCENARIO:
// The player moves from Level 1 to Level 2. The Level 1 Player object is 
// deleted. We capture a Delta Memento of their "volatile" state (damage 
// taken, active buffs) and apply it to a brand new Player object in Level 2.
// =========================================================================

namespace MEM
{
    struct PlayerState
    {
        int Health = 100;
        int Mana = 50;
        int PosX = 0;
        int PosY = 0;
        std::string ActiveBuff = "[ None ]";
    };

    // =========================================================================
    // 1. THE MEMENTO (The Snapshot)
    // =========================================================================
    // Uses std::optional to represent "No Change" from baseline.
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
    // 2. THE ORIGINATOR (The Object to Save)
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

        // Memento Logic
        std::unique_ptr<PlayerDeltaMemento> CaptureDelta(const PlayerState& Baseline);
        void ApplyDelta(const PlayerDeltaMemento& Delta);

        void DisplayStats() const;

    private:
        PlayerState CurrentState;
    };

    // =========================================================================
    // 3. THE CARETAKER (The Manager)
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

    void RunDemo();
}