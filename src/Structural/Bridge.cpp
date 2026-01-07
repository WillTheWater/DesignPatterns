#include "Structural/Bridge.h"

namespace BRG
{
    // --- KEYBOARD DEVICE ---
    ECommand KeyboardDevice::Translate(int KeyCode) const
    {
        std::cout << ">> [Keyboard] Checking Key Code: " << KeyCode << "\n";

        switch (KeyCode)
        {
        case 1: return ECommand::Forward;
        case 2: return ECommand::Backwards;
        case 3: return ECommand::Right;
        case 4: return ECommand::Left;
        default: return ECommand::None;
        }
    }

    std::string KeyboardDevice::GetDeviceName() const
    {
        return "Physical Keyboard";
    }

    // --- JOYPAD DEVICE ---
    ECommand JoypadDevice::Translate(int Axis) const
    {
        std::cout << ">> [Joypad] Reading Axis: " << Axis << "\n";

        switch (Axis)
        {
        case 1: return ECommand::Forward;
        case 2: return ECommand::Backwards;
        case 3: return ECommand::Right;
        case 4: return ECommand::Left;
        default: return ECommand::None;
        }
    }

    std::string JoypadDevice::GetDeviceName() const
    {
        return "Physical Joypad";
    }

    // --- GAMEPAD DEVICE ---
    ECommand GamepadDevice::Translate(int ButtonID) const
    {
        std::cout << ">> [Gamepad] Reading Button ID: " << ButtonID << "\n";

        switch (ButtonID)
        {
        case 1: return ECommand::Forward;
        case 2: return ECommand::Backwards;
        case 3: return ECommand::Right;
        case 4: return ECommand::Left;
        default: return ECommand::None;
        }
    }

    std::string GamepadDevice::GetDeviceName() const
    {
        return "Gamepad";
    }

    // =========================================================================
    // 3. DEMO IMPLEMENTATION
    // =========================================================================
    void RunDemo()
    {
        // Clear buffer
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        // --- STEP 1: INTRODUCTION ---
        HFL::ClearScreen();
        HFL::PrintHeader("Bridge Pattern (Input System)");

        std::cout << "Definition:\n";
        std::cout << "Decouple an abstraction from its implementation.\n";
        std::cout << "Allow the two to vary independently.\n\n";

        std::cout << "In This Demo:\n";
        std::cout << "The Game Logic (WASD Commands).\n";
        std::cout << "With Physical Input Hardware (Keyboard, Joypad, Gamepad).\n";
        std::cout << "That uses a Bridge to map Hardware to Commands.\n";
        std::cout << "With the ability to swap Hardware without effecting Game Logic.\n";

        HFL::WaitForInput();

        // --- STEP 2: THE ARCHITECTURE ---
        HFL::ClearScreen();
        HFL::PrintHeader("Step 1: The Architecture");

        std::cout << "There are 3 distinct layers:\n\n";

        std::cout << "1. The Abstraction (Game Logic):\n";
        std::cout << "   - Defines the (ECommand) input actions.\n";
        std::cout << "   - The Game Logic only knows about Commands.\n";
        std::cout << "   - It doesn't know about hardware buttons.\n\n";

        std::cout << "2. The Implementations (The Hardware):\n";
        std::cout << "   - Keyboard: The Bridge for WASD.\n";
        std::cout << "   - Joypad: The Bridge for Axis.\n";
        std::cout << "   - Gamepad: The Bridge for Buttons.\n\n";

        std::cout << "THE BRIDGE LOGIC:\n";
        std::cout << "1. Game Logic sends Commands.\n";
        std::cout << "2. Keyboard translates 'Foreward' -> 'W' Key.\n";
        std::cout << "3. Joypad translates 'Foreward' -> 'Up Axis'.\n";
        std::cout << "4. Gamepad translates 'Foreward' -> 'Up Stick'.\n\n";

        HFL::WaitForInput();

        // --- STEP 3: INTERACTIVE SYSTEM ---

        // Create the Bridges (Hardware)
        KeyboardDevice MyKeyboard;
        JoypadDevice MyJoypad;
        GamepadDevice MyGamepad;

        // Pointers to allow switching Bridges
        IInputDevice* ActiveDevice = &MyKeyboard;

        bool InDemo = true;
        while (InDemo)
        {
            HFL::ClearScreen();
            HFL::PrintHeader("Bridge Pattern (Input Mapper)");

            std::cout << "Select a Hardware Bridge:\n";
            std::cout << "1. Keyboard\n";
            std::cout << "2. Joypad\n";
            std::cout << "3. Gamepad\n";
            std::cout << "0. Exit Demo\n";
            std::cout << "\nChoice: ";

            int Choice;
            std::cin >> Choice;

            if (std::cin.fail()) { std::cin.clear(); std::cin.ignore(); continue; }

            if (Choice == 0) break;

            // SWITCH THE BRIDGE (DEPENDENCY INVERSION)
            // The Game Logic (User) talks to "ActiveDevice" (Abstraction).
            // Change "ActiveDevice" to point to different Hardware (Implementation).
            if (Choice == 1) ActiveDevice = &MyKeyboard;
            else if (Choice == 2) ActiveDevice = &MyJoypad;
            else if (Choice == 3) ActiveDevice = &MyGamepad;

            std::cout << "\nActive Device: " << ActiveDevice->GetDeviceName() << "\n";

            // --- STEP 4: COMMAND INPUT ---
            HFL::PrintHeader("Select a Command");

            std::cout << "1. Foreward\n";
            std::cout << "2. Backwards\n";
            std::cout << "3. Right\n";
            std::cout << "4. Left\n";
            std::cout << "0. Back to Hardware Selection\n";
            std::cout << "\nChoice: ";

            int CommandChoice;
            std::cin >> CommandChoice;

            if (std::cin.fail()) { std::cin.clear(); std::cin.ignore(); continue; }

            if (CommandChoice == 0) continue; // Go back to Hardware selection

            if (CommandChoice >= 1 && CommandChoice <= 4)
            {
                std::cout << "\n>> [Game Logic] Sending Raw Input ID: " << CommandChoice << "\n";

                ECommand Result = ActiveDevice->Translate(CommandChoice);

                if (Result == ECommand::Forward) std::cout << ">> [Game Logic] Forward!\n";
                else if (Result == ECommand::Backwards) std::cout << ">> [Game Logic] Backwards!\n";
                else if (Result == ECommand::Right) std::cout << ">> [Game Logic] Right!\n";
                else if (Result == ECommand::Left) std::cout << ">> [Game Logic] Left!\n";

                HFL::WaitForInput();
            }

        }

        // --- STEP 5: CONCLUSION ---
        HFL::ClearScreen();
        HFL::PrintHeader("Conclusion");

        std::cout << "Summary of Bridge Pattern:\n\n";
        std::cout << "1. Decoupling (The Abstraction):\n";
        std::cout << "   The Game Logic (Client) only knows EComands.\n";
        std::cout << "   It doesn't know what hardware it uses.\n\n";

        std::cout << "2. Independent Variation (The Hardware):\n";
        std::cout << "   When swapping devices.\n";
        std::cout << "   The Bridge translates commands.\n";
        std::cout << "   The Game Logic code is not changed.\n\n";

        std::cout << "3. Structure (The Connection):\n";
        std::cout << "   Uses 'IInputDevice' (The Abstraction).\n";
        std::cout << "   The hardware becomes (The Bridges).\n";
        std::cout << "   Which are connected via 'ActiveDevice' pointer (The Wiring).\n\n";

        std::cout << "This is the Bridge Pattern.\n";

        std::cout << std::setw(40) << "End of Demo\n";
        HFL::WaitForInput();
    }
}