#pragma once

#include "FunctionLibrary/HelperFunctionLibrary.h"

// =========================================================================
// STRUCTURAL DESIGN PATTERN: Facade
// =========================================================================
// "Provide a unified interface to a set of interfaces in a subsystem."
//
// THE GOAL:
// Hide the complex "wiring" of multiple independent subsystems behind a single, 
// simplified interface. This prevents the client from needing to manage 
// object lifecycles or dependency chains manually.
//
// THE BENEFIT:
// * Simplicity: The client only interacts with one class (the Facade).
// * Decoupling: Subsystems can be swapped or refactored without the client 
//   ever knowing, as long as the Facade's contract remains the same.
// * Safety: The Facade can enforce a specific execution order (e.g., Validate 
//   before Save) that a client might otherwise skip.
//
// THE EXAMPLE:
// [GameData]: The Product. A simple struct containing player state.
// [Subsystems]: Validator (Safety), Serializer (Format), and Storage (I/O).
// [SaveSystemFacade]: The Master Switch. Orchestrates the subsystems so the 
//   client only has to call a single Save() or Load() function.
// =========================================================================

namespace FAC
{
    // =========================================================================
    // THE PRODUCT (The Data)
    // ROLE: A simple container for data with no internal logic.
    // =========================================================================
    struct GameData
    {
        // Core Identity
        std::string Name;
        int Level;
        float XP;
        int Gold;

        // Combat Stats
        int Health;
        int Mana;
        int Strength;
        int Agility;
        int Intelligence;

        // Equipment & Inventory
        std::string PrimaryWeapon;
        int ArmorClass;
        int PotionsCount;

        // World State
        std::array<std::array<float, 3>, 1> Position3D = { 0.f, 0.f, 0.f };
        float PlayTimeSeconds;
    

        GameData()
            : Name("Unknown"), Level(1), XP(0.0f), Gold(0), Health(100), Mana(50)
            , Strength(10), Agility(10), Intelligence(10), PrimaryWeapon("Stick")
            , ArmorClass(5), PotionsCount(1), Position3D{ 0.f, 0.f, 0.f }, PlayTimeSeconds(0.0f)
        {
        }
    };

    // =========================================================================
    // THE SERIALIZER (The Low-Level Details)
    // This macro creates to_json() and from_json() automatically    
    // =========================================================================
    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE
    (
        GameData,
        Name, Level, XP, Gold, Health, Mana, Strength,
        Agility, Intelligence, PrimaryWeapon, ArmorClass,
        PotionsCount, Position3D, PlayTimeSeconds
    )

    // =========================================================================
    // THE SUBSYSTEMS (The Low-Level Details)
    // ROLE: Specialized workers that handle one specific part of the process.
    // =========================================================================

    // ======================== VALIDATOR (Safety) ========================
    class IValidator
    {
    public:
        virtual ~IValidator() = default;
        virtual bool Validate(const GameData& Data) const = 0;
    };

    class Validator : public IValidator
    {
    public:
        bool Validate(const GameData& Data) const override;
    };

    // ======================== SERIALIZER (Formatting) ========================
    class ISerializer
    {
    public:
        virtual ~ISerializer() = default;
        virtual std::string Serialize(const GameData& Data) const = 0;
        virtual GameData Deserialize(const std::string& Content) const = 0;
    };

    class JsonSerializer : public ISerializer
    {
    public:
        // Using nlohmann/json
        std::string Serialize(const GameData& Data) const override;
        GameData Deserialize(const std::string& Content) const override;
    };

    // ======================== STORAGE (File I/O) ========================
    class IStorage
    {
    public:
        virtual ~IStorage() = default;
        virtual void Write(const std::string& Filename, const std::string& Content) const = 0;
        virtual std::string Read(const std::string& Filename) const = 0;
    };

    class LocalDiskStorage : public IStorage
    {
    public:
        void Write(const std::string& Filename, const std::string& Content) const override;
        std::string Read(const std::string& Filename) const override;
    };

    // =========================================================================
    // THE FACADE (The Master Switch)
    // ROLE: The High-Level Policy. It knows the order of operations and
    // manages the lifecycles of the subsystems for the client.
    // =========================================================================
    class SaveSystemFacade
    {
    public:
        // THE SIMPLE INTERFACE:
        // The Client doesn't need to know that validation or serialization occurs.
        bool Save(int SlotID, const GameData& Data);
        bool Load(int SlotID, GameData& Data);
    };

    // =========================================================================
    // DEMO IMPLEMENTATION
    // =========================================================================
    void RunDemo();
}