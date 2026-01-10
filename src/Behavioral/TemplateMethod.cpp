#include "Behavioral/TemplateMethod.h"

namespace TMP
{
    // =========================================================================
    // BLACKSMITHING
    // =========================================================================
    void Blacksmithing::PrepareMaterials() { std::cout << "[Blacksmith] Smelting iron ore into ingots.\n"; }
    void Blacksmithing::ApplyHeat() { std::cout << "[Blacksmith] Heating metal in the coal forge.\n"; }
    void Blacksmithing::ShapeMaterial() { std::cout << "[Blacksmith] Hammering the sword on an anvil.\n"; }
    void Blacksmithing::ApplyTreatment() { std::cout << "[Blacksmith] Quenching steel in oil for tempering.\n"; }

    // =========================================================================
    // WOODWORKING
    // =========================================================================
    void Woodworking::PrepareMaterials() { std::cout << "[Woodworker] Selecting an oak log.\n"; }
    void Woodworking::ShapeMaterial() { std::cout << "[Woodworker] Chiseling and sanding the wood.\n"; }
    void Woodworking::ApplyTreatment() { std::cout << "[Woodworker] Applying protective varnish.\n"; }

    // =========================================================================
    // ALCHEMY
    // =========================================================================
    void Alchemy::PrepareMaterials() { std::cout << "[Alchemy] Grinding herbs and minerals into powder.\n"; }
    void Alchemy::ApplyHeat() { std::cout << "[Alchemy] Boiling the solution in a glass beaker.\n"; }
    void Alchemy::ShapeMaterial() { std::cout << "[Alchemy] Distilling the essence into a vial.\n"; }

    // =========================================================================
    // TAILORING
    // =========================================================================
    void Tailoring::PrepareMaterials() { std::cout << "[Tailor] Measuring and cutting fine silk sheets.\n"; }
    void Tailoring::ShapeMaterial() { std::cout << "[Tailor] Sewing the fabric into a noble's tunic.\n"; }

    // =========================================================================
    // DEMO IMPLEMENTATION
    // =========================================================================
    void RunDemo()
    {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        HFL::ClearScreen();
        HFL::PrintHeader("Template Method Pattern (Fantasy Crafting)");

        std::cout << "Definition:\n";
        std::cout << "Fix the algorithm's skeleton in the base class.\n";
        std::cout << "Defer specific step implementations to specialized subclasses.\n\n";

        std::cout << "In This Demo:\n";
        std::cout << "Crafting always follows: Prepare -> [Heat] -> Shape -> [Treatment].\n";
        std::cout << "Four professions (Smithing, Woodworking, Alchemy, Tailoring)\n use the same skeleton.\n";
        std::cout << "They use 'Control Hooks' to skip steps that don't apply to their craft.\n";

        HFL::WaitForInput();

        // --- STEP 2: THE RULES ---
        HFL::ClearScreen();
        HFL::PrintHeader("Step 1: The Architecture");

        std::cout << "To recognize a Template Method, look for these 4 ideas:\n\n";

        std::cout << "1. Non-Virtual Template Method:\n";
        std::cout << "   The base class defines the fixed order of operations.\n\n";

        std::cout << "2. Primitive Operations:\n";
        std::cout << "   Subclasses are forced to implement the essential parts (e.g., Prepare).\n\n";

        std::cout << "3. Hooks and Guards:\n";
        std::cout << "   Optional steps (Hooks) and Booleans (Guards) allow for branching.\n\n";

        std::cout << "4. Inversion of Control:\n";
        std::cout << "   The base class calls the subclass code.\n";

        HFL::WaitForInput();

        // Setup the Crafting Stations
        // We use a pointer to the base class to show that the Station 
        // doesn't care about the specific profession.
        CraftingProcess* CurrentWorker = nullptr;
        std::string StationName = "Empty Station";

        // Concrete instances
        Blacksmithing Forge;
        Woodworking Carpenter;
        Alchemy Lab;
        Tailoring Shop;

        while (true)
        {
            HFL::ClearScreen();
            HFL::PrintHeader("The Universal Crafting");

            std::cout << "CRAFT STATION STATUS:\n";
            std::cout << ">> Current Station: " << (CurrentWorker ? "ASSIGNED" : "NONE") << "\n";
            std::cout << "--------------------------------------------------\n\n";

            std::cout << "1. Blacksmith\n";
            std::cout << "2. Woodworking\n";
            std::cout << "3. Alchemy\n";
            std::cout << "4. Tailoring\n";
            std::cout << "5. [ CRAFT BUTTON ]\n";
            std::cout << "0. Exit\n\n";

            int Choice = HFL::GetValidMenuInput(5);
            if (Choice == 0) break;

            std::cout << "\n--- SYSTEM EXECUTION LOG ---\n";

            if (Choice >= 1 && Choice <= 4)
            {
                if (Choice == 1) { CurrentWorker = &Forge;     StationName = "Blacksmith Forge"; }
                else if (Choice == 2) { CurrentWorker = &Carpenter; StationName = "Woodworking Bench"; }
                else if (Choice == 3) { CurrentWorker = &Lab;       StationName = "Alchemy Lab"; }
                else if (Choice == 4) { CurrentWorker = &Shop;      StationName = "Tailoring Table"; }

                std::cout << ">> System: Swapped to " << StationName << ".\n";
                std::cout << ">> System: Template Method pointer updated.\n";
            }
            else if (Choice == 5)
            {
                if (CurrentWorker)
                {
                    // THIS IS THE MOMENT:
                    // We call the same function on the same pointer type,
                    // but the Template Method dictates a different flow.
                    std::cout << "[Station] 'Craft' signal sent to Template...\n";
                    CurrentWorker->CraftItem();
                }
                else
                {
                    std::cout << "!! ERROR: No assigned station.\n";
                }
            }
            std::cout << "----------------------------\n";

            HFL::WaitForInput();
        }

        // --- STEP 4: CONCLUSION ---
        HFL::ClearScreen();
        HFL::PrintHeader("Conclusion");

        std::cout << "Summary of Template Method:\n\n";

        std::cout << "1. Logic Centralization.\n";
        std::cout << "   Boilerplate flow code is written once and inherited by all crafts.\n\n";

        std::cout << "2. Protected Algorithm Structure.\n";
        std::cout << "   The non-virtual 'CraftItem' prevents subclasses from breaking the sequence.\n\n";

        std::cout << "3. Granular Customization.\n";
        std::cout << "   The Tailor only writes 2 methods. The Smith writes 4.\n";
        std::cout << "   Subclasses only implement the 'How' for the steps they need.\n\n";

        std::cout << std::setw(40) << "End of Demo\n";
        HFL::WaitForInput();
    }
}