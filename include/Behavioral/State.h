#pragma once

#include "FunctionLibrary/HelperFunctionLibrary.h"

// =========================================================================
// BEHAVIORAL DESIGN PATTERN: State
// =========================================================================
// "Allow an object to alter its behavior when its internal state changes. 
// The object will appear to change its class."
//
// THE GOAL:
// Eliminate massive 'if-else' or 'switch' blocks (Finite State Machines) 
// inside a core Update() function. Instead of one class handling every 
// possible logic branch, we encapsulate each behavior into its own 
// "State" object that the "Context" can swap out at runtime.
//
// THE BENEFIT:
// * Organization: Each state's logic is isolated in its own class.
// * Flexibility: Adding a new state (e.g., 'Crouch') does not require 
//   modifying the code for 'Idle' or 'Walk'.
// * Clarity: State transition rules are explicitly defined within the 
//   states themselves, making the workflow easier to follow.
//
// THE EXAMPLE:
// [IPlayerState]: The Interface. Defines the contract for all behaviors.
// [Concrete States]: Idle, Walk, Attack, and Jump. Each handles its own input.
// [Player]: The Context. It holds a pointer to the current "Brain" (State) 
//   and delegates all gameplay calls to it.
// =========================================================================

namespace STA
{
    class Player; // Forward declaration for State/Context relationship

    // =========================================================================
    // THE STATE INTERFACE
    // ROLE: Defines the "API" that every concrete behavior must implement.
    // The Player (Context) only communicates with this abstraction.
    // =========================================================================
    class IPlayerState
    {
    public:
        virtual ~IPlayerState() = default;

        // Lifecycle methods for state transitions
        virtual void Enter(Player& P) = 0;
        virtual void Update(Player& P) = 0;
        virtual void HandleInput(Player& P, int Input) = 0;
        virtual void Exit(Player& P) = 0;

        virtual std::string GetName() const = 0;
    };

    // =========================================================================
    // THE CONTEXT (The Player)
    // ROLE: The object whose behavior changes. It maintains the current state
    // and facilitates the "Brain Swap" (State Transition).
    // =========================================================================
    class Player
    {
    public:
        Player();

        // THE MASTER SWITCH: Exit old logic, enter new logic.
        void SetState(std::shared_ptr<IPlayerState> NewState);

        // DELEGATION: The Player doesn't know "how" to walk; it asks the State.
        void HandleInput(int Input);
        void Update();

        std::string GetCurrentStateName() const { return CurrentState->GetName(); }

    private:
        std::shared_ptr<IPlayerState> CurrentState;
    };

    // =========================================================================
    // CONCRETE STATES
    // ROLE: Specialized objects that contain the actual logic for a behavior.
    // They decide when it is time to transition to a different state.
    // =========================================================================

    // ======================== IDLE STATE ========================
    class IdleState : public IPlayerState
    {
    public:
        void Enter(Player& P) override;
        void Update(Player& P) override;
        void HandleInput(Player& P, int Input) override;
        void Exit(Player& P) override;
        std::string GetName() const override { return "IDLE"; }
    };

    // ======================== WALK STATE ========================
    class WalkState : public IPlayerState
    {
    public:
        void Enter(Player& P) override;
        void Update(Player& P) override;
        void HandleInput(Player& P, int Input) override;
        void Exit(Player& P) override;
        std::string GetName() const override { return "WALK"; }
    };

    // ======================== ATTACK STATE ========================
    class AttackState : public IPlayerState
    {
    public:
        void Enter(Player& P) override;
        void Update(Player& P) override;
        void HandleInput(Player& P, int Input) override;
        void Exit(Player& P) override;
        std::string GetName() const override { return "ATTACK"; }
    };

    // ======================== JUMP STATE ========================
    class JumpState : public IPlayerState
    {
    public:
        void Enter(Player& P) override;
        void Update(Player& P) override;
        void HandleInput(Player& P, int Input) override;
        void Exit(Player& P) override;
        std::string GetName() const override { return "JUMP"; }
    };

    // =========================================================================
    // DEMO IMPLEMENTATION
    // =========================================================================
    void RunDemo();
}