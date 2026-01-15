#pragma once

#include "FunctionLibrary/HelperFunctionLibrary.h"
#include <memory>
#include <string>
#include <vector>
#include <iostream>

// =========================================================================
// BEHAVIORAL DESIGN PATTERNS: Chain of Responsibility
// =========================================================================
// "Avoid coupling the sender of a request to its receiver by giving more 
// than one object a chance to handle the request. Chain the receiving 
// objects and pass the request along the chain until an object handles it."
//
// THE GOAL:
// Instead of a massive "God Function" with hundreds of if-statements, we 
// create an "Assembly Line" of specialized handlers. Each handler only 
// cares about one specific task and passes the result to the next.
//
// THE EXAMPLE:
// Action-RPG Loot Generator (Diablo-style).
// 1. Request (LootItem): The object being built (Sword, Armor, etc.).
// 2. Handler Interface (ItemHandler): The blueprint for all processing links.
// 3. Concrete Handlers (BaseType, Quality, Rarity, Affix):
//    - BaseType: Picks the item (Sword/Ring) and base stats.
//    - Quality: Rolls for "Superior" or "Elite" (+5 bonus).
//    - Rarity: Rolls for "Common", "Magic", or "Rare".
//    - Affix: Adds random adjectives and stats based on Rarity.
//
// BENEFIT:
// 1. Clean Code: No nested if-else nightmare for loot tables.
// 2. Scalability: Want "Holiday Event" loot? Just drop a new link in the chain.
// 3. Reusability: Handlers are independent and can be reordered easily.
// =========================================================================

namespace COR
{
    // =========================================================================
    // 1. DATA STRUCTURES
    // =========================================================================
    struct Affix
    {
        std::string Name;
        std::string Attribute;
    };

    struct LootItem 
    {
        std::string BaseType = "";
        std::string Rarity = "";
        std::string Quality = "";

        std::string MainStatName = "";
        int MainStatValue = 0;

        std::vector<Affix> Prefixes;
        std::vector<Affix> Suffixes;

        void Display() const;
    };

    // =========================================================================
    // 2. THE HANDLER INTERFACE
    // =========================================================================
    class ItemHandler 
    {
    protected:
        std::shared_ptr<ItemHandler> NextHandler;

    public:
        virtual ~ItemHandler() = default;

        // Link the next handler in the chain
        void SetNext(std::shared_ptr<ItemHandler> Next) { NextHandler = Next; }

        // Core processing method
        virtual void Handle(LootItem& Item) 
        {
            if (NextHandler) NextHandler->Handle(Item);
        }
    };

    // =========================================================================
    // 3. CONCRETE HANDLERS
    // =========================================================================

    // Sets the base item type and initial stats
    class BaseTypeHandler : public ItemHandler 
    {
    public:
        void Handle(LootItem& Item) override;
    };

    // Rolls for "Superior" or "Elite" (+5 to base stats)
    class QualityHandler : public ItemHandler 
    {
    public:
        void Handle(LootItem& Item) override;
    };

    // Determines if the item is Common, Magic, or Rare
    class RarityHandler : public ItemHandler 
    {
    public:
        void Handle(LootItem& Item) override;
    };

    // Adds Prefixes/Suffixes based on Rarity (Magic: 1 each, Rare: 2 each)
    class AffixHandler : public ItemHandler 
    {
    public:
        void Handle(LootItem& Item) override;
    private:
        static std::unordered_map<std::string, std::string> PrePool;
        static std::unordered_map<std::string, std::string> SufPool;
    };

    void RunDemo();
}