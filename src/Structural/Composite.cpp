#include "Structural/Composite.h"

namespace COM
{
    // =========================================================================
    // LEAF IMPLEMENTATIONS (Single Items)
    // =========================================================================
    // These are "Parts". They do not contain anything else.

    Coin::Coin(int GoldAmount)
        : Value(GoldAmount), Name("Coin " + std::to_string(GoldAmount))
    {
    }

    Weapon::Weapon(int GoldAmount, std::string Name)
        : Value(GoldAmount), Name(Name)
    {
    }

    Potion::Potion(int GoldAmount, std::string Name)
        : Value(GoldAmount), Name(Name)
    {
    }

    // =========================================================================
    // COMPOSITE IMPLEMENTATION (The Chest)
    // =========================================================================
    // The Chest is the "Composite". It contains a list of items.

    Chest::Chest(std::string Name)
        : BaseValue(0), Name(Name)
    {
        std::cout << "[System] Chest '" << Name << "' Created.\n";
    }

    void Chest::AddItem(std::unique_ptr<IInventoryItem> Item)
    {
        // "Has-A" Relationship.
        // The Chest "owns" the item.
        // When Chest is destroyed, items inside are destroyed.
        Items.push_back(std::move(Item));
    }

    // THE CORE LOGIC: RECURSION
    // This is the most important method in the Composite pattern.
    // It sums up values of itself PLUS children.
    int Chest::GetValue() const
    {
        std::cout << "\n----------------------------------------\n";
        std::cout << "   Calculating Value for Chest '" << Name << "'...\n";
        std::cout << "----------------------------------------\n";

        int TotalValue = 0;

        // 1. Add Self Value (The Weight of the Chest)
        TotalValue += BaseValue;
        std::cout << "   [Self] Chest Weight: " << BaseValue << "\n";

        // 2. Add Children Values (The Contents)
        // We iterate through every item inside the Chest.
        // Note: We iterate by reference (const auto&) for performance.
        for (const auto& Item : Items)
        {
            // IMPORTANT: POLYMORPHISM
            // We call 'item->GetValue()'.
            // Since 'item' IS-A 'IInventoryItem', this works.
            // If 'item' is a Coin, it returns 1.
            // If 'item' is a Weapon, it returns 500.
            // If 'item' is ANOTHER CHEST (Composite), this triggers recursion!
            // The other Chest starts calculating its own total.
            int ItemValue = Item->GetValue();

            std::cout << "   [Child] Found item: " << Item->GetName() << " (Value: " << ItemValue << ")\n";
            TotalValue += ItemValue;
        }

        std::cout << "----------------------------------------\n";
        std::cout << "   [RESULT] Total Value of Chest '" << Name << "': " << TotalValue << " Gold\n";
        std::cout << "----------------------------------------\n\n";

        return TotalValue;
    }

    // =========================================================================
    // CLIENT IMPLEMENTATION (The Player)
    // =========================================================================

    Player::Player()
    {
        std::cout << "[System] Player Created.\n";

        // Create the Main Inventory
        MainInventory = std::make_unique<Chest>("Player Inventory");
    }

    void Player::AddToInventory(std::unique_ptr<IInventoryItem> Item)
    {
        // The Player acts as a manager. It adds items to the Inventory.
        std::cout << ">> [Player] Adding " << Item->GetName() << " to Inventory...\n";
        MainInventory->AddItem(std::move(Item));
    }

    void Player::LootChest(Chest* TargetChest)
    {
        std::cout << ">> [Player] Attempting to loot chest...\n";

        if (!TargetChest)
        {
            std::cout << "   [Error] Target Chest is null.\n";
            return;
        }
        if (TargetChest->GetItems().empty())
        {
            std::cout << "   [Info] Chest is already empty.\n";
            return;
        }

        std::cout << "   [Info] Taking items from Chest...\n";

        // LOOTING LOGIC (Move Logic)
        // We "move" items from TargetChest to MainInventory.

        // 1. Calculate Total Loot Value
        // This calls GetValue() on TargetChest, which triggers Recursion.
        int LootValue = TargetChest->GetValue();

        // 2. Transfer items (Simulation)
        for (int i = static_cast<int>(TargetChest->GetItems().size()) - 1; i >= 0; --i)
        {
            // After this move, MainInventory owns the item.
            MainInventory->AddItem(std::move(TargetChest->GetItems()[i]));
        }

        // 3. Clear Target Chest
        TargetChest->GetItems().clear();

        std::cout << "   [Success] Looted " << LootValue << " Gold worth of items!\n";
    }

    void Player::PrintTotalValue()
    {
        // The Player just asks MainInventory for its total value.
        // It doesn't need to know if there are Sub-Chests.
        // The MainInventory handles the Recursion.
        if (MainInventory)
        {
            int TotalGold = MainInventory->GetValue();
            std::cout << "\n>> [Player] Current Inventory Value: " << TotalGold << " Gold\n";
        }
    }

    // =========================================================================
    // DEMO IMPLEMENTATION
    // =========================================================================
    void RunDemo()
    {
        // Clear buffer
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        // --- STEP 1: INTRODUCTION ---
        HFL::ClearScreen();
        HFL::PrintHeader("Composite Pattern (Chest Inventory)");

        std::cout << "Definition:\n";
        std::cout << "Compose objects into tree structures to represent part-whole hierarchies.\n";
        std::cout << "Allow treating single objects (Leafs) and groups (Composites) uniformly.\n\n";

        std::cout << "In This Demo:\n";
        std::cout << "Simulates an Inventory System.\n";
        std::cout << "1. Leaves: Coins, Weapons, Potions (Single Items).\n";
        std::cout << "2. Composites: Chests (Bags of Items).\n";
        std::cout << "   Chests can contain other Chests (Nested Trees).\n";
        std::cout << "3. Player (The Client): Holds a 'MainInventory' Chest.\n";

        HFL::WaitForInput();

        // --- STEP 2: THE ARCHITECTURE ---
        HFL::ClearScreen();
        HFL::PrintHeader("Step 1: The Part-Whole Hierarchy");

        std::cout << "Define an 'IInventoryItem' interface (The Contract).\n";
        std::cout << "It has 'GetValue()' which returns the Gold worth.\n\n";

        std::cout << "1. Leaves (The Parts):\n";
        std::cout << "   - Coin: Value is 1.\n";
        std::cout << "   - Weapon: Value is 500.\n";
        std::cout << "   - Potion: Value is 50.\n\n";

        std::cout << "2. Composites (The Wholes):\n";
        std::cout << "   - Chest: Contains items (Has-A relationship).\n";
        std::cout << "   - Chest 'GetValue()' sums up:\n";
        std::cout << "     a. Its own weight (Empty chest = 0).\n";
        std::cout << "     b. The value of EVERY item inside it.\n\n";

        std::cout << "THE RECURSION MAGIC:\n";
        std::cout << "   When MainInventory->GetValue(), it asks every Chest inside.\n";
        std::cout << "   If those chests contain other chests, they sum up too!\n";
        std::cout << "   This allows infinite nesting (Trees).\n\n";

        std::cout << "THE BENEFIT:\n";
        std::cout << "   No need to manually 'Add Up' values when looting.\n";
        std::cout << "   The Chest handles calculating totals automatically.\n";

        HFL::WaitForInput();

        // --- STEP 3: INTERACTIVE SYSTEM ---

        // Create Player
        Player MyPlayer;
        std::unique_ptr<Chest> currentSubChest; // Store the created sub-chest

        bool InDemo = true;
        while (InDemo)
        {
            HFL::ClearScreen();
            HFL::PrintHeader("Chest Inventory");

            // 1. Display Current Inventory Status
            std::cout << "Current Inventory Status:\n";
            MyPlayer.PrintTotalValue();

            std::cout << "\nSelect Action:\n";
            std::cout << "1. Add Coin to Inventory\n";
            std::cout << "2. Add Weapon to Inventory\n";
            std::cout << "3. Create Sub-Chest (Nested Chest)\n";
            std::cout << "4. Add Items to Sub-Chest\n";
            std::cout << "5. Loot Sub-Chest (Move to Main Inventory)\n";
            std::cout << "0. Exit Demo\n";
            std::cout << "\nChoice: ";

            int Choice;
            std::cin >> Choice;

            if (std::cin.fail()) { std::cin.clear(); std::cin.ignore(); continue; }

            if (Choice == 0) break;

            if (Choice == 1)
            {
                auto NewCoin = std::make_unique<Coin>(10);
                MyPlayer.AddToInventory(std::move(NewCoin));
                HFL::WaitForInput();
            }
            else if (Choice == 2)
            {
                auto Sword = std::make_unique<Weapon>(500, "Iron Sword");
                MyPlayer.AddToInventory(std::move(Sword));
                HFL::WaitForInput();
            }
            else if (Choice == 3)
            {
                // Create a Sub-Chest (A Composite)
                currentSubChest = std::make_unique<Chest>("Treasure Chest");
                std::cout << "\n>> [Player] Created Sub-Chest.\n";
                HFL::WaitForInput();
            }
            else if (Choice == 4)
            {
                if (currentSubChest)
                {
                    std::cout << "\n>> [Player] Adding items to Sub-Chest...\n";
                    currentSubChest->AddItem(std::make_unique<Coin>(50));
                    currentSubChest->AddItem(std::make_unique<Potion>(100, "Healing Potion"));
                    std::cout << ">> [Player] Added Coin (50) and Potion (100) to Sub-Chest.\n";
                }
                else
                {
                    std::cout << "\n>> [Error] No Sub-Chest created. Please create one first.\n";
                }
                HFL::WaitForInput();
            }
            else if (Choice == 5)
            {
                if (currentSubChest)
                {
                    std::cout << "\n>> [Player] Searching for Sub-Chest to loot...\n";

                    // LOOT!
                    MyPlayer.LootChest(currentSubChest.get());

                    // Clear the sub-chest pointer after looting
                    currentSubChest.reset();
                }
                else
                {
                    std::cout << "\n>> [Error] No Sub-Chest to loot. Please create one first.\n";
                }
                HFL::WaitForInput();
            }
        }

        // --- STEP 4: CONCLUSION ---
        HFL::ClearScreen();
        HFL::PrintHeader("Conclusion");

        std::cout << "Summary:\n\n";
        std::cout << "1. Uniformity:\n";
        std::cout << "   'Inventory->GetValue()' works for Coin and Chest.\n";
        std::cout << "   They are treated exactly the same way.\n\n";

        std::cout << "2. Recursion (Tree Traversal):\n";
        std::cout << "   MainInventory -> Chest A -> Chest B -> Coin.\n";
        std::cout << "   The 'GetValue()' call chains down the tree.\n";
        std::cout << "   It sums up everything automatically.\n\n";

        std::cout << "3. Composition (Has-A Relationship):\n";
        std::cout << "   'Chest' 'Has-A' Items.\n";
        std::cout << "   When Chest dies, Items are destroyed.\n";
        std::cout << "   This manages memory naturally.\n\n";

        std::cout << "This is Composite Pattern.\n";

        std::cout << std::setw(40) << "End of Demo\n";
        HFL::WaitForInput();
    }
}