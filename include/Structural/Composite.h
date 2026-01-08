#pragma once

#include "FunctionLibrary/HelperFunctionLibrary.h"

// =========================================================================
// PRINCIPLE: Composite Pattern (Structural)
// =========================================================================
// "Compose objects into tree structures to represent part-whole hierarchies."
//
// THE GOAL:
// Allows you to treat a single object (Coin) and a collection of objects (Chest)
// in exactly the same way.
//
// THE EXAMPLE:
// An Inventory System (Chests and Items).
// 1. Component (IInventoryItem): Defines 'GetValue()' and 'GetName()'.
// 2. Leaves (The Objects): Coin, Weapon, Potion. (Single units).
// 3. Composite (The Group): Chest.
//    - Contains a list of other items (Coins or other Chests).
//    - 'GetValue()' sums up values of itself PLUS all children (Recursion).
//
// THE SCENARIO:
// A 'Player' with a 'MainInventory'.
// Find a 'Treasure Chest'.
// Add 'Coins' and a 'Sword' to Chest.
// "Move" (Loot) items from Chest to MainInventory.
//
// HOW IT WORKS:
// Player->CalculateTotalValue() works on Inventory (Composite)
// Inventory->GetValue() works on Chests (Composite)
// Chest->GetValue() automatically checks inside itself (Recursion).
//
// BENEFIT:
// You don't have to manually add up values when you loot.
// The Chest handles its own total. The Inventory handles the grand total.
// =========================================================================

namespace COM
{
    // ------------------------------------------------------------------------
    // 1. THE COMPONENT (The Interface)
    // ------------------------------------------------------------------------
    // This interface represents a "Part-Whole" relationship.
    // Both a "Leaf" (Single Item) and a "Composite" (Chest) implement this.
    class IInventoryItem
    {
    public:
        virtual ~IInventoryItem() = default;

        // Returns how much the item is worth (in Gold).
        virtual int GetValue() const = 0;

        virtual std::string GetName() const = 0;
    };

    // ------------------------------------------------------------------------
    // 2. LEAVES (The Single Items)
    // ------------------------------------------------------------------------
    // These are "Leafs" in the Composite tree.

    // --- A. COIN ---
    class Coin : public IInventoryItem
    {
    public:
        Coin(int GoldAmount);

        int GetValue() const override { return Value; }
        std::string GetName() const override { return Name; }

    private:
        int Value;
        std::string Name;
    };

    // --- B. WEAPON ---
    class Weapon : public IInventoryItem
    {
    public:
        Weapon(int GoldAmount, std::string Name);

        int GetValue() const override { return Value; }
        std::string GetName() const override { return Name; }

    private:
        int Value;
        std::string Name;
    };

    // --- C. POTION ---
    class Potion : public IInventoryItem
    {
    public:
        Potion(int GoldAmount, std::string Name);

        int GetValue() const override { return Value; }
        std::string GetName() const override { return Name; }

    private:
        int Value;
        std::string Name;
    };

    // ------------------------------------------------------------------------
    // 3. THE COMPOSITE (The Group)
    // ------------------------------------------------------------------------
    // This is the "Chest".
    // It contains a list of other items (Leaves or other Chests).
    class Chest : public IInventoryItem
    {
    public:
        Chest(std::string Name);

        // THE "WHOLE" OPERATION 
        // Chest value, is summed up:
        // 1. Its own value.
        // 2. The value of every item inside it.
        int GetValue() const override;

        // COMPOSITION (Aggregation)
        // The Chest holds a list of items. This is "Has-A" relationship.
        void AddItem(std::unique_ptr<IInventoryItem> Item);
        std::vector<std::unique_ptr<IInventoryItem>>& GetItems() { return Items; }

        std::string GetName() const override { return Name; }

    private:
        std::vector<std::unique_ptr<IInventoryItem>> Items;

        int BaseValue;
        std::string Name;
    };

    // ------------------------------------------------------------------------
    // 4. THE CLIENT (The Player)
    // ------------------------------------------------------------------------
    // The "Game Logic" that holds the Inventory and interacts with Chests.
    class Player
    {
    public:
        Player();

        // The Client (Player) acts on the Composite (Chest).
        void AddToInventory(std::unique_ptr<IInventoryItem> Item);
        void LootChest(Chest* TargetChest);

        // The Client calls CalculateTotalValue().
        // It doesn't know if Inventory has 10 Coins or 1 Chest with 10 Coins.
        void PrintTotalValue();

    private:
        std::unique_ptr<Chest> MainInventory;
    };

    // ------------------------------------------------------------------------
    // 5. DEMO
    // ------------------------------------------------------------------------
    void RunDemo();
}