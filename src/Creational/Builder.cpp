#include "Creational/Builder.h"

namespace BLD
{
    // =========================================================================
    // PRODUCT IMPLEMENTATION: LEVEL
    // ROLE: A data container that stores the final structure.
    // =========================================================================

    void Level::AddStructure(const std::string& StructureName)
    {
        Structures.push_back(StructureName);
    }

    void Level::PrintLevel() const
    {
        HFL::ClearScreen();
        HFL::PrintHeader("LEVEL GENERATION REPORT");

        HFL::PrintSection("MAP STRUCTURE");

        if (Structures.empty())
        {
            HFL::SetColor(HFL::EColor::Red);
            std::cout << "  [Error] No structures found. Level is empty.\n";
        }
        else
        {
            for (const auto& s : Structures)
            {
                HFL::SetColor(HFL::EColor::Green);
                std::cout << "  [+] ";
                HFL::SetColor(HFL::EColor::White);
                std::cout << s << "\n";
            }
        }

        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "\n  ----------------------------------------\n";
        HFL::SetColor(HFL::EColor::White);
        std::cout << "  TOTAL COMPONENTS: ";
        HFL::SetColor(HFL::EColor::Green);
        std::cout << Structures.size() << "\n\n";

        HFL::PrintSection("END OF GENERATION");
    }

    // =========================================================================
    // CONCRETE BUILDER: DUNGEON
    // ROLE: Implements the "Stone" style construction logic.
    // =========================================================================

    DungeonBuilder::DungeonBuilder()
    {
        Reset();
    }

    void DungeonBuilder::Reset()
    {
        TempParts.clear();
    }

    void DungeonBuilder::AddRoom(const std::string& Name)
    {
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "    -> [DungeonBuilder] Carving Stone: " << Name << "\n";
        TempParts.push_back("Damp Stone " + Name);
    }

    void DungeonBuilder::AddStairs(const std::string& Direction)
    {
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "    -> [DungeonBuilder] Adding Mossy Stairs: " << Direction << "\n";
        TempParts.push_back("Stone Spiral Stairs (" + Direction + ")");
    }

    std::unique_ptr<Level> DungeonBuilder::Build()
    {
        auto NewLevel = std::make_unique<Level>();
        for (const auto& Part : TempParts) { NewLevel->AddStructure(Part); }
        Reset();
        return NewLevel;
    }

    // =========================================================================
    // CONCRETE BUILDER: CASTLE
    // ROLE: Implements the "Marble" style construction logic.
    // =========================================================================

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
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "    -> [CastleBuilder] Polishing Marble: " << Name << "\n";
        TempParts.push_back("Royal Marble " + Name);
    }

    void CastleBuilder::AddStairs(const std::string& Direction)
    {
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "    -> [CastleBuilder] Laying Carpet on Stairs: " << Direction << "\n";
        TempParts.push_back("Grand Red-Carpeted Marble Stairs (" + Direction + ")");
    }

    std::unique_ptr<Level> CastleBuilder::Build()
    {
        auto NewLevel = std::make_unique<Level>();
        for (const auto& Part : TempParts) { NewLevel->AddStructure(Part); }
        Reset();
        return NewLevel;
    }

    // =========================================================================
    // DIRECTOR IMPLEMENTATION: LEVEL DIRECTOR
    // ROLE: Defines the "Recipes" (The Order of execution).
    // =========================================================================

    LevelDirector::LevelDirector()
    {
        std::cout << "[Director] Waiting for Builder...\n";
    }

    void LevelDirector::SetBuilder(ILevelBuilder* NewBuilder)
    {
        // The Director depends on the ILevelBuilder abstraction.
        Builder = NewBuilder;
    }

    void LevelDirector::CreateDungeon()
    {
        HFL::PrintSection("DIRECTOR: EXECUTING 'DUNGEON' RECIPE");
        HFL::SetColor(HFL::EColor::Yellow);
        std::cout << "  [Step 1] Initializing Entrance";
        HFL::WaitDots(0.4f);
        Builder->AddRoom("Entrance Hall");

        HFL::SetColor(HFL::EColor::Yellow);
        std::cout << "  [Step 2] Adding Hazard Zones";
        HFL::WaitDots(0.4f);
        Builder->AddRoom("Spike Trap Corridor");
        Builder->AddRoom("Torture Chamber");

        HFL::SetColor(HFL::EColor::Yellow);
        std::cout << "  [Step 3] Creating Verticality";
        HFL::WaitDots(0.4f);
        Builder->AddStairs("Down");

        HFL::WaitForInput();
        std::unique_ptr<Level> Result = Builder->Build();
        Result->PrintLevel();
    }

    void LevelDirector::CreateCastle()
    {
        HFL::PrintSection("DIRECTOR: EXECUTING 'CASTLE' RECIPE");
        HFL::SetColor(HFL::EColor::Yellow);
        std::cout << "  [Step 1] Building Public Quarters";
        HFL::WaitDots(0.4f);
        Builder->AddRoom("Grand Ballroom");

        HFL::SetColor(HFL::EColor::Yellow);
        std::cout << "  [Step 2] Building Fortifications";
        HFL::WaitDots(0.4f);
        Builder->AddRoom("Watchtower Alpha");
        Builder->AddRoom("Watchtower Beta");

        HFL::SetColor(HFL::EColor::Yellow);
        std::cout << "  [Step 3] Creating Royal Access";
        HFL::WaitDots(0.4f);
        Builder->AddStairs("Up");

        HFL::WaitForInput();
        std::unique_ptr<Level> Result = Builder->Build();
        Result->PrintLevel();
    }

    // =========================================================================
    // DEMO IMPLEMENTATION
    // =========================================================================

    void RunDemo()
    {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        // ======================== INTRODUCTION ========================
        HFL::ClearScreen();
        HFL::PrintHeader("BUILDER PATTERN");

        HFL::PrintSection("THE DEFINITION");
        HFL::SetColor(HFL::EColor::White);
        std::cout << "Separate the construction of a complex object from its representation.\n"
            << "This allows the same construction process to create different representations.\n\n";

        HFL::PrintSection("THE GOAL");
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "The Builder Pattern addresses the 'Telescoping Constructor' anti-pattern.\n"
            << "Instead of a massive constructor with dozens of optional parameters, it is\n"
            << "delegated to the assembly of a specialized object that builds the product\n"
            << "step-by-step, keeping the final product class clean and simple.\n\n";

        HFL::PrintSection("THE EXAMPLE");
        HFL::SetColor(HFL::EColor::White);
        std::cout << "This demonstration features a Procedural Level Generator with three layers:\n\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] THE ABSTRACTION (INTERFACE): ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "ILevelBuilder defines the available build steps.\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] THE CONCRETIONS (WORKERS):   ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Dungeon and Castle Builders (implementing the style).\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] THE DIRECTOR (ARCHITECT):    ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "The LevelDirector that orchestrates the 'Recipe' order.\n\n";

        HFL::PrintSection("THE BENEFIT");
        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] STEP-BY-STEP:  ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Construction can be deferred, paused, or run recursively.\n";
        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] IMMUTABILITY:  ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Products can be fully assembled before being returned to the client.\n";
        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] SRP:           ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Singe Responsibility Principle Isolates complex assembly from the logic.\n";

        HFL::WaitForInput();

        // ======================== THE ARCHITECTURE ========================
        HFL::ClearScreen();
        HFL::PrintHeader("THE ARCHITECTURE");

        HFL::PrintSection("THE SEPARATION");
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "In the Builder pattern, the 'Director' owns the algorithm for HOW to\n"
            << "assemble a level, but it is 'blind' to the materials used. By swapping\n"
            << "the concrete Builder, we change the output without touching the recipe.\n\n";

        HFL::PrintSection("IMPLEMENTATION");

        // ======================== PRODUCT ========================
        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] Level (The Product)\n";
        HFL::SetColor(HFL::EColor::White);
        std::cout << "    ROLE:           ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "The resulting complex object. A simple data container.\n";
        HFL::SetColor(HFL::EColor::White);
        std::cout << "    SCOPE:          ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Stores the internal structures (Rooms, Stairs).\n";
        HFL::SetColor(HFL::EColor::White);
        std::cout << "    DEPENDENCY:     ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Knows nothing of the builders or the assembly process.\n\n";

        // ======================== BUILDER ========================
        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] ILevelBuilder (The Interface)\n";
        HFL::SetColor(HFL::EColor::White);
        std::cout << "    ROLE:           ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "The blueprint/contract for construction steps.\n";
        HFL::SetColor(HFL::EColor::White);
        std::cout << "    SCOPE:          ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Defines AddRoom(), AddStairs(), and Build().\n";
        HFL::SetColor(HFL::EColor::White);
        std::cout << "    CONSTRAINT:     ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Must provide a consistent interface for the Director to use.\n\n";

        // ======================== DIRECTOR ========================
        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] LevelDirector (The Manager)\n";
        HFL::SetColor(HFL::EColor::White);
        std::cout << "    ROLE:           ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "The High-Level Policy. It executes specific 'Recipes'.\n";
        HFL::SetColor(HFL::EColor::White);
        std::cout << "    SCOPE:          ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Calls builder methods in a specific sequence.\n";
        HFL::SetColor(HFL::EColor::White);
        std::cout << "    ADVANTAGE:      ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Can produce multiple variations of a product using one Director.\n\n";

        HFL::SetColor(HFL::EColor::White);
        HFL::WaitForInput();

        // ======================== INITIALIZATION ========================
        DungeonBuilder DungeonWorker;
        CastleBuilder CastleWorker;
        LevelDirector Architect;

        while (true)
        {
            HFL::ClearScreen();
            HFL::PrintHeader("BUILDER PATTERN");

            HFL::PrintSection("PHASE 1: CHOOSE STYLE");
            HFL::SetColor(HFL::EColor::White);

            HFL::SetColor(HFL::EColor::Green);
            std::cout << " [1] "; HFL::SetColor(HFL::EColor::White); std::cout << "DUNGEON BUILDER\n";
            HFL::SetColor(HFL::EColor::Green);
            std::cout << " [2] "; HFL::SetColor(HFL::EColor::White); std::cout << "CASTLE BUILDER\n\n";
            HFL::SetColor(HFL::EColor::Green);
            std::cout << " [0] "; HFL::SetColor(HFL::EColor::White); std::cout << "CONTINUE\n";

            int Choice = HFL::GetValidMenuInput(2);
            if (Choice == 0) break;

            if (Choice == 1) Architect.SetBuilder(&DungeonWorker);
            else Architect.SetBuilder(&CastleWorker);

            HFL::PrintSection("PHASE 2: CHOOSE DESIGN");
            HFL::SetColor(HFL::EColor::Green);
            std::cout << " [1] "; HFL::SetColor(HFL::EColor::White); std::cout << "DUNGEON RECIPE\n";
            HFL::SetColor(HFL::EColor::Green);
            std::cout << " [2] "; HFL::SetColor(HFL::EColor::White); std::cout << "CASTLE RECIPE\n\n";
            HFL::SetColor(HFL::EColor::Green);
            std::cout << " [0] "; HFL::SetColor(HFL::EColor::White); std::cout << "BACK\n";

            int Recipe = HFL::GetValidMenuInput(2);
            if (Recipe == 0) continue;

            HFL::ClearScreen();
            HFL::PrintHeader("CONSTRUCTION LOG");

            if (Recipe == 1) Architect.CreateDungeon();
            else Architect.CreateCastle();

            HFL::WaitForInput();
        }

        // ======================== CONCLUSION ========================
        HFL::ClearScreen();
        HFL::PrintHeader("CONCLUSION");

        HFL::PrintSection("ARCHITECTURE & SOLID");
        HFL::SetColor(HFL::EColor::White);
        std::cout << "The implementation of the Builder Pattern confirms the following:\n\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] SINGLE RESPONSIBILITY: ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "The Level class only stores data, the Director only manages\n"
            << "    the order, and the Builder only handles construction.\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] OPEN/CLOSED BOUNDARY:  ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Adding a 'ForestBuilder' or 'SpaceBuilder' requires zero\n"
            << "    changes to the Director's existing recipes or logic.\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] INTERFACE ISOLATION:   ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "The Director only knows the ILevelBuilder interface, preventing\n"
            << "    leaks of concrete implementation details into the policy layer.\n\n";

        HFL::PrintSection("SUMMARY");
        HFL::SetColor(HFL::EColor::White);
        std::cout << "The Builder Pattern ensures that software is:\n\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] CONTROLLED: ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Step-by-step construction prevents 'incomplete' objects from leaking.\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] VERSATILE: ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "The same 'Recipe' (algorithm) can produce vastly different visual\n"
            << "    representations by simply swapping the concrete worker.\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] CLEAN:     ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Bypasses 'Constructor Bloat' by using semantic method names like\n"
            << "    'AddRoom' instead of dozens of confusing bool/int parameters.\n\n";

        HFL::SetColor(HFL::EColor::White);
        std::cout << std::setw(40) << "END OF DEMO\n";
        HFL::WaitForInput();
    }
}