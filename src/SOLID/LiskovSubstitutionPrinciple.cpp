#include "SOLID/LiskovSubstitutionPrinciple.h"

namespace LSP
{
    // =========================================================================
    // ITEM IMPLEMENTATIONS
    // =========================================================================
    Item::Item(std::string Name, float Weight) : Name(Name), Weight(Weight) {}

    Weapon::Weapon(std::string Name, float Weight, int Damage)
        : Item(Name, Weight), Damage(Damage) {
    }
    int Weapon::GetValue() const { return Damage * 10; }

    Potion::Potion(std::string Name, float Weight, int HealAmount)
        : Item(Name, Weight), HealAmount(HealAmount) {
    }
    int Potion::GetValue() const { return HealAmount * 15; }

    RawResource::RawResource(std::string Name, float Weight, ResourceType Material)
        : Item(Name, Weight), RawMaterialType(Material) {
    }
    int RawResource::GetValue() const { return static_cast<int>(RawMaterialType) + (static_cast<int>(Weight) * 2); }

    // =========================================================================
    // TRADER SYSTEM (LSP IN ACTION)
    // =========================================================================
    void Trader::AppraiseItem(const Item* Target) const
    {
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "[Trader] ";
        HFL::SetColor(HFL::EColor::White);
        std::cout << Target->GetName() << "...\n";

        // LSP demonstrates that there is no need to know if it's a Weapon or Potion.
        // It just calls the base contract: GetValue().
        int Value = Target->GetValue();

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[Result] Market Value: " << Value << " Gold\n";
    }

    void Trader::SellItem(int Index, std::vector<std::unique_ptr<Item>>& Inventory) const
    {
        if (Index < 0 || Index >= static_cast<int>(Inventory.size())) return;

        Item* Target = Inventory[Index].get();
        int Value = Target->GetValue();

        HFL::SetColor(HFL::EColor::Cyan);
        std::cout << "[Transaction] Sold " << Target->GetName() << " for " << Value << " Gold.\n";

        Inventory.erase(Inventory.begin() + Index);
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
        HFL::PrintHeader("LISKOV SUBSTITUTION PRINCIPLE");

        HFL::PrintSection("THE DEFINITION");
        HFL::SetColor(HFL::EColor::White);
        std::cout << "Derived classes must be substitutable for their base classes.\n\n";

        HFL::PrintSection("THE GOAL");
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "The Liskov Substitution Principle (LSP) ensures that a class hierarchy\n"
            << "remains logically consistent. It dictates that any instance of a parent\n"
            << "class should be replaceable by an instance of its child class without\n"
            << "altering the correctness of the program. This eliminates the need for\n"
            << "manual type-checking and prevents 'hidden' runtime errors.\n\n";

        HFL::PrintSection("THE EXAMPLE");
        HFL::SetColor(HFL::EColor::White);
        std::cout << "This demonstration features an Item Appraisal System with three distinct\n"
            << "implementation layers:\n\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] THE CONTRACT (BASE): ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Abstract item defining universal logic (Value/Weight).\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] THE SUBSTITUTES (CHILDREN): ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Weapons, Potions, and Resources implementing the contract.\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] THE CONSUMER (SYSTEM): ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "A Trader class that processes items via pure polymorphism.\n\n";

        HFL::PrintSection("THE BENEFIT");
        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] PREDICTABILITY: ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Functions work reliably regardless of the specific subclass passed.\n";
        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] SIMPLICITY: ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Removes 'if-else' type-checking and complex casting logic.\n";
        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] SCALABILITY: ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "New item types (Armor, Spells) can be added without changing the Trader.\n";

        HFL::WaitForInput();

        // ======================== THE ARCHITECTURE ========================
        HFL::ClearScreen();
        HFL::PrintHeader("THE ARCHITECTURE");

        HFL::PrintSection("EXAMPLE");
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "To adhere to LSP, the code must avoid 'Polluting the Base.' To ensure the parent\n"
            << "class represents the 'Lowest Common Denominator' of all children. If a child\n"
            << "cannot fulfill a specific method, that method does not belong in the base.\n\n";

        HFL::PrintSection("IMPLEMENTATION");

        // ======================== BASE CLASS ========================
        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] Item (Abstract Base)\n";
        HFL::SetColor(HFL::EColor::White);
        std::cout << "    ROLE:           ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Defines the shared identity of all inventory objects.\n";
        HFL::SetColor(HFL::EColor::White);
        std::cout << "    SCOPE:          ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Virtual GetValue() and GetName().\n";
        HFL::SetColor(HFL::EColor::White);
        std::cout << "    CONSTRAINT:     ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "No 'GetDamage()' as non-combat items cannot substitute it.\n\n";

        // ======================== CHILD CLASSES ========================
        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] Weapon / Potion / RawResource\n";
        HFL::SetColor(HFL::EColor::White);
        std::cout << "    ROLE:           ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Concrete specializations.\n";
        HFL::SetColor(HFL::EColor::White);
        std::cout << "    SCOPE:          ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Fulfills GetValue() using unique internal logic (Damage vs Rarity).\n";
        HFL::SetColor(HFL::EColor::White);
        std::cout << "    CONSTRAINT:     ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Guaranteed to work anywhere a base 'Item' is expected.\n\n";

        HFL::SetColor(HFL::EColor::White);
        HFL::WaitForInput();

        // ======================== INITIALIZATION ========================
        std::vector<std::unique_ptr<Item>> Inventory;
        Inventory.emplace_back(std::make_unique<Weapon>("Dragon Shield", 15.0f, 40));
        Inventory.emplace_back(std::make_unique<Weapon>("Vorpal Blade", 4.0f, 85));
        Inventory.emplace_back(std::make_unique<Potion>("Elixir of Life", 0.5f, 50));
        Inventory.emplace_back(std::make_unique<RawResource>("Flawless Ruby", 0.1f, ResourceType::Gem));

        Trader Shopkeeper;

        // ======================== SHOP INTERACTION ========================
        while (true)
        {
            HFL::ClearScreen();
            HFL::PrintHeader("TRADER SHOP");

            HFL::PrintSection("INVENTORY");
            if (Inventory.empty()) { std::cout << " [Empty]\n"; }
            for (size_t i = 0; i < Inventory.size(); ++i)
            {
                HFL::SetColor(HFL::EColor::Green);
                std::cout << " [" << i + 1 << "] ";
                HFL::SetColor(HFL::EColor::White);
                std::cout << std::left << std::setw(20) << Inventory[i]->GetName();
                HFL::SetColor(HFL::EColor::Gray);
                std::cout << " (" << Inventory[i]->GetWeight() << "kg)\n";
            }

            HFL::SetColor(HFL::EColor::Green);
            std::cout << "\n [0] "; HFL::SetColor(HFL::EColor::White); std::cout << "CONTINUE\n";

            int Choice = HFL::GetValidMenuInput(Inventory.size());
            if (Choice == 0) break;

            int Index = Choice - 1;

            // ======================== Action Sub-Menu ========================
            // Redraw the specific item context so the user sees what they selected
            HFL::PrintSection("SELECTED ITEM");
            HFL::SetColor(HFL::EColor::White);
            std::cout << " >> " << Inventory[Index]->GetName() << " <<\n\n";

            HFL::PrintSection("ACTION MENU");
            HFL::SetColor(HFL::EColor::Green);
            std::cout << "[1]            [2]        [0]\n";
            HFL::SetColor(HFL::EColor::White);
            std::cout << "APPRAISE   |   SELL   |   CANCEL\n";

            int Action = HFL::GetValidMenuInput(2);

            if (Action == 1)
            {
                HFL::SetColor(HFL::EColor::Gray);
                std::cout << "\n[System] Accessing Item Interface";
                for (int i = 0; i < 3; ++i) { HFL::Wait(0.4f); std::cout << "."; }
                std::cout << "\n";

                Shopkeeper.AppraiseItem(Inventory[Index].get());
                HFL::WaitForInput();
            }
            else if (Action == 2)
            {
                Shopkeeper.SellItem(Index, Inventory);
                HFL::Wait(1.0f);
            }
        }

        // ======================== CONCLUSION ========================
        HFL::ClearScreen();
        HFL::PrintHeader("CONCLUSION");

        HFL::PrintSection("ARCHITECTURE");
        HFL::SetColor(HFL::EColor::White);
        std::cout << "The strict adherence to Liskov Substitution confirms the following:\n\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] CONTRACT INTEGRITY: ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "The 'Trader' system interacts with the 'Item' interface exclusively.\n"
            << "    It remains oblivious to specific implementations (Weapons/Potions).\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] TYPE AGNOSTICISM: ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Subclasses like 'RawResource' were added and processed without\n"
            << "    requiring a single line of code change in the Trader logic.\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] DESIGN PURITY: ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "By excluding 'GetDamage()' from the base, we avoided a 'Liskov Violation'\n"
            << "    where calling a parent method on a child would result in an error.\n\n";

        HFL::PrintSection("SUMMARY");
        HFL::SetColor(HFL::EColor::White);
        std::cout << "The Liskov Substitution Principle ensures that software is:\n\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] PREDICTABLE: ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Polymorphic behavior is guaranteed to be safe and consistent.\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] EXTENSIBLE: ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "New subtypes can be integrated instantly if they honor the contract.\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] DECOUPLED: ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "High-level systems depend on abstractions, not concrete details.\n\n";

        HFL::WaitForInput();
    }
}