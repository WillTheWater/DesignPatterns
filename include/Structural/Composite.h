#pragma once

#include "FunctionLibrary/HelperFunctionLibrary.h"

// =========================================================================
// STRUCTURAL DESIGN PATTERN: Composite
// =========================================================================
// "Compose objects into tree structures to represent part-whole hierarchies."
//
// THE GOAL:
// Allows treating individual objects (Leaf) and compositions of objects 
// (Composite) uniformly. In this case, a single "Coin" and a "Chest" full 
// of items both behave as an "Inventory Item."
//
// THE BENEFIT:
// * Uniformity: The client doesn't need to check if an item is a single unit 
//   or a container; it simply calls GetValue().
// * Recursive Hierarchy: Composites can contain other Composites (a Chest 
//   inside a Chest), and the math "just works."
// * Extensibility: New item types can be added without changing the 
//   logic of the container or the client.
//
// THE EXAMPLE:
// [IInventoryItem]: The Component. Interface for all lootable objects.
// [Coin/Items/etc]: The Leaves. Primitive items with a fixed value.
// [Chest]: The Composite. A container that sums its children's values.
// [Player]: The Client. Manages a MainInventory (the root Composite).
// =========================================================================

namespace COM
{
    // =========================================================================
    // THE COMPONENT (The Interface)
    // ROLE: Defines the common interface for both simple and complex objects.
    // =========================================================================
    class IInventoryItem
    {
    public:
        virtual ~IInventoryItem() = default;

        // The uniform operation used by both Leaf and Composite
        virtual int GetValue() const = 0;
        virtual std::string GetName() const = 0;
    };

    inline std::string ITEM_NAMES[20] =
    {
        "Harlequin Crest", "Windforce", "The Grandfather", "Stone of Jordan",
        "Arkaine's Valor", "Stormshield", "Skin of the Vipermagi", "Wizardspike",
        "War Traveler", "The Eye of Etlich", "Bonesnap", "Buriza-Do Kyanon",
        "Shaftstop", "String of Ears", "Magefist", "Raven Frost",
        "Mara's Kaleidoscope", "The Oculus", "Doombringer", "Guardian Angel"
    };

    // =========================================================================
    // THE LEAVES (The Single Items)
    // ROLE: Primitive objects that have no children. They perform the 
    // actual work of returning base values.
    // =========================================================================

    // ======================== COIN ========================
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

    // ======================== ITEM ========================
    class Item : public IInventoryItem
    {
    public:
        Item(int GoldAmount, std::string Name);

        int GetValue() const override { return Value; }
        std::string GetName() const override { return Name; }

    private:
        int Value;
        std::string Name;
    };

    // =========================================================================
    // THE COMPOSITE (The Group)
    // ROLE: Stores child components and implements the interface by 
    // delegating work to those children (Recursion).
    // =========================================================================
    class Chest : public IInventoryItem
    {
    public:
        Chest(std::string Name);

        // THE RECURSIVE OPERATION: Sums base value + all children
        int GetValue() const override;
        std::string GetName() const override { return Name; }

        // Composition Management
        void AddItem(std::unique_ptr<IInventoryItem> Item);
        std::vector<std::unique_ptr<IInventoryItem>>& GetItems() { return Items; }

    private:
        std::vector<std::unique_ptr<IInventoryItem>> Items;
        int BaseValue;
        std::string Name;
    };

    // =========================================================================
    // THE CLIENT (The Player)
    // ROLE: Interacts with the Component interface to manipulate the hierarchy.
    // =========================================================================
    class Player
    {
    public:
        Player();

        // High-level loot logic
        void AddToInventory(std::unique_ptr<IInventoryItem> Item);
        void LootChest(Chest* TargetChest);

        // Uses the uniform interface to calculate the grand total
        void PrintTotalValue();

    private:
        // The root of the hierarchy is itself a Composite (Chest)
        std::unique_ptr<Chest> MainInventory;
    };

    // =========================================================================
    // DEMO IMPLEMENTATION
    // =========================================================================
    void RunDemo();
}