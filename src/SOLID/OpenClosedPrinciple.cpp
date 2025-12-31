#include "SOLID/OpenClosedPrinciple.h"

namespace OCP
{
    // ------------------------------------------------------------------------
    // PLAYER IMPLEMENTATION
    // ------------------------------------------------------------------------
    Player::Player()
        : Health(80), MaxHealth(100), Gold(0)
    {
    }

    void Player::InteractWith(IInteractable* Target)
    {
        // The Player is "Closed".
        // It performs the generic act of interacting without knowing the details
        // of what happens when it interacts.
        std::cout << "You approach the " << Target->GetName() << "...\n";
        Target->Interact(*this);
    }

    // ------------------------------------------------------------------------
    // DOOR IMPLEMENTATION
    // ------------------------------------------------------------------------
    Door::Door() : IsOpen(false) {}

    void Door::Interact(Player& PlayerRef)
    {
        // Door doesn't use the player reference so Cast to void to ignore.
        // This prevents compilation Warnings
        (void)PlayerRef;

        IsOpen = !IsOpen; // Toggle the door state
        if (IsOpen)
        {
            std::cout << "and the door creaks open.\n";
        }
        else
        {
            std::cout << "and the door shuts.\n";
        }
    }

    // ------------------------------------------------------------------------
    // CHEST IMPLEMENTATION
    // ------------------------------------------------------------------------
    Chest::Chest(int GoldAmount) 
        : IsLooted(false)
        , GoldAmount(GoldAmount) 
    {
    }

    void Chest::Interact(Player& PlayerRef)
    {
        if (!IsLooted)
        {
            std::cout << "You open the chest and find " << GoldAmount << " gold!\n";
            PlayerRef.AddGold(GoldAmount);
            IsLooted = true;
        }
        else
        {
            std::cout << "The chest is empty.\n";
        }
    }

    // ------------------------------------------------------------------------
    // WATER SPRING IMPLEMENTATION
    // ------------------------------------------------------------------------
    void WaterSpring::Interact(Player& PlayerRef)
    {
        if (PlayerRef.GetHealth() < PlayerRef.GetMaxHealth())
        {
            int OldHealth = PlayerRef.GetHealth();
            PlayerRef.SetHealth(OldHealth + 10);
            std::cout << "You drink from the spring. \nRefreshing!\n";
            std::cout << "HP restored by 10.\n";
        }
        else
        {
            std::cout << "You can't drink any more.\n";
            std::cout << "You are already full.\n";
        }
    }

    // ------------------------------------------------------------------------
    // DEMO IMPLEMENTATION
    // ------------------------------------------------------------------------
    void RunDemo()
    {
        // Clear initial buffer
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        // --- STEP 1: INTRODUCTION ---
        HFL::ClearScreen();
        HFL::PrintHeader("Open-Closed Principle (OCP)");

        std::cout << "Definition:\n";
        std::cout << "A class should be open for extension,\n";
        std::cout << "but closed for modification.\n\n";

        std::cout << "In This Demo:\n";
        std::cout << "We will create an Interaction System.\n";
        std::cout << "The 'Player' (Closed) can interact with many objects,\n";
        std::cout << "but never defines what those objects are.\n";

        HFL::WaitForInput();

        // --- STEP 2: THE SETUP ---
        HFL::ClearScreen();
        HFL::PrintHeader("Step 1: The Architecture");

        std::cout << "We have defined an Interface 'IInteractable'.\n";
        std::cout << "Any object that implements this Interface can be interacted with.\n\n";

        std::cout << "1. [Player]:\n";
        std::cout << "   - Responsibility: Initiates interaction.\n";
        std::cout << "   - Constraint: Does NOT know about Doors, Chests, or Water.\n";
        std::cout << "   - It just calls Interact() on the object.\n\n";

        std::cout << "2. [The Objects]:\n";
        std::cout << "   - Door, Chest, and WaterSpring implement the Interface.\n";
        std::cout << "   - They define EXACTLY what happens when interacted with.\n";
        std::cout << "   - The Player is blind to their internal logic.\n";

        HFL::WaitForInput();

        // --- STEP 3: INTERACTIVE PLAYGROUND ---

        // Initialize Objects
        // Note: We use pointers to the Interface. This allows the Player
        // to hold a list of completely different objects uniformly.
        Player MyHero;
        Door WoodenDoor;
        Chest TreasureChest(50); // Chest contains 50 gold
        WaterSpring HealingSpring;

        // Store objects in a vector for the menu
        std::vector<IInteractable*> SceneObjects;
        SceneObjects.push_back(&WoodenDoor);
        SceneObjects.push_back(&TreasureChest);
        SceneObjects.push_back(&HealingSpring);

        bool InDemo = true;
        while (InDemo)
        {
            HFL::ClearScreen();
            HFL::PrintHeader("Interaction Playground");

            // 1. Display Player Stats
            std::cout << "Player Status:\n";
            std::cout << "  Health: " << MyHero.GetHealth() << "/" << MyHero.GetMaxHealth() << "\n";
            std::cout << "  Gold:   " << MyHero.GetGold() << "\n\n";

            // 2. Display Menu
            std::cout << "What would you like to interact with?\n\n";
            for (size_t i = 0; i < SceneObjects.size(); ++i)
            {
                std::cout << static_cast<int>(i) + 1 << ". "
                    << SceneObjects[i]->GetName() << "\n";
            }
            std::cout << "0. Continue\n";
            std::cout << "\nChoice: ";

            int Choice;
            std::cin >> Choice;

            // Handle bad input
            if (std::cin.fail())
            {
                std::cin.clear();
                std::cin.ignore();
                continue;
            }

            if (Choice == 0) break;

            // Validate selection
            if (Choice >= 1 && Choice <= static_cast<int>(SceneObjects.size()))
            {
                IInteractable* Target = SceneObjects[Choice - 1];

                std::cout << "\n----------------------------------------\n";
                // The Player acts blindly through the interface
                MyHero.InteractWith(Target);
                std::cout << "----------------------------------------\n";
            }
            else
            {
                std::cout << "\n[!] Invalid selection.\n";
            }

            HFL::WaitForInput();
        }

        // --- STEP 4: CONCLUSION ---
        HFL::ClearScreen();
        HFL::PrintHeader("Conclusion");

        std::cout << "1. The Player Class is 'Closed'.\n";
        std::cout << "   And three distinct objects that can beinteracted with (Door, Chest, Spring).\n";
        std::cout << "   However, the Player source code is never edited.\n";
        std::cout << "   It does not have methods like 'OpenDoor()' or 'Drink()'.\n";
        std::cout << "   By keeping Player closed, it guarantees it stays stable.\n\n";

        std::cout << "2. The interactablity is extended by using Interfaces.\n";
        std::cout << "   You implement 'IInteractable' for any new object.\n";
        std::cout << "   The Player doesn't care what the object IS, only that it\n";
        std::cout << "   implements the interface.\n\n";

        std::cout << "3. Future Proofing.\n";
        std::cout << "   All that is needed to add a new 'Interactable',\n";
        std::cout << "   is simply making a NEW class that implements the interface.\n";
        std::cout << "   This way there is no risk breaking the Player or existing items.\n\n";

        std::cout << "This is the Open-Closed Principle in action:\n";
        std::cout << "Closed for modification (Safety) vs Open for Extension (Growth).\n\n";

        std::cout << std::setw(40) << "End of Demo\n";

        HFL::WaitForInput();
    }
}