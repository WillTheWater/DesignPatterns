#include "SOLID/SingleResponsibilityPrinciple.h"

namespace SRP
{
    // =========================================================================
    // INVENTORY
    // =========================================================================
    void Inventory::AddItem(const std::string& Name, int Qty, float Weight)
    { 
        // Check if the item already exists in the inventory.
        auto NewItem = Items.find(Name);

        if (NewItem != Items.end())
        {
            // Item exists: Stack it.
            NewItem->second.Quantity += Qty;
            NewItem->second.Weight += (Weight * Qty);
        }
        else
        {
            // Item doesn't exist: Create it and add it.
            Items[Name] = Item{ Name, Qty, (Weight * Qty) };
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
            // If there is more than what is being removed, reduce the count.
            RemovedItem->second.Quantity -= Qty;
            return true;
        }
        else
        {
            // If removing >=, just remove the entry entirely.
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
        // they just need to iterate over a list of Item structs.
        //
        // Note: Pair.second is the Item struct (Name, Qty, Weight).

        std::vector<Item> ResultList;
        ResultList.reserve(Items.size()); // Optimization: Pre-allocates memory

        for (const auto& Pair : Items) 
        {
            // Copies the Item struct into the vector
            ResultList.push_back(Pair.second);
        }

        return ResultList;
    }

    // =========================================================================
    // DISPLAY
    // =========================================================================
    void InventoryDisplay::DisplayInventory(const Inventory& inventory) const
    {
        // ======================== Header Configuration ========================
        HFL::SetColor(HFL::EColor::Green);
        std::cout << std::left
            << std::setw(25) << "ITEM NAME"
            << std::right
            << std::setw(8) << "QUANTITY"
            << std::setw(12) << "WEIGHT"
            << "\n";

        // ======================== Divider ========================
        HFL::SetColor(HFL::EColor::Cyan);
        std::cout << std::setfill('-') << std::setw(45) << "" << std::setfill(' ') << "\n";

        // ======================== Data Rendering ========================
        const auto Items = inventory.GetItems();

        if (Items.empty())
        {
            HFL::SetColor(HFL::EColor::Gray);
            std::cout << "          [Inventory Status: Empty]\n";
        }
        else
        {
            HFL::SetColor(HFL::EColor::Gray);
            for (const auto& Item : Items)
            {
                std::cout << std::left << std::setw(25) << Item.Name
                    << std::right << std::setw(8) << Item.Quantity
                    << std::setw(12) << std::fixed << std::setprecision(2) << Item.Weight
                    << "\n";
            }
        }

        // ======================== Footer ========================
        HFL::SetColor(HFL::EColor::Cyan);
        std::cout << std::setfill('-') << std::setw(45) << "" << std::setfill(' ') << "\n\n";

        HFL::SetColor(HFL::EColor::White); // Reset to default
    }

    // =========================================================================
    // SAVE/LOAD
    // =========================================================================
    void InventorySaveLoad::SaveInventory(const Inventory& inventory, const std::string& filename) const
    {
        std::ofstream File(filename);
        if (!File.is_open())
        {
            std::cerr << "[Error] Could not open file for saving: " << filename << std::endl;
            return;
        }

        std::cout << "[System] Saving inventory to " << filename << "...\n";

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
                    std::cerr << "[Warning] Skipping invalid item data: " << Line
                        << " (Reason: " << e.what() << ")\n";
                }
            }
        }

        File.close();
        std::cout << "[System] Load successful!\n";
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
        HFL::PrintHeader("SINGLE RESPONSIBILITY PRINCIPLE");

        HFL::PrintSection("THE DEFINITION");
        HFL::SetColor(HFL::EColor::White);
        std::cout << "A class should have one, and only one, reason to change.\n\n";

        HFL::PrintSection("THE GOAL");
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "The Single Responsibility Principle is a foundational rule of software\n"
            << "engineering designed to increase system stability. It dictates that\n"
            << "every module or class should encapsulate a single part of the software's\n"
            << "functionality. This reduces dependencies (coupling) and ensures that\n"
            << "modifications to one domain do not result in unintended side effects\n"
            << "in unrelated subsystems.\n\n";

        HFL::PrintSection("THE EXAMPLE");
        HFL::SetColor(HFL::EColor::White);
        std::cout << "This demonstration has a Game Inventory System with three distinct\n"
            << "functional layers:\n\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] INVENTORY: ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Data storage, item stacking, and state management.\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] DISPLAY (UI): ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Data formatting for displaying the Inventory.\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] SAVING/LOADING: ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Serialization and file I/O operations.\n\n";

        HFL::PrintSection("THE BENEFIT");
        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] MAINTAINABILITY: ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Updates to the UI layer occur without logic modification.\n";
        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] REUSABILITY: ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "The core logic persists regardless of the save medium.\n";
        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] CLEAN CODE: ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Error handling is localized to the relevant module.\n";

        HFL::WaitForInput();

        // ======================== THE ARCHITECTURE ========================
        HFL::ClearScreen();
        HFL::PrintHeader("THE ARCHITECTURE");

        HFL::PrintSection("EXAMPLE");
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "To adhere to SRP, the system is decomposed into three distinct classes.\n"
            << "Each class encapsulates a specific domain, ensuring that changes to\n"
            << "one system do not necessitate modifications to others.\n\n";

        HFL::PrintSection("IMPLEMENTATION");

        // ======================== INVENTORY CLASS ========================
        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] Inventory\n";
        HFL::SetColor(HFL::EColor::White);
        std::cout << "    ROLE:           ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Internal State Management.\n";
        HFL::SetColor(HFL::EColor::White);
        std::cout << "    SCOPE:          ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Addition, removal, and retrieval of item data.\n";
        HFL::SetColor(HFL::EColor::White);
        std::cout << "    CONSTRAINT:     ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Maintains no knowledge of UI or File I/O.\n\n";

        // ======================== UI CLASS ========================
        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] InventoryDisplay\n";
        HFL::SetColor(HFL::EColor::White);
        std::cout << "    ROLE:           ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Visual Representation.\n";
        HFL::SetColor(HFL::EColor::White);
        std::cout << "    SCOPE:          ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Formats raw inventory data for the console interface.\n";
        HFL::SetColor(HFL::EColor::White);
        std::cout << "    CONSTRAINT:     ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Cannot modify the inventory data it observes.\n\n";

        // ======================== SAVE/LOAD CLASS ========================
        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] InventorySaveLoad\n";
        HFL::SetColor(HFL::EColor::White);
        std::cout << "    ROLE:           ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Data Persistence.\n";
        HFL::SetColor(HFL::EColor::White);
        std::cout << "    SCOPE:          ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Serialization of items to disk and restoration from files.\n";
        HFL::SetColor(HFL::EColor::White);
        std::cout << "    CONSTRAINT:     ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Functions independently of the display implementation.\n\n";

        HFL::SetColor(HFL::EColor::White);
        HFL::WaitForInput();

        // ======================== THE IMPLEMENTATION ========================
        Inventory PlayerInventory;
        InventoryDisplay Display;
        InventorySaveLoad Persistence;

        std::filesystem::path SavePath = HFL::GetSaveDirectory("SRP");
        std::string Filename = (SavePath / "inventory_data.txt").string();

        struct MenuItems { std::string Name; float Weight; };
        std::vector<MenuItems> ItemDB = 
        {
            {"Health Potion", 0.5f},
            {"Iron Sword", 5.0f},
            {"Wooden Shield", 3.0f},
            {"Magic Scroll", 0.1f}
        };

        // ======================== THE INTERACTION ========================
        while (true)
        {
            HFL::ClearScreen();
            HFL::PrintHeader("INVENTORY");

            Display.DisplayInventory(PlayerInventory);

            HFL::PrintSection("MENU");
            HFL::SetColor(HFL::EColor::Green);
            std::cout << " [1] "; HFL::SetColor(HFL::EColor::White); std::cout << "ADD ITEM\n";
            HFL::SetColor(HFL::EColor::Green);
            std::cout << " [2] "; HFL::SetColor(HFL::EColor::White); std::cout << "REMOVE ITEM\n\n";
            HFL::SetColor(HFL::EColor::Green);
            std::cout << " [0] "; HFL::SetColor(HFL::EColor::White); std::cout << "CONTINUE\n\n";

            HFL::SetColor(HFL::EColor::Gray);
            int UserAction = HFL::GetValidMenuInput(2);

            if (UserAction == 0) break;

            int SelectedIndex = -1;
            int TransactionQuantity = 0;

            if (UserAction == 1 || UserAction == 2)
            {
                HFL::ClearScreen();
                HFL::PrintHeader(UserAction == 1 ? "TRANSACTION: ADD" : "TRANSACTION: REMOVE");
                Display.DisplayInventory(PlayerInventory);

                // ======================== Display Items ========================
                HFL::PrintSection("AVAILABLE ITEMS");
                for (size_t i = 0; i < ItemDB.size(); ++i)
                {
                    HFL::SetColor(HFL::EColor::Green);
                    std::cout << " [" << i + 1 << "] ";
                    HFL::SetColor(HFL::EColor::White);
                    std::cout << ItemDB[i].Name << "\n";
                }

                HFL::SetColor(HFL::EColor::White);
                std::cout << "\nSELECT ITEM: ";
                std::cin >> SelectedIndex;

                std::cout << "ENTER QUANTITY:  ";
                std::cin >> TransactionQuantity;

                // ======================== Processing ========================
                if (SelectedIndex >= 1 && SelectedIndex <= static_cast<int>(ItemDB.size()) && TransactionQuantity > 0)
                {
                    const auto& SelectedItem = ItemDB[SelectedIndex - 1];

                    HFL::SetColor(HFL::EColor::Gray);
                    std::cout << "\nProcessing ";
                    for (int i = 0; i < 3; ++i) { HFL::Wait(0.5f); std::cout << "."; }
                    std::cout << "\n";

                    if (UserAction == 1)
                    {
                        PlayerInventory.AddItem(SelectedItem.Name, TransactionQuantity, SelectedItem.Weight);
                        HFL::SetColor(HFL::EColor::Green);
                        std::cout << "[SUCCESS] ";
                        HFL::SetColor(HFL::EColor::White);
                        std::cout << "Transferred " << TransactionQuantity << " " << SelectedItem.Name << "(s) to inventory.\n";
                    }
                    else
                    {
                        if (PlayerInventory.RemoveItem(SelectedItem.Name, TransactionQuantity))
                        {
                            HFL::SetColor(HFL::EColor::Green);
                            std::cout << "[SUCCESS] ";
                            HFL::SetColor(HFL::EColor::White);
                            std::cout << "Removed " << TransactionQuantity << " " << SelectedItem.Name << "(s) from inventory.\n";
                        }
                        else
                        {
                            HFL::SetColor(HFL::EColor::BrightRed);
                            std::cout << "[FAILURE] ";
                            HFL::SetColor(HFL::EColor::White);
                            std::cout << "Insufficient stock or item not found: " << SelectedItem.Name << ".\n";
                        }
                    }
                }
                else
                {
                    HFL::SetColor(HFL::EColor::BrightRed);
                    std::cout << "\n[ERROR] Invalid input parameters. Transaction aborted.\n";
                }

                HFL::Wait(2.5f);
            }
        }

        // ======================== DISPLAYING DATA ========================
        HFL::ClearScreen();
        HFL::PrintHeader("DISPLAY");

        HFL::PrintSection("ACTION");
        HFL::SetColor(HFL::EColor::White);
        std::cout << "Injecting Inventory reference into the Display system.\n\n";

        HFL::PrintSection("PROCESS");
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "The Inventory class maintains zero logic regarding visual output.\n"
            << "By passing the data to InventoryDisplay, the system achieves a strict\n"
            << "decoupling of raw state from the presentation layer.\n\n";

        Display.DisplayInventory(PlayerInventory);

        HFL::WaitForInput();

        // ======================== SAVING DATA ========================
        HFL::ClearScreen();
        HFL::PrintHeader("SAVING");

        HFL::PrintSection("ACTION");
        HFL::SetColor(HFL::EColor::White);
        std::cout << "Executing external file serialization.\n\n";

        HFL::PrintSection("PROCESS");
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "File paths, stream buffers, and disk I/O are encapsulated within\n"
            << "InventorySaveLoad. The Inventory class remains unaware of the\n"
            << "underlying storage medium.\n\n";

        HFL::WaitForInput();

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[SYSTEM]: ";
        HFL::SetColor(HFL::EColor::White);
        std::cout << "Accessing: " << Filename << "\n";
        HFL::Wait(0.8f);

        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Writing data packets";
        for (int i = 0; i < 3; ++i) { HFL::Wait(1.f); std::cout << "."; }
        std::cout << "\n";

        Persistence.SaveInventory(PlayerInventory, Filename);

        HFL::WaitForInput();

        // ======================== LOADING DATA ========================
        HFL::ClearScreen();
        HFL::PrintHeader("SIMULATING SYSTEM RESTART");

        HFL::PrintSection("ACTION");
        HFL::SetColor(HFL::EColor::White);
        std::cout << "Instantiating a secondary Inventory and performing restoration.\n\n";

        HFL::PrintSection("PROCESS");
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "By populating a 'NewInventory' object from disk, the system shows\n"
            << "that the SaveLoad module functions independently of any specific\n"
            << "active instance or state.\n\n";

        HFL::WaitForInput();

        Inventory NewInventory;
        HFL::SetColor(HFL::EColor::White);
        std::cout << "Creating a NEW empty Invetory\n";
        Display.DisplayInventory(NewInventory);
        HFL::WaitForInput();

        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Loading Inventory";
        for (int i = 0; i < 3; ++i) { HFL::Wait(1.f); std::cout << "."; }
        std::cout << "\n\n";

        Persistence.LoadInventory(NewInventory, Filename);
        std::cout << "\n";

        HFL::PrintSection("LOADED INVENTORY");
        Display.DisplayInventory(NewInventory);

        HFL::WaitForInput();

        // ======================== CONCLUSION ========================
        HFL::ClearScreen();
        HFL::PrintHeader("CONCLUSION");

        HFL::PrintSection("ARCHITECTURE");
        HFL::SetColor(HFL::EColor::White);
        std::cout << "The modular approach to the Inventory System confirms the following:\n\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] LOGIC ISOLATION: ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "The Inventory class remains focused strictly on data integrity.\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] UI INDEPENDENCE: ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Modifications to visual formatting do not impact core logic.\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] STORAGE AGNOSTIC: ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Transitioning between file formats or databases occurs without\n"
            << "    refactoring the Inventory or Display systems.\n\n";

        HFL::PrintSection("SUMMARY");
        HFL::SetColor(HFL::EColor::White);
        std::cout << "The Single Responsibility Principle ensures that software is:\n\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] FLEXIBLE: ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Easy to extend with new features.\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] ROBUST: ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Highly resistant to regressive bugs during maintenance.\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] SCALABLE: ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Capable of supporting complex systems through separation of concerns.\n\n";

        HFL::WaitForInput();
    }
}