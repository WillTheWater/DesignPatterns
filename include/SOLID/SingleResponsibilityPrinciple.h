#pragma once

#include "FunctionLibrary/HelperFunctionLibrary.h"

// =========================================================================
// SOLID DESIGN PRINCIPLE: Single Responsibility Principle
// =========================================================================
// "A class should have only one reason to change."
//
// THE GOAL:
// To avoid fragile code. When a class sticks to one job (high cohesion) and
// relies less on other classes' internal details (low coupling), small changes
// stop causing chain reactions of bugs.
// This principle isn't about limiting a class to one function. It means grouping
// code together because it changes for the same reason. Logic, user interface,
// and saving data are separate worlds. Changing the visuals shouldn't force you
// to rewrite the save system. Keeping them apart prevents accidental breakage.
//
// THE BENEFIT:
// * Maintenance: Fixing a bug in the save system won't accidentally break the UI.
// * Reusability: You can reuse the inventory logic in a new project without
//   dragging the old console UI code along with it.
// * Testability: You can test the logic without needing a hard drive or screen.
//
// THE EXAMPLE:
// [Inventory]:  Holds the data and rules.
// [InventoryDisplay]: Draws the data.
// [InventorySaveLoad]: Saves/Loads the data.
// =========================================================================

namespace SRP 
{
    // =========================================================================
    // ITEM
    // 
    // Defines the data that makes an item.
    // =========================================================================
    struct Item 
    {
        std::string Name;
        int Quantity;
        float Weight;
    };

    // =========================================================================
    // INVENTORY
    // 
    // The Inventory class has a SINGLE responsibility:
    // Add and remove items from the inventory.
    // =========================================================================
    class Inventory 
    {
    public:
        Inventory() = default;

        void AddItem(const std::string& Name, int Qty, float Weight);
        bool RemoveItem(const std::string& Name, const int Qty);
        std::vector<Item> GetItems() const;
        int GetTotalCount() const { return static_cast<int>(Items.size()); }

    private:
        std::unordered_map<std::string, Item> Items;
    };

    // =========================================================================
    // PRESENTATION (UI)
    //
    // This class has a SINGLE responsibility:
    // Display the inventory changes.
    // It takes a const reference to Inventory. It doesn't own it,
    // it just looks at it.
    // =========================================================================
    class InventoryDisplay 
    {
    public:
        void DisplayInventory(const Inventory& inventory) const; 
    };

    // =========================================================================
    // SAVE/LOAD:
    //
    // This class has a SINGLE responsibility:
    // Save and load the player's inventory.
    // =========================================================================
    class InventorySaveLoad 
    {
    public:
        void SaveInventory(const Inventory& inventory, const std::string& filename) const;
        void LoadInventory(Inventory& inventory, const std::string& filename) const;
    };

    // =========================================================================
    // DEMO: 
    // =========================================================================
    void RunDemo();

}