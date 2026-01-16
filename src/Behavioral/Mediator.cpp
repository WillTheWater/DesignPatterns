#include "Behavioral/Mediator.h"

namespace MED
{
    // =========================================================================
    // 1. INVENTORY IMPLEMENTATION (The Colleague)
    // =========================================================================
    Inventory::Inventory(IMediator* Manager, std::string Name)
        : Mediator(Manager), OwnerName(Name)
    {
        Grid.assign(9, { " . ", "Empty", 0 });
    }

    void Inventory::SelectSlot(int Index)
    {
        // --- THE PATTERN ---
        // The colleague performs NO logic itself. It reports the event 
        // to the mediator and it decides what happens next.
        if (Index >= 0 && Index < 9)
        {
            Mediator->Notify(this, Index);
        }
    }

    int Inventory::GetFirstEmptySlot() const
    {
        for (int i = 0; i < 9; ++i)
        {
            if (Grid[i].Name == "Empty") return i;
        }
        return -1;
    }

    void Inventory::Draw(int StartX) const
    {
        HFL::SetColor(HFL::EColor::BrightCyan);
        std::cout << "      [" << OwnerName << "]\n";
        HFL::SetColor(HFL::EColor::White);

        std::cout << "      1   2   3\n";
        std::cout << "    -------------\n";

        for (int row = 0; row < 3; ++row)
        {
            int rowStartDisplay = (row * 3) + 1;
            std::cout << " " << rowStartDisplay << " |";

            for (int col = 0; col < 3; ++col)
            {
                int idx = (row * 3) + col;
                if (Grid[idx].Name != "Empty") HFL::SetColor(HFL::EColor::BrightGreen);
                else HFL::SetColor(HFL::EColor::Gray);

                std::cout << " " << Grid[idx].Icon << " ";
                HFL::SetColor(HFL::EColor::White);
            }
            std::cout << "|\n";
        }
        std::cout << "    -------------\n";
    }

    // =========================================================================
    // 2. TRADE MEDIATOR IMPLEMENTATION (The Brain)
    // =========================================================================
    TradeMediator::TradeMediator()
    {
        PlayerInv = std::make_shared<Inventory>(this, "Player Bag");
        MerchantInv = std::make_shared<Inventory>(this, "Merchant");
        InitializeMarket();
    }

    void TradeMediator::InitializeMarket()
    {
        MerchantInv->SetItem(0, { "[|]", "Steel Sword", 40 });
        MerchantInv->SetItem(1, { "[o]", "Wood Shield", 25 });
        MerchantInv->SetItem(2, { "[!]", "Red Potion", 10 });
        MerchantInv->SetItem(3, { "[D]", "Long Bow", 35 });
        MerchantInv->SetItem(4, { "[^]", "Wizard Hat", 50 });
        MerchantInv->SetItem(5, { "[*]", "Blue Ruby", 80 });

        PlayerInv->SetItem(0, { "[~]", "Old Bread", 2 });
        PlayerInv->SetItem(1, { "[#]", "Mana Potion", 40 });
        PlayerInv->SetItem(5, { "[V]", "Diamond", 120 });
    }

    void TradeMediator::Notify(Inventory* Sender, int SlotIndex)
    {
        ItemSlot SelectedItem = Sender->GetItem(SlotIndex);
        if (SelectedItem.Name == "Empty") return;

        // The Mediator encapsulates the interaction rules between two objects.
        if (Sender == MerchantInv.get())
        {
            if (PlayerGold >= SelectedItem.Value)
            {
                int Target = PlayerInv->GetFirstEmptySlot();
                if (Target != -1)
                {
                    PlayerGold -= SelectedItem.Value;
                    PlayerInv->SetItem(Target, SelectedItem);
                    MerchantInv->SetItem(SlotIndex, { " . ", "Empty", 0 });
                    LastMessage = "Purchased " + SelectedItem.Name + "!";
                }
                else { LastMessage = "Inventory Full!"; }
            }
            else { LastMessage = "Not enough Gold!"; }
        }
        else
        {
            // Transaction: Sell to Merchant (75% value)
            int Target = MerchantInv->GetFirstEmptySlot();
            if (Target != -1)
            {
                PlayerGold += static_cast<int>(SelectedItem.Value * 0.75f);
                MerchantInv->SetItem(Target, SelectedItem);
                PlayerInv->SetItem(SlotIndex, { " . ", "Empty", 0 });
                LastMessage = "Sold " + SelectedItem.Name + "!";
            }
            else { LastMessage = "Merchant has no room!"; }
        }
    }

    // =========================================================================
    // 3. DEMO IMPLEMENTATION
    // =========================================================================
    void RunDemo()
    {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        // --- STEP 1: INTRODUCTION ---
        HFL::ClearScreen();
        HFL::PrintHeader("Mediator Pattern: Marketplace");

        std::cout << "Definition:\n";
        std::cout << "Define an object that encapsulates how a set of objects interact.\n";
        std::cout << "Promotes loose coupling by keeping objects from referring to each other explicitly.\n\n";

        std::cout << "In This Demo:\n";
        std::cout << "There is a Player Bag and a Merchant Inventory (Colleagues).\n";
        std::cout << "Neither inventory knows the other exists. A 'TradeMediator' handles\n";
        std::cout << "all gold validation and item movement between them.\n";

        HFL::WaitForInput();

        // --- STEP 2: THE ROLES ---
        HFL::ClearScreen();
        HFL::PrintHeader("Step 1: The Architecture");

        std::cout << "1. The Mediator (TradeMediator):\n";
        std::cout << "   - Acts as the 'Controller'.\n";
        std::cout << "   - Contains the business logic for buying/selling.\n\n";

        std::cout << "2. The Colleagues (Inventory):\n";
        std::cout << "   - Reusable components that only know how to hold items.\n";
        std::cout << "   - They 'Notify' the mediator when a slot is clicked.\n\n";

        HFL::WaitForInput();


        // --- STEP 3: INTERACTIVE LOOP ---
        TradeMediator Market;
        while (true)
        {
            HFL::ClearScreen();
            HFL::PrintHeader("Marketplace Simulation");

            HFL::SetColor(HFL::EColor::BrightYellow);
            std::cout << " [ GOLD: " << Market.GetPlayerGold() 
                << "g ]          [ LOG: " << Market.GetLastMessage() << " ]\n\n";
            HFL::SetColor(HFL::EColor::White);

            Market.GetMerchantInv()->Draw(0);
            std::cout << "\n";
            Market.GetPlayerInv()->Draw(0);

            std::cout << "\nActions:\n";
            std::cout << "1. Buy from Merchant\n";
            std::cout << "2. Sell to Merchant\n";
            std::cout << "0. Continue\n\n";

            int Choice = HFL::GetValidMenuInput(2);
            if (Choice == 0) break;

            std::cout << "Enter Slot Index (1-9): ";
            int UserSlot;
            std::cin >> UserSlot;

            int InternalIndex = UserSlot - 1;

            if (Choice == 1) Market.GetMerchantInv()->SelectSlot(InternalIndex);
            else if (Choice == 2) Market.GetPlayerInv()->SelectSlot(InternalIndex);
        }

        // --- STEP 4: CONCLUSION ---
        HFL::ClearScreen();
        HFL::PrintHeader("Conclusion");

        std::cout << "1. Decoupling:\n";
        std::cout << "   The Inventory class is 'Pure'. It doesn't know about Gold,\n";
        std::cout << "   Merchant logic, or other Inventories.\n\n";

        std::cout << "2. Centralized Logic:\n";
        std::cout << "   If we want to add a 10% Trade Tax, we change one line in the\n";
        std::cout << "   Mediator's Notify() function, not every inventory class.\n\n";

        std::cout << "3. Reusability:\n";
        std::cout << "   The same Inventory class can be used for a Player, a Chest,\n";
        std::cout << "   or a Loot pile. Only the Mediator changes.\n\n";

        std::cout << std::setw(40) << "End of Demo\n";
        HFL::WaitForInput();
    }
}