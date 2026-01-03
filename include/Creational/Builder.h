#pragma once

#include "FunctionLibrary/HelperFunctionLibrary.h"

// =========================================================================
// CREATIONAL DESIGN PATTERNS: Builder Pattern
// =========================================================================
// "Separate the construction of a complex object from its representation."
//
// THE GOAL:
// Allow a Level to be constructed step-by-step (Room, Corridor, Stairs)
// so we don't have one massive constructor with 50 arguments.
//
// THE EXAMPLE:
// A Procedural Level Generator.
// 1. LevelBuilder: The Interface. Defines 'AddRoom', 'AddStairs'.
// 2. DungeonBuilder: Builds a stone dungeon level.
// 3. CastleBuilder: Builds a marble castle with towers.
// 4. LevelDirector: Controls the which builder builds.
//
// BENEFIT:
// The Director decides HOW to build.
// The Builder knows WHAT to build.
// The Level object just holds the data.
// =========================================================================

namespace BLD
{
    // ------------------------------------------------------------------------
    // 1. THE PRODUCT (The Result)
    // ------------------------------------------------------------------------
    class Level
    {
    public:
        void PrintLevel() const;

        // Helper to add structure data (Mock)
        void AddStructure(const std::string& StructureName);

    private:
        std::vector<std::string> Structures;
    };

    // ------------------------------------------------------------------------
    // 2. THE BUILDER INTERFACE (The Contractor)
    // ------------------------------------------------------------------------
    class ILevelBuilder
    {
    public:
        virtual ~ILevelBuilder() = default;

        virtual void Reset() = 0;
        virtual void AddRoom(const std::string& Name) = 0;
        virtual void AddStairs(const std::string& Direction) = 0;
        virtual std::unique_ptr<Level> Build() = 0;
    };

    // ------------------------------------------------------------------------
    // 3. CONCRETE BUILDERS (The Workers)
    // ------------------------------------------------------------------------

    // Type A: Dungeon
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

    // Type B: Castle
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

    // ------------------------------------------------------------------------
    // 4. THE DIRECTOR (The Architect)
    // ------------------------------------------------------------------------
    class LevelDirector
    {
    public:
        LevelDirector();

        void SetBuilder(ILevelBuilder* NewBuilder);

        // The Methods
        void CreateDungeon();
        void CreateCastle();

    private:
        ILevelBuilder* Builder = nullptr;
    };

    // ------------------------------------------------------------------------
    // 5. DEMO
    // ------------------------------------------------------------------------
    void RunDemo();
}