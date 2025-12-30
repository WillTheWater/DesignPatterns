#pragma once

#include "FunctionLibrary/HelperFunctionLibrary.h"

// =========================================================================
// PRINCIPLE: Single Responsibility Principle (SRP)
// =========================================================================
// "A class should have one, and only one, reason to change."
//
// THE GOAL:
// Decouple the different aspects of a system so that a change in one 
// area (like UI) does not force refactoring unrelated Logic.
//
// EXAMPLE:
// A Game Inventory system.
// 1. Inventory: Responsible only for storing and accessing item data.
// 2. InventoryDisplay: Responsible only for formatting data for UI.
// 3. InventorySaveLoad: Responsible only for File I/O (Writing/Reading).
//
// BENEFIT:
// If you decide to save to the Cloud instead of a file, you ONLY change 
// InventorySaveLoad. The Inventory and Display classes remain uneffected.
// =========================================================================

namespace SRP 
{
    // ------------------------------------------------------------------------
    // 1. ITEM DATA MODEL
    // Holds data about an item.
    // ------------------------------------------------------------------------
    struct Item 
    {
        std::string Name;
        int Quantity;
        float Weight;
    };

    // ------------------------------------------------------------------------
    // 2. INVENTORY
    // 
    // The Inventory class has ONE reason to change:
    // How to store or access items.
    // ------------------------------------------------------------------------
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

    // ------------------------------------------------------------------------
    // 3. PRESENTATION (UI)
    //
    // This class has ONE reason to change:
    // To change the format of the display.
    //
    // It takes a const reference to Inventory. It doesn't own it,
    // it just looks at it.
    // ------------------------------------------------------------------------
    class InventoryDisplay 
    {
    public:
        void DisplayInventory(const Inventory& inventory) const; 
    };

    // ------------------------------------------------------------------------
    // 4. SAVE/LOAD:
    //
    // This class has ONE reason to change:
    // To change the save file format
    // ------------------------------------------------------------------------
    class InventorySaveLoad 
    {
    public:
        void SaveInventory(const Inventory& inventory, const std::string& filename) const;
        void LoadInventory(Inventory& inventory, const std::string& filename) const;
    };

    // ------------------------------------------------------------------------
    // DEMO: 
    // ------------------------------------------------------------------------
    void RunDemo();

}