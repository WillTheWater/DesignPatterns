#include "SOLID/OpenClosedPrinciple.h"

namespace OCP
{
    // =========================================================================
    // PLAYER IMPLEMENTATION
    // =========================================================================
    Player::Player()
        : Health(80), MaxHealth(100), Gold(0)
    {
    }

    void Player::InteractWith(IInteractable* Target)
    {
        if (!Target) return;

        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "[Action] Initiating contact with: ";
        HFL::SetColor(HFL::EColor::White);
        std::cout << Target->GetName() << "...\n";

        // The Player is "Closed". It delegates the logic to the Target.
        Target->Interact(*this);
    }

    // =========================================================================
    // DOOR IMPLEMENTATION
    // =========================================================================
    Door::Door() : IsOpen(false) {}

    void Door::Interact(Player& PlayerRef)
    {
        (void)PlayerRef; // Door does not modify player state.

        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Result: ";
        IsOpen = !IsOpen;

        HFL::SetColor(HFL::EColor::Green);
        std::cout << (IsOpen ? "The door opens.\n" : "The door closes.\n");
    }

    // =========================================================================
    // CHEST IMPLEMENTATION
    // =========================================================================
    Chest::Chest(int GoldAmount) 
        : IsLooted(false)
        , GoldAmount(GoldAmount) 
    {
    }

    void Chest::Interact(Player& PlayerRef)
    {
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Result: ";
        if (!IsLooted)
        {
            HFL::SetColor(HFL::EColor::Green);
            std::cout << "CONTENTS ACQUIRED. [+" << GoldAmount << " Gold]\n";
            PlayerRef.AddGold(GoldAmount);
            IsLooted = true;
        }
        else
        {
            HFL::SetColor(HFL::EColor::Gray);
            std::cout << "EMPTY. No items remaining.\n";
        }
    }

    // =========================================================================
    // WATER SPRING IMPLEMENTATION
    // =========================================================================
    void WaterSpring::Interact(Player& PlayerRef)
    {
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Result: ";
        if (PlayerRef.GetHealth() < PlayerRef.GetMaxHealth())
        {
            PlayerRef.SetHealth(std::min(PlayerRef.GetHealth() + 10, PlayerRef.GetMaxHealth()));
            HFL::SetColor(HFL::EColor::Green);
            std::cout << "HEALTH RESTORED. [+10 HP]\n";
        }
        else
        {
            HFL::SetColor(HFL::EColor::Gray);
            std::cout << "NO EFFECT. Player is at full health.\n";
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
        HFL::PrintHeader("OPEN-CLOSED PRINCIPLE");

        HFL::PrintSection("THE DEFINITION");
        HFL::SetColor(HFL::EColor::White);
        std::cout << "Entities should be open for extension, but closed for modification.\n\n";

        HFL::PrintSection("THE GOAL");
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "The Open-Closed Principle (OCP) prevents code fragility by allowing\n"
            << "new behaviors to be added through abstraction. Instead of editing\n"
            << "existing logic, it extends the system by implementing interfaces.\n\n";

        HFL::PrintSection("THE EXAMPLE");
        HFL::SetColor(HFL::EColor::White);
        std::cout << "This demonstration features an Interaction System with two primary\n"
            << "architectural roles:\n\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] THE ACTOR (PLAYER): ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Triggers actions via an interface. (Closed)\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] THE INTERFACE: ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Defines a contract for all interactable objects. (Open)\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] EXTENSIONS: ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Unique objects (Door, Chest, Spring) that define logic.\n\n";

        HFL::WaitForInput();

        // ======================== THE ARCHITECTURE ========================
        HFL::ClearScreen();
        HFL::PrintHeader("THE ARCHITECTURE");

        HFL::PrintSection("EXAMPLE");
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "To adhere to OCP, the Player does not contain logic for specific objects.\n"
            << "It only knows how to speak to the IInteractable interface.\n\n";

        HFL::PrintSection("IMPLEMENTATION");

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] Player\n";
        HFL::SetColor(HFL::EColor::White);
        std::cout << "    ROLE:           ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "The Interactor.\n";
        HFL::SetColor(HFL::EColor::White);
        std::cout << "    CONSTRAINT:     ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Closed. No code changes required to add new objects.\n\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] Concrete Objects\n";
        HFL::SetColor(HFL::EColor::White);
        std::cout << "    ROLE:           ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "The Extensions.\n";
        HFL::SetColor(HFL::EColor::White);
        std::cout << "    SCOPE:          ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Defining distinct behaviors (Healing, Looting, Opening).\n\n";

        HFL::SetColor(HFL::EColor::White);
        HFL::WaitForInput();

        // ======================== INITIALIZATION ========================
        Player MyHero;
        Door WoodenDoor;
        Chest TreasureChest(HFL::GetRandom(10, 150));
        WaterSpring HealingSpring;

        std::vector<IInteractable*> SceneObjects = 
        { &WoodenDoor, &TreasureChest, &HealingSpring };

        // ======================== INTERACTION LOOP ========================
        while (true)
        {
            HFL::ClearScreen();
            HFL::PrintHeader("INTERACTION PLAYGROUND");

            // ======================== Status Panel ========================
            HFL::PrintSection("PLAYER STATS");
            HFL::SetColor(HFL::EColor::White);
            std::cout << " HEALTH: "; HFL::SetColor(HFL::EColor::Green); std::cout << MyHero.GetHealth() << "/" << MyHero.GetMaxHealth();
            HFL::SetColor(HFL::EColor::White);
            std::cout << " | GOLD: "; HFL::SetColor(HFL::EColor::Green); std::cout << MyHero.GetGold() << "\n\n";

            // ======================== Menu ========================
            HFL::PrintSection("INTERACTABLE OBJECTS");
            for (size_t i = 0; i < SceneObjects.size(); ++i)
            {
                HFL::SetColor(HFL::EColor::Green);
                std::cout << " [" << i + 1 << "] ";
                HFL::SetColor(HFL::EColor::White);
                std::cout << SceneObjects[i]->GetName() << "\n";
            }
            HFL::SetColor(HFL::EColor::Green);
            std::cout << "\n [0] "; HFL::SetColor(HFL::EColor::White); std::cout << "CONTINUE\n\n";

            HFL::SetColor(HFL::EColor::Gray);
            int Choice = HFL::GetValidMenuInput(SceneObjects.size());

            if (Choice == 0) break;

            // ======================== Execution ========================
            HFL::ClearScreen();
            HFL::PrintHeader("EXECUTING INTERACTION");

            HFL::PrintSection("INTERACTING");
            HFL::SetColor(HFL::EColor::Gray);
            std::cout << "The Player class is currently executing a generic Interact() call.\n"
                << "It remains unaware of the target's underlying class type.\n\n";

            HFL::SetColor(HFL::EColor::Gray);
            std::cout << "Analyzing object";
            for (int i = 0; i < 3; ++i) { HFL::Wait(0.4f); std::cout << "."; }
            std::cout << "\n\n";

            MyHero.InteractWith(SceneObjects[Choice - 1]);

            HFL::WaitForInput();
        }

        // ======================== CONCLUSION ========================
        HFL::ClearScreen();
        HFL::PrintHeader("CONCLUSION");

        HFL::PrintSection("ARCHITECTURE");
        HFL::SetColor(HFL::EColor::White);
        std::cout << "The interaction confirms the following OCP advantages:\n\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] ZERO-MODIFICATION: ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "The Player source code remained untouched.\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] UNIFORMITY: ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Different object types are stored and accessed identically.\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] FUTURE PROOF: ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Adding a 'Trap' or 'NPC' requires only a new subclass.\n\n";

        HFL::PrintSection("SUMMARY");
        HFL::SetColor(HFL::EColor::White);
        std::cout << "The Open-Closed Principle ensures that systems are:\n\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] STABLE: ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Core logic is protected from regression bugs.\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] EXTENSIBLE: ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "New features are added by writing NEW code, not changing old code.\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] DECOUPLED: ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "The interactor and the interactable exist in separate domains.\n\n";

        HFL::WaitForInput();
    }
}