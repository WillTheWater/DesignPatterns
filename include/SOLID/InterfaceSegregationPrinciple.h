#pragma once

#include "FunctionLibrary/HelperFunctionLibrary.h"

// =========================================================================
// SOLID DESIGN PRINCIPLE: Interface Segregation Principle (ISP)
// =========================================================================
// "Clients should not be forced to depend on methods they do not use."
//
// THE GOAL:
// To avoid "Fat Interfaces"—large, monolithic interfaces that force classes 
// to implement dummy or empty methods for functionality they don't need. 
// Instead, we split interfaces into smaller, specialized roles.
//
// THE BENEFIT:
// * Flexibility: Systems are easier to refactor because changes to one 
//   interface don't affect unrelated implementations.
// * Safety: Prevents illegal state transitions (e.g., trying to loop a 
//   one-shot gunshot sound).
// * Readability: Classes clearly signal their purpose by which specific 
//   interfaces they implement.
//
// THE EXAMPLE:
// [ISoundEffect]: Interface for short, positional sounds (Spatial 3D Audio).
// [IMusicTrack]: Interface for long-form audio (Looping/State Management).
// [AudioManager]: A client that enforces strict role-based audio playback.
// =========================================================================

namespace ISP
{
    // =========================================================================
    // SEGREGATED INTERFACES
    // We split Audio into two roles. One for 3D FX, one for Background Music.
    // =========================================================================

    // Role: Short, trigger-based audio with 3D spatial properties.
    class ISoundEffect
    {
    public:
        virtual ~ISoundEffect() = default;
        virtual void Play() = 0;
        virtual void Stop() = 0;

        // 3D Audio: Only spatial effects need coordinate tracking.
        virtual void SetPosition3D(float x, float y, float z) = 0;
    };

    // Role: Continuous audio with looping logic and playback state.
    class IMusicTrack
    {
    public:
        virtual ~IMusicTrack() = default;
        virtual void Play() = 0;
        virtual void Stop() = 0;

        // Looping: Background music requires persistence logic.
        virtual void SetLooping(bool ShouldLoop) = 0;
        virtual bool IsPlaying() const = 0;
    };

    // =========================================================================
    // CONCRETE IMPLEMENTATIONS
    // =========================================================================

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

    // =========================================================================
    // THE CLIENT: AUDIO MANAGER
    // =========================================================================
    class AudioManager
    {
    public:
        // ISP enforces that only valid sound types are passed to specific logic.
        void PlayEffect(ISoundEffect* Effect);
        void PlayMusic(IMusicTrack* Music);
    };

    void RunDemo();
}