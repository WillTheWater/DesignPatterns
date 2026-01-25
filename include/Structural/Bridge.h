#pragma once

#include "FunctionLibrary/HelperFunctionLibrary.h"

// =========================================================================
// STRUCTURAL DESIGN PATTERN: Bridge
// =========================================================================
// "Decouple an abstraction from its implementation so that the two can vary independently."
//
// THE GOAL:
// Bind "Game Logic" (Movement Commands) to "Input Hardware" (Keyboard, Joypad, Gamepad).
// This prevents a combinatorial explosion of classes when adding new hardware
// or new game commands.
//
// THE BENEFIT:
// * Independence: High-level logic and low-level hardware can evolve separately.
// * Runtime Switching: Swap the physical input source (e.g., switching from 
//   Keyboard to Gamepad) without the Game Logic needing to be recompiled.
// * Extensibility: Adding a "VR Controller" implementation requires zero 
//   changes to the core GameInput abstraction.
//
// THE EXAMPLE:
// [ECommand]: The shared language. An enum representing player intent.
// [IInputDevice]: The Implementor. Defines how raw hardware signals are translated.
// [GameInput]: The Abstraction. The high-level interface used by the game engine.
// [Devices]: Keyboard, Joypad, and Gamepad implementations.
// =========================================================================

namespace BRG
{
    // =========================================================================
    // THE ABSTRACTION DOMAIN
    // ROLE: Defines the high-level intent of the system.
    // =========================================================================
    enum class ECommand
    {
        Forward,
        Backwards,
        Left,
        Right,
        None
    };

    // =========================================================================
    // THE IMPLEMENTOR INTERFACE (The Bridge)
    // ROLE: Defines the hardware-side contract. It handles the "translation"
    // of raw machine data into the shared Abstraction Domain (ECommand).
    // =========================================================================
    class IInputDevice
    {
    public:
        virtual ~IInputDevice() = default;

        // Translate raw, device-specific input into a game command.
        virtual ECommand Translate(int RawInput) const = 0;
        virtual std::string GetDeviceName() const = 0;
    };

    // =========================================================================
    // THE ABSTRACTION
    // ROLE: The High-Level Policy. This is the "face" of the input system
    // used by the Game Engine. It maintains a reference to the Bridge.
    // =========================================================================
    class GameInput
    {
    public:
        explicit GameInput(IInputDevice* InDevice)
            : Device(InDevice)
        {
        }

        // The Bridge allows the change implementation at runtime.
        void SetDevice(IInputDevice* InDevice)
        {
            Device = InDevice;
        }

        // DELEGATION: The abstraction doesn't handle input itself; 
        // it asks the current implementor to do it.
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

    // =========================================================================
    // THE IMPLEMENTATIONS (The Low-Level Details)
    // ROLE: Specific hardware drivers that implement the IInputDevice contract.
    // =========================================================================

    // ======================== KEYBOARD ========================
    class KeyboardDevice : public IInputDevice
    {
    public:
        ECommand Translate(int KeyCode) const override;
        std::string GetDeviceName() const override;
    };

    // ======================== JOYPAD ========================
    class JoypadDevice : public IInputDevice
    {
    public:
        ECommand Translate(int Axis) const override;
        std::string GetDeviceName() const override;
    };

    // ======================== GAMEPAD ========================
    class GamepadDevice : public IInputDevice
    {
    public:
        ECommand Translate(int ButtonID) const override;
        std::string GetDeviceName() const override;
    };

    // =========================================================================
    // DEMO IMPLEMENTATION
    // =========================================================================
    void RunDemo();
}