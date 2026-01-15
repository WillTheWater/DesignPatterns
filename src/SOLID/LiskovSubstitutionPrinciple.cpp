#include "SOLID/LiskovSubstitutionPrinciple.h"

namespace LSP
{
    // =========================================================================
    // BASE CLASS IMPLEMENTATION
    // =========================================================================
    Item::Item(std::string Name, float Weight)
        : Name(Name), Weight(Weight)
    {
    }

    // =========================================================================
    // WEAPON IMPLEMENTATION
    // =========================================================================
    Weapon::Weapon(std::string Name, float Weight, int Damage)
        : Item(Name, Weight)
        , Damage(Damage)
    {
    }

    int Weapon::GetValue() const
    {
        // Value formula: Damage * 10
        return Damage * 10;
    }

    // =========================================================================
    // POTION IMPLEMENTATION
    // =========================================================================
    Potion::Potion(std::string Name, float Weight, int HealAmount)
        : Item(Name, Weight)
        , HealAmount(HealAmount)
    {
    }

    int Potion::GetValue() const
    {
        // Value formula: HealAmount * 15
        return HealAmount * 15;
    }

    RawResource::RawResource(std::string Name, float Weight, ResourceType Material)
        : Item{Name, Weight}
        , RawMaterialType{Material}
    {
    }

    int RawResource::GetValue() const
    {
        return static_cast<int>(RawMaterialType) + ((int)Weight * 2);
    }

    // =========================================================================
    // TRADER IMPLEMENTATION
    // =========================================================================
    void Trader::AppraiseItem(const Item* Target) const
    {
        std::cout << "\n----------------------------------------\n";
        std::cout << "The Trader examines the " << Target->GetName() << "...\n";

        // The Trader doesn't care IF it's a weapon or potion.
        // It just asks for Value. This is LSP in action.
        int Value = Target->GetValue();

        std::cout << ">> Appraisal Value: " << Value << " Gold\n";
        std::cout << "----------------------------------------\n";
    }

    void Trader::SellItem(int Index, std::vector<std::unique_ptr<Item>>& Inventory) const
    {
        // 1. Validate Index
        if (Index < 0 || Index >= static_cast<int>(Inventory.size()))
        {
            std::cout << "[!] Item not found.\n";
            return;
        }

        // 2. Access the item via Index
        Item* Target = Inventory[Index].get();
        int Value = Target->GetValue();

        std::cout << "\n----------------------------------------\n";
        std::cout << "You sold the " << Target->GetName() << "!\n";
        std::cout << ">> Earned: " << Value << " Gold\n";
        std::cout << "----------------------------------------\n";

        // 3. Remove from vector
        Inventory.erase(Inventory.begin() + Index);
    }

    // =========================================================================
    // DEMO IMPLEMENTATION
    // =========================================================================
    void RunDemo()
    {
        // Clear buffer
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        // --- STEP 1: INTRODUCTION ---
        HFL::ClearScreen();
        HFL::PrintHeader("Liskov Substitution Principle (LSP)");

        std::cout << "Definition:\n";
        std::cout << "Subclasses must be substitutable for their base classes.\n";
        std::cout << "If a function uses a Parent, a Child must act exactly like the Parent.\n\n";

        std::cout << "In This Demo:\n";
        std::cout << "There is an Item Appraisal System.\n";
        std::cout << "The 'Trader' (System) expects an 'Item' (Base).\n";
        std::cout << "It passes 'Weapons', 'Potions' and 'RawResources' (Children) to it.\n";

        HFL::WaitForInput();

        // --- STEP 2: THE SETUP ---
        HFL::ClearScreen();
        HFL::PrintHeader("Step 1: The Architecture");

        std::cout << "Defined an abstract Base Class 'Item'.\n";
        std::cout << "It defines ONLY what is true for EVERY item.\n\n";

        std::cout << "Universal Traits (In Base):\n";
        std::cout << "   - Name\n";
        std::cout << "   - Weight\n";
        std::cout << "   - GetValue() (Virtual)\n\n";

        std::cout << "LSP Rule:\n";
        std::cout << "   We did NOT add 'GetDamage()' to Item.\n";
        std::cout << "   Why? Because Potions don't have damage.\n";
        std::cout << "   If a Potion was substituted, 'GetDamage()' would make no sense.\n";

        HFL::WaitForInput();

        // --- STEP 3: INTERACTIVE APPRAISAL ---

        // Create Objects (Upcasting automatically happens here)
        // Store them as pointers to the Base Class
        std::vector<std::unique_ptr<Item>> Inventory;

        // Uses raw pointers for the array, but manages with unique_ptr for cleanup
        Inventory.emplace_back(std::make_unique<Weapon>("Rusty Dagger", 2.0f, 5));
        Inventory.emplace_back(std::make_unique<Weapon>("War Hammer", 10.0f, 25));
        Inventory.emplace_back(std::make_unique<Potion>("Health Vial", 0.5f, 20));
        Inventory.emplace_back(std::make_unique<Potion>("Mana Potion", 0.5f, 15));
        Inventory.emplace_back(std::make_unique<RawResource>("Oak Log", 5.0f, ResourceType::Wood));
        Inventory.emplace_back(std::make_unique<RawResource>("Ruby", 0.1f, ResourceType::Gem));

        Trader Shopkeeper;

        bool InDemo = true;
        while (InDemo)
        {
            HFL::ClearScreen();
            HFL::PrintHeader("Trader Shop");

            // Display Inventory
            std::cout << "Select an item:\n\n";
            for (size_t i = 0; i < Inventory.size(); ++i)
            {
                std::cout << static_cast<int>(i) + 1 << ". " << Inventory[i]->GetName() << "\n";
            }
            std::cout << "0. Exit Demo\n";
            std::cout << "\nChoice: ";

            int ItemChoice;
            std::cin >> ItemChoice;

            if (std::cin.fail()) { std::cin.clear(); std::cin.ignore(); continue; }
            if (ItemChoice == 0) break;

            // Convert 1-based choice to 0-based index
            int ItemIndex = ItemChoice - 1;

            if (ItemIndex < 0 || ItemIndex >= static_cast<int>(Inventory.size())) continue;

            // --- ACTION MENU ---
            bool InActionMenu = true;
            while (InActionMenu)
            {
                HFL::ClearScreen();
                HFL::PrintHeader(Inventory[ItemIndex]->GetName());

                std::cout << "What do you want to do with this item?\n\n";
                std::cout << "1. Appraise\n";
                std::cout << "2. Sell\n";
                std::cout << "0. Back\n";
                std::cout << "\nChoice: ";

                int Action;
                std::cin >> Action;

                if (std::cin.fail()) { std::cin.clear(); std::cin.ignore(); continue; }

                if (Action == 0) { InActionMenu = false; }
                else if (Action == 1)
                {
                    Shopkeeper.AppraiseItem(Inventory[ItemIndex].get());
                    HFL::WaitForInput();
                }
                else if (Action == 2)
                {
                    Shopkeeper.SellItem(ItemIndex, Inventory);
                    // Since we sold it, we must exit the inner menu 
                    // because the vector size has changed and our index is invalid.
                    HFL::WaitForInput();
                    InActionMenu = false;
                }
            }
        }

        // --- STEP 4: CONCLUSION ---
        HFL::ClearScreen();
        HFL::PrintHeader("Conclusion");

        std::cout << "Summary of LSP:\n\n";
        std::cout << "1. The Trader code never changed.\n";
        std::cout << "   It didn't need 'if (IsWeapon) ... else if (IsPotion)'.\n\n";

        std::cout << "2. Substituted freely.\n";
        std::cout << "   Passes Weapons and Potions to a function expecting 'Item'.\n";
        std::cout << "   Because both honored the 'GetValue()' contract, it worked perfectly.\n\n";

        std::cout << "3. Common Trap Avoided.\n";
        std::cout << "   Avoides putting 'GetDamage()' in the Base.\n";
        std::cout << "   This ensures 'Potion' remains a valid substitute for 'Item'.\n\n";

        std::cout << std::setw(40) << "End of Demo\n";
    }
}