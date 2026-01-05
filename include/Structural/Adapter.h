#pragma once

#include "FunctionLibrary/HelperFunctionLibrary.h"

// =========================================================================
// PRINCIPLE: Adapter Pattern (Structural)
// =========================================================================
// "Match interfaces of incompatible classes."
//
// THE GOAL:
// Create a translator between two incompatible interfaces (or APIs).
//
// THE EXAMPLE:
// Input Handling (Game Logic vs OS Hardware).
// 1. Target Interface (IInputDevice): Defines 'IsActionPressed'.
// 2. Adaptee (Keyboard): Uses 'IsKeyPressed'.
// 3. Adapter (KeyboardAdapter): Translates -> key.
//
// THE SCENARIO:
// The Game Logic knows about 'Jump' and 'Move' (Actions).
// The OS Hardware knows about (KeyCodes).
// They are incompatible.
//
// BENEFIT:
// We decouple Game Logic from OS Input Drivers.
// =========================================================================

namespace ADP
{
    // ------------------------------------------------------------------------
    // 1. Actions (The Vocabulary)
    // ------------------------------------------------------------------------
    // We define specific actions here. This acts as the "Dictionary"
    // that both Client and Adapter agree on.
    enum class EInputAction
    {
        MoveUp,
        MoveLeft,
        MoveDown,
        MoveRight,
        Jump,
        None
    };

    // ------------------------------------------------------------------------
    // 2. THE TARGET INTERFACE
    // ------------------------------------------------------------------------
    // This interface defines what the Game Logic.
    class IInputDevice
    {
    public:
        virtual ~IInputDevice() = default;

        // The Client calls this with Action IDs.
        virtual bool IsActionPressed(EInputAction Action) const = 0;
    };


    // ------------------------------------------------------------------------
    // 3. THE ADAPEE
    // ------------------------------------------------------------------------
    // This class represents the specific input device (Keyboard).
    // It speaks a different "Language" (KeyCodes) than the Game Logic.
    // The Adapter will translate Game Logic requests into this language.
    class Keyboard
    {
    public:
        bool IsKeyPressed(int KeyCode) const;
    };

    // ------------------------------------------------------------------------
    // 4. THE ADAPTER
    // ------------------------------------------------------------------------
    // This class implements IInputDevice (Target Interface).
    // However, it wraps a Keyboard (Adaptee).
    // Inside 'IsActionPressed', it translates 'Jump' (Action) -> 'Spacebar' (Key).
    class KeyboardInputAdapter : public IInputDevice
    {
    public:
        // The Adapter accepts the Keyboard.
        KeyboardInputAdapter(Keyboard* OSKeyboard);

        // Override IInputDevice method.
        // Using the EInputAction enum.
        bool IsActionPressed(EInputAction Action) const override;

    private:
        // The Adaptee. The Adapter holds a reference to the OS Keyboard.
        Keyboard* OSKeyboard;
    };

    // ------------------------------------------------------------------------
    // 5. DEMO
    // ------------------------------------------------------------------------
    void RunDemo();
}