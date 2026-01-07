#pragma once

#include "FunctionLibrary/HelperFunctionLibrary.h"

// =========================================================================
// STRUCTURAL DESIGN PATTERNS: Bridge
// =========================================================================
// "Decouple an abstraction from its implementation so that the two can vary independently."
//
// THE GOAL:
// Bind "Game Logic" (Movement Commands) to "Input Hardware" (Keyboard, Joypad, Gamepad).
//
// THE EXAMPLE:
// An Input Mapping System.
// 1. Abstraction (GameInput): Defines how the game requests actions.
// 2. Implementor Interface (IInputDevice): Defines how raw input is translated.
// 3. Implementations (The Hardware): Keyboard, Joypad, Gamepad.
//
// THE SCENARIO:
// The Game Logic says "Process Input".
// The Abstraction delegates to the active Hardware.
// - Keyboard reads key codes.
// - Joypad reads axis values.
// - Gamepad reads button IDs.
// Each device translates raw input into a game command.
//
// BENEFIT:
// You can add new input hardware without changing Game Logic.
// The Abstraction and Implementations vary independently.
// =========================================================================

namespace BRG
{
    // ------------------------------------------------------------------------
    // 1. THE ABSTRACTION DOMAIN (Game Commands)
    // ------------------------------------------------------------------------
    enum class ECommand
    {
        Forward,
        Backwards,
        Left,
        Right,
        None
    };

    // ------------------------------------------------------------------------
    // 2. THE IMPLEMENTOR INTERFACE (Bridge Interface)
    // ------------------------------------------------------------------------
    // This interface represents the hardware side of the bridge.
    // It knows how to translate raw input into game commands.
    class IInputDevice
    {
    public:
        virtual ~IInputDevice() = default;

        // Translate raw, device-specific input into a game command.
        virtual ECommand Translate(int RawInput) const = 0;
        virtual std::string GetDeviceName() const = 0;
    };

    // ------------------------------------------------------------------------
    // 3. THE ABSTRACTION (Game Logic Side)
    // ------------------------------------------------------------------------
    // This is what the game uses.
    // It delegates input handling to the selected hardware implementation.
    class GameInput
    {
    public:
        explicit GameInput(IInputDevice* InDevice)
            : Device(InDevice)
        {
        }

        void SetDevice(IInputDevice* InDevice)
        {
            Device = InDevice;
        }

        ECommand HandleInput(int RawInput) const
        {
            return Device ? Device->Translate(RawInput) : ECommand::None;
        }

        std::string GetActiveDeviceName() const
        {
            return Device ? Device->GetDeviceName() : "None";
        }

    private:
        IInputDevice* Device;
    };

    // ------------------------------------------------------------------------
    // 4. THE IMPLEMENTATIONS (Hardware Devices)
    // ------------------------------------------------------------------------

    class KeyboardDevice : public IInputDevice
    {
    public:
        ECommand Translate(int KeyCode) const override;
        std::string GetDeviceName() const override;
    };

    class JoypadDevice : public IInputDevice
    {
    public:
        ECommand Translate(int Axis) const override;
        std::string GetDeviceName() const override;
    };

    class GamepadDevice : public IInputDevice
    {
    public:
        ECommand Translate(int ButtonID) const override;
        std::string GetDeviceName() const override;
    };

    // ------------------------------------------------------------------------
    // 5. DEMO
    // ------------------------------------------------------------------------
    void RunDemo();
}