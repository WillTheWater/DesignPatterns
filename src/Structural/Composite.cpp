#include "Structural/Composite.h"

namespace COM
{
    // =========================================================================
    // LEAF IMPLEMENTATIONS (Single Items)
    // ROLE: Primitive objects. They do not contain children and return 
    // their own base values directly.
    // =========================================================================

    Coin::Coin(int GoldAmount)
        : Value(GoldAmount), Name("Coins")
    {
    }

    Item::Item(int GoldAmount, std::string Name)
        : Value(GoldAmount), Name(Name)
    {
    }

    // =========================================================================
    // COMPOSITE IMPLEMENTATION (The Chest)
    // ROLE: Orchestrates children. Implements recursion to treat a 
    // collection of items as a single unit.
    // =========================================================================

    Chest::Chest(std::string Name)
        : BaseValue(0), Name(Name)
    {
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "[System] Container '" << Name << "' initialized.\n";
    }

    void Chest::AddItem(std::unique_ptr<IInventoryItem> Item)
    {
        // Ownership Transfer: The Composite now manages the lifecycle of the Leaf.
        Items.push_back(std::move(Item));
    }

    int Chest::GetValue() const
    {
        int TotalValue = BaseValue;

        for (const auto& Item : Items)
        {
            int ItemValue = Item->GetValue();
            TotalValue += ItemValue;

            HFL::SetColor(HFL::EColor::White);
            std::cout << "     [Item]: ";
            HFL::SetColor(HFL::EColor::Gray);
            std::cout << std::left << std::setw(20) << Item->GetName();
            HFL::SetColor(HFL::EColor::Green);
            std::cout << " Value: " << ItemValue << " Gold\n";
        }
        return TotalValue;
    }

    // =========================================================================
    // CLIENT IMPLEMENTATION (The Player)
    // =========================================================================

    Player::Player()
    {
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "[System] Player joined the world.\n";
        MainInventory = std::make_unique<Chest>("Main Inventory");
    }

    void Player::AddToInventory(std::unique_ptr<IInventoryItem> Item)
    {
        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[Player] Looted: " << Item->GetName() << "\n";
        MainInventory->AddItem(std::move(Item));
    }

    void Player::LootChest(Chest* TargetChest)
    {
        HFL::PrintSection("LOOTING");

        if (!TargetChest) return;

        // Instead of unpacking the chest, this treats the chest as an IInventoryItem.
        // To do this, it wraps the raw pointer back into a unique_ptr.

        int LootValue = TargetChest->GetValue();

        for (auto& item : TargetChest->GetItems())
        {
            MainInventory->AddItem(std::move(item));
        }
        TargetChest->GetItems().clear();

        HFL::SetColor(HFL::EColor::Yellow);
        std::cout << "\n>> [Success] Transferred " << LootValue << " Gold worth of items to player.\n";
    }

    void Player::PrintTotalValue()
    {
        int TotalGold = MainInventory->GetValue();

        if (TotalGold <= 0)
        {
            HFL::SetColor(HFL::EColor::Green);
            std::cout << "\n\t\t\tEMPTY\n\n";
        }
        else
        {
            HFL::SetColor(HFL::EColor::Green);
            std::cout << "\n\t\t\t" << "TOTAL: " << TotalGold << " Gold\n\n";
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
        HFL::PrintHeader("COMPOSITE DESIGN PATTERN");

        HFL::PrintSection("THE DEFINITION");
        HFL::SetColor(HFL::EColor::White);
        std::cout << "Compose objects into tree structures to represent part-whole hierarchies.\n"
            << "Composite lets clients treat individual objects and compositions uniformly.\n\n";

        HFL::PrintSection("THE GOAL");
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "The Composite Pattern is about 'Recursive Aggregation'.\n"
            << "It allows building complex hierarchies (Chests within Chests)\n"
            << "while keeping the Client logic simple. The Client calls GetValue()\n"
            << "once, and the pattern handles the traversal of the entire tree.\n\n";

        HFL::PrintSection("THE EXAMPLE");
        HFL::SetColor(HFL::EColor::White);
        std::cout << "This demonstration features a Recursive Inventory System with tree nodes:\n\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] THE LEAVES:          ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Primitive items (Coins, Items) that return a direct value.\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] THE COMPOSITE:       ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Containers (Chests) that hold other items or even other chests.\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] THE RECURSION:       ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "The mechanism that drills down through the 'tree' to sum values.\n\n";

        HFL::PrintSection("THE BENEFIT");
        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] UNIFORMITY:      ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "The Client treats a single Coin and a massive Chest exactly the same.\n";
        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] TREE STRUCTURE:  ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Supports infinite nesting (e.g., a Bag inside a Chest inside a Vault).\n";
        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] SCALABILITY:     ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "New item types can be added without changing the tree-traversal logic.\n";

        HFL::WaitForInput();

        // ======================== THE ARCHITECTURE ========================
        HFL::ClearScreen();
        HFL::PrintHeader("THE ARCHITECTURE");

        HFL::PrintSection("THE 'PART-WHOLE' RELATIONSHIP");
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "In most systems, there is a seperation between an object and a container.\n"
            << "Composite 'flattens' this distinction by making the container (the Whole)\n"
            << "implement the same interface as the object (the Part).\n\n";

        HFL::PrintSection("IMPLEMENTATION");

        // ======================== THE LEAVES ========================
        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] Leaf Nodes (Coins, Items, etc...)\n";
        HFL::SetColor(HFL::EColor::White);
        std::cout << "    ROLE:           ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "End-points. They do not have children and return raw data.\n";
        HFL::SetColor(HFL::EColor::White);
        std::cout << "    INTERFACE:      ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Implements GetValue() to return its specific gold worth.\n\n";

        // ======================== THE COMPOSITE ========================
        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] Composite Node (The Chest)\n";
        HFL::SetColor(HFL::EColor::White);
        std::cout << "    ROLE:           ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "The Container. It stores a list of pointers to 'IInventoryItem'.\n";
        HFL::SetColor(HFL::EColor::White);
        std::cout << "    RECURSION:      ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "GetValue() iterates through children and calls their GetValue().\n";
        HFL::SetColor(HFL::EColor::White);
        std::cout << "    FLEXIBILITY:    ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Since children are stored as the interface, it can hold other chests.\n\n";

        HFL::SetColor(HFL::EColor::White);
        HFL::WaitForInput();

        // ======================== INITIALIZATION ========================
        Player MyPlayer;
        std::unique_ptr<Chest> ActiveChest = nullptr;

        // ======================== GAME LOOP ========================
        while (true)
        {
            HFL::ClearScreen();
            HFL::PrintHeader("COMPOSITE INVENTORY");

            // ======================== DATA DISPLAY ========================
            HFL::PrintSection("PLAYER INVENTORY");
            MyPlayer.PrintTotalValue();

            HFL::PrintSection("ACTIVE CHEST");
            if (ActiveChest)
            {
                HFL::SetColor(HFL::EColor::Cyan);
                std::cout << " [Selected: " << ActiveChest->GetName() << "]\n";
                HFL::SetColor(HFL::EColor::Gray);

                int CurrentChestValue = ActiveChest->GetValue();

                std::cout << " Status: ";
                if (CurrentChestValue == 0)
                {
                    HFL::SetColor(HFL::EColor::Red);
                    std::cout << "Empty\n\n";
                }
                else
                {
                    HFL::SetColor(HFL::EColor::Green);
                    std::cout << CurrentChestValue << " Gold Total\n\n";
                }
            }
            else
            {
                HFL::SetColor(HFL::EColor::Gray);
                std::cout << " [No container selected - Items go to main inventory]\n\n";
            }

            // ======================== COMMANDS ========================
            HFL::PrintSection("COMMANDS");
            HFL::SetColor(HFL::EColor::Green);
            std::cout << " [1] "; HFL::SetColor(HFL::EColor::White); std::cout << "CREATE CONTAINER\n";
            HFL::SetColor(HFL::EColor::Green);
            std::cout << " [2] "; HFL::SetColor(HFL::EColor::White); std::cout << "ADD ITEM\n";
            HFL::SetColor(HFL::EColor::Green);
            std::cout << " [3] "; HFL::SetColor(HFL::EColor::White); std::cout << "ADD COINS\n";
            HFL::SetColor(HFL::EColor::Green);
            std::cout << " [4] "; HFL::SetColor(HFL::EColor::White); std::cout << "PICKUP CONTAINER\n\n";
            HFL::SetColor(HFL::EColor::Green);
            std::cout << " [0] "; HFL::SetColor(HFL::EColor::White); std::cout << "CONTINUE\n\n";

            int Choice = HFL::GetValidMenuInput(4);
            if (Choice == 0) break;

            switch (Choice)
            {
            case 1:
            {
                HFL::PrintSection("CONTAINER CREATION");
                std::string CName;
                std::cout << "Enter Container Name: ";
                std::cin >> CName;

                ActiveChest = std::make_unique<Chest>(CName);
                std::cout << ">> " << CName << " created.\n";
                break;
            }
            case 2: // Add a Leaf (Item)
            {
                auto NewItem = std::make_unique<Item>(HFL::GetRandom(100,2000), ITEM_NAMES[HFL::GetRandom(0,19)]);
                if (ActiveChest) 
                {
                    std::cout << ">> Adding Item to " << ActiveChest->GetName() << "...\n";
                    ActiveChest->AddItem(std::move(NewItem));
                }
                else 
                {
                    std::cout << ">> Adding Item directly to Player...\n";
                    MyPlayer.AddToInventory(std::move(NewItem));
                }
                break;
            }
            case 3: // Add a Leaf (Coins)
            {
                auto NewCoins = std::make_unique<Coin>(HFL::GetRandom(5, 40));
                if (ActiveChest) 
                {
                    std::cout << ">> Dropping coins into " << ActiveChest->GetName() << "...\n";
                    ActiveChest->AddItem(std::move(NewCoins));
                }
                else 
                {
                    std::cout << ">> Adding coins to Player...\n";
                    MyPlayer.AddToInventory(std::move(NewCoins));
                }
                break;
            }
            case 4: // Move Composite to Player
            {
                if (ActiveChest) 
                {
                    HFL::SetColor(HFL::EColor::Green);
                    std::cout << ">> Picking up the " << ActiveChest->GetName() << "!\n";
                    HFL::SetColor(HFL::EColor::Gray);
                    std::cout << "NOTICE: The Player treats container as a single item.\n";

                    // Move the whole tree into the player's inventory
                    MyPlayer.LootChest(ActiveChest.get());
                    ActiveChest.reset();
                }
                else 
                {
                    HFL::SetColor(HFL::EColor::Red);
                    std::cout << "!! Error: No container !!\n";
                }
                break;
            }
            }
            HFL::WaitForInput();
        }

        // ======================== CONCLUSION ========================
        HFL::ClearScreen();
        HFL::PrintHeader("CONCLUSION");

        HFL::PrintSection("ARCHITECTURE");
        HFL::SetColor(HFL::EColor::White);
        std::cout << "The implementation of the Composite Pattern confirms the following:\n\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] UNIFORMITY OF TYPES: ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "The Client treats a single 'Coin' and a massive 'Chest' identically.\n"
            << "    Polymorphism eliminates the need for messy 'type-checking' logic.\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] RECURSIVE AGGREGATION: ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "The Chest delegates GetValue() to its children, who may delegate again.\n"
            << "    Complex tree-traversal is encapsulated entirely within the objects.\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] NESTED COMPOSITION: ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Structures can be nested infinitely (Chest > Bag > Pouch > Coin).\n"
            << "    The top-level call remains a single, simple request for data.\n\n";

        HFL::PrintSection("SUMMARY");
        HFL::SetColor(HFL::EColor::White);
        std::cout << "The Composite Pattern ensures that software is:\n\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] HIERARCHICAL:";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << " Models complex 'Part-Whole' relationships naturally.\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] TRANSPARENT: ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "The Client interacts with the Interface, not the implementation.\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] SCALABLE:    ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "New Leaf types can be added without modifying the Composite logic.\n\n";

        HFL::SetColor(HFL::EColor::White);
        HFL::WaitForInput();
    }
}