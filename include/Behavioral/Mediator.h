#pragma once

#include "FunctionLibrary/HelperFunctionLibrary.h"

// =========================================================================
// BEHAVIORAL DESIGN PATTERNS: Mediator
// =========================================================================
// "Define an object that encapsulates how a set of objects interact."
//
// THE GOAL:
// Reduce chaotic dependencies between objects. Instead of objects talking 
// directly to each other (Many-to-Many), they talk only to a central 
// Mediator (Many-to-One). This promotes loose coupling.
//
// THE EXAMPLE:
// A Marketplace Trading System (Player vs. Merchant).
// 1. The Colleague (Inventory): A 3x3 grid that holds items. Knows nothing 
//    of the other inventory's existence.
// 2. The Mediator (IMediator): The interface for communication.
// 3. The Concrete Mediator (TradeMediator): The "Transaction Hub." It 
//    validates gold, moves items between inventories.
//
// THE SCENARIO:
// The Player wants to buy a Sword. If the Merchant inventory is clicked, 
// the Mediator checks the Player's gold, removes the item from the Merchant,
// and places it in the Player's bag.
// =========================================================================

namespace MED
{
    struct ItemSlot 
    {
        std::string Icon = " . ";
        std::string Name = "Empty";
        int Value = 0;
    };

    class IMediator;

    // =========================================================================
    // 1. THE COLLEAGUE (The Participant)
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
        IMediator* Mediator;           // The Hub
        std::vector<ItemSlot> Grid;
        std::string OwnerName;
    };

    // =========================================================================
    // 2. THE MEDIATOR INTERFACE (The Protocol)
    // =========================================================================
    class IMediator 
    {
    public:
        virtual ~IMediator() = default;

        // This is the "Message Center" where Colleagues report actions
        virtual void Notify(Inventory* Sender, int SlotIndex) = 0;
    };

    class TradeMediator : public IMediator
    {
    public:
        TradeMediator();

        void Notify(Inventory* Sender, int SlotIndex) override;
        void RunDemo();

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

    void RunDemo();
}