#pragma once

#include "FunctionLibrary/HelperFunctionLibrary.h"

// =========================================================================
// STRUCTURAL DESIGN PATTERNS: Facade
// =========================================================================
// "Provide a unified interface to a set of interfaces in a subsystem."
//
// THE GOAL:
// Hide complex wiring between multiple subsystems behind one simple interface.
//
// THE EXAMPLE:
// A "Save System" Facade.
// 1. The Facade: "Save()" "Load()" functions hide the complexity.
// 2. Subsystems: Serializer, Storage, Validator.
//
// THE SCENARIO:
// The User has a 'GameData' struct (Name, Level, XP, Gold).
// They just want to call 'Save()'. They don't care about binary vs text,
// or checking file handles. The Facade handles the logic.
//
// BENEFIT:
// Reduces complexity for the user. You can add new subsystems (Cloud storage)
// behind the Facade without changing the interface code.
// =========================================================================

namespace FAC
{
    // ------------------------------------------------------------------------
    // 1. THE PRODUCT (The Data)
    // ------------------------------------------------------------------------
    struct GameData
    {
        std::string Name;
        int Level;
        float XP;
        int Gold;

        GameData(){}
        GameData(std::string Name, int Level, float XP, int Gold)
            : Name(Name), Level(Level), XP(XP), Gold(Gold) {
        }
    };

    // ------------------------------------------------------------------------
    // 2. SUBSYSTEMS (The Wiring)
    // ------------------------------------------------------------------------
    // These classes are the "Low Level" details. The Facade manages them.
    // Note: We use Interfaces for Subsystems too, keeping the code decoupled.

    // A. VALIDATOR (Safety)
    class IValidator
    {
    public:
        virtual ~IValidator() = default;
        virtual bool Validate(const GameData& Data) const = 0;
    };

    // --- 1. Concrete Validator (Unchanged) ---
    class Validator : public IValidator
    {
    public:
        bool Validate(const GameData& Data) const override;
    };

    // B. SERIALIZER (Formatting)
    class ISerializer
    {
    public:
        virtual ~ISerializer() = default;
        virtual std::string Serialize(const GameData& Data) const = 0;
        virtual GameData Deserialize(const std::string& Content) const = 0;
    };

    // --- 2. Concrete Serializer (Unchanged) ---
    class Serializer : public ISerializer
    {
    public:
        std::string Serialize(const GameData& Data) const override;
        GameData Deserialize(const std::string& Content) const override;
    };

    // C. STORAGE (File I/O - Updated with Load)
    class IStorage
    {
    public:
        virtual ~IStorage() = default;
        virtual void Write(const std::string& Filename, const std::string& Content) const = 0;
        virtual std::string Read(const std::string& Filename) const = 0;
    };

    // --- 3. Concrete Storage (UPDATED FOR REAL FILES) ---
    class LocalDiskStorage : public IStorage
    {
    public:
        void Write(const std::string& Filename, const std::string& Content) const override
        {
            // 1. Get the Cross-Platform Path
            std::filesystem::path saveDir = HFL::GetSaveDirectory("FAC");

            // 2. Construct the full file path
            std::filesystem::path fullPath = saveDir / Filename;

            // 3. Open the file using std::ofstream
            std::ofstream outFile(fullPath);

            if (outFile.is_open())
            {
                outFile << Content;
                outFile.close();
                std::cout << "[Storage] Successfully saved to: " << fullPath << "\n";
            }
            else
            {
                std::cout << "[Storage] ERROR: Could not open file for writing: " << fullPath << "\n";
            }
        }

        std::string Read(const std::string& Filename) const override
        {
            // 1. Get the Path
            std::filesystem::path saveDir = HFL::GetSaveDirectory("FAC");
            std::filesystem::path fullPath = saveDir / Filename;

            // 2. Check if file exists
            if (!std::filesystem::exists(fullPath))
            {
                std::cout << "[Storage] ERROR: Save file not found: " << fullPath << "\n";
                return "";
            }

            // 3. Open the file using std::ifstream
            std::ifstream inFile(fullPath);

            if (inFile.is_open())
            {
                std::cout << "[Storage] Reading from: " << fullPath << "\n";

                // Read entire file content into a string
                std::stringstream buffer;
                buffer << inFile.rdbuf();
                inFile.close();

                return buffer.str();
            }
            else
            {
                std::cout << "[Storage] ERROR: Could not open file for reading.\n";
                return "";
            }
        }
    };

    // ------------------------------------------------------------------------
    // 3. THE FACADE (The Master Switch)
    // ------------------------------------------------------------------------
    // This is the only class the User knows about.
    // It delegates work to the Subsystems.
    class SaveSystemFacade
    {
    public:
        // THE SIMPLE INTERFACE
        // The User doesn't need to know about Validators or Serializers.
        // They just call Save().
        bool Save(int SlotID, const GameData& Data);
        bool Load(int SlotID, GameData& Data);
    };

    // ------------------------------------------------------------------------
    // 4. DEMO
    // ------------------------------------------------------------------------
    void RunDemo();
}