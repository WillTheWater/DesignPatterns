#include "Behavioral/Command.h"

namespace CMD
{
    // =========================================================================
    // RECEIVER IMPLEMENTATION
    // =========================================================================
    void PlayerCharacter::Move(int Distance)
    {
        Position += Distance;
        std::cout << "   [Player] Moved " << (Distance > 0 ? "Forward" : "Backward")
            << ". New Position: " << Position << "\n";
    }

    void PlayerCharacter::Jump()
    {
        std::cout << "   [Player] Jumped!\n";
    }

    // =========================================================================
    // CONCRETE COMMANDS
    // =========================================================================

    // Move Command
    MoveCommand::MoveCommand(PlayerCharacter& Target, int Dist)
        : Receiver(Target), Distance(Dist) {
    }

    void MoveCommand::Execute() { Receiver.Move(Distance); }

    void MoveCommand::Undo()
    {
        std::cout << "   [Undo] Reversing Move...\n";
        Receiver.Move(-Distance);
    }

    // Jump Command
    JumpCommand::JumpCommand(PlayerCharacter& Target) : Receiver(Target) {}

    void JumpCommand::Execute() { Receiver.Jump(); }

    void JumpCommand::Undo()
    {
        std::cout << "   [Undo] Reversing Jump...\n";
    }

    // =========================================================================
    // DEMO IMPLEMENTATION
    // =========================================================================
    void RunDemo()
    {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        // --- STEP 1: INTRODUCTION ---
        HFL::ClearScreen();
        HFL::PrintHeader("Command Pattern");

        std::cout << "Definition:\n";
        std::cout << "Encapsulate a request as an object, letting you parameterize\n";
        std::cout << "clients with different requests and support undoable operations.\n\n";

        std::cout << "In This Demo:\n";
        std::cout << "There is a Player (Receiver) and an Input Handler (Invoker).\n";
        std::cout << "That maps buttons to Commands and keeps a 'History Stack'\n";
        std::cout << "to allow for a full Undo system.\n";

        HFL::WaitForInput();

        // --- STEP 2: THE ROLES ---
        HFL::ClearScreen();
        HFL::PrintHeader("Step 1: The Roles");

        std::cout << "1. The Receiver (PlayerCharacter):\n";
        std::cout << "   - Does the actual work (Moving, Jumping).\n\n";

        std::cout << "2. The Command (ICommand):\n";
        std::cout << "   - The 'Order Slip' containing the logic to Execute and Undo.\n\n";

        std::cout << "3. The Invoker (Input System):\n";
        std::cout << "   - Doesn't know WHAT the command does, only how to trigger it.\n";

        HFL::WaitForInput();

        // Setup
        PlayerCharacter Hero;

        // ------------------------------------------------------------------------
        // THE COMMAND HISTORY (The Undo Stack)
        // ------------------------------------------------------------------------
        // std::stack?
        // 1. LIFO (Last-In, First-Out): The most recent command performed is 
        //    always the first one we want to Undo. A stack enforces this rule.
        // 2. Restricted Access: Unlike a Vector, you cannot accidentally undo 
        //    the 5th command back without undoing the most recent 4 first.
        // 3. Efficiency: Adding (push) and removing (pop) from the top are 
        //    extremely fast O(1) operations.
        //
        // Key Methods used in this Demo:
        // - .push(item): Adds a command to the top of the stack.
        // - .top():      Looks at the top command (but doesn't remove it).
        // - .pop():      Removes the top command from the stack.
        // - .empty():    Returns true if there's nothing left to undo.
        // ------------------------------------------------------------------------
        std::stack<std::shared_ptr<ICommand>> History;
        // ------------------------------------------------------------------------

        // Button Mappings (The "Invoker" setup)
        std::shared_ptr<ICommand> MOVE = std::make_shared<MoveCommand>(Hero, 10);
        std::shared_ptr<ICommand> JUMP = std::make_shared<JumpCommand>(Hero);

        // --- STEP 3: INTERACTIVE CONSTRUCTOR ---
        while (true)
        {
            HFL::ClearScreen();
            HFL::PrintHeader("Command Input System");

            std::cout << "Player Position: " << Hero.GetPosition() << "\n";
            std::cout << "History Size:    " << History.size() << "\n";
            std::cout << "--------------------------------------------------\n\n";

            std::cout << "1. Press [MOVE]\n";
            std::cout << "2. Press [JUMP]\n";
            std::cout << "3. Press [UNDO]\n";
            std::cout << "4. Remap [MOVE] to [Dash] (Changes Command at Runtime)\n";
            std::cout << "0. Exit\n\n";

            int Choice = HFL::GetValidMenuInput(4);
            if (Choice == 0) break;

            std::cout << "\n--- EXECUTION LOG ---\n";

            if (Choice == 1 || Choice == 2)
            {
                std::shared_ptr<ICommand> ActiveCmd = (Choice == 1) ? MOVE : JUMP;

                // EXECUTE and RECORD
                ActiveCmd->Execute();
                History.push(ActiveCmd);

                // --- AUTO-REVERT DASH ---
                if (Choice == 1)
                {
                    MOVE = std::make_shared<MoveCommand>(Hero, 10);
                }
            }
            else if (Choice == 3)
            {
                if (!History.empty())
                {
                    History.top()->Undo();
                    History.pop();
                }
                else
                {
                    std::cout << ">> History is empty. Nothing to undo!\n";
                }
            }
            else if (Choice == 4)
            {
                // RUNTIME REMAPPING
                MOVE = std::make_shared<MoveCommand>(Hero, 50);
                std::cout << ">> System: [MOVE] has been remapped to [Dash]\n";
            }

            std::cout << "---------------------\n";
            HFL::WaitForInput();
        }

        // --- STEP 5: CONCLUSION ---
        HFL::ClearScreen();
        HFL::PrintHeader("Conclusion");

        std::cout << "Summary:\n\n";
        std::cout << "1. Decoupling:\n";
        std::cout << "   The Input loop never calls 'Hero.Move()'. It only calls 'Command->Execute()'.\n\n";

        std::cout << "2. Undo Capability:\n";
        std::cout << "   By storing commands in a stack, we get a 'Time Machine' for free.\n\n";

        std::cout << "3. Runtime Flexibility:\n";
        std::cout << "   Remapping controls is as simple as swapping a pointer to a new Command object.\n\n";

        std::cout << std::setw(40) << "End of Demo\n";
        HFL::WaitForInput();
    }
}