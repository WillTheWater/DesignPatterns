#include "Structural/Adapter.h"

namespace ADP
{
    // =========================================================================
    // ADAPEE IMPLEMENTATION
    // =========================================================================
    bool Keyboard::IsKeyPressed(int KeyCode) const
    {

        switch (KeyCode)
        {
        case 32:  // Space
        case 87:  // W
        case 65:  // A
        case 83:  // S
        case 68:  // D
            std::cout << "   [OS] Reading Key " << KeyCode << " ... PRESSED\n";
            return true;
        default:
            std::cout << "   [OS] Reading Key " << KeyCode << " ... RELEASED\n";
            return false;
        }
    }

    // =========================================================================
    // ADAPTER IMPLEMENTATION
    // =========================================================================
    KeyboardInputAdapter::KeyboardInputAdapter(Keyboard* OSKeyboard)
        : OSKeyboard(OSKeyboard)
    {
    }

    bool KeyboardInputAdapter::IsActionPressed(EInputAction Action) const
    {
        std::cout << "----------------------------------------\n";
        std::cout << "        INPUT ADAPTER LOGIC\n";
        std::cout << "----------------------------------------\n";

        int KeyCode = -1;

        switch (Action)
        {
        case EInputAction::Forward:
            std::cout << "   [Adapter] Forward -> W (87)\n";
            KeyCode = 87;
            break;
        case EInputAction::MoveLeft:
            std::cout << "   [Adapter] Left -> A (65)\n";
            KeyCode = 65;
            break;
        case EInputAction::MoveDown:
            std::cout << "   [Adapter] Back -> S (83)\n";
            KeyCode = 83;
            break;
        case EInputAction::MoveRight:
            std::cout << "   [Adapter] Right -> D (68)\n";
            KeyCode = 68;
            break;
        case EInputAction::Jump:
            std::cout << "   [Adapter] Jump -> Space (32)\n";
            KeyCode = 32;
            break;
        default:
            std::cout << "   [Adapter] Invalid Action\n";
            return false;
        }

        bool bPressed = OSKeyboard->IsKeyPressed(KeyCode);

        if (bPressed)
        {
            if (Action == EInputAction::Jump)
                std::cout << "   >> [Game Logic] jump!\n";
            else
                std::cout << "   >> [Game Logic] Move\n";
        }

        std::cout << "----------------------------------------\n";
        return bPressed;
    }

    // =========================================================================
    // DEMO IMPLEMENTATION
    // =========================================================================
    void RunDemo()
    {
        // Clear buffer
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        // --- STEP 1: INTRODUCTION ---
        HFL::ClearScreen();
        HFL::PrintHeader("Adapter Pattern");

        std::cout << "Definition:\n";
        std::cout << "Match interfaces of incompatible classes.\n";
        std::cout << "Create a translator between different interfaces.\n\n";

        std::cout << "In This Demo:\n";
        std::cout << "Game Logic (Actions) are bound to OS Hardware (Keys).\n";
        std::cout << "Mapping specific WASD keys to Movement Actions.\n";
        std::cout << "The Game Logic doesn't know about KeyCodes.\n";

        HFL::WaitForInput();

        // --- STEP 2: THE INCOMPATIBILITY ---
        HFL::ClearScreen();
        HFL::PrintHeader("Step 1: The Incompatibility");

        std::cout << "There are two systems that don't speak the same language.\n\n";

        std::cout << "1. Game Logic (The Client) wants Actions:\n";
        std::cout << "   enum EInputAction { Jump, Attack, Forward, Backwards... }\n";
        std::cout << "   It calls: IsActionPressed(MoveUp)\n\n";

        std::cout << "2. OS Keyboard (The Hardware) returns KeyCodes:\n";
        std::cout << "   bool IsKeyPressed(int KeyCode)\n";
        std::cout << "   It returns: 32 (Space), 87 (Up), 83 (Down)...\n\n";

        std::cout << "THE PROBLEM:\n";
        std::cout << "   'MoveUp' (Action) cannot be passed to 'IsKeyPressed' (Key).\n";
        std::cout << "   The types don't match.\n";
        std::cout << "   Hardcoded keys into Game Logic, prevents support for other input methods.\n";

        HFL::WaitForInput();

        // --- STEP 3: THE ADAPTER SOLUTION ---
        HFL::ClearScreen();
        HFL::PrintHeader("Step 2: The Adapter Solution");

        std::cout << "Create a 'KeyboardInputAdapter' class.\n\n";

        std::cout << "1. It Implements IInputDevice (Target Interface).\n";
        std::cout << "   This makes it compatible with Game Logic.\n\n";

        std::cout << "2. It 'Has-A' (Wraps) a Keyboard (Adaptee).\n";
        std::cout << "   It holds the raw OS Keyboard object.\n\n";

        std::cout << "3. The 'IsActionPressed' method acts as a Translator:\n";
        std::cout << "   - Input: MoveUp (Action)\n";
        std::cout << "   - Logic: 'If (Action == MoveUp) return IsKeyPressed(W)'\n";

        HFL::WaitForInput();

        // --- STEP 4: INTERACTIVE SYSTEM ---

        auto KeyboardDevice = std::make_unique<Keyboard>();
        auto Adapter = std::make_unique<KeyboardInputAdapter>(KeyboardDevice.get());
        IInputDevice* Input = Adapter.get();

        bool bRunning = true;
        while (bRunning)
        {
            HFL::ClearScreen();
            HFL::PrintHeader("WASD Input Adapter");

            std::cout << "Choose Action:\n";
            std::cout << "W - Move Up\n";
            std::cout << "A - Move Left\n";
            std::cout << "S - Move Down\n";
            std::cout << "D - Move Right\n";
            std::cout << "Space - Jump\n";
            std::cout << "0 - Exit\n\n";
            std::cout << "Input: ";

            char Choice;
            std::cin >> std::noskipws >> Choice;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            if (Choice == '0')
                break;

            EInputAction Action = EInputAction::None;

            switch (Choice)
            {
            case 'w': case 'W': Action = EInputAction::Forward; break;
            case 'a': case 'A': Action = EInputAction::MoveLeft; break;
            case 's': case 'S': Action = EInputAction::MoveDown; break;
            case 'd': case 'D': Action = EInputAction::MoveRight; break;
            case ' ':           Action = EInputAction::Jump; break;
            default:            continue;
            }

            std::cout << ">> [Game Logic] Querying input device...\n";
            Input->IsActionPressed(Action);

            HFL::WaitForInput();
        }

        // --- STEP 5: CONCLUSION ---
        HFL::ClearScreen();
        HFL::PrintHeader("Conclusion");

        std::cout << "Summary of Adapter Pattern:\n\n";

        std::cout << "1. Translation:\n";
        std::cout << "   Game Logic calls (Action).\n";
        std::cout << "   Adapter translated this to (KeyCode).\n";
        std::cout << "   OS Keyboard performed the read.\n\n";

        std::cout << "2. Decoupling:\n";
        std::cout << "   Game Logic is independent of KeyCodes.\n";
        std::cout << "   A Gamepad Adapter can be added easily.\n";
        std::cout << "   Game Logic would never change.\n\n";

        std::cout << "3. Structure:\n";
        std::cout << "   Uses an Enum (EInputAction) for clarity.\n";
        std::cout << "   Adapter used Switch(Map) to translate Actions.\n";

        std::cout << std::setw(40) << "End of Demo\n";
        HFL::WaitForInput();
    }
}