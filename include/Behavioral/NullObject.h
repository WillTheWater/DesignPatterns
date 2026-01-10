#pragma once

#include "FunctionLibrary/HelperFunctionLibrary.h"

// =========================================================================
// BEHAVIORAL DESIGN PATTERNS: Null Object
// =========================================================================
// "Provide an object as a surrogate for the lack of an object of a given type."
//
// THE GOAL:
// Elimination of 'nullptr' checks across the codebase. Instead of verifying 
// object existence before invocation, a "do-nothing" surrogate is provided.
//
// THE EXAMPLE:
// UI Button Sound System.
// 1. The Interface (IUISound): Defines the required "Play" behavior.
// 2. The Real Object (ButtonClickSound): Executes standard audio logic.
// 3. The Null Object (NullSound): Implements "Play" as a no-op (No Operation).
// 4. The Context (UIButton): Maintains a reference and invokes logic blindly.
//
// THE SCENARIO:
// In a system with numerous buttons, some possess audio triggers while others are muted.
// Without Null Object: Logic requires 'if (sound != nullptr)' checks for every interaction.
// With Null Object: Logic calls 'sound->Play()' directly. The Null Object handles silence.
// =========================================================================

namespace NUL
{
    // ------------------------------------------------------------------------
    // 1. THE INTERFACE (The Contract)
    // ------------------------------------------------------------------------
    class IUISound
    {
    public:
        virtual ~IUISound() = default;
        virtual void Play() = 0;
        virtual std::string GetStatus() const = 0;
    };

    // ------------------------------------------------------------------------
    // 2. THE REAL OBJECT (The Active Implementation)
    // ------------------------------------------------------------------------
    class ButtonClickSound : public IUISound
    {
    public:
        void Play() override;
        std::string GetStatus() const override { return "Sound Enabled (Real Object)"; }
    };

    // ------------------------------------------------------------------------
    // 3. THE NULL OBJECT (The Silent Implementation)
    // ------------------------------------------------------------------------
    class NullSound : public IUISound
    {
    public:
        // Implementation is intentionally empty.
        // Interface requirements are met without performing operations.
        void Play() override {}
        std::string GetStatus() const override { return "Muted (Null Object)"; }
    };

    // ------------------------------------------------------------------------
    // 4. THE CONTEXT (The Consumer)
    // ------------------------------------------------------------------------
    class UIButton
    {
    public:
        // Dependency is injected via constructor. 
        // The button remains agnostic of the concrete implementation.
        UIButton(std::string Name, std::shared_ptr<IUISound> SoundEffect);

        void Click();

        // Supports runtime behavior swapping.
        void SetSound(std::shared_ptr<IUISound> NewSound) { MySound = NewSound; }

        std::string GetButtonName() const { return ButtonName; }
        std::string GetSoundStatus() const { return MySound->GetStatus(); }

    private:
        std::string ButtonName;
        std::shared_ptr<IUISound> MySound;
    };

    void RunDemo();
}