#include "SOLID/InterfaceSegregationPrinciple.h"

namespace ISP
{
    // =========================================================================
    // SOUND EFFECT IMPLEMENTATION
    // =========================================================================
    Gunshot::Gunshot()
    {}

    void Gunshot::Play()
    {
        std::cout << ">> SFX: [BANG!]\n";
    }

    void Gunshot::Stop()
    {
        std::cout << ">> SFX: Gunshot stopped.\n";
    }

    void Gunshot::SetPosition3D(float x, float y, float z)
    {
        (void)x; (void)z; // Suppress unused warning
        std::cout << "   (SFX positioned at X:" << x << " Y:" << y << ")\n";
    }

    // =========================================================================
    // MUSIC TRACK IMPLEMENTATION
    // =========================================================================
    MainMenuTheme::MainMenuTheme()
        : bIsPlaying{false}
    {}

    void MainMenuTheme::Play()
    {
        bIsPlaying = true;
        std::cout << ">> Music: [Intro Begins]\n";
    }

    void MainMenuTheme::Stop()
    {
        bIsPlaying = false;
        std::cout << ">> Music: Track stopped.\n";
    }

    void MainMenuTheme::SetLooping(bool ShouldLoop)
    {
        if (ShouldLoop)
        {
            std::cout << "   (Music Looping Enabled)\n";
        }
        else
        {
            std::cout << "   (Music Play Once)\n";
        }
    }

    bool MainMenuTheme::IsPlaying() const
    {
        return bIsPlaying;
    }

    // =========================================================================
    // SOUND ENGINE IMPLEMENTATION
    // =========================================================================
    void AudioManager::PlayEffect(ISoundEffect* Effect)
    {
        std::cout << "\n--- Processing Sound Effect ---\n";
        // Safely call SetPosition because it's a SoundEffect
        Effect->SetPosition3D(10.5f, 3.0f, 0.0f);
        Effect->Play();
        std::cout << "------------------------------\n";
    }

    void AudioManager::PlayMusic(IMusicTrack* Music)
    {
        std::cout << "\n--- Processing Music Track ---\n";

        // TOGGLE LOGIC:
        // If it is already playing, its stops.
        // If it is stopped, start it.
        if (Music->IsPlaying())
        {
            std::cout << ">> Stopping currently playing track.\n";
            Music->Stop();
        }
        else
        {
            std::cout << ">> Starting new track.\n";
            Music->SetLooping(true);
            Music->Play();
        }
        std::cout << "------------------------------\n";
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
        HFL::PrintHeader("Interface Segregation Principle (ISP)");

        std::cout << "Definition:\n";
        std::cout << "Users should not be forced to depend on methods they do not use.\n";
        std::cout << "Avoid 'Fat Interfaces'.\n\n";

        std::cout << "In This Demo:\n";
        std::cout << "There is an Audio System.\n";
        std::cout << "With separate 'Sound Effects' (3D) from 'Music' (Looping).\n";
        std::cout << "This prevents empty methods and confusing code.\n";

        HFL::WaitForInput();

        // --- STEP 2: THE SETUP ---
        HFL::ClearScreen();
        HFL::PrintHeader("Step 1: The Architecture");

        std::cout << "There are two Interfaces instead of one Fat one.\n\n";

        std::cout << "1. ISoundEffect (SFX):\n";
        std::cout << "   - Methods: Play, Stop, SetPosition3D.\n";
        std::cout << "   - Used by: Gunshots, Footsteps.\n\n";

        std::cout << "2. IMusicTrack (Music):\n";
        std::cout << "   - Methods: Play, Stop, SetLooping.\n";
        std::cout << "   - Used by: Themes, Background Audio.\n\n";

        std::cout << "ISP Compliance:\n";
        std::cout << "   A Gunshot does NOT have a Loop method.\n";
        std::cout << "   Music does NOT have a 3D Position method.\n";

        HFL::WaitForInput();

        // --- STEP 3: INTERACTIVE SOUNDBOARD ---

        Gunshot SFX;
        MainMenuTheme Theme;
        AudioManager AudioMgr;

        bool InDemo = true;
        while (InDemo)
        {
            HFL::ClearScreen();
            HFL::PrintHeader("Audio Engine");

            std::cout << "Select an audio type to play:\n\n";
            std::cout << "1. Play Sound Effect (Gunshot)\n";
            std::cout << "2. Play Music Track (Theme)\n";
            std::cout << "0. Exit Demo\n";
            std::cout << "\nChoice: ";

            int Choice;
            std::cin >> Choice;

            if (std::cin.fail()) { std::cin.clear(); std::cin.ignore(); continue; }

            if (Choice == 0) break;

            if (Choice == 1)
            {
                // Pass specifically to Effect Player
                AudioMgr.PlayEffect(&SFX);
            }
            else if (Choice == 2)
            {
                // Pass specifically to Music Player
                AudioMgr.PlayMusic(&Theme);
            }

            HFL::WaitForInput();
        }

        // --- STEP 4: CONCLUSION ---
        HFL::ClearScreen();
        HFL::PrintHeader("Conclusion");

        std::cout << "Summary of ISP:\n\n";
        std::cout << "1. SoundEngine has specific methods.\n";
        std::cout << "   'PlayEffect' accepts ONLY ISoundEffect.\n";
        std::cout << "   'PlayMusic' accepts ONLY IMusicTrack.\n\n";

        std::cout << "2. No Empty Methods.\n";
        std::cout << "   Gunshot never implements 'Loop'.\n";
        std::cout << "   Theme never implements 'SetPosition3D'.\n\n";

        std::cout << "3. Safer Code.\n";
        std::cout << "   The compiler prevents you from calling Loop on a Gunshot.\n";
        std::cout << "   This is Interface Segregation.\n\n";

        std::cout << std::setw(40) << "End of Demo\n";
        HFL::WaitForInput();
    }
}