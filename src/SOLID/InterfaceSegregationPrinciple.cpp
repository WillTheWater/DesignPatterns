#include "SOLID/InterfaceSegregationPrinciple.h"

namespace ISP
{
    // =========================================================================
    // SOUND EFFECT IMPLEMENTATION
    // =========================================================================
    Gunshot::Gunshot() {}

    void Gunshot::Play()
    {
        std::cout << " >> SFX: [BANG!]\n";
    }

    void Gunshot::Stop()
    {
        std::cout << " >> SFX: Gunshot audio terminated.\n";
    }

    void Gunshot::SetPosition3D(float x, float y, float z)
    {
        (void)z; // Suppress unused warning
        std::cout << "    (SFX spatialized at X: " << x << " | Y: " << y << ")\n";
    }

    // =========================================================================
    // MUSIC TRACK IMPLEMENTATION
    // =========================================================================
    MainMenuTheme::MainMenuTheme() : bIsPlaying{ false } {}

    void MainMenuTheme::Play()
    {
        bIsPlaying = true;
        std::cout << " >> MUSIC: Begins playing\n";
    }

    void MainMenuTheme::Stop()
    {
        bIsPlaying = false;
        std::cout << " >> MUSIC: Stops.\n";
    }

    void MainMenuTheme::SetLooping(bool ShouldLoop)
    {
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << (ShouldLoop ? "    (Playback Mode: Infinite Loop)\n" : "    (Playback Mode: Play Once)\n");
    }

    bool MainMenuTheme::IsPlaying() const { return bIsPlaying; }

    // =========================================================================
    // SOUND ENGINE IMPLEMENTATION
    // =========================================================================
    void AudioManager::PlayEffect(ISoundEffect* Effect)
    {
        HFL::PrintSection("PROCESSING SOUND EFFECT");
        // Because of ISP, the 'Effect' supports 3D positioning safely.
        Effect->SetPosition3D(10.5f, 3.0f, 0.0f);
        Effect->Play();
        HFL::SetColor(HFL::EColor::Cyan);
        std::cout << std::setfill('-') << std::setw(45) << "" << std::setfill(' ') << "\n";
    }

    void AudioManager::PlayMusic(IMusicTrack* Music)
    {
        HFL::PrintSection("PROCESSING MUSIC TRACK");

        if (Music->IsPlaying())
        {
            HFL::SetColor(HFL::EColor::White);
            std::cout << " >> Signal: Current track is active. Stopping...\n";
            Music->Stop();
        }
        else
        {
            HFL::SetColor(HFL::EColor::White);
            std::cout << " >> Signal: Channel idle. Initializing stream...\n";
            Music->SetLooping(true);
            Music->Play();
        }
        HFL::SetColor(HFL::EColor::Cyan);
        std::cout << std::setfill('-') << std::setw(45) << "" << std::setfill(' ') << "\n";
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
        HFL::PrintHeader("INTERFACE SEGREGATION PRINCIPLE");

        HFL::PrintSection("THE DEFINITION");
        HFL::SetColor(HFL::EColor::White);
        std::cout << "Clients should not be forced to depend on methods they do not use.\n\n";

        HFL::PrintSection("THE GOAL");
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "The Interface Segregation Principle (ISP) prevents the creation of\n"
            << "'Fat Interfaces' that force implementers to write 'garbage' code for\n"
            << "functions they don't need. By breaking large interfaces into smaller,\n"
            << "highly specific ones, it ensures that a class only knows \n"
            << "methods relevant to its actual behavior.\n\n";

        HFL::PrintSection("THE EXAMPLE");
        HFL::SetColor(HFL::EColor::White);
        std::cout << "This demonstration features a Game Audio System with two distinct\n"
            << "functional interfaces:\n\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] ISoundEffect: ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Logic for one-shot 3D spatial sounds (Gunshots).\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] IMusicTrack: ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Logic for persistent background audio (Music).\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] AUDIO MANAGER: ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "The consumer that utilizes these segregated roles.\n\n";

        HFL::PrintSection("THE BENEFIT");
        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] TYPE SAFETY: ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "You cannot accidentally 'Loop' a one-shot sound effect.\n";
        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] REDUCED COUPLING: ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Music updates don't force a recompile of SFX logic.\n";
        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] CLEAN CODE: ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Implementers have zero empty or 'NotImplemented' methods.\n";

        HFL::WaitForInput();

        // ======================== THE ARCHITECTURE ========================
        HFL::ClearScreen();
        HFL::PrintHeader("THE ARCHITECTURE");

        HFL::PrintSection("ROLE-BASED DESIGN");
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Instead of one 'IAudio' interface, it is decomposed into\n"
            << "specific capabilities. This allows the AudioManager to request exactly\n"
            << "what it needs and nothing more.\n\n";

        HFL::PrintSection("IMPLEMENTATION");

        // ======================== INTERFACE 1 ========================
        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] ISoundEffect (3D Role)\n";
        HFL::SetColor(HFL::EColor::White);
        std::cout << "    ROLE:           ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Handles spatial triggers and one-off execution.\n";
        HFL::SetColor(HFL::EColor::White);
        std::cout << "    SCOPE:          ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Play(), Stop(), SetPosition3D().\n";
        HFL::SetColor(HFL::EColor::White);
        std::cout << "    CONSTRAINT:     ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "No looping logic�SFX are inherently finite.\n\n";

        // ======================== INTERFACE 2 ========================
        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] IMusicTrack (Background Role)\n";
        HFL::SetColor(HFL::EColor::White);
        std::cout << "    ROLE:           ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Handles stateful, continuous audio playback.\n";
        HFL::SetColor(HFL::EColor::White);
        std::cout << "    SCOPE:          ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Play(), SetLooping(), IsPlaying().\n";
        HFL::SetColor(HFL::EColor::White);
        std::cout << "    CONSTRAINT:     ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "No 3D positioning�music is usually global/2D.\n\n";

        HFL::SetColor(HFL::EColor::White);
        HFL::WaitForInput();

        // ======================== INTERACTION ========================
        Gunshot GunSFX;
        MainMenuTheme MusicTrack;
        AudioManager AudioMgr;

        while (true)
        {
            HFL::ClearScreen();
            HFL::PrintHeader("AUDIO BOARD");

            HFL::PrintSection("SYSTEM STATUS");
            HFL::SetColor(HFL::EColor::White);
            std::cout << " [TRACK]: Main_Menu_v1.wav\n";
            std::cout << " [STATE]: " << (MusicTrack.IsPlaying() ? "PLAYING" : "STOPPED") << "\n\n";

            HFL::PrintSection("CONTROLS");
            HFL::SetColor(HFL::EColor::Green);
            std::cout << " [1] "; HFL::SetColor(HFL::EColor::White); std::cout << "TRIGGER SFX\n";
            HFL::SetColor(HFL::EColor::Green);
            std::cout << " [2] "; HFL::SetColor(HFL::EColor::White); std::cout << "TOGGLE MUSIC\n\n";
            HFL::SetColor(HFL::EColor::Green);
            std::cout << " [0] "; HFL::SetColor(HFL::EColor::White); std::cout << "CONTINUE\n\n";

            int Choice = HFL::GetValidMenuInput(2);
            if (Choice == 0) break;

            if (Choice == 1)
            {
                AudioMgr.PlayEffect(&GunSFX);
            }
            else if (Choice == 2)
            {
                AudioMgr.PlayMusic(&MusicTrack);
            }

            HFL::WaitForInput();
        }

        // ======================== CONCLUSION ========================
        HFL::ClearScreen();
        HFL::PrintHeader("CONCLUSION");

        HFL::PrintSection("ARCHITECTURE");
        HFL::SetColor(HFL::EColor::White);
        std::cout << "The segregation of the Audio interfaces confirms the following:\n\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] NO FAT INTERFACES: ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Classes only implement methods that serve their specific logic.\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] COMPILE-TIME SAFETY: ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "The compiler prevents passing a Music object to an SFX-only system.\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] GRANULAR CONTROL: ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Easily add a 'IVoiceLine' interface later without breaking\n"
            << "    the Gunshot or MainMenuTheme implementations.\n\n";

        HFL::PrintSection("SUMMARY");
        HFL::SetColor(HFL::EColor::White);
        std::cout << "The Interface Segregation Principle ensures that software is:\n\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] COHESIVE: ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Interfaces are focused and represent a single capability.\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] MAINTAINABLE: ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Changes to spatial audio logic do not impact music playback.\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] LEAN: ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Objects don't carry the overhead of unused, empty functions.\n\n";

        HFL::WaitForInput();
    }
}