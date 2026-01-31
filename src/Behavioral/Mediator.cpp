#include "Behavioral/Mediator.h"

namespace MED
{
    // =========================================================================
    // THE COLLEAGUE IMPLEMENTATION (Inventory)
    // ROLE: Manages local data and reports interactions to the central hub.
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
        // to the mediator, which decides the "business rules" of the trade.
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

        std::cout << "      1  2  3\n";
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
    // THE CONCRETE MEDIATOR IMPLEMENTATION (TradeMediator)
    // ROLE: The Brain. Orchestrates interactions between different Inventories.
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
            // Logic: Buying from Merchant
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
            // Logic: Selling to Merchant (75% value)
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
    // DEMO IMPLEMENTATION
    // =========================================================================

    void RunDemo()
    {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        // ======================== INTRODUCTION ========================
        HFL::ClearScreen();
        HFL::PrintHeader("MEDIATOR DESIGN PATTERN");

        HFL::PrintSection("THE DEFINITION");
        HFL::SetColor(HFL::EColor::White);
        std::cout << "Define an object that encapsulates how a set of objects interact.\n"
            << "Promotes loose coupling by keeping objects from referring to each other explicitly.\n\n";

        HFL::PrintSection("THE GOAL");
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "The Mediator Pattern is about 'Communication Centralization'.\n"
            << "Instead of Inventories, Chests, and Vendors talking to each other directly,\n"
            << "they all report to a single Hub that enforces the transaction rules.\n\n";

        HFL::PrintSection("THE EXAMPLE");
        HFL::SetColor(HFL::EColor::White);
        std::cout << "A Trading System where two Inventories are kept completely decoupled:\n\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] THE COLLEAGUES:   ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "The Player and Merchant bags. They only handle internal grids.\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] THE NOTIFICATION: ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "When an item is clicked, the Bag simply says: 'Something happened here'.\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] THE HUB:          ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "The TradeMediator validates Gold, checks space, and moves the data.\n\n";

        HFL::WaitForInput();

        // ======================== THE ARCHITECTURE ========================
        HFL::ClearScreen();
        HFL::PrintHeader("THE ARCHITECTURE");

        HFL::PrintSection("THE 'TRAFFIC CONTROLLER'");
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Without a Mediator, every Inventory would need a reference to the Player's\n"
            << "Gold and every other Inventory. The Mediator 'flattens' this Many-to-Many\n"
            << "relationship into a manageable Many-to-One structure.\n\n";

        HFL::PrintSection("IMPLEMENTATION");

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] Inventory (The Colleague)\n";
        HFL::SetColor(HFL::EColor::White);
        std::cout << "    REUSABILITY:    ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "The same class works for Players, NPCs, or Loot Chests.\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] TradeMediator (The Master Hub)\n";
        HFL::SetColor(HFL::EColor::White);
        std::cout << "    ENFORCEMENT:    ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "The only place where 'Buying' and 'Selling' logic exists.\n\n";

        HFL::WaitForInput();

        // ======================== INTERACTIVE LOOP ========================
        TradeMediator Market;
        while (true)
        {
            HFL::ClearScreen();
            HFL::PrintHeader("MARKETPLACE SIMULATION");

            HFL::PrintSection("TRADING HUB");
            HFL::SetColor(HFL::EColor::BrightYellow);
            std::cout << " [ GOLD: " << Market.GetPlayerGold()
                << "g ]           [ LOG: " << Market.GetLastMessage() << " ]\n\n";

            Market.GetMerchantInv()->Draw(0);
            std::cout << "\n";
            Market.GetPlayerInv()->Draw(0);

            HFL::PrintSection("COMMANDS");
            HFL::SetColor(HFL::EColor::Green);
            std::cout << " [1] "; HFL::SetColor(HFL::EColor::White); std::cout << "BUY FROM MERCHANT\n";
            HFL::SetColor(HFL::EColor::Green);
            std::cout << " [2] "; HFL::SetColor(HFL::EColor::White); std::cout << "SELL TO MERCHANT\n";
            HFL::SetColor(HFL::EColor::Green);
            std::cout << " [0] "; HFL::SetColor(HFL::EColor::White); std::cout << "CONTINUE\n\n";

            int Choice = HFL::GetValidMenuInput(2);
            if (Choice == 0) break;

            std::cout << "Enter Slot Index (1-9): ";
            int UserSlot;
            std::cin >> UserSlot;

            int InternalIndex = UserSlot - 1;

            if (Choice == 1) Market.GetMerchantInv()->SelectSlot(InternalIndex);
            else if (Choice == 2) Market.GetPlayerInv()->SelectSlot(InternalIndex);
        }

        // ======================== CONCLUSION ========================
        HFL::ClearScreen();
        HFL::PrintHeader("CONCLUSION");

        HFL::PrintSection("ARCHITECTURE");
        HFL::SetColor(HFL::EColor::White);
        std::cout << "The implementation of the Mediator Pattern confirms the following:\n\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] DECOUPLING: ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "The Inventory class is 'Pure'. It has zero knowledge of Gold,\n"
            << "    Merchant logic, or the existence of other Inventories.\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] CENTRALIZED LOGIC: ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "To add a 'Tax' or 'Discount', we modify a single line in the\n"
            << "    Mediator's Notify() function instead of every Inventory class.\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] COMPONENT REUSE: ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "The same Inventory code can represent a Player, a Chest, or\n"
            << "    a Dead Body. Only the Mediator needs to change for each context.\n\n";

        HFL::PrintSection("SUMMARY");
        HFL::SetColor(HFL::EColor::White);
        std::cout << "The Mediator Pattern ensures that software is:\n\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] SCALABLE:  ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Adding new participants doesn't increase complexity exponentially.\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] CLEAN:     ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "The 'Spider Web' of object dependencies is replaced by a 'Star' topology.\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] TESTABLE:  ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Business rules can be tested inside the Mediator without mocking a UI.\n\n";

        HFL::SetColor(HFL::EColor::White);
        HFL::WaitForInput();
    }
}