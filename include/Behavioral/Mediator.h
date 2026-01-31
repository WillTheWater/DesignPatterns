#pragma once

#include "FunctionLibrary/HelperFunctionLibrary.h"

// =========================================================================
// BEHAVIORAL DESIGN PATTERN: Mediator
// =========================================================================
// "Define an object that encapsulates how a set of objects interact."
//
// THE GOAL:
// Reduce chaotic dependencies between objects. Instead of objects talking 
// directly to each other (Many-to-Many), they talk only to a central 
// Mediator (Many-to-One). This promotes loose coupling and simplifies
// the maintenance of complex communication workflows.
//
// THE BENEFIT:
// * Decoupling: Colleagues (Participants) don't need to know each other exist.
// * Centralization: Transaction or communication logic is in one place.
// * Reusability: Colleagues can be reused in different contexts because
//   they aren't hardcoded to talk to specific external classes.
//
// THE EXAMPLE:
// [ItemSlot]: The Data. A simple container for item properties.
// [Inventory]: The Colleague. A grid that holds items. It reports clicks
//   to the mediator but has no logic for "buying" or "selling."
// [TradeMediator]: The Hub. Orchestrates the exchange of items and gold.
//   It acts as the referee between the Player and Merchant inventories.
// =========================================================================

namespace MED
{
    // =========================================================================
    // THE DATA (The Item)
    // ROLE: A simple pod representing an object in the market.
    // =========================================================================
    struct ItemSlot
    {
        std::string Icon = " . ";
        std::string Name = "Empty";
        int Value = 0;
    };

    class IMediator;

    // =========================================================================
    // THE COLLEAGUE (The Participant)
    // ROLE: Manages local state (the grid) and notifies the Mediator of
    // user interactions. It never communicates with other Inventories directly.
    // =========================================================================
    class Inventory
    {
    public:
        Inventory(IMediator* Manager, std::string Name);

        // Notify the mediator that a specific slot was interacted with
        void SelectSlot(int Index);

        // Internal logic managed by the Mediator
        void SetItem(int Index, ItemSlot Item) { Grid[Index] = Item; }
        ItemSlot GetItem(int Index) const { return Grid[Index]; }
        int GetFirstEmptySlot() const;

        void Draw(int StartX) const;
        std::string GetOwnerName() const { return OwnerName; }

    private:
        IMediator* Mediator;           // The Transaction Hub
        std::vector<ItemSlot> Grid;
        std::string OwnerName;
    };

    // =========================================================================
    // THE MEDIATOR INTERFACE (The Protocol)
    // ROLE: Defines the "Message Center" through which all Colleagues talk.
    // =========================================================================
    class IMediator
    {
    public:
        virtual ~IMediator() = default;

        // This is the core contract where Colleagues report actions
        virtual void Notify(Inventory* Sender, int SlotIndex) = 0;
    };

    // =========================================================================
    // THE CONCRETE MEDIATOR (The Hub)
    // ROLE: The High-Level Policy. It holds the reference to all colleagues
    // and enforces the rules of the marketplace (Gold checks, space checks).
    // =========================================================================
    class TradeMediator : public IMediator
    {
    public:
        TradeMediator();

        // IMPLEMENTATION: Handle the logic for "What happens if a Merchant item is clicked?"
        void Notify(Inventory* Sender, int SlotIndex) override;

        // Demo specific accessors
        int GetPlayerGold() const { return PlayerGold; }
        std::string GetLastMessage() const { return LastMessage; }
        std::shared_ptr<Inventory> GetPlayerInv() { return PlayerInv; }
        std::shared_ptr<Inventory> GetMerchantInv() { return MerchantInv; }

    private:
        void InitializeMarket();

        std::shared_ptr<Inventory> PlayerInv;
        std::shared_ptr<Inventory> MerchantInv;
        int PlayerGold = 100;
        std::string LastMessage = "Welcome to the Market!";
    };

    // =========================================================================
    // DEMO ENTRY POINT
    // =========================================================================
    void RunDemo();
}