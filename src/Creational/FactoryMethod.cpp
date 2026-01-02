#include "Creational/FactoryMethod.h"

namespace FTM
{
    // ------------------------------------------------------------------------
    // MELEE FACTORY IMPLEMENTATION
    // ------------------------------------------------------------------------
    Enemy* MeleeFactory::CreateEnemy(int TypeID)
    {
        // Type 1 = Skeleton, Type 2 = Golem
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

    // ------------------------------------------------------------------------
    // RANGED FACTORY IMPLEMENTATION
    // ------------------------------------------------------------------------
    Enemy* RangedFactory::CreateEnemy(int TypeID)
    {
        // Type 1 = Skeleton Mage, Type 2 = Skeleton Archer
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

    // ------------------------------------------------------------------------
    // GAME LEVEL IMPLEMENTATION
    // ------------------------------------------------------------------------

    void GameLevel::SpawnEnemies()
    {
        if (!Factory) return;

        std::cout << "\n--- Spawning Wave ---\n";

        // Spawn two enemies from whatever factory is currently set
        for (int i = 1; i <= 2; ++i)
        {
            Enemy* NewEnemy = Factory->CreateEnemy(i);
            if (NewEnemy)
            {
                NewEnemy->Attack();
                delete NewEnemy; // Cleanup for demo purposes
            }
        }
        std::cout << "---------------------\n";
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
        HFL::PrintHeader("Factory Method Pattern");
    
        std::cout << "Definition:\n";
        std::cout << "Define an interface for creating an object,\n";
        std::cout << "but let subclasses decide which class to instantiate.\n\n";
    
        std::cout << "In This Demo:\n";
        std::cout << "An Enemy Spawner system is built.\n";
        std::cout << "Factories are used to handle 'new' command.\n";
        std::cout << "The Spawner just calls 'Factory->CreateEnemy()'.\n";
        std::cout << "The Spawner doesn't know about Skeletons or Golems.\n";
    
        HFL::WaitForInput();
    
        // --- STEP 2: THE SETUP ---
        HFL::ClearScreen();
        HFL::PrintHeader("Step 1: The Factories");
    
        std::cout << "We created two Factories to organize enemy types:\n\n";
    
        std::cout << "1. MeleeFactory:\n";
        std::cout << "   - ID 1: Skeleton\n";
        std::cout << "   - ID 2: Golem\n";
    
        std::cout << "2. RangedFactory:\n";
        std::cout << "   - ID 1: Skeleton Mage\n";
        std::cout << "   - ID 2: Skeleton Archer\n";
    
        std::cout << "\nThe Enemy Spawner doesn't know these details.\n";
        std::cout << "It just delegates to 'Factory->CreateEnemy(ID)'.\n";
    
        HFL::WaitForInput();
    
        // --- STEP 3: INTERACTIVE SYSTEM ---
    
        MeleeFactory MeleeSpawner;
        RangedFactory RangedSpawner;
        IEnemyFactory* CurrentFactory = nullptr;
    
        bool InDemo = true;
        while (InDemo)
        {
            HFL::ClearScreen();
            HFL::PrintHeader("Enemy Spawner");
    
            // --- SELECT FACTORY ---
            std::cout << "Select a Factory to use:\n";
            std::cout << "1. MeleeFactory (Skeletons, Golems)\n";
            std::cout << "2. RangedFactory (Mages, Archers)\n";
            std::cout << "0. Exit Demo\n";
            std::cout << "\nFactory: ";
    
            int FactoryChoice;
            std::cin >> FactoryChoice;
    
            if (std::cin.fail()) { std::cin.clear(); std::cin.ignore(); continue; }
    
            if (FactoryChoice == 0) break;
    
            if (FactoryChoice == 1) CurrentFactory = &MeleeSpawner;
            else if (FactoryChoice == 2) CurrentFactory = &RangedSpawner;
    
            if (!CurrentFactory) continue;
    
            // --- SELECT ENEMY TYPE ---
            HFL::ClearScreen();
            std::cout << "Factory Selected: " << (FactoryChoice == 1 ? "Melee" : "Ranged") << "\n\n";
            std::cout << "Select an Enemy Type to spawn:\n";
    
            if (FactoryChoice == 1)
            {
                std::cout << "1. Skeleton\n";
                std::cout << "2. Golem\n";
            }
            else if (FactoryChoice == 2)
            {
                std::cout << "1. Skeleton Mage\n";
                std::cout << "2. Skeleton Archer\n";
            }
            std::cout << "0. Back to Factory Selection\n";
            std::cout << "\nEnemy Type: ";
    
            int EnemyType;
            std::cin >> EnemyType;
    
            if (std::cin.fail()) { std::cin.clear(); std::cin.ignore(); continue; }
    
            if (EnemyType == 0) continue; // Go back to Factory loop
    
            // --- SPAWN ---
            Enemy* NewEnemy = CurrentFactory->CreateEnemy(EnemyType);
            if (NewEnemy)
            {
                NewEnemy->Attack();
                delete NewEnemy;
                std::cout << "[System] Enemy dies from boredom.\n";
            }
            else
            {
                std::cout << "[System] Invalid Enemy Type ID.\n";
            }
    
            HFL::WaitForInput();
        }
    
        // --- STEP 4: CONCLUSION ---
        HFL::ClearScreen();
        HFL::PrintHeader("Conclusion");
    
        std::cout << "Summary of Factory Method:\n\n";
    
        std::cout << "1. Decoupling:\n";
        std::cout << "   The Enemy Spawner is decoupled from 'new Skeleton'.\n";
        std::cout << "   Creation logic is inside Melee/Ranged Factories.\n\n";
    
        std::cout << "2. Clean Code:\n";
        std::cout << "   Spawner code is short and generic.\n";
        std::cout << "   To add a 'Boss', we just edit the Factory, not the Spawner.\n\n";
    
        std::cout << "3. Other Uses:\n";
        std::cout << "   Imagine you switch to 'WeaponFactory'.\n";
        std::cout << "   The Spawner code wouldn't change. It would just call\n";
        std::cout << "   WeaponFactory->CreateWeapon()'.\n\n";
    
        std::cout << std::setw(40) << "End of Demo\n";
        HFL::WaitForInput();
    }
}