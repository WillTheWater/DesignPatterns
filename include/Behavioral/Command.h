#pragma once

#include "FunctionLibrary/HelperFunctionLibrary.h"

// =========================================================================
// BEHAVIORAL DESIGN PATTERN: Command
// =========================================================================
// "Encapsulate a request as an object, thereby letting you parameterize 
// clients with different requests, queue or log requests, and support 
// undoable operations."
//
// THE GOAL:
// Decouple the object that triggers the action (Invoker) from the object 
// that performs the action (Receiver). This turns a 'function call' into 
// a 'data object' that can be stored, moved, or reversed.
//
// THE BENEFIT:
// * Undo/Redo: Since commands are objects, they can be stored in a history stack.
// * Deferred Execution: Commands can be queued and executed at a later time.
// * Composition: Complex sequences can be built by combining multiple commands.
// * Flexibility: The Invoker (Input System) doesn't need to know how 'Jump' works.
//
// THE EXAMPLE:
// [PlayerCharacter]: The Receiver. Knows the physics of moving and jumping.
// [ICommand]: The Interface. Defines the contract for Execute and Undo.
// [MoveCommand]: Concrete implementation. Stores the state needed to reverse movement.
// =========================================================================

namespace CMD
{
    // =========================================================================
    // THE RECEIVER (The Worker)
    // ROLE: Performs the actual logic. It has no knowledge of the Command Pattern.
    // =========================================================================
    class PlayerCharacter
    {
    public:
        void Move(int Distance);
        void Jump();

        int GetPosition() const { return Position; }
        void SetPosition(int NewPos) { Position = NewPos; }

    private:
        int Position = 0;
    };

    // =========================================================================
    // THE COMMAND INTERFACE
    // ROLE: Defines the 'Remote Control' buttons that all concrete actions 
    // must implement, allowing for polymorphic execution and history tracking.
    // =========================================================================
    class ICommand
    {
    public:
        virtual ~ICommand() = default;

        virtual void Execute() = 0;
        virtual void Undo() = 0;
    };

    // =========================================================================
    // CONCRETE COMMANDS (The Action Objects)
    // ROLE: Link a specific action to a Receiver and store the state required 
    // to perform (or reverse) that action.
    // =========================================================================

    // ======================== MOVE COMMAND ========================
    class MoveCommand : public ICommand
    {
    public:
        MoveCommand(PlayerCharacter& Target, int Distance);

        void Execute() override;
        void Undo() override;

    private:
        PlayerCharacter& Receiver;
        int Distance;
    };

    // ======================== JUMP COMMAND ========================
    class JumpCommand : public ICommand
    {
    public:
        JumpCommand(PlayerCharacter& Target);

        void Execute() override;
        void Undo() override;

    private:
        PlayerCharacter& Receiver;
    };

    // =========================================================================
    // DEMO IMPLEMENTATION
    // =========================================================================
    void RunDemo();
}