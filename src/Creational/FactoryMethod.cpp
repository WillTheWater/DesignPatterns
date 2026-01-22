#include "Creational/FactoryMethod.h"

namespace FTM
{
    // =========================================================================
    // MELEE FACTORY
    // ROLE: Handles instantiation of physical, close-quarters combatants.
    // =========================================================================
    Enemy* MeleeFactory::CreateEnemy(int TypeID)
    {
        switch (TypeID)
        {
        case 1:
            std::cout << "[MeleeFactory] Creating a Skeleton.\n";
            return new Skeleton();
        case 2:
            std::cout << "[MeleeFactory] Creating a Golem.\n";
            return new Golem();
        default:
            return nullptr;
        }
    }

    // =========================================================================
    // RANGED FACTORY
    // ROLE: Handles instantiation of magical or projectile-based combatants.
    // =========================================================================
    Enemy* RangedFactory::CreateEnemy(int TypeID)
    {
        switch (TypeID)
        {
        case 1:
            std::cout << "[RangedFactory] Creating a Skeleton Mage.\n";
            return new SkeletonMage();
        case 2:
            std::cout << "[RangedFactory] Creating a Skeleton Archer.\n";
            return new SkeletonArcher();
        default:
            return nullptr;
        }
    }

    // =========================================================================
    // HIGH LEVEL MODULE: GAME LEVEL
    // ROLE: The Client that uses the factory abstraction.
    // =========================================================================
    void GameLevel::SpawnEnemies()
    {
        if (!CurrentFactory) return;

        HFL::SetColor(HFL::EColor::Yellow);
        std::cout << "\n--- Spawning Wave via Abstract Factory Interface ---\n";
        HFL::SetColor(HFL::EColor::Gray);

        for (int i = 1; i <= 2; ++i)
        {
            Enemy* NewEnemy = CurrentFactory->CreateEnemy(i);
            if (NewEnemy)
            {
                NewEnemy->Attack();
                delete NewEnemy;
            }
        }
        std::cout << "---------------------------------------------------\n";
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
        HFL::PrintHeader("FACTORY METHOD PATTERN");

        HFL::PrintSection("THE DEFINITION");
        HFL::SetColor(HFL::EColor::White);
        std::cout << "Define an interface for creating an object, but let\n"
            << "subclasses decide which class to instantiate.\n\n";

        HFL::PrintSection("THE GOAL");
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "The Factory Method lets a class defer instantiation to subclasses.\n"
            << "This keeps the High-Level logic (Spawner) decoupled from the\n"
            << "concrete Low-Level types (Skeletons, Golems, etc).\n\n";

        HFL::PrintSection("THE EXAMPLE");
        HFL::SetColor(HFL::EColor::White);
        std::cout << "This demo features a Battle System with two specialized factories:\n\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] THE ABSTRACTION: ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "IEnemyFactory defines the Create contract.\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] THE CONCRETIONS: ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Melee and Ranged factory implementations.\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] THE CLIENT:      ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "GameLevel spawns enemies without knowing their class.\n\n";

        HFL::WaitForInput();

        // ======================== THE ARCHITECTURE ========================
        HFL::ClearScreen();
        HFL::PrintHeader("THE ARCHITECTURE");

        HFL::PrintSection("IMPLEMENTATION");

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] MeleeFactory (Concrete Creator)\n";
        HFL::SetColor(HFL::EColor::White);
        std::cout << "    PRODUCES:       ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Skeletons and Golems.\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] RangedFactory (Concrete Creator)\n";
        HFL::SetColor(HFL::EColor::White);
        std::cout << "    PRODUCES:       ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Mages and Archers.\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] GameLevel (The Client)\n";
        HFL::SetColor(HFL::EColor::White);
        std::cout << "    LOGIC:          ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Uses Factory->CreateEnemy() to stay Type Blind.\n\n";

        HFL::SetColor(HFL::EColor::White);
        HFL::WaitForInput();

        // ======================== INITIALIZATION ========================
        MeleeFactory MeleeSpawner;
        RangedFactory RangedSpawner;
        IEnemyFactory* CurrentFactory = nullptr;
        int Skeletons = 0, Golems = 0, Mages = 0, Archers = 0;
        int TotalSpawned = 0;

        // ======================== SPAWNING LOOP ========================
        while (true)
        {
            HFL::ClearScreen();
            HFL::PrintHeader("ENEMY SPAWNER");

            // --- DISPLAY METRICS ---
            HFL::PrintSection("TOTAL STATISTICS");
            HFL::SetColor(HFL::EColor::White);
            std::cout << "Total Enemies: " << TotalSpawned << "\n\n";

            HFL::SetColor(HFL::EColor::Gray);
            std::cout << " [MELEE]  Skeletons: " << Skeletons << " | Golems:  " << Golems << "\n";
            std::cout << " [RANGED] Mages:     " << Mages << " | Archers: " << Archers << "\n\n";

            // --- MENU ---
            HFL::PrintSection("SELECT TYPE");
            HFL::SetColor(HFL::EColor::Green);
            std::cout << " [1] "; HFL::SetColor(HFL::EColor::White); std::cout << "MELEE\n";
            HFL::SetColor(HFL::EColor::Green);
            std::cout << " [2] "; HFL::SetColor(HFL::EColor::White); std::cout << "RANGED\n\n";
            HFL::SetColor(HFL::EColor::Green);
            std::cout << " [0] "; HFL::SetColor(HFL::EColor::White); std::cout << "CONTINUE\n\n";

            int FactoryChoice = HFL::GetValidMenuInput(2);
            if (FactoryChoice == 0) break;

            CurrentFactory = (FactoryChoice == 1) ? (IEnemyFactory*)&MeleeSpawner : (IEnemyFactory*)&RangedSpawner;

            int SpawnCount = HFL::GetRandom(1, 7);

            HFL::ClearScreen();
            HFL::PrintHeader("SPAWNING ENEMIES");
            HFL::SetColor(HFL::EColor::Yellow);
            std::cout << "Requesting " << SpawnCount << " enemies from "
                << (FactoryChoice == 1 ? "Melee" : "Ranged") << " Factory...\n\n";

            HFL::SetColor(HFL::EColor::Gray);

            for (int i = 0; i < SpawnCount; ++i)
            {
                int RandomSubtype = HFL::GetRandom(1, 2);

                HFL::Wait(0.4f);

                Enemy* NewEnemy = CurrentFactory->CreateEnemy(RandomSubtype);
                if (NewEnemy)
                {
                    TotalSpawned++;

                    if (FactoryChoice == 1)
                    {
                        if (RandomSubtype == 1) Skeletons++;
                        else Golems++;
                    }
                    else
                    {
                        if (RandomSubtype == 1) Mages++;
                        else Archers++;
                    }

                    delete NewEnemy;
                }
            }

            HFL::SetColor(HFL::EColor::Green);
            std::cout << "\n>> Spawning complete.\n";
            HFL::SetColor(HFL::EColor::White);
            HFL::WaitForInput();
        }

        // ======================== CONCLUSION ========================
        HFL::ClearScreen();
        HFL::PrintHeader("CONCLUSION");

        HFL::PrintSection("ARCHITECTURE");
        HFL::SetColor(HFL::EColor::White);
        std::cout << "Applying the Factory Method confirms the following:\n\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] DEFERRED INSTANTIATION: ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "The logic never uses the 'new' keyword on concrete\n"
            << "     enemies. It delegates the responsibility to specialized workers.\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] TYPE BLINDNESS:         ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "The High-level logic stays clean by only interacting with the\n"
            << "     'IEnemyFactory' interface and 'Enemy' abstract product.\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] EXTENSION WITHOUT IMPACT: ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Adding a 'BossFactory' or a new unit ID requires zero changes\n"
            << "      to the dispatching loop, upholding the Open/Closed Principle.\n\n";

        HFL::PrintSection("SUMMARY");
        HFL::SetColor(HFL::EColor::White);
        std::cout << "The Factory Method ensures that software is:\n\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] ENCAPSULATED: ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Complex construction logic (and randomization) is hidden from the client.\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] FLEXIBLE:    ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Factories can be swapped at runtime to change the 'flavor' of the game.\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] SCALABLE: ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Concrete dependencies are centralized in factories, not scattered in logic.\n\n";

        HFL::SetColor(HFL::EColor::White);
        HFL::WaitForInput();
    }
}