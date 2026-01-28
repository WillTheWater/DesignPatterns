#include "Behavioral/TemplateMethod.h"

namespace TMP
{
    // =========================================================================
    // CONCRETE STEP IMPLEMENTATIONS
    // ROLE: Subclasses provide the "How-To" for specific steps. The overall
    // sequence is managed by the Base Class Template Method in the header.
    // =========================================================================

        // ======================== BLACKSMITHING ========================
    void Blacksmithing::PrepareMaterials() { std::cout << "   [Blacksmith] Smelting iron ore into ingots.\n"; }
    void Blacksmithing::ApplyHeat() { std::cout << "   [Blacksmith] Heating metal in the coal forge.\n"; }
    void Blacksmithing::ShapeMaterial() { std::cout << "   [Blacksmith] Hammering the sword on an anvil.\n"; }
    void Blacksmithing::ApplyTreatment() { std::cout << "   [Blacksmith] Quenching steel in oil for tempering.\n"; }

    // ======================== WOODWORKING ========================
    void Woodworking::PrepareMaterials() { std::cout << "   [Woodworker] Selecting an oak log.\n"; }
    void Woodworking::ShapeMaterial() { std::cout << "   [Woodworker] Chiseling and sanding the wood.\n"; }
    void Woodworking::ApplyTreatment() { std::cout << "   [Woodworker] Applying protective varnish.\n"; }

    // ======================== ALCHEMY ========================
    void Alchemy::PrepareMaterials() { std::cout << "   [Alchemy] Grinding herbs and minerals into powder.\n"; }
    void Alchemy::ApplyHeat() { std::cout << "   [Alchemy] Boiling the solution in a glass beaker.\n"; }
    void Alchemy::ShapeMaterial() { std::cout << "   [Alchemy] Distilling the essence into a vial.\n"; }

    // ======================== TAILORING ========================
    void Tailoring::PrepareMaterials() { std::cout << "   [Tailor] Measuring and cutting fine silk sheets.\n"; }
    void Tailoring::ShapeMaterial() { std::cout << "   [Tailor] Sewing the fabric into a noble's tunic.\n"; }

    // =========================================================================
    // DEMO IMPLEMENTATION
    // =========================================================================

    void RunDemo()
    {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        // ======================== INTRODUCTION ========================
        HFL::ClearScreen();
        HFL::PrintHeader("TEMPLATE METHOD PATTERN");

        HFL::PrintSection("THE DEFINITION");
        HFL::SetColor(HFL::EColor::White);
        std::cout << "Define the skeleton of an algorithm in an operation, deferring some steps to subclasses.\n"
            << "Template Method lets subclasses redefine certain steps of an algorithm without\n"
            << "changing the algorithm's structure.\n\n";

        HFL::PrintSection("THE GOAL");
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "The Template Method Pattern is about 'Structural Enforcement'.\n"
            << "It locks the sequence of a process (The Skeleton) in a base class,\n"
            << "ensuring that subclasses can only change 'What' happens in a step,\n"
            << "but never 'When' or in what order those steps occur.\n\n";

        HFL::PrintSection("THE EXAMPLE");
        HFL::SetColor(HFL::EColor::White);
        std::cout << "This demonstration features a Universal Crafting Station:\n\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] THE TEMPLATE: ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "The fixed order: Prepare -> Heat? -> Shape -> Treatment? -> Done.\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] THE HOOKS:    ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Optional steps like 'Heat' that are toggled by specific professions.\n\n";

        HFL::PrintSection("THE BENEFIT");
        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] NO DUPLICATION:   ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Common workflow logic is written once in the base class.\n";
        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] RIGIDITY:         ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Subclasses cannot accidentally skip the 'Preparation' phase.\n";
        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] CLEAN SUBCLASSES: ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "A Tailor only writes 2 methods; the base handles the rest.\n";

        HFL::WaitForInput();

        // ======================== THE ARCHITECTURE ========================
        HFL::ClearScreen();
        HFL::PrintHeader("THE ARCHITECTURE");

        HFL::PrintSection("RECOGNIZING THE PATTERN");
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "To recognize a Template Method, look for these 4 markers:\n\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] NON-VIRTUAL TEMPLATE METHOD\n";
        HFL::SetColor(HFL::EColor::White);
        std::cout << "    The base class defines a final method that dictates the sequence.\n\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] PRIMITIVE OPERATIONS\n";
        HFL::SetColor(HFL::EColor::White);
        std::cout << "    Subclasses are forced (via = 0) to implement essential core logic.\n\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] HOOKS AND GUARDS\n";
        HFL::SetColor(HFL::EColor::White);
        std::cout << "    Optional steps and booleans allow subclasses to branch the logic.\n\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] INVERSION OF CONTROL\n";
        HFL::SetColor(HFL::EColor::White);
        std::cout << "    The 'Hollywood Principle': Don't call us, we'll call you.\n\n";

        HFL::WaitForInput();

        // Object Setup
        Blacksmithing Forge;
        Woodworking Carpenter;
        Alchemy Lab;
        Tailoring Shop;

        CraftingProcess* CurrentWorker = nullptr;
        std::string StationName = "UNASSIGNED";

        // ======================== GAME LOOP ========================
        while (true)
        {
            HFL::ClearScreen();
            HFL::PrintHeader("UNIVERSAL CRAFTING STATION");

            HFL::PrintSection("STATION STATUS");
            HFL::SetColor(HFL::EColor::White);
            std::cout << "  ACTIVE WORKER:   "; HFL::SetColor(HFL::EColor::Cyan);
            std::cout << StationName << "\n";
            HFL::SetColor(HFL::EColor::White);
            std::cout << "  PTR STATUS:      "; HFL::SetColor(HFL::EColor::Gray);
            std::cout << (CurrentWorker ? "VALID BINDING" : "NULL") << "\n\n";

            HFL::PrintSection("COMMANDS");
            HFL::SetColor(HFL::EColor::Green);
            std::cout << " [1] "; HFL::SetColor(HFL::EColor::White); std::cout << "ASSIGN BLACKSMITH\n";
            HFL::SetColor(HFL::EColor::Green);
            std::cout << " [2] "; HFL::SetColor(HFL::EColor::White); std::cout << "ASSIGN WOODWORKER\n";
            HFL::SetColor(HFL::EColor::Green);
            std::cout << " [3] "; HFL::SetColor(HFL::EColor::White); std::cout << "ASSIGN ALCHEMIST\n";
            HFL::SetColor(HFL::EColor::Green);
            std::cout << " [4] "; HFL::SetColor(HFL::EColor::White); std::cout << "ASSIGN TAILOR\n";
            HFL::SetColor(HFL::EColor::Yellow);
            std::cout << " [5] "; HFL::SetColor(HFL::EColor::White); std::cout << "CRAFT\n\n";
            HFL::SetColor(HFL::EColor::Green);
            std::cout << " [0] "; HFL::SetColor(HFL::EColor::White); std::cout << "CONTINUE\n\n";

            int Choice = HFL::GetValidMenuInput(5);
            if (Choice == 0) break;

            std::cout << "\n======================== EXECUTION LOG ========================\n";
            HFL::SetColor(HFL::EColor::Gray);

            if (Choice >= 1 && Choice <= 4)
            {
                if (Choice == 1) { CurrentWorker = &Forge;     StationName = "BLACKSMITH"; }
                else if (Choice == 2) { CurrentWorker = &Carpenter; StationName = "WOODWORKER"; }
                else if (Choice == 3) { CurrentWorker = &Lab;       StationName = "ALCHEMIST"; }
                else if (Choice == 4) { CurrentWorker = &Shop;      StationName = "TAILOR"; }

                std::cout << ">> System: Swapped polymorphism binding to " << StationName << ".\n";
            }
            else if (Choice == 5)
            {
                if (CurrentWorker)
                {
                    std::cout << "[Context] Calling: Base->CraftItem();\n";
                    HFL::SetColor(HFL::EColor::White);
                    CurrentWorker->CraftItem();
                }
                else
                {
                    HFL::SetColor(HFL::EColor::Red);
                    std::cout << "!! Error: Select a profession before crafting.\n";
                }
            }

            std::cout << "===============================================================\n";
            HFL::WaitForInput();
        }

        // ======================== CONCLUSION ========================
        HFL::ClearScreen();
        HFL::PrintHeader("CONCLUSION");

        HFL::PrintSection("ARCHITECTURE");
        HFL::SetColor(HFL::EColor::White);
        std::cout << "The implementation of the Template Method Pattern confirms the following:\n\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] LOGIC CENTRALIZATION:     ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "The sequence 'Prepare -> Heat -> Shape' is written once.\n"
            << "    This prevents errors where a developer might forget a step.\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] PROTECTED STRUCTURE:      ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Because CraftItem() is non-virtual, subclasses literally cannot\n"
            << "    change the order. They can only fill in the specifics.\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] HOOK FLEXIBILITY:         ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "By using Hooks like 'RequiresHeat', the algorithm remains generic\n"
            << "    enough to support wildly different use-cases like Tailoring vs Smithing.\n\n";

        HFL::PrintSection("SUMMARY");
        HFL::SetColor(HFL::EColor::White);
        std::cout << "The Template Method Pattern ensures that software is:\n\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] STANDARDIZED: ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "All subclasses follow the exact same procedural standard.\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] EFFICIENT:    ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Redundant 'workflow' code is removed from subclasses.\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] PREDICTABLE:  ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "The execution flow is guaranteed by the base class.\n\n";

        HFL::SetColor(HFL::EColor::White);
        HFL::WaitForInput();
    }
}