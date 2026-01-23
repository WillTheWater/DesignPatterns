#pragma once

#include "FunctionLibrary/HelperFunctionLibrary.h"

// =========================================================================
// CREATIONAL DESIGN PATTERNS: BUILDER PATTERN
// =========================================================================
// "Separate the construction of a complex object from its representation."
//
// THE GOAL:
// Allow a Level to be constructed step-by-step (Room, Corridor, Stairs)
// so ther isn't one massive constructor with 50 arguments.
//
// THE BENEFIT:
// * Step-by-Step Construction: Control the sequence of creation precisely.
// * Reusability: Use the same Director logic to build entirely different 
//   visual styles by simply swapping the Concrete Builder.
// * Isolation: The complex assembly logic is hidden from the final 
//   Product (Level) and the Client.
//
// THE EXAMPLE:
// [Level]: The Product. A simple data container holding the final result.
// [ILevelBuilder]: The Interface. Defines the steps (AddRoom, AddStairs).
// [Dungeon/CastleBuilder]: The Workers. They implement the specific style.
// [LevelDirector]: The Architect. Controls the order of the build steps.
// =========================================================================

namespace BLD
{
    // =========================================================================
    // 1. THE PRODUCT (The Result)
    // =========================================================================
    class Level
    {
    public:
        void PrintLevel() const;

        void AddStructure(const std::string& StructureName);

    private:
        std::vector<std::string> Structures;
    };

    // =========================================================================
    // 2. THE BUILDER INTERFACE (The Contractor)
    // =========================================================================
    class ILevelBuilder
    {
    public:
        virtual ~ILevelBuilder() = default;

        virtual void Reset() = 0;
        virtual void AddRoom(const std::string& Name) = 0;
        virtual void AddStairs(const std::string& Direction) = 0;
        virtual std::unique_ptr<Level> Build() = 0;
    };

    // =========================================================================
    // 3. CONCRETE BUILDERS (The Workers)
    // =========================================================================

    class DungeonBuilder : public ILevelBuilder
    {
    public:
        DungeonBuilder();

        void Reset() override;
        void AddRoom(const std::string& Name) override;
        void AddStairs(const std::string& Direction) override;
        std::unique_ptr<Level> Build() override;

    private:
        std::vector<std::string> TempParts;
    };

    class CastleBuilder : public ILevelBuilder
    {
    public:
        CastleBuilder();

        void Reset() override;
        void AddRoom(const std::string& Name) override;
        void AddStairs(const std::string& Direction) override;
        std::unique_ptr<Level> Build() override;

    private:
        std::vector<std::string> TempParts;
    };

    // =========================================================================
    // 4. THE DIRECTOR (The Architect)
    // =========================================================================
    class LevelDirector
    {
    public:
        LevelDirector();

        void SetBuilder(ILevelBuilder* NewBuilder);

        void CreateDungeon();
        void CreateCastle();

    private:
        ILevelBuilder* Builder = nullptr;
    };

    // =========================================================================
    // 5. DEMO
    // =========================================================================
    void RunDemo();
}