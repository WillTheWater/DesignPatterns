#include "Behavioral/NullObject.h"

namespace NUL
{
    // =========================================================================
    // IMPLEMENTATIONS
    // =========================================================================

    void ButtonClickSound::Play()
    {
        std::cout << "[AUDIO] ** CLICK! ** (Sound Plays)\n";
    }

    UIButton::UIButton(std::string Name, std::shared_ptr<IUISound> SoundEffect)
        : ButtonName(Name), MySound(SoundEffect)
    {
        // The pointer remains valid regardless of the behavior logic.
    }

    void UIButton::Click()
    {
        std::cout << "[Button] User clicked: " << ButtonName << "\n";

        // --- THE KEY MOMENT ---
        // Absence of null checks. The call is made directly.
        // NullSound instances result in silence; Real objects result in audio.
        MySound->Play();
    }

    // =========================================================================
    // 5. DEMO IMPLEMENTATION
    // =========================================================================
    void RunDemo()
    {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        // --- STEP 1: INTRODUCTION ---
        HFL::ClearScreen();
        HFL::PrintHeader("Null Object Pattern (UI System)");

        std::cout << "Definition:\n";
        std::cout << "Provide an object as a surrogate for the lack of an object of a given type.\n";
        std::cout << "Enable the treatment of 'nothing' as a valid, functional state.\n\n";

        std::cout << "In This Demo:\n";
        std::cout << "A UIButton class interacts with an IUISound interface.\n";
        std::cout << "Behavior alternates between real audio output and a Null Object surrogate.\n";
        std::cout << "The client code remains agnostic of the audio state.\n";

        HFL::WaitForInput();

        // --- STEP 2: THE RULES ---
        HFL::ClearScreen();
        HFL::PrintHeader("Step 1: The Architecture");

        std::cout << "To recognize a Null Object, there are 4 key ideas:\n\n";

        std::cout << "1. Shared Interface:\n";
        std::cout << "   Both the real object and the null object implement the same base interface.\n";
        std::cout << "   The client interacts only with this abstraction.\n\n";

        std::cout << "2. No-Op Implementation:\n";
        std::cout << "   The null object implements all interface methods as 'No Operations' (empty).\n";
        std::cout << "   It provides expected default values (e.g., return 0 or empty string).\n\n";

        std::cout << "3. Elimination of Branching:\n";
        std::cout << "   The pattern removes 'if (ptr != nullptr)' checks from the business logic.\n";
        std::cout << "   Logic flow remains linear and predictable.\n\n";

        std::cout << "4. Type Safety:\n";
        std::cout << "   The system always possesses a valid instance.\n";
        std::cout << "   Runtime crashes due to null pointer dereferencing are architecturally prevented.\n";

        HFL::WaitForInput();

        // Setup the two behaviors
        auto RealEffect = std::make_shared<ButtonClickSound>();
        auto MutedEffect = std::make_shared<NullSound>();

        // The button starts with sound ENABLED
        UIButton PlayButton("Start Game", RealEffect);
        bool IsMuted = false;

        // --- STEP 3: DYNAMIC LOOP ---
        while (true)
        {
            HFL::ClearScreen();
            HFL::PrintHeader("UI Sound Simulator");

            std::cout << "SYSTEM SETTINGS:\n";
            std::cout << ">> Audio State: " << (IsMuted ? "[MUTED]" : "[SOUND ON]") << "\n";
            std::cout << ">> Active Object: " << PlayButton.GetSoundStatus() << "\n";
            std::cout << "--------------------------------------------------\n\n";

            std::cout << "Select an action:\n";
            std::cout << "1. Click [" << PlayButton.GetButtonName() << "]\n";
            std::cout << "2. Toggle Mute (Swap Object)\n";
            std::cout << "0. Exit\n\n";

            int Choice = HFL::GetValidMenuInput(2);
            if (Choice == 0) break;

            if (Choice == 1)
            {
                std::cout << "\n--- EXECUTION LOG ---\n";
                std::cout << "[Code] Executing: PlayButton.Click();\n";
                PlayButton.Click(); // This code NEVER CHANGES
                std::cout << "---------------------\n";
            }
            else if (Choice == 2)
            {
                IsMuted = !IsMuted;
                // --- THE PATTERN IN ACTION ---
                // We physically swap the object inside the button.
                if (IsMuted) PlayButton.SetSound(MutedEffect);
                else PlayButton.SetSound(RealEffect);

                std::cout << "\n>> System: Swapped to " << (IsMuted ? "Null Object" : "Real Object") << ".\n";
            }

            HFL::WaitForInput();
        }

        // --- STEP 4: CONCLUSION ---
        HFL::ClearScreen();
        HFL::PrintHeader("Conclusion");

        std::cout << "Summary of Null Object:\n\n";

        std::cout << "1. Procedural Consistency.\n";
        std::cout << "   UIButton::Click() remains static regardless of audio settings.\n";
        std::cout << "   The invocation 'MySound->Play()' requires no conditional guards.\n\n";

        std::cout << "2. Structural Integrity.\n";
        std::cout << "   The risk of Null Pointer Exceptions is eliminated by design.\n";
        std::cout << "   The system treats 'absence of behavior' as a first-class object.\n\n";

        std::cout << "3. Adherence to Interface.\n";
        std::cout << "   The NullSound class satisfies the IUISound contract.\n";
        std::cout << "   The client interacts with a valid, predictable API at all times.\n\n";

        std::cout << std::setw(40) << "End of Demo\n";
        HFL::WaitForInput();
    }
}