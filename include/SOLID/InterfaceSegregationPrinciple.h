#pragma once

#include "FunctionLibrary/HelperFunctionLibrary.h"

// =========================================================================
// PRINCIPLE: Interface Segregation Principle (ISP)
// =========================================================================
// "Clients should not be forced to depend on methods they do not use."
//
// THE GOAL:
// Avoid "Fat Interfaces" where an interface tries to do too much.
// Split interfaces into smaller, specific roles.
//
// THE EXAMPLE:
// An Audio System.
// 1. Fat Interface (Bad): One 'IAudio' with Play, Loop, and SetPosition.
//    -> SFX would be forced to have empty Loop().
//    -> Music would be forced to have empty SetPosition().
// 2. Segregated Interfaces (Good):
//    -> ISoundEffect: Play, Stop, SetPosition (for 3D sounds).
//    -> IMusicTrack: Play, Stop, Loop (for background music).
//
// BENEFIT:
// The SoundEngine code is cleaner and less prone to errors.
// It is impossible to accidentally try to 'Loop' a Gunshot SFX.
// =========================================================================

namespace ISP
{
    // ------------------------------------------------------------------------
    // 1. SEGREGATED INTERFACES
    // ------------------------------------------------------------------------

    // Interface for short, positional sounds (Footsteps, Gunshots, Explosions)
    class ISoundEffect
    {
    public:
        virtual ~ISoundEffect() = default;
        virtual void Play() = 0;
        virtual void Stop() = 0;

        // 3D Audio: Only SFX needs this.
        virtual void SetPosition3D(float x, float y, float z) = 0;
    };

    // Interface for background audio (Music, Ambiance)
    class IMusicTrack
    {
    public:
        virtual ~IMusicTrack() = default;
        virtual void Play() = 0;
        virtual void Stop() = 0;

        // Looping: Only Music needs this. SFX plays once.
        virtual void SetLooping(bool ShouldLoop) = 0;
        virtual bool IsPlaying() const = 0;

    private:
        bool bIsPlaying;
    };

    // ------------------------------------------------------------------------
    // 2. CONCRETE IMPLEMENTATIONS
    // ------------------------------------------------------------------------

    class Gunshot : public ISoundEffect
    {
    public:
        Gunshot();

        void Play() override;
        void Stop() override;
        void SetPosition3D(float x, float y, float z) override;
    };

    class MainMenuTheme : public IMusicTrack
    {
    public:
        MainMenuTheme();

        void Play() override;
        void Stop() override;
        void SetLooping(bool ShouldLoop) override;
        bool IsPlaying() const override;

    private:
        bool bIsPlaying;
        
    };

    // ------------------------------------------------------------------------
    // 3. THE CLIENT (Sound Engine)
    // ------------------------------------------------------------------------
    class AudioManager
    {
    public:
        // Because of ISP, we can enforce correct usage.
        // You cannot pass a MusicTrack to the SFX player.
        void PlayEffect(ISoundEffect* Effect);

        // You cannot pass a SFX to the Music player.
        void PlayMusic(IMusicTrack* Music);
        
    };

    // ------------------------------------------------------------------------
    // DEMO
    // ------------------------------------------------------------------------
    void RunDemo();
}