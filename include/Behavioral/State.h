#pragma once

#include "FunctionLibrary/HelperFunctionLibrary.h"
#include <memory>
#include <string>
#include <iostream>

// =========================================================================
// BEHAVIORAL DESIGN PATTERNS: State Pattern
// =========================================================================
// "Allow an object to alter its behavior when its internal state changes. 
// The object will appear to change its class."
//
// THE GOAL:
// Eliminate massive 'if-else' or 'switch' blocks in the Update() function.
// Instead of one class handling every possible behavior, we encapsulate 
// each behavior into its own "State" object.
//
// THE EXAMPLE:
// Animation State Machine (Idle, Walk, Attack).
// 1. Context (Player): The character that "wears" the states.
// 2. State Interface (IPlayerState): The blueprint for all behaviors.
// 3. Concrete States (Idle, Walk, Attack): The actual logic for each behavior.
//
// THE SCENARIO:
// When the Player is 'Idle', the 'Attack' button triggers an animation.
// When the Player is 'Attacking', the 'Move' button is ignored because 
// the Attack state doesn't define movement logic.
//
// BENEFIT:
// 1. Organization: Each state's logic is isolated in its own class.
// 2. Flexibility: Adding a 'Jump' state doesn't require touching 'Idle' code.
// 3. Clarity: The transition rules are explicitly defined.
// =========================================================================

namespace STA
{
    class Player; // Forward declaration

    // ------------------------------------------------------------------------
    // 1. THE STATE INTERFACE
    // ------------------------------------------------------------------------
    // This defines the "API" that every state must implement.
    // The Player (Context) only speaks to this interface.
    class IPlayerState
    {
    public:
        virtual ~IPlayerState() = default;

        virtual void Enter(Player& P) = 0;                  // Logic when entering state
        virtual void Update(Player& P) = 0;                 // Logic called every frame
        virtual void HandleInput(Player& P, int Input) = 0; // Logic for button presses
        virtual void Exit(Player& P) = 0;                   // Logic when leaving state

        virtual std::string GetName() const = 0;
    };

    // ------------------------------------------------------------------------
    // 2. THE CONTEXT (Player)
    // ------------------------------------------------------------------------
    // This is the object whose behavior changes. It holds a pointer to 
    // the current state and delegates all work to it.
    class Player
    {
    public:
        Player();

        // The "Brain Swap": Exit old state, enter new one.
        void SetState(std::shared_ptr<IPlayerState> NewState);

        // Core Loop: The Player just asks the "Brain" what to do.
        void HandleInput(int Input);
        void Update();

        std::string GetCurrentStateName() const { return CurrentState->GetName(); }

    private:
        std::shared_ptr<IPlayerState> CurrentState;
    };

    // ------------------------------------------------------------------------
    // 3. CONCRETE STATES
    // ------------------------------------------------------------------------

    // --- IDLE STATE ---
    class IdleState : public IPlayerState {
    public:
        void Enter(Player& P) override;
        void Update(Player& P) override;
        void HandleInput(Player& P, int Input) override;
        void Exit(Player& P) override;
        std::string GetName() const override { return "IDLE"; }
    };

    // --- WALK STATE ---
    class WalkState : public IPlayerState {
    public:
        void Enter(Player& P) override;
        void Update(Player& P) override;
        void HandleInput(Player& P, int Input) override;
        void Exit(Player& P) override;
        std::string GetName() const override { return "WALK"; }
    };

    // --- ATTACK STATE ---
    class AttackState : public IPlayerState {
    public:
        void Enter(Player& P) override;
        void Update(Player& P) override;
        void HandleInput(Player& P, int Input) override;
        void Exit(Player& P) override;
        std::string GetName() const override { return "ATTACK"; }
    };

    // --- JUMP STATE ---
    class JumpState : public IPlayerState {
    public:
        void Enter(Player& P) override;
        void Update(Player& P) override;
        void HandleInput(Player& P, int Input) override;
        void Exit(Player& P) override;
        std::string GetName() const override { return "JUMP"; }
    };

    void RunDemo();
}