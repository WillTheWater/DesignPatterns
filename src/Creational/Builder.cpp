#include "Creational/Builder.h"

namespace BLD
{
    // ------------------------------------------------------------------------
    // LEVEL PRODUCT IMPLEMENTATION
    // ------------------------------------------------------------------------
    void Level::AddStructure(const std::string& StructureName)
    {
        Structures.push_back(StructureName);
    }

    void Level::PrintLevel() const
    {
        std::cout << "=========================================\n";
        std::cout << "           LEVEL MAP\n";
        std::cout << "========================================\n";
        for (const auto& s : Structures)
        {
            std::cout << s << "\n";
        }
        std::cout << "========================================\n";
    }

    // ------------------------------------------------------------------------
    // DUNGEON BUILDER IMPLEMENTATION
    // ------------------------------------------------------------------------
    DungeonBuilder::DungeonBuilder()
    {
        // Reset() is called so every new build,
        // the slate is clean (no old room data from the previous level).
        Reset();
    }

    void DungeonBuilder::Reset()
    {
        TempParts.clear();
    }

    void DungeonBuilder::AddRoom(const std::string& Name)
    {
        std::cout << "   [Dungeon] Adding Stone: " << Name << "...\n";
        // This is a temporary "staging" list (TempParts).
        // They are not added to the Level yet because they might need changes to the order.
        TempParts.push_back(Name);
    }

    void DungeonBuilder::AddStairs(const std::string& Direction)
    {
        std::cout << "   [Dungeon] Adding " << Direction << " stairs...\n";
        TempParts.push_back("Stone Stairs " + Direction);
    }

    std::unique_ptr<Level> DungeonBuilder::Build()
    {
        // CONSTRUCTION PHASE
        // Once the Director defines the correct order (e.g., "Stairs Down"),
        // the "TempParts" (The Blueprint) are used to create the final Level.
        auto NewLevel = std::make_unique<Level>();

        for (const auto& Part : TempParts)
        {
            NewLevel->AddStructure("  " + Part + " ");
        }

        std::cout << ">> [DungeonBuilder] Dungeon Construction Complete.\n";
        // CLEANUP PHASE
        // After building, clear the staging list.
        Reset();
        return NewLevel;
    }

    // ------------------------------------------------------------------------
    // CASTLE BUILDER IMPLEMENTATION
    // ------------------------------------------------------------------------
    CastleBuilder::CastleBuilder()
    {
        Reset();
    }

    void CastleBuilder::Reset()
    {
        TempParts.clear();
    }

    void CastleBuilder::AddRoom(const std::string& Name)
    {
        std::cout << "   [Castle] Marble: " << Name << "...\n";
        TempParts.push_back("Marble Hall " + Name);
    }

    void CastleBuilder::AddStairs(const std::string& Direction)
    {
        std::cout << "   [Castle] Adding Marble Staircase " << Direction << "...\n";
        TempParts.push_back("Marble Stairs " + Direction);
    }

    std::unique_ptr<Level> CastleBuilder::Build()
    {
        auto NewLevel = std::make_unique<Level>();

        for (const auto& Part : TempParts)
        {
            NewLevel->AddStructure(Part);
        }

        std::cout << ">> [CastleBuilder] Castle Construction Complete.\n";
        Reset();
        return NewLevel;
    }

    // ------------------------------------------------------------------------
    // LEVEL DIRECTOR IMPLEMENTATION
    // ------------------------------------------------------------------------
    LevelDirector::LevelDirector()
    {
        std::cout << "[Director] Waiting for Builder...\n";
    }

    void LevelDirector::SetBuilder(ILevelBuilder* NewBuilder)
    {
        // The Director doesn't know HOW to build (Reset, AddRoom).
        // It just holds a pointer to the (Builder) that knows.
        Builder = NewBuilder;
    }

    // --- RECIPE 1: DUNGEON ---
    void LevelDirector::CreateDungeon()
    {
        std::cout << ">> [Director] Executing Dungeon Recipe...\n";

        // Step 1: Add Main Room
        Builder->AddRoom("Dungeon Entrance");

        // Step 2: Add Traps
        Builder->AddRoom("Mess Pit");
        Builder->AddRoom("Jail Chamber");

        // Step 3: Add Stairs Down
        Builder->AddStairs("Down");

        // Step 4: Build
        std::unique_ptr<Level> MyLevel = Builder->Build();

        std::cout << ">> [Director] Dungeon Ready.\n\n";
        // The Director now owns the finished Level.
        // But note: It doesn't know it was a DungeonBuilder specifically.
        // It just knows it's an ILevelBuilder.
    }

    // --- RECIPE 2: CASTLE ---
    void LevelDirector::CreateCastle()
    {
        std::cout << ">> [Director] Executing Castle Recipe...\n";

        // Step 1: Build Foundation
        Builder->AddRoom("Throne Room");
        Builder->AddRoom("Barracks");

        // Step 2: Build Towers
        Builder->AddRoom("North Tower");
        Builder->AddRoom("South Tower");

        // Step 3: Add Stairs Up
        Builder->AddStairs("Up");

        // Step 4: Build
        std::unique_ptr<Level> MyLevel = Builder->Build();

        std::cout << ">> [Director] Castle Ready.\n\n";
    }

    // ------------------------------------------------------------------------
    // DEMO IMPLEMENTATION
    // ------------------------------------------------------------------------
    void RunDemo()
    {
        // Clear buffer
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        // --- STEP 1: INTRODUCTION ---
        HFL::ClearScreen();
        HFL::PrintHeader("Builder Pattern");

        std::cout << "Definition:\n";
        std::cout << "Separate the construction of a complex object from its representation.\n";
        std::cout << "Allow construction step-by-step, rather than one giant constructor.\n\n";

        std::cout << "In This Demo:\n";
        std::cout << "There is a 'Level Director' (Architect).\n";
        std::cout << "It wants to build levels, but it doesn't know HOW to build them.\n";
        std::cout << "It tells the 'Level Builder' (Contractor) to do it.\n";

        HFL::WaitForInput();

        // --- STEP 2: THE PARTS ---
        HFL::ClearScreen();
        HFL::PrintHeader("Step 1: The Roles");

        std::cout << "The Builder Pattern involves 3 key roles:\n\n";

        std::cout << "1. The Product (The Level):\n";
        std::cout << "   - The final result.\n";
        std::cout << "   - It is 'Complex' but the code to create it is simplified.\n\n";

        std::cout << "2. The Builder (The Worker):\n";
        std::cout << "   - Knows HOW to build.\n";
        std::cout << "   - Has methods like 'AddRoom' and 'Build'.\n";
        std::cout << "   - There are 2 Builders: DungeonBuilder and CastleBuilder.\n\n";

        std::cout << "3. The Director (The Architect):\n";
        std::cout << "   - Knows WHAT to build and the ORDER.\n";
        std::cout << "   - Doesn't know the implementation details of building.\n";
        std::cout << "   - Just calls 'AddRoom' then 'Build'.\n";

        HFL::WaitForInput();

        // --- STEP 3: INTERACTIVE CONSTRUCTOR ---

        // Create the Workers
        DungeonBuilder DungeonContractor;
        CastleBuilder CastleContractor;

        // Create the Architect
        LevelDirector Architect;

        bool InDemo = true;
        while (InDemo)
        {
            HFL::ClearScreen();
            HFL::PrintHeader("Level Architect");

            std::cout << "Select a Builder:\n";
            std::cout << "1. Dungeon Builder\n";
            std::cout << "2. Castle Builder\n";
            std::cout << "0. Exit Demo\n";
            std::cout << "\nChoice: ";

            int Choice;
            std::cin >> Choice;

            if (std::cin.fail()) { std::cin.clear(); std::cin.ignore(); continue; }

            if (Choice == 0) break;

            // INJECTION
            if (Choice == 1) Architect.SetBuilder(&DungeonContractor);
            if (Choice == 2) Architect.SetBuilder(&CastleContractor);

            // --- STEP 4: RECIEPE SELECTION ---
            HFL::ClearScreen();
            HFL::PrintHeader("Select a Design");

            std::cout << "Choose a Construction Type:\n";
            std::cout << "1. A Dungeon\n";
            std::cout << "2. A Castle\n";
            std::cout << "0. Back\n";
            std::cout << "\nChoice: ";

            int RecipeChoice;
            std::cin >> RecipeChoice;

            if (std::cin.fail()) { std::cin.clear(); std::cin.ignore(); continue; }

            if (RecipeChoice == 0) continue;

            if (RecipeChoice == 1)
            {
                Architect.CreateDungeon();
                HFL::WaitForInput();
            }
            else if (RecipeChoice == 2)
            {
                Architect.CreateCastle();
                HFL::WaitForInput();
            }
        }

        // --- STEP 5: CONCLUSION ---
        HFL::ClearScreen();
        HFL::PrintHeader("Conclusion");

        std::cout << "Summary:\n\n";
        std::cout << "1. Director vs Builder:\n";
        std::cout << "   The Director (Architect) knows the ORDER.\n";
        std::cout << "   The Builder (Worker) knows HOW to build.\n\n";

        std::cout << "2. Separation of Concerns:\n";
        std::cout << "   The 'Level' class (The Product) only holds the resulting data.\n";
        std::cout << "   The 'Builder' classes handles the construction details.\n";
        std::cout << "   The 'Director' class decides what should be built and by who.\n\n";

        std::cout << "3. Builder Pattern:\n";
        std::cout << "   Decouples the 'What' from the 'How'.\n";
        std::cout << "   This makes code readable and easy to maintain.\n";

        std::cout << std::setw(40) << "End of Demo\n";
        HFL::WaitForInput();
    }
}