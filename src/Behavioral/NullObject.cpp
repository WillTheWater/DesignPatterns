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
        : ButtonName(Name), MySound(SoundEffect) {
    }

    void UIButton::Click()
    {
        std::cout << "[Button] User clicked: " << ButtonName << "\n";

        // ======================== THE KEY MOMENT ========================
        // Absence of null checks. The call is made directly.
        // NullSound instances result in silence; Real objects result in audio.
        MySound->Play();
    }

    // =========================================================================
    // DEMO RUNNER
    // =========================================================================

    void RunDemo()
    {
        // ======================== INTRODUCTION ========================
        HFL::ClearScreen();
        HFL::PrintHeader("NULL OBJECT DESIGN PATTERN");

        HFL::PrintSection("THE DEFINITION");
        HFL::SetColor(HFL::EColor::White);
        std::cout << "Provide an object as a surrogate for the lack of an object of a given type.\n"
            << "Enable the treatment of 'nothing' as a valid, functional state.\n\n";

        HFL::PrintSection("THE GOAL");
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "The Null Object Pattern is about 'Procedural Consistency'.\n"
            << "By providing a concrete 'do-nothing' instance, it eliminates the need\n"
            << "for repetitive 'if (ptr != nullptr)' checks throughout the engine.\n\n";

        HFL::PrintSection("THE EXAMPLE");
        HFL::SetColor(HFL::EColor::White);
        std::cout << "This demonstration features a UI Sound System with two behaviors:\n\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] REAL OBJECT:  ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Plays a physical .wav file and updates audio registers.\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] NULL OBJECT:  ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "A 'Silent' surrogate that satisfies the interface contract.\n\n";

        HFL::WaitForInput();

        // ======================== THE ARCHITECTURE ========================
        HFL::ClearScreen();
        HFL::PrintHeader("THE ARCHITECTURE");

        HFL::PrintSection("THE 'NO-OP' SURROGATE");
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Without a Null Object, every single UI interaction requires a safety guard\n"
            << "to prevent crashes. With a Null Object, 'silence' becomes a valid state\n"
            << "that can be passed around just like a real sound.\n\n";

        HFL::PrintSection("IMPLEMENTATION");

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] IUISound Interface\n";
        HFL::SetColor(HFL::EColor::White);
        std::cout << "    ROLE:           ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "The common language between the Button and the Audio Engine.\n\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] UIButton Context\n";
        HFL::SetColor(HFL::EColor::White);
        std::cout << "    SIMPLICITY:     ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Invokes behavior blindly. 100% agnostic of sound settings.\n";
        HFL::SetColor(HFL::EColor::White);
        std::cout << "    TYPE SAFETY:    ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Guaranteed to have a valid pointer at all times.\n\n";

        HFL::WaitForInput();

        auto RealEffect = std::make_shared<ButtonClickSound>();
        auto MutedEffect = std::make_shared<NullSound>();
        UIButton PlayButton("Start Game", RealEffect);
        bool IsMuted = false;

        // ======================== GAME LOOP ========================
        while (true)
        {
            HFL::ClearScreen();
            HFL::PrintHeader("UI SOUND SIMULATOR");

            HFL::PrintSection("SYSTEM SETTINGS");
            HFL::SetColor(HFL::EColor::White);
            std::cout << "  AUDIO STATE:     "; HFL::SetColor(IsMuted ? HFL::EColor::Red : HFL::EColor::Green);
            std::cout << (IsMuted ? "[MUTED]" : "[SOUND ON]") << "\n";

            HFL::SetColor(HFL::EColor::White);
            std::cout << "  ACTIVE OBJECT:   "; HFL::SetColor(HFL::EColor::Cyan);
            std::cout << PlayButton.GetSoundStatus() << "\n\n";

            HFL::PrintSection("COMMANDS");
            HFL::SetColor(HFL::EColor::Green);
            std::cout << " [1] "; HFL::SetColor(HFL::EColor::White); std::cout << "CLICK [" << PlayButton.GetButtonName() << "]\n";
            HFL::SetColor(HFL::EColor::Green);
            std::cout << " [2] "; HFL::SetColor(HFL::EColor::White); std::cout << "TOGGLE MUTE (SWAP OBJECT)\n\n";
            HFL::SetColor(HFL::EColor::Green);
            std::cout << " [0] "; HFL::SetColor(HFL::EColor::White); std::cout << "CONTINUE\n\n";

            int Choice = HFL::GetValidMenuInput(2);
            if (Choice == 0) break;

            if (Choice == 1)
            {
                std::cout << "\n--- LOG ---\n";
                HFL::SetColor(HFL::EColor::Gray);
                std::cout << "[Code] Executing: PlayButton.Click();\n";
                HFL::SetColor(HFL::EColor::White);
                PlayButton.Click();
                std::cout << "---------------------\n";
                HFL::WaitForInput();
            }
            else if (Choice == 2)
            {
                IsMuted = !IsMuted;
                if (IsMuted) PlayButton.SetSound(MutedEffect);
                else PlayButton.SetSound(RealEffect);

                std::cout << "\n>> System: Swapped to " << (IsMuted ? "Null Object" : "Real Object") << ".\n";
                HFL::WaitDots(0.5f);
            }
        }

        // ======================== CONCLUSION ========================
        HFL::ClearScreen();
        HFL::PrintHeader("CONCLUSION");

        HFL::PrintSection("ARCHITECTURE");
        HFL::SetColor(HFL::EColor::White);
        std::cout << "The implementation of the Null Object confirms the following:\n\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] PROCEDURAL CONSISTENCY: ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "The UIButton::Click() logic never changes. It remains\n"
            << "    agnostic of whether the sound system is active or muted.\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] ELIMINATION OF BRANCHING: ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "We successfully removed 'if(ptr != nullptr)' from the loop.\n"
            << "    Logic flow is now linear, predictable, and easier to test.\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] TYPE SAFETY:             ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "By ensuring the button always holds a valid instance, it\n"
            << "    architecturally prevent 'Null Reference' runtime crashes.\n\n";

        HFL::PrintSection("SUMMARY");
        HFL::SetColor(HFL::EColor::White);
        std::cout << "The Null Object Pattern ensures that software is:\n\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] ROBUST:    ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Crashes due to missing dependencies are eliminated.\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] CLEAN:     ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Business logic is not cluttered with safety checks.\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] PREDICTABLE:";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "The API always responds, even if the response is 'do nothing'.\n\n";

        HFL::SetColor(HFL::EColor::White);
        HFL::WaitForInput();
    }
}