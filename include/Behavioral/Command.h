#pragma once

#include "FunctionLibrary/HelperFunctionLibrary.h"

// =========================================================================
// BEHAVIORAL DESIGN PATTERNS: Command
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
// THE EXAMPLE:
// Game Input System.
// 1. The Receiver (PlayerCharacter): Knows HOW to move or jump.
// 2. The Command Interface (ICommand): Defines 'Execute' and 'Undo'.
// 3. Concrete Commands (MoveCommand, JumpCommand): Links a specific action 
//    to the Receiver.
// 4. The Invoker (InputHandler): Holds commands and triggers them based 
//    on user input.
// =========================================================================

namespace CMD
{
    // ------------------------------------------------------------------------
    // 1. THE RECEIVER
    // ------------------------------------------------------------------------
    // This is the object that actually performs the logic. 
    // It doesn't know about commands.
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

    // ------------------------------------------------------------------------
    // 2. THE COMMAND INTERFACE
    // ------------------------------------------------------------------------
    class ICommand
    {
    public:
        virtual ~ICommand() = default;
        virtual void Execute() = 0;
        virtual void Undo() = 0;
    };

    // ------------------------------------------------------------------------
    // 3. CONCRETE COMMANDS
    // ------------------------------------------------------------------------

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

    class JumpCommand : public ICommand
    {
    public:
        JumpCommand(PlayerCharacter& Target);
        void Execute() override;
        void Undo() override;

    private:
        PlayerCharacter& Receiver;
    };

    void RunDemo();
}