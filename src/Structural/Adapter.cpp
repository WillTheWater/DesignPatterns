#include "Structural/Adapter.h"

namespace ADP
{
    // =========================================================================
    // LOW LEVEL MODULE: KEYBOARD (ADAPTEE)
    // ROLE: Simulates the raw OS hardware driver speaking in KeyCodes.
    // =========================================================================

    bool Keyboard::IsKeyPressed(int KeyCode) const
    {
        // Hardware signal simulation
        switch (KeyCode)
        {
        case 32:  // Space
        case 87:  // W
        case 65:  // A
        case 83:  // S
        case 68:  // D
            HFL::SetColor(HFL::EColor::Gray);
            std::cout << "   [OS-HW] Register " << KeyCode << " ... SIGNAL HIGH (Pressed)\n";
            return true;
        default:
            HFL::SetColor(HFL::EColor::Gray);
            std::cout << "   [OS-HW] Register " << KeyCode << " ... SIGNAL LOW (Released)\n";
            return false;
        }
    }

    // =========================================================================
    // STRUCTURAL MODULE: KEYBOARD INPUT ADAPTER
    // ROLE: Translates EInputAction (High-Level) into KeyCodes (Low-Level).
    // =========================================================================

    KeyboardInputAdapter::KeyboardInputAdapter(Keyboard* OSKeyboard)
        : OSKeyboard(OSKeyboard)
    {
    }

    bool KeyboardInputAdapter::IsActionPressed(EInputAction Action) const
    {
        HFL::SetColor(HFL::EColor::Yellow);
        HFL::PrintSection("INPUT ADAPTER TRANSLATION");

        int KeyCode = -1;
        std::string MappingName = "";

        // The Translation Logic: Mapping Actions to Hardware Keys
        switch (Action)
        {
        case EInputAction::Forward:
            std::cout << "   [Adapter] Action: Forward -> Key: W\n";
            KeyCode = 87;
            break;
        case EInputAction::MoveLeft:
            std::cout << "   [Adapter] Action: MoveLeft -> Key: A\n";
            KeyCode = 65;
            break;
        case EInputAction::MoveDown:
            std::cout << "   [Adapter] Action: MoveDown -> Key: S\n";
            KeyCode = 83;
            break;
        case EInputAction::MoveRight:
            std::cout << "   [Adapter] Action: MoveRight -> Key: D\n";
            KeyCode = 68;
            break;
        case EInputAction::Jump:
            std::cout << "   [Adapter] Action: Jump -> Key: Space\n";
            KeyCode = 32;
            break;
        default:
            std::cout << "   [Adapter] Action: Unknown -> No Mapping Found\n";
            return false;
        }

        // Delegating the work to the Adaptee (The Hardware)
        bool bPressed = OSKeyboard->IsKeyPressed(KeyCode);

        if (bPressed)
        {
            HFL::SetColor(HFL::EColor::Green);
            std::cout << "   >> [Game Logic] Action Validated: Execute Gameplay Code\n";
        }

        HFL::SetColor(HFL::EColor::Yellow);
        std::cout << "--------------------------------------------------\n";

        return bPressed;
    }

    // =========================================================================
    // DEMO IMPLEMENTATION
    // =========================================================================

    void RunDemo()
    {
        // Clear input buffer
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        // ======================== INTRODUCTION ========================
        HFL::ClearScreen();
        HFL::PrintHeader("ADAPTER PATTERN");

        HFL::PrintSection("THE DEFINITION");
        HFL::SetColor(HFL::EColor::White);
        std::cout << "Match interfaces of incompatible classes.\n"
            << "Create a translator between different interfaces.\n\n";

        HFL::PrintSection("THE GOAL");
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "The Adapter Pattern bridges the gap between the Game Logic (Actions)\n"
            << "and the OS Hardware (KeyCodes). By implementing a Target Interface,\n"
            << "the Adapter allows the Game Logic to stay 'pure' and unaware of\n"
            << "specific hardware implementation details.\n\n";

        HFL::PrintSection("THE PLAYERS");
        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] TARGET INTERFACE: "; HFL::SetColor(HFL::EColor::Gray);
        std::cout << "IInputDevice (The contract the Boss expects).\n";
        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] THE ADAPTEE:      "; HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Keyboard (The hardware speaking a foreign language).\n";
        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] THE ADAPTER:      "; HFL::SetColor(HFL::EColor::Gray);
        std::cout << "KeyboardInputAdapter (The bilingual translator).\n\n";

        HFL::WaitForInput();

        // ======================== THE ARCHITECTURE ========================
        HFL::ClearScreen();
        HFL::PrintHeader("THE ARCHITECTURE");

        HFL::PrintSection("STEP 1: THE INCOMPATIBILITY");
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "The Client wants to call: IsActionPressed(Jump).\n"
            << "The Hardware only understands: IsKeyPressed(32).\n";
        HFL::SetColor(HFL::EColor::Red);
        std::cout << "[!] Result: Direct communication is impossible due to type mismatch.\n\n";

        HFL::PrintSection("STEP 2: THE ADAPTER SOLUTION");
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "1. Create 'KeyboardInputAdapter' implementing IInputDevice.\n"
            << "2. Wrap the 'Keyboard' object inside the Adapter.\n"
            << "3. Map 'Jump' to '32' inside the override method.\n\n";

        HFL::SetColor(HFL::EColor::White);
        HFL::WaitForInput();

        // ======================== INITIALIZATION ========================
        auto KeyboardDevice = std::make_unique<Keyboard>();
        auto Adapter = std::make_unique<KeyboardInputAdapter>(KeyboardDevice.get());
        IInputDevice* Input = Adapter.get(); // Polymorphic Interface

        // ======================== INTERACTIVE LOOP ========================
        while (true)
        {
            HFL::ClearScreen();
            HFL::PrintHeader("INPUT ADAPTER");

            HFL::PrintSection("PHYSICAL INPUTS (MAPPED ACTIONS)");
            HFL::SetColor(HFL::EColor::Green);
            std::cout << " [W] "; HFL::SetColor(HFL::EColor::White); std::cout << "MOVE FORWARD\n";
            HFL::SetColor(HFL::EColor::Green);
            std::cout << " [A] "; HFL::SetColor(HFL::EColor::White); std::cout << "MOVE LEFT\n";
            HFL::SetColor(HFL::EColor::Green);
            std::cout << " [S] "; HFL::SetColor(HFL::EColor::White); std::cout << "MOVE DOWN\n";
            HFL::SetColor(HFL::EColor::Green);
            std::cout << " [D] "; HFL::SetColor(HFL::EColor::White); std::cout << "MOVE RIGHT\n";
            HFL::SetColor(HFL::EColor::Green);
            std::cout << " [Space] "; HFL::SetColor(HFL::EColor::White); std::cout << "JUMP\n\n";
            HFL::SetColor(HFL::EColor::Green);
            std::cout << " [0] "; HFL::SetColor(HFL::EColor::White); std::cout << "EXIT DEMO\n\n";

            std::cout << "Press Key: ";
            char Choice;
            std::cin >> std::noskipws >> Choice;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            if (Choice == '0') break;

            EInputAction Action = EInputAction::None;
            switch (Choice)
            {
            case 'w': case 'W': Action = EInputAction::Forward;   break;
            case 'a': case 'A': Action = EInputAction::MoveLeft;  break;
            case 's': case 'S': Action = EInputAction::MoveDown;  break;
            case 'd': case 'D': Action = EInputAction::MoveRight; break;
            case ' ':           Action = EInputAction::Jump;      break;
            default:            continue;
            }

            HFL::PrintSection("PROCESS FLOW");
            std::cout << ">> [GameEngine] Querying IInputDevice Interface";
            HFL::WaitDots(0.5);
            Input->IsActionPressed(Action);

            HFL::WaitForInput();
        }

        // ======================== CONCLUSION ========================
        HFL::ClearScreen();
        HFL::PrintHeader("CONCLUSION");

        HFL::PrintSection("ARCHITECTURE");
        HFL::SetColor(HFL::EColor::White);
        std::cout << "The implementation of the Adapter Pattern confirms the following:\n\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] INTERFACE UNIFICATION: ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "The engine only talks to 'IInputDevice'. It doesn't care if the\n"
            << "    source is a Keyboard, a Gamepad, or a Network Stream.\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] REUSABILITY:           ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "The 'Keyboard' class (Adaptee) was never modified. We adapted\n"
            << "    it externally, preserving third-party or legacy code integrity.\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] DECOUPLING:            ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "KeyCodes (like '87' for W) are isolated inside the Adapter.\n"
            << "    High-level logic uses clean, readable enums like 'Forward'.\n\n";

        HFL::PrintSection("SUMMARY");
        HFL::SetColor(HFL::EColor::White);
        std::cout << "The Adapter Pattern ensures that software is:\n\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] COMPATIBLE: "; HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Incompatible APIs can work together seamlessly.\n";
        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] EXTENSIBLE: "; HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Adding support for a VR Controller just means writing one new Adapter.\n";
        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] CLEAN:      "; HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Prevents low-level hardware from leaking into game systems.\n\n";

        HFL::SetColor(HFL::EColor::White);
        HFL::WaitForInput();
    }
}