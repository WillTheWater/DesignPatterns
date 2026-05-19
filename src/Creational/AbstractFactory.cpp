#include "Creational/AbstractFactory.h"

namespace AFT
{
    // =========================================================================
    // CONCRETE PRODUCTS: IMPLEMENTATION
    // ROLE: Defining specific behaviors for each biome-related product.
    // =========================================================================

    // ======================== FOREST ========================
    void ForestCombat::Attack() const { std::cout << ">> [Combat] Attacks with sticks and stones.\n"; }
    void ForestBehavior::Behavior() const { std::cout << ">> [AI] Hides behind trees and waits to ambush.\n"; }
    void ForestLoot::DropLoot() const { std::cout << ">> [Loot] Drops healing herbs and magic wood.\n"; }

    // ======================== DESERT ========================
    void DesertCombat::Attack() const { std::cout << ">> [Combat] Throws fire and blinding sand.\n"; }
    void DesertBehavior::Behavior() const { std::cout << ">> [AI] Uses hit-and-run tactics across the dunes.\n"; }
    void DesertLoot::DropLoot() const { std::cout << ">> [Loot] Drops rare minerals and desert cloth.\n"; }

    // ======================== SWAMP ========================
    void SwampCombat::Attack() const { std::cout << ">> [Combat] Spits poison and performs a death-roll.\n"; }
    void SwampBehavior::Behavior() const { std::cout << ">> [AI] Chases endlessly in packs through the muck.\n"; }
    void SwampLoot::DropLoot() const { std::cout << ">> [Loot] Drops toxic scales and predator teeth.\n"; }

    // =========================================================================
    // CONCRETE FACTORIES: IMPLEMENTATION
    // ROLE: Enforcing consistency by only creating related product families.
    // =========================================================================

    // ======================== FOREST BIOME FACTORY ========================
    std::unique_ptr<INPC> ForestBiomeFactory::CreateNPC() { return std::make_unique<ForestNPC>(); }
    std::unique_ptr<ICombatBehavior> ForestBiomeFactory::CreateCombat() { return std::make_unique<ForestCombat>(); }
    std::unique_ptr<IAIBehavior> ForestBiomeFactory::CreateBehavior() { return std::make_unique<ForestBehavior>(); }
    std::unique_ptr<ILootTable> ForestBiomeFactory::CreateLootTable() { return std::make_unique<ForestLoot>(); }

    // ======================== DESERT BIOME FACTORY ========================
    std::unique_ptr<INPC> DesertBiomeFactory::CreateNPC() { return std::make_unique<DesertNPC>(); }
    std::unique_ptr<ICombatBehavior> DesertBiomeFactory::CreateCombat() { return std::make_unique<DesertCombat>(); }
    std::unique_ptr<IAIBehavior> DesertBiomeFactory::CreateBehavior() { return std::make_unique<DesertBehavior>(); }
    std::unique_ptr<ILootTable> DesertBiomeFactory::CreateLootTable() { return std::make_unique<DesertLoot>(); }

    // ======================== SWAMP BIOME FACTORY ========================
    std::unique_ptr<INPC> SwampBiomeFactory::CreateNPC() { return std::make_unique<SwampNPC>(); }
    std::unique_ptr<ICombatBehavior> SwampBiomeFactory::CreateCombat() { return std::make_unique<SwampCombat>(); }
    std::unique_ptr<IAIBehavior> SwampBiomeFactory::CreateBehavior() { return std::make_unique<SwampBehavior>(); }
    std::unique_ptr<ILootTable> SwampBiomeFactory::CreateLootTable() { return std::make_unique<SwampLoot>(); }

    // =========================================================================
    // CLIENT IMPLEMENTATION: NPC SPAWNER
    // =========================================================================

    void NPCSpawner::SpawnNPC()
    {
        if (!CurrentFactory)
        {
            HFL::SetColor(HFL::EColor::Red);
            std::cout << "[Error] No Biome Factory set!\n";
            return;
        }

        // The client creates multiple related objects without knowing their concrete types.
        auto NewNPC = CurrentFactory->CreateNPC();
        auto AI = CurrentFactory->CreateBehavior();
        auto Combat = CurrentFactory->CreateCombat();
        auto Loot = CurrentFactory->CreateLootTable();

        if (NewNPC)
        {
            HFL::SetColor(HFL::EColor::White);
            std::cout << "\nA " << NewNPC->GetName() << " emerges from the shadows!\n";

            HFL::SetColor(HFL::EColor::Gray);
            AI->Behavior();
            Combat->Attack();

            std::cout << "\n>> [Action] You defeat the " << NewNPC->GetName() << "!\n";
            Loot->DropLoot();
        }
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
        HFL::PrintHeader("ABSTRACT FACTORY");

        HFL::PrintSection("THE DEFINITION");
        HFL::SetColor(HFL::EColor::White);
        std::cout << "Provide an interface for creating families of related or dependent\n"
            << "objects without specifying their concrete classes.\n\n";

        HFL::PrintSection("THE GOAL");
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "The Abstract Factory models 'NPC Ecosystems'. In complex games, objects\n"
            << "rarely exist in isolation. A Biome defines a consistent 'Family' of\n"
            << "logic (AI, Combat, Loot). This pattern ensures that these components\n"
            << "always remain compatible without hard-coding them into the spawner.\n\n";

        HFL::PrintSection("THE EXAMPLE");
        HFL::SetColor(HFL::EColor::White);
        std::cout << "This demonstration simulates a World Generator with three distinct layers:\n\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] THE CATALOG (ABSTRACT FACTORY): ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "IBiomeNPCFactory defines the blueprint.\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] THE BIOMES (CONCRETIONS):       ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Forest, Desert, and Swamp sub-factories.\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] THE SPAWNER (CLIENT):           ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "The NPCSpawner that requests thematic units.\n\n";

        HFL::PrintSection("THE BENEFIT");
        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] CONSISTENCY:   ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Guarantees that Forest NPCs never use Swamp attacks.\n";
        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] ENCAPSULATION: ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Creation logic is hidden; the client only sees high-level interfaces.\n";
        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] FLEXIBILITY:   ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Adding a 'Volcano' biome requires zero changes to existing Spawner code.\n";

        HFL::WaitForInput();

        // ======================== THE ARCHITECTURE ========================
        HFL::ClearScreen();
        HFL::PrintHeader("THE ARCHITECTURE");

        HFL::PrintSection("THE ECOSYSTEM MODEL");
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Instead of creating a single object, create a 'Factory of Factories'.\n"
            << "The Client holds a pointer to the Abstract interface, allowing it to\n"
            << "reconfigure the entire game's NPC logic by swapping a single factory instance.\n\n";

        HFL::PrintSection("IMPLEMENTATION");

        // ======================== INTERFACES ========================
        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] INPC / ICombat / IAI / ILoot (Abstract Products)\n";
        HFL::SetColor(HFL::EColor::White);
        std::cout << "    ROLE:           ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "The Interfaces. Defines what an NPC 'is' and 'does'.\n";
        HFL::SetColor(HFL::EColor::White);
        std::cout << "    SCOPE:          ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Pure virtual functions for Attack(), Behavior(), and DropLoot().\n";
        HFL::SetColor(HFL::EColor::White);
        std::cout << "    RELATIONSHIP:   ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "These are the generic 'parts' that make up a complete NPC.\n\n";

        // ======================== FACTORIES ========================
        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] Forest / Desert / Swamp (Concrete Factories)\n";
        HFL::SetColor(HFL::EColor::White);
        std::cout << "    ROLE:           ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "The Assemblers. They know which specific parts belong together.\n";
        HFL::SetColor(HFL::EColor::White);
        std::cout << "    SCOPE:          ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Implements IBiomeNPCFactory. Returns biome-specific concretions.\n";
        HFL::SetColor(HFL::EColor::White);
        std::cout << "    CONSTRAINT:     ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Enforces that a Desert Factory ONLY produces Desert components.\n\n";

        // ======================== CLIENT ========================
        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] NPCSpawner (The Client)\n";
        HFL::SetColor(HFL::EColor::White);
        std::cout << "    ROLE:           ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "The Orchestrator. It populates the world based on the current biome.\n";
        HFL::SetColor(HFL::EColor::White);
        std::cout << "    SCOPE:          ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Uses Dependency Injection to receive a Biome Factory.\n";
        HFL::SetColor(HFL::EColor::White);
        std::cout << "    ADVANTAGE:      ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Spawner remains 100% clean of 'switch' statements and type-checks.\n\n";

        HFL::SetColor(HFL::EColor::White);
        HFL::WaitForInput();

        // ======================== INITIALIZATION ========================
        ForestBiomeFactory ForestFactory;
        DesertBiomeFactory DesertFactory;
        SwampBiomeFactory  SwampFactory;
        NPCSpawner WorldSpawner;

        // ======================== GAME LOOP ========================
        while (true)
        {
            HFL::ClearScreen();
            HFL::PrintHeader("WORLD MAP: TRAVELER");

            HFL::PrintSection("SELECT DESTINATION");
            HFL::SetColor(HFL::EColor::Green);
            std::cout << " [1] "; HFL::SetColor(HFL::EColor::White); std::cout << "THE FOREST\n";
            HFL::SetColor(HFL::EColor::Green);
            std::cout << " [2] "; HFL::SetColor(HFL::EColor::White); std::cout << "THE DESERT\n";
            HFL::SetColor(HFL::EColor::Green);
            std::cout << " [3] "; HFL::SetColor(HFL::EColor::White); std::cout << "THE SWAMPLAND\n\n";
            HFL::SetColor(HFL::EColor::Green);
            std::cout << " [0] "; HFL::SetColor(HFL::EColor::White); std::cout << "CONTINUE\n";

            int Choice = HFL::GetValidMenuInput(3);
            if (Choice == 0) break;

            IBiomeNPCFactory* SelectedFactory = nullptr;

            HFL::ClearScreen();
            HFL::PrintHeader("TRAVELING");

            if (Choice == 1)
            {
                SelectedFactory = &ForestFactory;
                std::cout << "You trek into the dense woods. The air is cool and misty";
            }
            else if (Choice == 2)
            {
                SelectedFactory = &DesertFactory;
                std::cout << "The heat is oppressive. Sand stings your eyes";
            }
            else if (Choice == 3)
            {
                SelectedFactory = &SwampFactory;
                std::cout << "The ground sinks beneath your boots. Foul odors rise from the mud";
            }

            if (SelectedFactory)
            {
                HFL::WaitDots(0.5f);
                WorldSpawner.SetBiomeFactory(SelectedFactory);
                HFL::PrintSection("ENCOUNTER");
                WorldSpawner.SpawnNPC();
            }

            HFL::WaitForInput();
        }

        // ======================== CONCLUSION ========================
        HFL::ClearScreen();
        HFL::PrintHeader("CONCLUSION");

        HFL::PrintSection("ARCHITECTURE");
        HFL::SetColor(HFL::EColor::White);
        std::cout << "The implementation of the Abstract Factory Pattern confirms the following:\n\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] FAMILY CONSISTENCY:    ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "The 'NPCSpawner' is guaranteed to receive a compatible set of\n"
            << "    objects. It is impossible to spawn a 'Desert Raider' that\n"
            << "    accidentally uses 'Forest AI' logic.\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] CREATION DECOUPLING:   ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "The logic for *how* to build an NPC ecosystem is removed from\n"
            << "    the Game World and tucked away into specialized Factory classes.\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] INTERFACE RELIANCE:    ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "The Client (Spawner) remains 'blind' to concrete classes like\n"
            << "    'SwampGator' or 'ForestImp', preventing header pollution.\n\n";

        HFL::PrintSection("SUMMARY");
        HFL::SetColor(HFL::EColor::White);
        std::cout << "The Abstract Factory Pattern ensures that software is:\n\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] COHESIVE: ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Related products are grouped together, ensuring thematic integrity.\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] EXTENSIBLE: ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Adding new Biomes (Volcano, Tundra) only requires a new Factory\n"
            << "    worker, leaving the high-level spawning logic untouched.\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] PLUGGABLE: ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "The entire 'look and feel' of an encounter can be swapped at\n"
            << "    runtime by injecting a different Factory instance.\n\n";

        HFL::SetColor(HFL::EColor::White);
        HFL::WaitForInput();
    }
}