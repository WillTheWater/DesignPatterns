#include "SOLID/SingleResponsibilityPrinciple.h"

namespace SRP
{
    // ------------------------------------------------------------------------
    // INVENTORY IMPLEMENTATION
    // ------------------------------------------------------------------------
    void Inventory::AddItem(const std::string& Name, int Qty, float Weight)
    { 
        // Check if the item already exists in the inventory.
        auto NewItem = Items.find(Name);

        if (NewItem != Items.end())
        {
            // Item exists: Stack it.
            NewItem->second.Quantity += Qty;
            NewItem->second.Weight += Weight;
        }
        else
        {
            // Item doesn't exist: Create it and add it.
            Items[Name] = Item{ Name, Qty, Weight };
        }
    }

    bool Inventory::RemoveItem(const std::string& Name, const int Qty)
    {
        // Find the item
        auto RemovedItem = Items.find(Name);

        // 1. Check if item exists
        if (RemovedItem == Items.end())
        {
            return false;
        }

        // 2. Check quantities
        if (RemovedItem->second.Quantity > Qty)
        {
            // You have more than we are removing. Reduce the count.
            RemovedItem->second.Quantity -= Qty;
            return true;
        }
        else
        {
            // If you are removing >= what you have, just remove the entry entirely.
            // This covers the "Remove more than you have" case.
            Items.erase(RemovedItem);
            return true;
        }
    }

    std::vector<Item> Inventory::GetItems() const
    {
        // Returns a copy of the inventory as a vector.
        //
        // This hides the internal implementation (unordered_map).
        // The Display and Save classes don't need to know how items are stored,
        // they just need to iterate over a list of full Item structs.
        //
        // Note: Pair.second is the Item struct (Name, Qty, Weight).

        std::vector<Item> ResultList;
        ResultList.reserve(Items.size()); // Optimization: Pre-allocates memory

        for (const auto& Pair : Items) 
        {
            // Copies the Item struct into the list
            ResultList.push_back(Pair.second);
        }

        return ResultList;
    }

    // ------------------------------------------------------------------------
    // DISPLAY IMPLEMENTATION
    // ------------------------------------------------------------------------
    void InventoryDisplay::DisplayInventory(const Inventory& inventory) const
    {
        std::cout << "\n--- Current Inventory ---\n";

        // 1. HEADER ROW
        // Set left alignment for text, fixed widths for columns
        std::cout << std::left
            << std::setw(25) << "Item Name"
            << std::right
            << std::setw(5) << "Qty"
            << std::setw(10) << "Weight"
            << "\n";

        // 2. SEPARATOR LINE
        std::cout << std::setfill('-');
        std::cout << std::setw(40) << ""
            << std::setfill(' ')
            << "\n";

        // 3. DATA ROWS
        for (const auto& Item : inventory.GetItems())
        {
            // std::left: Align names to the left of the 25-char slot
            // std::right: Align numbers to the right (standard for accounting/math)
            // std::fixed & setprecision(2): Ensure weight always shows 2 decimals (e.g., 5.00 kg)

            std::cout << std::left << std::setw(25) << Item.Name
                << std::right << std::setw(5) << Item.Quantity
                << std::setw(10) << std::fixed << std::setprecision(2) << Item.Weight
                << "\n";
        }

        // 4. BOTTOM LINE
        std::cout << std::setfill('-') << std::setw(40) << "" << std::setfill(' ') << "\n";
    }

    // ------------------------------------------------------------------------
    // SAVE/LOAD IMPLEMENTATION
    // ------------------------------------------------------------------------
    void InventorySaveLoad::SaveInventory(const Inventory& inventory, const std::string& filename) const
    {
        std::ofstream File(filename);
        if (!File.is_open())
        {
            std::cerr << "[Error] Could not open file for saving: " << filename << std::endl;
            return;
        }

        std::cout << "[System] Saving inventory to " << filename << "...\n";

        // Format: Name,Quantity,Weight
        for (const auto& Item : inventory.GetItems())
        {
            File << Item.Name << "," << Item.Quantity << "," << Item.Weight << "\n";
        }

        File.close();
        std::cout << "[System] Save successful!\n";
    }

    void InventorySaveLoad::LoadInventory(Inventory& inventory, const std::string& filename) const
    {
        std::ifstream File(filename);
        if (!File.is_open())
        {
            std::cerr << "[Error] Could not open file for loading: " << filename << std::endl;
            return;
        }

        std::cout << "[System] Loading inventory from " << filename << "...\n";

        std::string Line;
        while (std::getline(File, Line))
        {
            if (Line.empty()) continue;

            std::stringstream SS(Line);
            std::string Name;
            std::string QtyStr;
            std::string WeightStr;

            if (std::getline(SS, Name, ',') &&
                std::getline(SS, QtyStr, ',') &&
                std::getline(SS, WeightStr, ','))
            {
                try
                {
                    int Qty = std::stoi(QtyStr);
                    float Weight = std::stof(WeightStr);
                    inventory.AddItem(Name, Qty, Weight);
                }
                catch (const std::exception& e)
                {
                    // Use e.what() to print the actual error
                    std::cerr << "[Warning] Skipping invalid item data: " << Line
                        << " (Reason: " << e.what() << ")\n";
                }
            }
        }

        File.close();
        std::cout << "[System] Load successful!\n";
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
        HFL::PrintHeader("Single Responsibility Principle (SRP)");

        std::cout << "Definition:\n";
        std::cout << "A class should have one, and only one, reason to change.\n\n";

        std::cout << "In This Demo:\n";
        std::cout << "We will simulate a Game Inventory System. You will see how\n";
        std::cout << "separating Logic, Display, and Persistence makes the code\n";
        std::cout << "cleaner, safer, and easier to maintain.\n";

        HFL::WaitForInput();

        // --- STEP 2: THE SETUP ---
        HFL::ClearScreen();
        HFL::PrintHeader("Step 1: The Setup");

        std::cout << "We have created three distinct classes:\n\n";

        std::cout << "1. [Inventory]:\n";
        std::cout << "   - Responsibility: Manages data (Add/Remove items).\n";
        std::cout << "   - Does NOT know how to display or save itself.\n\n";

        std::cout << "2. [InventoryDisplay]:\n";
        std::cout << "   - Responsibility: Display the contents of the Inventory.\n";
        std::cout << "   - Does NOT know how items are stored.\n\n";

        std::cout << "3. [InventorySaveLoad]:\n";
        std::cout << "   - Responsibility: Read/Write files to disk.\n";
        std::cout << "   - Does NOT know Inventory or UI logic.\n\n";

        HFL::WaitForInput();

        // =========================================================================
        // SHARED SETUP
        // =========================================================================
        // Define these objects HERE so they are shared across all steps below.
        // =========================================================================
        Inventory MyInventory;
        InventoryDisplay Display;
        InventorySaveLoad Persistence;

        // Pre-calculate the file path once
        std::filesystem::path SavePath = HFL::GetSaveDirectory("SRP");
        std::string Filename = (SavePath / "inventory_data.txt").string();

        // Setup a simple database of items to choose from
        struct MenuItem { std::string Name; float Weight; };
        std::vector<MenuItem> ItemDB = {
            {"Health Potion", 0.5f},
            {"Iron Sword", 5.0f},
            {"Wooden Shield", 3.0f},
            {"Magic Scroll", 0.1f}
        };

        // --- STEP 3: POPULATING DATA ---
        HFL::ClearScreen();
        HFL::PrintHeader("Step 2: Interacting with Data");

        std::cout << "Action: We will manually add and remove items.\n\n";
        std::cout << "[Analysis]:\n";
        std::cout << "The Inventory class manages its own state. The Display class\n";
        std::cout << "simply observes that state and displays it to the user.\n\n";

        HFL::WaitForInput();

        // Interactive Loop
        bool InStep = true;
        while (InStep)
        {
            HFL::ClearScreen();
            HFL::PrintHeader("Inventory Editor");

            // 1. Always show the current state
            Display.DisplayInventory(MyInventory);

            std::cout << "\nSelect an action:\n";
            std::cout << "1. Add Item\n";
            std::cout << "2. Remove Item\n";
            std::cout << "0. Continue to next step\n";
            std::cout << "Choice: ";

            int Action;
            std::cin >> Action;

            // Clear errors in case user typed a letter
            if (std::cin.fail()) {
                std::cin.clear();
                std::cin.ignore();
                continue;
            }

            if (Action == 0) break;
            if (Action < 0 || Action > 2) continue;

            // --- ADD / REMOVE LOGIC ---
            int ItemIndex = -1;
            int Qty = 0;

            if (Action == 1 || Action == 2)
            {
                HFL::PrintHeader("Select Item");
                for (size_t i = 0; i < ItemDB.size(); ++i)
                {
                    std::cout << static_cast<int>(i) + 1 << ". " << ItemDB[i].Name << "\n";
                }
                std::cout << "\nItem Number: ";
                std::cin >> ItemIndex;

                std::cout << "Quantity: ";
                std::cin >> Qty;

                // Validate selection
                if (ItemIndex >= 1 && ItemIndex <= static_cast<int>(ItemDB.size()) && Qty > 0)
                {
                    const auto& SelectedItem = ItemDB[ItemIndex - 1];

                    if (Action == 1) // Action == 1 (Add)
                    {
                        MyInventory.AddItem(SelectedItem.Name, Qty, SelectedItem.Weight);
                        std::cout << "\n[Success] Added " << Qty << " " << SelectedItem.Name << "(s).\n";
                    }
                    else // Action == 2 (Remove)
                    {
                        if (MyInventory.RemoveItem(SelectedItem.Name, Qty))
                        {
                            std::cout << "\n[Success] Removed " << Qty << " " << SelectedItem.Name << "(s).\n";
                        }
                        else
                        {
                            // --- HANDLE FAILURE ---
                            std::cout << "\n[Error] You do not own any " << SelectedItem.Name << "(s).\n";
                        }
                    }
                }
                else
                {
                    std::cout << "\n[Error] Invalid selection.\n";
                }

                HFL::WaitForInput();
            }
        }

        // --- STEP 4: DISPLAYING DATA ---
        HFL::ClearScreen();
        HFL::PrintHeader("Step 3: Displaying Data");

        std::cout << "Action: Passing Inventory to the Display class.\n\n";
        std::cout << "[Analysis]:\n";
        std::cout << "The Inventory class doesn't know how to display itself. It is passed to\n";
        std::cout << "InventoryDisplay. This decouples the DATA from the Display functionality.\n\n";

        // We use the 'Display' object we created at the top of the function
        Display.DisplayInventory(MyInventory);

        HFL::WaitForInput();

        // --- STEP 5: SAVING DATA ---
        HFL::ClearScreen();
        HFL::PrintHeader("Step 4: Saving Data");

        std::cout << "Action: Saving Inventory.\n\n";
        std::cout << "[Analysis]:\n";
        std::cout << "The Inventory class knows nothing about file paths or writing.\n";
        std::cout << "It is delegated to the InventorySaveLoad.\n";

        // We use 'Persistence' object from the top
        // We use 'Filename' from the top
        std::cout << "\nSaving to: " << Filename << "\n";
        Persistence.SaveInventory(MyInventory, Filename);

        std::cout << "[Status]: File written successfully.\n";
        std::cout << "You can verify this by opening the file in a text editor.\n";

        HFL::WaitForInput();

        // --- STEP 6: LOADING DATA (PROOF) ---
        HFL::ClearScreen();
        HFL::PrintHeader("Step 5: Simulating a Restart");

        std::cout << "Action: Creating a NEW empty Inventory.\n";
        std::cout << "        Loading data from the file.\n\n";
        std::cout << "[Analysis]:\n";
        std::cout << "We create a 'NewInventory' object. It is empty by default.\n";
        std::cout << "We use InventorySaveLoad to populate it from our save file.\n";
        std::cout << "This proves that InventorySaveLoad is independent of the Inventory state.\n\n";

        Inventory NewInventory;
        std::cout << "New Inventory Count (Before Load): " << NewInventory.GetTotalCount() << "\n\n";

        // Using Persistence and Filename from the top
        Persistence.LoadInventory(NewInventory, Filename);

        std::cout << "\nDisplaying New Inventory (Loaded from file):\n";
        Display.DisplayInventory(NewInventory);

        HFL::WaitForInput();

        // --- STEP 7: CONCLUSION ---
        HFL::ClearScreen();
        HFL::PrintHeader("Conclusion");

        std::cout << "1. The Inventory class remained simple and focused on items.\n";
        std::cout << "2. Changing how we display items (UI) does not break Inventory.\n";
        std::cout << "3. Changing how we save files (Database vs File) does not break Inventory.\n\n";
        std::cout << "This is the Single Responsibility Principle in action:\n";
        std::cout << "Keep responsibilities separated to make code flexible & robust.\n\n";

        std::cout << std::setw(40) << "End of Demo\n";
    }
}