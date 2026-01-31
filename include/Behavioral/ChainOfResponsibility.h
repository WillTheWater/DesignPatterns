#pragma once

#include "FunctionLibrary/HelperFunctionLibrary.h"

// =========================================================================
// BEHAVIORAL DESIGN PATTERN: Chain of Responsibility
// =========================================================================
// "Avoid coupling the sender of a request to its receiver by giving more 
// than one object a chance to handle the request. Chain the receiving 
// objects and pass the request along the chain until an object handles it."
//
// THE GOAL:
// Instead of a massive "God Function" with hundreds of if-statements to 
// calculate complex object properties, we create an "Assembly Line" of 
// specialized handlers. Each handler performs one specific transformation 
// and passes the object to the next link in the chain.
//
// THE BENEFIT:
// * Clean Code: Eliminates deeply nested conditional logic (Loot Tables).
// * Scalability: New features (like a 'Holiday Event' modifier) can be 
//   inserted into the chain without modifying existing handlers.
// * Reusability: Handlers are independent and can be reordered to create 
//   entirely different item generation logic.
//
// THE EXAMPLE:
// [LootItem]: The Request. The object being built (Sword, Armor, etc.).
// [ItemHandler]: The Interface. The blueprint for all processing links.
// [Concrete Handlers]:
//   - BaseTypeHandler: Selects the core item (e.g., "Greatsword").
//   - QualityHandler: Applies quality multipliers (e.g., "Superior").
//   - RarityHandler: Determines the tier (Common, Magic, Rare).
//   - AffixHandler: Rolls for magical prefixes and suffixes.
// =========================================================================

namespace COR
{
    // =========================================================================
    // DATA STRUCTURES (The Request)
    // ROLE: The state object that travels through the "Assembly Line."
    // =========================================================================
    struct Affix
    {
        std::string Name;
        std::string Attribute;
    };

    struct LootItem
    {
        // Category Data
        std::string BaseType = "";
        std::string Rarity = "";
        std::string Quality = "";

        // Core Combat Stats
        std::string MainStatName = "";
        int MainStatValue = 0;

        // Magical Modifiers
        std::vector<Affix> Prefixes;
        std::vector<Affix> Suffixes;

        void Display() const;
    };

    // =========================================================================
    // THE HANDLER INTERFACE
    // ROLE: Defines the contract for all processing links and manages
    // the pointer to the next handler in the sequence.
    // =========================================================================
    class ItemHandler
    {
    protected:
        std::shared_ptr<ItemHandler> NextHandler;

    public:
        virtual ~ItemHandler() = default;

        // Link the next handler in the chain
        void SetNext(std::shared_ptr<ItemHandler> Next) { NextHandler = Next; }

        // Core processing method: Logic + Delegation
        virtual void Handle(LootItem& Item)
        {
            if (NextHandler) NextHandler->Handle(Item);
        }
    };

    // =========================================================================
    // CONCRETE HANDLERS
    // ROLE: Perform one specific task and then delegate to the next link.
    // =========================================================================

    // ======================== BASETYPE HANDLER ========================
    // Sets the base item identity and initial core stats.
    class BaseTypeHandler : public ItemHandler
    {
    public:
        void Handle(LootItem& Item) override;
    };

    // ======================== QUALITY HANDLER ========================
    // Rolls for "Superior" or "Elite" (+5 to base stats).
    class QualityHandler : public ItemHandler
    {
    public:
        void Handle(LootItem& Item) override;
    };

    // ======================== RARITY HANDLER ========================
    // Determines if the item is Common, Magic, or Rare.
    class RarityHandler : public ItemHandler
    {
    public:
        void Handle(LootItem& Item) override;
    };

    // ======================== AFFIX HANDLER ========================
    // Adds magical adjectives based on the Rarity determined previously.
    class AffixHandler : public ItemHandler
    {
    public:
        void Handle(LootItem& Item) override;

    private:
        static std::unordered_map<std::string, std::string> PrePool;
        static std::unordered_map<std::string, std::string> SufPool;
    };

    // =========================================================================
    // DEMO IMPLEMENTATION
    // =========================================================================
    void RunDemo();
}