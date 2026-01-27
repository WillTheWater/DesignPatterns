#pragma once

#include "FunctionLibrary/HelperFunctionLibrary.h"

// =========================================================================
// BEHAVIORAL DESIGN PATTERNS: NULL OBJECT
// =========================================================================
// "Provide an object as a surrogate for the lack of an object of a given type."
//
// THE GOAL:
// Elimination of 'nullptr' checks across the codebase. Instead of verifying 
// object existence before invocation, a "do-nothing" surrogate is provided.
//
// THE EXAMPLE:
// UI Button Sound System featuring three key workers:
// 1. THE INTERFACE (IUISound):   Defines the 'Play' contract.
// 2. THE REAL OBJECT (Click):    Executes standard audio logic.
// 3. THE NULL OBJECT (Muted):    Implements 'Play' as a no-op (No Operation).
//
// THE BENEFIT:
// [*] LINEAR LOGIC:     Client code calls 'sound->Play()' without if-checks.
// [*] CRASH PREVENTION: Null Pointer Exceptions are architecturally impossible.
// [*] FLEXIBILITY:      Toggle sounds by swapping objects, not changing logic.
// =========================================================================

namespace NUL
{
    // =========================================================================
    // THE INTERFACE (The Shared Contract)
    // ROLE: Defines the required behavior that both real and null objects
    // must implement. The Client only ever sees this abstraction.
    // =========================================================================
    class IUISound
    {
    public:
        virtual ~IUISound() = default;
        virtual void Play() = 0;
        virtual std::string GetStatus() const = 0;
    };

    // =========================================================================
    // THE REAL OBJECT (The Active Implementation)
    // ROLE: The specialist. Performs the actual work (playing audio) when 
    // the system is in an active state.
    // =========================================================================
    class ButtonClickSound : public IUISound
    {
    public:
        void Play() override;
        std::string GetStatus() const override { return "Active (Real Object)"; }
    };

    // =========================================================================
    // THE NULL OBJECT (The Silent Implementation)
    // ROLE: The surrogate. Implements the interface as a "No-Operation" to
    // satisfy dependencies without causing null pointer crashes.
    // =========================================================================
    class NullSound : public IUISound
    {
    public:
        void Play() override {}
        std::string GetStatus() const override { return "Muted (Null Object)"; }
    };

    // =========================================================================
    // THE CONTEXT (The Consumer)
    // ROLE: The high-level UI element. It invokes the sound behavior blindly,
    // relying on the Null Object to handle the "Muted" state safely.
    // =========================================================================
    class UIButton
    {
    public:
        UIButton(std::string Name, std::shared_ptr<IUISound> SoundEffect);

        void Click();
        void SetSound(std::shared_ptr<IUISound> NewSound) { MySound = NewSound; }

        std::string GetButtonName() const { return ButtonName; }
        std::string GetSoundStatus() const { return MySound->GetStatus(); }

    private:
        std::string ButtonName;
        std::shared_ptr<IUISound> MySound;
    };

    void RunDemo();
}