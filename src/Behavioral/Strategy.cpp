#include "Behavioral/Strategy.h"

namespace STR
{
    // =========================================================================
    // STRATEGY IMPLEMENTATIONS
    // ROLE: Specialized workers that perform isolated algorithms. These classes
    // contain the specific math and rules for different travel modes.
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
    // ROLE: The high-level Navigator. It does not implement pathfinding logic 
    // itself; it delegates that task to the active strategy.
    // =========================================================================

    MapNavigator::MapNavigator(std::shared_ptr<IPathStrategy> InitialStrategy)
        : ActiveStrategy(InitialStrategy)
    {
    }

    void MapNavigator::UpdateMiniMap(const std::string& Goal)
    {
        // ======================== THE KEY MOMENT ========================
        // The Navigator contains no conditional logic. It blindly invokes 
        // whatever strategy is currently active.
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

        // ======================== INTRODUCTION ========================
        HFL::ClearScreen();
        HFL::PrintHeader("STRATEGY DESIGN PATTERN");

        HFL::PrintSection("THE DEFINITION");
        HFL::SetColor(HFL::EColor::White);
        std::cout << "Define a family of algorithms, encapsulate each one, and make them interchangeable.\n"
            << "Strategy lets the algorithm vary independently from the clients that use it.\n\n";

        HFL::PrintSection("THE GOAL");
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "The Strategy Pattern is about 'Interchangeable Behavior'.\n"
            << "It replaces hard-coded conditional logic (if/switch) with encapsulated\n"
            << "objects, allowing a system to switch its logic at runtime.\n\n";

        HFL::PrintSection("THE EXAMPLE");
        HFL::SetColor(HFL::EColor::White);
        std::cout << "This demonstration features a Fantasy Navigation system for a Mini-Map:\n\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] THE STRATEGY: ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Encapsulated pathfinding logic (Walking, Riding, Flying).\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] THE CONTEXT:  ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "The MapNavigator class that uses the active strategy.\n\n";

        HFL::PrintSection("THE BENEFIT");
        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] CLEAN CODE:    ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Eliminates massive switch statements in the navigator.\n";
        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] RUNTIME SWAP:  ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Behaviors can change instantly (e.g., when a player mounts).\n";
        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] EXTENSIBLE:    ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Adding 'Swimming' requires zero changes to existing classes.\n";

        HFL::WaitForInput();

        // ======================== THE ARCHITECTURE ========================
        HFL::ClearScreen();
        HFL::PrintHeader("THE ARCHITECTURE");

        HFL::PrintSection("DECOUPLING LOGIC");
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Strategy adherence is identified via 4 key markers:\n\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] ENCAPSULATED ALGORITHMS\n";
        HFL::SetColor(HFL::EColor::White);
        std::cout << "    Walking, Riding, and Flying rules are isolated in separate classes.\n\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] DELEGATION OF RESPONSIBILITY\n";
        HFL::SetColor(HFL::EColor::White);
        std::cout << "    MapNavigator::UpdateMiniMap() contains no 'if' or 'switch' logic.\n\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] RUNTIME INTERCHANGEABILITY\n";
        HFL::SetColor(HFL::EColor::White);
        std::cout << "    The strategy pointer is reassigned without re-instantiating the Context.\n\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] OPEN-CLOSED PRINCIPLE\n";
        HFL::SetColor(HFL::EColor::White);
        std::cout << "    Adding new mounts requires no code changes to the Navigator class.\n\n";

        HFL::WaitForInput();

        // ======================== INITIALIZATION ========================
        auto Walk = std::make_shared<WalkingPath>();
        auto Mount = std::make_shared<MountPath>();
        auto Flight = std::make_shared<FlyingPath>();

        // Data Setup
        std::array<std::string, 10> Locations =
        {
            "Rogue Encampment", "Tristram", "The Monastery", "Lut Gholein", "Arcane Sanctuary",
            "Kurast Docks", "Durance of Hate", "Pandemonium Fortress", "Chaos Sanctuary",
            "Harrogath"
        };

        MapNavigator Navigator(Walk);
        std::string CurrentGoal = Locations[0];

        // ======================== GAME LOOP ========================
        while (true)
        {
            HFL::ClearScreen();
            HFL::PrintHeader("FANTASY TRAVEL");

            HFL::PrintSection("NAVIGATION STATUS");
            HFL::SetColor(HFL::EColor::White);
            std::cout << "  DESTINATION:     "; HFL::SetColor(HFL::EColor::Yellow);
            std::cout << CurrentGoal << "\n";
            HFL::SetColor(HFL::EColor::White);
            std::cout << "  ACTIVE MODE:     "; HFL::SetColor(HFL::EColor::Cyan);
            std::cout << Navigator.GetCurrentMode() << "\n\n";

            HFL::PrintSection("COMMANDS");
            HFL::SetColor(HFL::EColor::Green);
            std::cout << " [1] "; HFL::SetColor(HFL::EColor::White); std::cout << "REFRESH MINI-MAP\n";
            HFL::SetColor(HFL::EColor::Green);
            std::cout << " [2] "; HFL::SetColor(HFL::EColor::White); std::cout << "SWITCH TO WALKING\n";
            HFL::SetColor(HFL::EColor::Green);
            std::cout << " [3] "; HFL::SetColor(HFL::EColor::White); std::cout << "SWITCH TO BEAR MOUNT\n";
            HFL::SetColor(HFL::EColor::Green);
            std::cout << " [4] "; HFL::SetColor(HFL::EColor::White); std::cout << "SWITCH TO GRIFFON MOUNT\n";
            HFL::SetColor(HFL::EColor::Green);
            std::cout << " [5] "; HFL::SetColor(HFL::EColor::White); std::cout << "CHANGE DESTINATION\n\n";
            HFL::SetColor(HFL::EColor::Green);
            std::cout << " [0] "; HFL::SetColor(HFL::EColor::White); std::cout << "CONTINUE\n\n";

            int Choice = HFL::GetValidMenuInput(5);
            if (Choice == 0) break;

            if (Choice == 5)
            {
                HFL::PrintSection("LOCATION SELECTION");
                for (int i = 0; i < Locations.size(); ++i)
                {
                    HFL::SetColor(HFL::EColor::Green);
                    std::cout << " [" << i + 1 << "] ";
                    HFL::SetColor(HFL::EColor::White);
                    std::cout << Locations[i] << "\n";
                }
                int LocChoice = HFL::GetValidMenuInput(static_cast<int>(Locations.size()));
                CurrentGoal = Locations[LocChoice - 1];
                continue;
            }

            std::cout << "\n======================== EXECUTION LOG ========================\n";
            HFL::SetColor(HFL::EColor::Gray);
            if (Choice == 1)
            {
                std::cout << "[Context] Calling: Navigator.UpdateMiniMap();\n";
                HFL::SetColor(HFL::EColor::White);
                Navigator.UpdateMiniMap(CurrentGoal);
            }
            else if (Choice == 2) { Navigator.ChangeTravelMode(Walk); std::cout << ">> Strategy swapped to Walking.\n"; }
            else if (Choice == 3) { Navigator.ChangeTravelMode(Mount); std::cout << ">> Strategy swapped to Bear Mount.\n"; }
            else if (Choice == 4) { Navigator.ChangeTravelMode(Flight); std::cout << ">> Strategy swapped to Flying.\n"; }

            std::cout << "===============================================================================\n";
            HFL::WaitForInput();
        }

        // ======================== CONCLUSION ========================
        HFL::ClearScreen();
        HFL::PrintHeader("CONCLUSION");

        HFL::PrintSection("ARCHITECTURE");
        HFL::SetColor(HFL::EColor::White);
        std::cout << "The implementation of the Strategy Pattern confirms the following:\n\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] ALGORITHMIC INDEPENDENCE: ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "The MapNavigator is unaware of terrain constraints or flight math.\n"
            << "    The logic is entirely contained within the concrete Strategy objects.\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] SIMPLIFIED CONTEXT LOGIC: ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "The absence of conditional branching prevents the Navigator from\n"
            << "    growing too complex as more travel modes are added.\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] RUNTIME FLUIDITY:         ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Behaviors are swapped instantly as the game state changes,\n"
            << "    ensuring the Mini-Map always uses the optimal calculation.\n\n";

        HFL::PrintSection("SUMMARY");
        HFL::SetColor(HFL::EColor::White);
        std::cout << "The Strategy Pattern ensures that software is:\n\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] MODULAR:   ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "New behaviors are added by creating new classes, not editing old ones.\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] TESTABLE:  ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Individual algorithms can be unit tested in complete isolation.\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] FLEXIBLE:  ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "The system adapts its internal logic dynamically at runtime.\n\n";

        HFL::SetColor(HFL::EColor::White);
        HFL::WaitForInput();
    }
}