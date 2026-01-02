#include "Creational/AbstractFactory.h"

namespace AFT
{
    // ------------------------------------------------------------------------
    // 3. CONCRETE FACTORIES IMPLEMENTATION
    // ------------------------------------------------------------------------

    // --- FOREST BIOME FACTORY ---
    INPC* ForestBiomeFactory::CreateNPC()
    {
        std::cout << "[ForestBiomeFactory] Spawned a Forest NPC.\n";
        return new ForestNPC();
    }

    ICombatBehavior* ForestBiomeFactory::CreateCombat()
    {
        std::cout << "[ForestBiomeFactory] Combat.\n";
        return new ForestCombat();
    }

    IAIBehavior* ForestBiomeFactory::CreateBehavior()
    {
        std::cout << "[ForestBiomeFactory] Behavior (AI).\n";
        return new ForestBehavior();
    }

    ILootTable* ForestBiomeFactory::CreateLootTable()
    {
        std::cout << "[ForestBiomeFactory] Loot Table.\n";
        return new ForestLoot();
    }

    // --- DESERT BIOME FACTORY ---
    INPC* DesertBiomeFactory::CreateNPC()
    {
        std::cout << "[DesertBiomeFactory] Spawned a Desert NPC.\n";
        return new DesertNPC();
    }

    ICombatBehavior* DesertBiomeFactory::CreateCombat()
    {
        std::cout << "[DesertBiomeFactory] Desert Combat.\n";
        return new DesertCombat();
    }

    IAIBehavior* DesertBiomeFactory::CreateBehavior()
    {
        std::cout << "[DesertBiomeFactory] Behavior (AI).\n";
        return new DesertBehavior();
    }

    ILootTable* DesertBiomeFactory::CreateLootTable()
    {
        std::cout << "[DesertBiomeFactory] Loot Table.\n";
        return new DesertLoot();
    }

    // --- SWAMP EXTENSION FACTORY ---
    INPC* SwampBiomeFactory::CreateNPC()
    {
        std::cout << "[SwampBiomeFactory] Spawned a Swamp NPC.\n";
        return new SwampNPC();
    }

    ICombatBehavior* SwampBiomeFactory::CreateCombat()
    {
        std::cout << "[SwampBiomeFactory] Combat.\n";
        return new SwampCombat();
    }

    IAIBehavior* SwampBiomeFactory::CreateBehavior()
    {
        std::cout << "[SwampBiomeFactory] Behavior (AI).\n";
        return new SwampBehavior();
    }

    ILootTable* SwampBiomeFactory::CreateLootTable()
    {
        std::cout << "[SwampBiomeFactory] Loot Table.\n";
        return new SwampLoot();
    }

    // ------------------------------------------------------------------------
    // 4. CLIENT IMPLEMENTATION (THE SPAWNER)
    // ------------------------------------------------------------------------

    void NPCSpawner::SpawnNPC()
    {
        std::cout << "\n--- Spawning NPC ---\n";
        
         INPC* NewNPC = CurrentFactory->CreateNPC();
         if (NewNPC)
         {
             std::cout << "\nA " << NewNPC->GetName() << " appears!\n\n";
             
             CurrentFactory->CreateBehavior()->Behavior();
             CurrentFactory->CreateCombat()->Attack();

             // --- PLAYER RESPONSE ---
             std::cout << "\nYou strike back!\n";
             std::cout << "The enemy is defeated.\n\n";

             CurrentFactory->CreateLootTable()->DropLoot();

             delete NewNPC;
         }
        std::cout << "---------------------\n";
    }

    // ------------------------------------------------------------------------
    // 5. DEMO IMPLEMENTATION (TRAVEL & ENCOUNTER)
    // ------------------------------------------------------------------------
    void RunDemo()
    {
        // Clear input buffer
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        // --- INTRODUCTION ---
        HFL::ClearScreen();
        HFL::PrintHeader("Abstract Factory Pattern — Biome Encounters");

        std::cout << "In this demo, you will TRAVEL between biomes.\n";
        std::cout << "Each biome spawns an NPC with its own:\n";
        std::cout << " - AI Behavior\n";
        std::cout << " - Combat Style\n";
        std::cout << " - Loot Table\n\n";

        std::cout << "The important part:\n";
        std::cout << "The game logic never knows WHICH biome implementation is used.\n";
        std::cout << "That decision is hidden behind an Abstract Factory.\n";

        HFL::WaitForInput();

        // --- FACTORIES ---
        ForestBiomeFactory ForestFactory;
        DesertBiomeFactory DesertFactory;
        SwampBiomeFactory  SwampFactory;

        NPCSpawner Spawner;

        bool InDemo = true;
        while (InDemo)
        {
            // --- TRAVEL MENU ---
            HFL::ClearScreen();
            HFL::PrintHeader("World Map");

            std::cout << "Where would you like to travel?\n\n";
            std::cout << "1. Travel to the Forest\n";
            std::cout << "2. Travel to the Desert\n";
            std::cout << "3. Travel to the Swampland\n";
            std::cout << "0. Return to Town (Exit Demo)\n";
            std::cout << "\nDestination: ";

            int Choice;
            std::cin >> Choice;

            if (std::cin.fail())
            {
                std::cin.clear();
                std::cin.ignore();
                continue;
            }

            if (Choice == 0)
                break;

            // --- BIOME SELECTION ---
            if (Choice == 1)
            {
                Spawner.SetBiomeFactory(&ForestFactory);
                std::cout << "\nYou arrive in the Forest...\n";
                std::cout << "The trees are dense and the air feels alive.\n";
            }
            else if (Choice == 2)
            {
                Spawner.SetBiomeFactory(&DesertFactory);
                std::cout << "\nYou arrive in the Desert...\n";
                std::cout << "The sun burns and sand stretches endlessly.\n";
            }
            else if (Choice == 3)
            {
                Spawner.SetBiomeFactory(&SwampFactory);
                std::cout << "\nYou enter the Swampland...\n";
                std::cout << "The ground is wet and something moves beneath the surface.\n";
            }
            else
            {
                continue;
            }

            HFL::WaitForInput();

            // --- ENCOUNTER ---
            HFL::ClearScreen();
            HFL::PrintHeader("Encounter!");

            Spawner.SpawnNPC();

            // --- DESIGN NOTE  ---
            std::cout << "\n[Design Note]\n";
            std::cout << "This entire encounter was defined by the biome.\n";
            std::cout << "Only the Abstract Factory is changed.\n";

            HFL::WaitForInput();
        }

        // --- STEP 4: CONCLUSION ---
        HFL::ClearScreen();
        HFL::PrintHeader("Conclusion");

        std::cout << "Summary of Abstract Factory:\n\n";
        std::cout << "1. The Spawner (Client) was decoupled.\n";
        std::cout << "   It didn't know how to construct a Swamp NPC.\n";
        std::cout << "   It just asked 'Factory->CreateNPC()'.\n\n";

        std::cout << "2. The Catalog (Abstract Factory) managed Extensions.\n";
        std::cout << "   The 'Swamp Biome' sub-factory uses DIP.\n";

        std::cout << "3. Component Logic (Polymorphism):\n";
        std::cout << "   Each NPC uses it's unique behaviors.\n\n";

        std::cout << "4. Extendability:\n";
        std::cout << "   To add a 'Volcano' Biome later, just add a factory.\n";
        std::cout << "   The Spawner code remains unchanged.\n\n";

        std::cout << "This is how Abstract Factory works.\n";

        std::cout << std::setw(40) << "End of Demo\n";
        HFL::WaitForInput();
    }

}