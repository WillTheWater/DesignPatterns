#include "Structural/Bridge.h"

namespace BRG
{
    // =========================================================================
    // CONCRETE IMPLEMENTATIONS (The Hardware Bridges)
    // ROLE: Specialized workers that handle device-specific translation.
    // They "bridge" the gap between raw hardware signals and game commands.
    // =========================================================================

    // ======================== KEYBOARD ========================
    ECommand KeyboardDevice::Translate(int KeyCode) const
    {
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << ">> [Keyboard] Mapping Key Code: " << KeyCode << " to Action";
        HFL::WaitDots(0.3f);

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
        return "Keyboard";
    }

    // ======================== JOYPAD ========================
    ECommand JoypadDevice::Translate(int Axis) const
    {
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << ">> [Joypad] Parsing Axis Input: " << Axis;
        HFL::WaitDots(0.3f);

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
        return "Joypad";
    }

    // ======================== GAMEPAD ========================
    ECommand GamepadDevice::Translate(int ButtonID) const
    {
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << ">> [Gamepad] Reading Button Buffer: " << ButtonID;
        HFL::WaitDots(0.3f);

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
    // DEMO IMPLEMENTATION
    // =========================================================================

    void RunDemo()
    {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        // ======================== INTRODUCTION ========================
        HFL::ClearScreen();
        HFL::PrintHeader("BRIDGE DESIGN PATTERN");

        HFL::PrintSection("THE DEFINITION");
        HFL::SetColor(HFL::EColor::White);
        std::cout << "Decouple an abstraction from its implementation so that the\n"
            << "two can vary independently.\n\n";

        HFL::PrintSection("THE GOAL");
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "The Bridge Pattern is about 'Separation of Concerns'.\n"
            << "Allowing high-level Game Logic (The Abstraction)\n"
            << "and low-level Input Hardware (The Implementation) separately,\n"
            << "linking them via a stable interface (The Bridge).\n\n";

        HFL::PrintSection("THE EXAMPLE");
        HFL::SetColor(HFL::EColor::White);
        std::cout << "Hardware independent Input Mapping System where:\n\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] THE ABSTRACTION: ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "A 'GameInput' class that only cares about ECommands.\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] THE BRIDGE:      ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "The 'IInputDevice' interface that translates raw data.\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] THE HARDWARE:    ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Concrete classes for Keyboard, Joypad, and Gamepad.\n\n";

        HFL::WaitForInput();

        // ======================== THE ARCHITECTURE ========================
        HFL::ClearScreen();
        HFL::PrintHeader("THE ARCHITECTURE");

        HFL::PrintSection("INDEPENDENT VARIATION");
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "By placing a 'Bridge' between the Logic and the Hardware, it avoids\n"
            << "an exponential explosion of classes. The Game Logic can change its\n"
            << "internal structure without needing to know how a Joypad is wired.\n\n";

        HFL::PrintSection("IMPLEMENTATION");

        // ======================== THE ABSTRACTION ========================
        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] GameInput (The Abstraction)\n";
        HFL::SetColor(HFL::EColor::White);
        std::cout << "    ROLE:           ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "The high-level UI/Logic. It only speaks in 'Commands'.\n";
        HFL::SetColor(HFL::EColor::White);
        std::cout << "    FLEXIBILITY:    ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Can switch its internal hardware reference at runtime.\n\n";

        // ======================== THE IMPLEMENTORS ========================
        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] Input Devices (The Implementors)\n";
        HFL::SetColor(HFL::EColor::White);
        std::cout << "    ROLE:           ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Platform-specific workers. They handle the messy raw data.\n";
        HFL::SetColor(HFL::EColor::White);
        std::cout << "    DECOUPLING:     ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "They don't know about the game; they only know how to translate.\n\n";

        HFL::SetColor(HFL::EColor::White);
        HFL::WaitForInput();

        // ======================== INITIALIZATION ========================
        KeyboardDevice MyKeyboard;
        JoypadDevice MyJoypad;
        GamepadDevice MyGamepad;
        GameInput InputAbstraction(&MyKeyboard);

        // ======================== INTERACTIVE LOOP ========================
        while (true)
        {
            HFL::ClearScreen();
            HFL::PrintHeader("INPUT MAPPING");

            HFL::PrintSection("CURRENT CONFIGURATION");
            HFL::SetColor(HFL::EColor::White);
            std::cout << "  ACTIVE DEVICE:  ";
            HFL::SetColor(HFL::EColor::Green);
            std::cout << InputAbstraction.GetActiveDeviceName() << "\n\n";

            HFL::PrintSection("HARDWARE SELECTION");
            HFL::SetColor(HFL::EColor::Green);
            std::cout << " [1] "; HFL::SetColor(HFL::EColor::White); std::cout << "KEYBOARD\n";
            HFL::SetColor(HFL::EColor::Green);
            std::cout << " [2] "; HFL::SetColor(HFL::EColor::White); std::cout << "JOYPAD\n";
            HFL::SetColor(HFL::EColor::Green);
            std::cout << " [3] "; HFL::SetColor(HFL::EColor::White); std::cout << "GAMEPAD\n\n";

            HFL::PrintSection("COMMAND MENU");
            HFL::SetColor(HFL::EColor::Cyan);
            std::cout << " [4] "; HFL::SetColor(HFL::EColor::White); std::cout << "MOVE FORWARD\n";
            HFL::SetColor(HFL::EColor::Cyan);
            std::cout << " [5] "; HFL::SetColor(HFL::EColor::White); std::cout << "MOVE BACKWARDS\n";
            HFL::SetColor(HFL::EColor::Cyan);
            std::cout << " [6] "; HFL::SetColor(HFL::EColor::White); std::cout << "MOVE RIGHT\n";
            HFL::SetColor(HFL::EColor::Cyan);
            std::cout << " [7] "; HFL::SetColor(HFL::EColor::White); std::cout << "MOVE LEFT\n\n";

            HFL::SetColor(HFL::EColor::Red);
            std::cout << " [0] "; HFL::SetColor(HFL::EColor::White); std::cout << "CONTINUE\n\n";

            int Choice = HFL::GetValidMenuInput(7);
            if (Choice == 0) break;

            if (Choice == 1) InputAbstraction.SetDevice(&MyKeyboard);
            else if (Choice == 2) InputAbstraction.SetDevice(&MyJoypad);
            else if (Choice == 3) InputAbstraction.SetDevice(&MyGamepad);

            if (Choice >= 4 && Choice <= 7)
            {
                int RawSignal = Choice - 3;

                HFL::PrintSection("PROCESSING INPUT");
                HFL::SetColor(HFL::EColor::White);
                std::cout << ">> [Game Logic] Requesting translation for raw signal: " << RawSignal << "\n";

                ECommand Result = InputAbstraction.HandleInput(RawSignal);

                HFL::SetColor(HFL::EColor::Green);
                std::cout << ">> [Action] Executing: ";

                switch (Result)
                {
                case ECommand::Forward:   std::cout << "Character Moves Forward\n"; break;
                case ECommand::Backwards: std::cout << "Character Moves Backwards\n"; break;
                case ECommand::Right:     std::cout << "Character Moves Right\n"; break;
                case ECommand::Left:      std::cout << "Character Moves Left\n"; break;
                default:                  std::cout << "No Action Mapped\n"; break;
                }
                HFL::WaitForInput();
            }
        }

        // ======================== CONCLUSION ========================
        HFL::ClearScreen();
        HFL::PrintHeader("CONCLUSION");

        HFL::PrintSection("ARCHITECTURE");
        HFL::SetColor(HFL::EColor::White);
        std::cout << "The implementation of the Bridge Pattern confirms the following:\n\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] INDEPENDENT SCALING:    ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Could add a 'VR Controller' hardware implementations without\n"
            << "    ever modifying the high-level 'GameInput' abstraction.\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] RUNTIME FLEXIBILITY:    ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "The Abstraction can swap its internal implementation pointer instantly,\n"
            << "    allowing players to change hardware without restarting the logic.\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] ABSTRACTION PURITY:     ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "The Game Logic remains pure. It only understands 'ECommand', with\n"
            << "    zero knowledge of raw codes, axes, or platform-specific APIs.\n\n";

        HFL::PrintSection("SUMMARY");
        HFL::SetColor(HFL::EColor::White);
        std::cout << "The Bridge Pattern ensures that software is:\n\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] DECOUPLED: ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "The 'Logic' is separated from the 'Hardware'.\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] EXTENSIBLE:";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Adding new platforms doesn't cause a 'Class Explosion'.\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] CLEAN:     ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Maintains the Single Responsibility Principle across hierarchies.\n\n";

        HFL::SetColor(HFL::EColor::White);
        HFL::WaitForInput();
    }
}