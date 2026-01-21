#pragma once

#include "FunctionLibrary/HelperFunctionLibrary.h"

// =========================================================================
// SOLID DESIGN PRINCIPLE: Dependency Inversion Principle (DIP)
// =========================================================================
// "Depend on abstractions, not on concretions."
//
// THE GOAL:
// High-level modules (like game logic) should not depend on low-level 
// modules (like specific file writers). Instead, both should depend on 
// abstractions (interfaces). This "inverts" the traditional dependency 
// flow, decoupling the core logic from the implementation details.
//
// THE BENEFIT:
// * Flexibility: Swap storage methods (Local vs Cloud) without changing 
//   a single line of GameEngine code.
// * Testability: Easily "inject" mock save systems for unit testing.
// * Modularity: Low-level changes (updating a database driver) won't 
//   break high-level business logic.
//
// THE EXAMPLE:
// [ISaveSystem]: The Abstraction. Defines the contract for saving/loading.
// [Text/Binary/Cloud]: Low-level modules that implement the contract.
// [GameEngine]: High-level module that uses whatever saver is "injected" 
//               into it at runtime.
// =========================================================================

namespace DIP
{
    // =========================================================================
    // THE ABSTRACTION (The Interface)
    // Defines the contract. The Engine doesn't care HOW the data is stored,
    // only that these three functions exist.
    // =========================================================================
    class ISaveSystem
    {
    public:
        virtual ~ISaveSystem() = default;

        virtual void SaveData(int Score) const = 0;
        virtual int LoadData() const = 0;
        virtual std::string GetDescription() const = 0; // For menu display
    };

    // =========================================================================
    // LOW LEVEL MODULES (The Details)
    // These satisfy the dependency by implementing the ISaveSystem interface.
    // =========================================================================

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

    // =========================================================================
    // HIGH LEVEL MODULE (The Boss)
    // This class demonstrates DIP by depending solely on the ISaveSystem 
    // abstraction. It is completely "blind" to the file system or network.
    // =========================================================================
    class GameEngine
    {
    public:
        GameEngine();

        // INJECTION POINT: This allows swaping the concretion at runtime.
        void SetSaveSystem(ISaveSystem* Saver) { Storage = Saver; }

        void PlayGame();       // Logic to generate score
        void SaveGame() const; // Logic to trigger save via abstraction

        int GetScore() const { return Score; }
        void SetScore(int NewScore) { Score = NewScore; }

    private:
        int Score;
        ISaveSystem* Storage; // Dependence on Abstraction
    };

    // =========================================================================
    // DEMO
    // =========================================================================
    void RunDemo();
}