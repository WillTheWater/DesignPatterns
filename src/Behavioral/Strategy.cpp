#include "Behavioral/Strategy.h"

namespace STR
{
    // =========================================================================
    // STRATEGY IMPLEMENTATIONS
    // =========================================================================

    void WalkingPath::CalculatePath(const std::string& Destination)
    {
        std::cout << "   [Pathfinder] Scanning terrain for roads and bridges...\n";
        std::cout << "   [Pathfinder] Logic: Avoiding mountains, water, and dense forests.\n";
        std::cout << "   [Pathfinder] Route to " << Destination << " established via main path.\n";
    }

    void MountPath::CalculatePath(const std::string& Destination)
    {
        std::cout << "   [Pathfinder] Scanning for cross-country shortcuts...\n";
        std::cout << "   [Pathfinder] Logic: Bypassing fences and streams. Avoiding high mountains.\n";
        std::cout << "   [Pathfinder] Route to " << Destination << " established via wilderness trail.\n";
    }

    void FlyingPath::CalculatePath(const std::string& Destination)
    {
        std::cout << "   [Pathfinder] Calculating direct vector (Line-of-Sight)...\n";
        std::cout << "   [Pathfinder] Logic: Ignoring all ground terrain and obstacles.\n";
        std::cout << "   [Pathfinder] Route to " << Destination << " established via direct flight.\n";
    }

    // =========================================================================
    // CONTEXT IMPLEMENTATION
    // =========================================================================

    MapNavigator::MapNavigator(std::shared_ptr<IPathStrategy> InitialStrategy)
        : ActiveStrategy(InitialStrategy) {
    }

    void MapNavigator::UpdateMiniMap(const std::string& Goal)
    {
        // The Navigator itself contains no pathfinding logic. 
        // It delegates entirely to the encapsulated strategy.
        ActiveStrategy->CalculatePath(Goal);
    }

    void MapNavigator::ChangeTravelMode(std::shared_ptr<IPathStrategy> NewStrategy)
    {
        ActiveStrategy = NewStrategy;
    }

    // =========================================================================
    // DEMO IMPLEMENTATION
    // =========================================================================
    void RunDemo()
    {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        // --- STEP 1: INTRODUCTION ---
        HFL::ClearScreen();
        HFL::PrintHeader("Strategy Pattern (Fantasy Navigation)");

        std::cout << "Definition:\n";
        std::cout << "Encapsulate a family of algorithms within interchangeable objects.\n";
        std::cout << "Decouple the 'How' (Algorithm) from the 'Who' (Context).\n\n";

        std::cout << "In This Demo:\n";
        std::cout << "A MapNavigator manages route calculation to 'The Dragon's Lair'.\n";
        std::cout << "Pathfinding rules change dynamically based on the active Travel Strategy.\n";
        std::cout << "The Navigator class remains unchanged as new mounts are discovered.\n";

        HFL::WaitForInput();

        // --- STEP 2: THE ARCHITECTURE ---
        HFL::ClearScreen();
        HFL::PrintHeader("Step 1: The Architecture");

        std::cout << "Strategy adherence is identified via 4 key markers:\n\n";

        std::cout << "1. Encapsulated Algorithms:\n";
        std::cout << "   Walking, Riding, and Flying rules are isolated in separate classes.\n\n";



        std::cout << "2. Delegation of Responsibility:\n";
        std::cout << "   MapNavigator::UpdateMiniMap() contains no 'if' or 'switch' logic.\n\n";

        std::cout << "3. Runtime Interchangeability:\n";
        std::cout << "   The strategy pointer is reassigned without re-instantiating the Navigator.\n\n";

        std::cout << "4. Open-Closed Principle:\n";
        std::cout << "   Adding a 'Swimming' or 'Teleport' strategy requires no code changes to MapNavigator.\n";

        HFL::WaitForInput();

        // Object Setup
        auto Walk = std::make_shared<WalkingPath>();
        auto Mount = std::make_shared<MountPath>();
        auto Flight = std::make_shared<FlyingPath>();

        MapNavigator Navigator(Walk);
        std::string CurrentGoal = "The Dragon's Lair";

        // --- STEP 3: DYNAMIC SIMULATION ---
        while (true)
        {
            HFL::ClearScreen();
            HFL::PrintHeader("Fantasy Mini-Map Simulator");

            std::cout << "NAVIGATION STATUS:\n";
            std::cout << ">> Target Destination: " << CurrentGoal << "\n";
            std::cout << ">> Active Travel Mode: " << Navigator.GetCurrentMode() << "\n";
            std::cout << "--------------------------------------------------\n\n";

            std::cout << "Select Action:\n";
            std::cout << "1. Refresh Mini-Map (Execute Strategy)\n";
            std::cout << "2. Switch to Walking\n";
            std::cout << "3. Switch to Bear Mount\n";
            std::cout << "4. Switch to Griffon Mount\n";
            std::cout << "0. Exit\n\n";

            int Choice = HFL::GetValidMenuInput(4);
            if (Choice == 0) break;

            std::cout << "\n--- EXECUTION LOG ---\n";
            if (Choice == 1)
            {
                std::cout << "[Context] Calling: Navigator.UpdateMiniMap();\n";
                Navigator.UpdateMiniMap(CurrentGoal);
            }
            else if (Choice == 2) Navigator.ChangeTravelMode(Walk);
            else if (Choice == 3) Navigator.ChangeTravelMode(Mount);
            else if (Choice == 4) Navigator.ChangeTravelMode(Flight);

            if (Choice != 1) std::cout << ">> System: Pathfinding Strategy Updated.\n";
            std::cout << "---------------------\n";

            HFL::WaitForInput();
        }

        // --- STEP 4: CONCLUSION ---
        HFL::ClearScreen();
        HFL::PrintHeader("Conclusion");

        std::cout << "Summary of Strategy:\n\n";

        std::cout << "1. Algorithmic Independence.\n";
        std::cout << "   The MapNavigator is unaware of terrain constraints or flight math.\n";
        std::cout << "   The logic is entirely contained within the concrete Strategy objects.\n\n";

        std::cout << "2. Simplified Context Logic.\n";
        std::cout << "   The absence of conditional branching prevents the Navigator from growing too complex.\n";
        // 
        std::cout << "   Maintenance is performed on individual strategies in isolation.\n\n";

        std::cout << "3. Runtime Fluidity.\n";
        std::cout << "   Behaviors are swapped instantly as the game state changes.\n";
        std::cout << "   The user experience is seamless while the internal math is replaced.\n\n";

        std::cout << std::setw(40) << "End of Demo\n";
        HFL::WaitForInput();
    }
}