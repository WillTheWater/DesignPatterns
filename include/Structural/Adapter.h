#pragma once

#include "FunctionLibrary/HelperFunctionLibrary.h"

// =========================================================================
// STRUCTURAL DESIGN PATTERN: Adapter Pattern
// =========================================================================
// "Match interfaces of incompatible classes."
//
// THE GOAL:
// Create a translator between two incompatible interfaces (or APIs). In this
// scenario, there is a bridge between High-Level Game Logic (Actions) and
// Low-Level OS Hardware (KeyCodes).
//
// THE BENEFIT:
// * Flexibility: Decouples the Game Engine from specific hardware drivers.
// * Scalability: Support for new input devices (Gamepads, Joysticks) can be
//   added by creating new Adapters without modifying core logic.
// * Cleanliness: Prevents "magic numbers" (KeyCodes) from polluting the 
//   high-level gameplay systems.
//
// THE EXAMPLE:
// [EInputAction]: The shared vocabulary (Jump, Move, etc.).
// [IInputDevice]: The Target Interface expected by the Game Logic.
// [Keyboard]: The Adaptee. An incompatible low-level OS module.
// [KeyboardInputAdapter]: The Adapter. Translates Actions into KeyCodes.
// =========================================================================

namespace ADP
{
    // =========================================================================
    // ACTIONS (The Shared Vocabulary)
    // Define specific actions that act as the "Dictionary"
    // that both Client and Adapter agree on.
    // =========================================================================
    enum class EInputAction
    {
        Forward,
        MoveLeft,
        MoveDown,
        MoveRight,
        Jump,
        None
    };

    // =========================================================================
    // THE TARGET INTERFACE
    // This interface defines what the Game Logic expects.
    // =========================================================================
    class IInputDevice
    {
    public:
        virtual ~IInputDevice() = default;

        // The Client calls this with Action IDs.
        virtual bool IsActionPressed(EInputAction Action) const = 0;
    };

    // =========================================================================
    // THE ADAPEE
    // This class represents the specific input device (Keyboard).
    // It speaks a different "Language" (KeyCodes) than the Game Logic.
    // =========================================================================
    class Keyboard
    {
    public:
        bool IsKeyPressed(int KeyCode) const;
    };

    // =========================================================================
    // THE ADAPTER
    // This class implements IInputDevice (Target Interface).
    // Inside 'IsActionPressed', it translates Actions into KeyCodes.
    // =========================================================================
    class KeyboardInputAdapter : public IInputDevice
    {
    public:
        // The Adapter accepts the Keyboard via the constructor.
        KeyboardInputAdapter(Keyboard* OSKeyboard);

        // Override IInputDevice method to perform the translation logic.
        bool IsActionPressed(EInputAction Action) const override;

    private:
        // The Adaptee reference.
        Keyboard* OSKeyboard;
    };

    // =========================================================================
    // DEMO
    // =========================================================================
    void RunDemo();
}