#include "Behavioral/Command.h"

namespace CMD
{
    // =========================================================================
    // RECEIVER IMPLEMENTATION (The Worker)
    // ROLE: Performs the actual logic. It has no knowledge of the Command Pattern.
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
    // CONCRETE COMMANDS (The Action Objects)
    // ROLE: Encapsulate a specific request and its reverse logic (Undo).
    // =========================================================================

    // ======================== MOVE COMMAND ========================
    MoveCommand::MoveCommand(PlayerCharacter& Target, int Dist)
        : Receiver(Target), Distance(Dist)
    {
    }

    void MoveCommand::Execute()
    {
        Receiver.Move(Distance);
    }

    void MoveCommand::Undo()
    {
        HFL::SetColor(HFL::EColor::Cyan);
        std::cout << "   [Undo] Reversing Move Action...\n";
        HFL::SetColor(HFL::EColor::Gray);
        Receiver.Move(-Distance);
    }

    // ======================== JUMP COMMAND ========================
    JumpCommand::JumpCommand(PlayerCharacter& Target)
        : Receiver(Target)
    {
    }

    void JumpCommand::Execute()
    {
        Receiver.Jump();
    }

    void JumpCommand::Undo()
    {
        HFL::SetColor(HFL::EColor::Cyan);
        std::cout << "   [Undo] Reversing Jump (Visual Only)...\n";
        HFL::SetColor(HFL::EColor::Gray);
    }

    // =========================================================================
    // DEMO IMPLEMENTATION
    // =========================================================================

    void RunDemo()
    {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        // ======================== INTRODUCTION ========================
        HFL::ClearScreen();
        HFL::PrintHeader("COMMAND DESIGN PATTERN");

        HFL::PrintSection("THE DEFINITION");
        HFL::SetColor(HFL::EColor::White);
        std::cout << "Encapsulate a request as an object, thereby parameterizing\n"
            << "clients with different requests, queue or log requests, and support\n"
            << "undoable operations.\n\n";

        HFL::PrintSection("THE GOAL");
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "The Command Pattern is about 'Request Decoupling'. It turns a simple\n"
            << "function call into a first-class object. This allows it to be treated as actions\n"
            << "and data—storing them, moving them, and reversing them.\n\n";

        HFL::PrintSection("THE EXAMPLE");
        HFL::SetColor(HFL::EColor::White);
        std::cout << "This demonstration features a Game Input System with an Undo History:\n\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] THE RECEIVER:        ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "The PlayerCharacter who knows HOW to move and jump.\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] THE COMMANDS:        ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Move and Jump objects that hold the parameters for the action.\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] THE INVOKER:         ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "The InputHandler that triggers actions and manages the History Stack.\n\n";

        HFL::PrintSection("THE BENEFIT");
        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] TIME TRAVEL:     ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "By pushing commands onto a stack, we get 'Undo' functionality for free.\n";
        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] HOT-SWAPPING:    ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "We can remap 'Move' to 'Dash' at runtime by simply swapping an object.\n";

        HFL::WaitForInput();

        // ======================== THE ARCHITECTURE ========================
        HFL::ClearScreen();
        HFL::PrintHeader("THE ARCHITECTURE");

        HFL::PrintSection("THE 'HISTORY STACK'");
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Uses a std::stack<ICommand*> to track history. Because a stack is\n"
            << "LIFO (Last-In, First-Out), the most recent action is always at the top,\n"
            << "making it the perfect structure for an Undo system.\n\n";

        HFL::PrintSection("IMPLEMENTATION");

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] PlayerCharacter (The Receiver)\n";
        HFL::SetColor(HFL::EColor::White);
        std::cout << "    ROLE:           ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "The implementation engine. Knows the physics and the logic.\n\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] ICommand (The Abstract Contract)\n";
        HFL::SetColor(HFL::EColor::White);
        std::cout << "    ROLE:           ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Ensures every action provides both an Execute() and an Undo() method.\n\n";

        HFL::SetColor(HFL::EColor::White);
        HFL::WaitForInput();

        // ======================== INITIALIZATION ========================
        PlayerCharacter Hero;
        std::stack<std::shared_ptr<ICommand>> History;

        // ======================== INPUT ========================
        std::shared_ptr<ICommand> MOVE = std::make_shared<MoveCommand>(Hero, 10);
        std::shared_ptr<ICommand> JUMP = std::make_shared<JumpCommand>(Hero);

        // ======================== GAME LOOP ========================
        while (true)
        {
            HFL::ClearScreen();
            HFL::PrintHeader("INPUT SYSTEM");

            HFL::PrintSection("STATE");
            HFL::SetColor(HFL::EColor::White);
            std::cout << "  PLAYER POSITION: "; HFL::SetColor(HFL::EColor::Green);
            std::cout << Hero.GetPosition() << "\n";
            HFL::SetColor(HFL::EColor::White);
            std::cout << "  HISTORY DEPTH:   "; HFL::SetColor(HFL::EColor::Cyan);
            std::cout << History.size() << " commands recorded\n\n";

            HFL::PrintSection("INPUTS");
            HFL::SetColor(HFL::EColor::Green);
            std::cout << " [1] "; HFL::SetColor(HFL::EColor::White); std::cout << "PRESS [MOVE] BUTTON\n";
            HFL::SetColor(HFL::EColor::Green);
            std::cout << " [2] "; HFL::SetColor(HFL::EColor::White); std::cout << "PRESS [JUMP] BUTTON\n";
            HFL::SetColor(HFL::EColor::Green);
            std::cout << " [3] "; HFL::SetColor(HFL::EColor::White); std::cout << "PRESS [UNDO]\n";
            HFL::SetColor(HFL::EColor::Green);
            std::cout << " [4] "; HFL::SetColor(HFL::EColor::White); std::cout << "REMAP [MOVE] TO [DASH]\n";
            HFL::SetColor(HFL::EColor::Green);
            std::cout << " [5] "; HFL::SetColor(HFL::EColor::White); std::cout << "REMAP [DASH] TO [MOVE]\n\n";
            HFL::SetColor(HFL::EColor::Green);
            std::cout << " [0] "; HFL::SetColor(HFL::EColor::White); std::cout << "CONTINUE\n\n";

            int Choice = HFL::GetValidMenuInput(5);
            if (Choice == 0) break;

            HFL::PrintSection("EXECUTION LOG");
            HFL::SetColor(HFL::EColor::Gray);

            if (Choice == 1 || Choice == 2)
            {
                std::shared_ptr<ICommand> ActiveCmd = (Choice == 1) ? MOVE : JUMP;

                std::cout << ">> Invoker: Triggering command...\n";
                ActiveCmd->Execute();
                History.push(ActiveCmd);
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
                    HFL::SetColor(HFL::EColor::Red);
                    std::cout << "!! Error: No history to undo.\n";
                }
            }
            else if (Choice == 4)
            {
                MOVE = std::make_shared<MoveCommand>(Hero, 50);
                HFL::SetColor(HFL::EColor::Yellow);
                std::cout << ">> System: Input mapped. [MOVE] is now [DASH] (+50 units).\n";
            }
            else if (Choice == 5)
            {
                MOVE = std::make_shared<MoveCommand>(Hero, 10);
                HFL::SetColor(HFL::EColor::Yellow);
                std::cout << ">> System: Input mapped. [DASH] is now [MOVE] (+10 units).\n";
            }

            HFL::SetColor(HFL::EColor::White);
            HFL::WaitForInput();
        }

        // ======================== CONCLUSION ========================
        HFL::ClearScreen();
        HFL::PrintHeader("CONCLUSION");

        HFL::PrintSection("ARCHITECTURE");
        HFL::SetColor(HFL::EColor::White);
        std::cout << "The implementation of the Command Pattern confirms the following:\n\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] DECOUPLED INVOKER:       ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "The input loop never calls 'Hero.Move()'. It only interacts with\n"
            << "    the ICommand interface, making the UI independent of player logic.\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] COMMAND AS DATA:         ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Actions are now objects. This allowed us to store them in a stack,\n"
            << "    enabling a robust Undo system with zero extra effort.\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] RUNTIME FLEXIBILITY:     ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "We remapped the Move button to a Dash at runtime without changing\n"
            << "    a single line of the input-handling loop.\n\n";

        HFL::PrintSection("SUMMARY");
        HFL::SetColor(HFL::EColor::White);
        std::cout << "The Command Pattern ensures that software is:\n\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] EXTENSIBLE: ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Adding 'Crouch' or 'Attack' requires new classes, not new UI logic.\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] REVERSIBLE: ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "State changes are packaged with their own 'Self-Correction' logic.\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] VERSATILE:  ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Commands can be sent over a network, queued for later, or logged to disk.\n\n";

        HFL::SetColor(HFL::EColor::White);
        HFL::WaitForInput();
    }
}