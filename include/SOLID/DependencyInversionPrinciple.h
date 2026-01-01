#pragma once

#include "FunctionLibrary/HelperFunctionLibrary.h"

// =========================================================================
// PRINCIPLE: Dependency Inversion Principle (DIP)
// =========================================================================
// "Depend on abstractions, not on concretions."
//
// THE GOAL:
// The High Level Module (GameEngine) should not depend on Low Level 
// Modules (File system, Network). Both should depend on Abstractions.
//
// THE EXAMPLE:
// A flexible Save System.
// 1. GameEngine (High Level): Holds Score. Needs to Save.
// 2. ISaveSystem (Abstraction): Defines Save() and Load().
// 3. TextFile, BinaryFile, Cloud (Low Level): Implement Save().
//
// BENEFIT:
// We can choose HOW to save (Text vs Binary vs Cloud) at runtime
// without changing a single line of GameEngine code.
// =========================================================================

namespace DIP
{
    // ------------------------------------------------------------------------
    // 1. THE ABSTRACTION (The Interface)
    // ------------------------------------------------------------------------
    class ISaveSystem
    {
    public:
        virtual ~ISaveSystem() = default;
        virtual void SaveData(int Score) const = 0;
        virtual int LoadData() const = 0;
        virtual std::string GetDescription() const = 0; // For menu display
    };

    // ------------------------------------------------------------------------
    // 2. LOW LEVEL MODULES (The Details)
    // ------------------------------------------------------------------------

    class TextFileSave : public ISaveSystem
    {
    public:
        TextFileSave();
        void SaveData(int Score) const override;
        int LoadData() const override;
        std::string GetDescription() const override;

    private:
        std::string FilePath;
    };

    class BinaryFileSave : public ISaveSystem
    {
    public:
        BinaryFileSave();
        void SaveData(int Score) const override;
        int LoadData() const override;
        std::string GetDescription() const override;

    private:
        std::string FilePath;
    };

    class CloudServerSave : public ISaveSystem
    {
    public:
        CloudServerSave();
        void SaveData(int Score) const override;
        int LoadData() const override;
        std::string GetDescription() const override;

    private:
        std::string FilePath;
    };

    // ------------------------------------------------------------------------
    // 3. HIGH LEVEL MODULE (The Boss)
    // ------------------------------------------------------------------------
    class GameEngine
    {
    public:
        GameEngine();

        // INJECTION POINT: We set the saver here.
        void SetSaveSystem(ISaveSystem* Saver) { Storage = Saver; }

        void PlayGame(); // Logic to generate score
        void SaveGame() const; // Logic to trigger save
        int GetScore() const { return Score; }
        void SetScore(int NewScore) { Score = NewScore; }

    private:
        int Score;
        ISaveSystem* Storage; // Dependence on Abstraction
    };

    // ------------------------------------------------------------------------
    // DEMO
    // ------------------------------------------------------------------------
    void RunDemo();
}