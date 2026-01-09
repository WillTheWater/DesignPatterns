#include "Structural/Decorator.h"

namespace DEC
{
    BasicSword::BasicSword(std::string Name, int BaseDamage)
        : Name(Name), BaseDamage(BaseDamage)
    {
        std::cout << "[System] BasicSword Created: " << Name << " (Dmg: " << BaseDamage << ")\n";
    }

    IEnchantment::IEnchantment(std::unique_ptr<IWeapon> RawItem)
        : WrappedItem(std::move(RawItem)) {
    }

    FireEnchantment::FireEnchantment(std::unique_ptr<IWeapon> RawItem)
        : IEnchantment(std::move(RawItem)) {
    }

    int FireEnchantment::GetDamage() const
    {
        std::cout << " [Enchantment] Adding Fire Damage (+20)...\n";

        int BaseDamage = WrappedItem->GetDamage();
        int TotalDamage = BaseDamage + 20;

        std::cout << " [Enchantment] Result: ";
        return TotalDamage;
    }

    IceEnchantment::IceEnchantment(std::unique_ptr<IWeapon> RawItem)
        : IEnchantment(std::move(RawItem)) {
    }

    int IceEnchantment::GetDamage() const
    {
        std::cout << " [Enchantment] Adding Ice Damage (+10)...\n";

        int BaseDamage = WrappedItem->GetDamage();
        int TotalDamage = BaseDamage + 10;

        std::cout << " [Enchantment] Result: ";
        return TotalDamage;
    }

    PoisonEnchantment::PoisonEnchantment(std::unique_ptr<IWeapon> RawItem)
        : IEnchantment(std::move(RawItem)) {
    }

    int PoisonEnchantment::GetDamage() const
    {
        std::cout << " [Enchantment] Adding Poison (+15)...\n";

        int BaseDamage = WrappedItem->GetDamage();
        int TotalDamage = BaseDamage + 15;

        std::cout << " [Enchantment] Result: ";
        return TotalDamage;
    }

    // ------------------------------------------------------------------------
    // DEMO IMPLEMENTATION
    // ------------------------------------------------------------------------
    void RunDemo()
    {
        // Clear buffer
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        // --- STEP 1: INTRODUCTION ---
        HFL::ClearScreen();
        HFL::PrintHeader("Decorator Pattern");

        std::cout << "Definition:\n";
        std::cout << "Attach additional responsibilities to an object dynamically.\n";
        std::cout << "Add features without changing the class.\n\n";

        std::cout << "In This Demo:\n";
        std::cout << "There is a 'BasicSword'.\n";
        std::cout << "That gets wrapped with 'Enchantments'.\n";
        std::cout << "Each Enchantment adds a modifier (Fire, Ice, Poison).\n";
        std::cout << "They can be stacked: Fire + Ice + Poison.\n";
        std::cout << "The Sword (Core Item) stays the same.\n";

        HFL::WaitForInput();

        // --- STEP 2: THE PROBLEM (Class Explosion) ---
        HFL::ClearScreen();
        HFL::PrintHeader("Step 1: The Problem");

        std::cout << "To build a 'Fast Fire Sword' or a 'Slow Ice Sword'.\n";
        std::cout << "Using Inheritance, would bloat the classes:\n";
        std::cout << "   - class BasicSword\n";
        std::cout << "   - class FastSword : public BasicSword\n";
        std::cout << "   - class FastFireSword : public FastSword\n";
        std::cout << "   ... and so on.\n";
        std::cout << "Every combination would require another class.\n\n";

        HFL::WaitForInput();

        // --- STEP 3: THE DECORATOR SOLUTION ---
        HFL::ClearScreen();
        HFL::PrintHeader("Step 2: The Decorator Solution");

        std::cout << "Use 'Wrappers' (Decorators).\n";
        std::cout << "1. The 'Wrapper' (Enchantments) inherits from 'IWeapon'.\n";
        std::cout << "   It *IS-A* Weapon. It looks like a Sword to the Game.\n";
        std::cout << "2. Inside the Wrapper, it (Points to) a specific Sword.\n";
        std::cout << "3. When Game Logic calls 'GetDamage()', the Wrapper adds its modifier,\n";
        std::cout << "   then calls 'WrappedSword->GetDamage()'.\n\n";

        std::cout << "THE VISUAL:\n";
        std::cout << "   [Game] -> Calls GetDamage()\n";
        std::cout << "   [FireWrapper] -> Adds +20, Calls [Sword]->GetDamage()\n";
        std::cout << "   [IceWrapper]  -> Adds +10, Calls [Sword]->GetDamage()\n";

        HFL::WaitForInput();

        // --- STEP 4: INTERACTIVE SYSTEM ---
        HFL::ClearScreen();

        // 1. Create the Core Component (The Sword)
        std::cout << "\n[Setup] Forging 'Iron Sword' (Damage 10)...\n";
        // FIX: ActiveWeapon must be the unique_ptr to maintain the chain ownership
        std::unique_ptr<IWeapon> ActiveWeapon = std::make_unique<BasicSword>("Iron Sword", 10);

        // Tracks to prevent duplicate enchantments
        bool bHasFire = false;
        bool bHasIce = false;
        bool bHasPoison = false;

        HFL::WaitForInput();

        bool InDemo = true;
        while (InDemo)
        {
            HFL::ClearScreen();
            HFL::PrintHeader("RPG Enchantments");

            std::cout << "Active Weapon: " << ActiveWeapon->GetName() << " (Damage: " << ActiveWeapon->GetDamage() << ")\n\n";

            std::cout << "Select an Action:\n";
            std::cout << "1. Enchant 'Fire' (+20 Dmg)\n";
            std::cout << "2. Enchant 'Ice' (+10 Dmg)\n";
            std::cout << "3. Enchant 'Poison' (+15 Dmg)\n";
            std::cout << "0. Exit Demo\n";
            std::cout << "\nChoice: ";

            int Choice;
            std::cin >> Choice;

            if (std::cin.fail()) { std::cin.clear(); std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); continue; }

            if (Choice == 0) break;

            if (Choice >= 1 && Choice <= 3)
            {
                // Create a new Decorator wrapper.
                // Passing the 'ActiveWeapon' to it.

                if (Choice == 1)
                {
                    if (bHasFire)
                    {
                        std::cout << "\n>> [Error] Weapon already has Fire!\n";
                    }
                    else
                    {
                        std::cout << "\nApplying 'Fire' Enchantment...\n";
                        ActiveWeapon = std::make_unique<FireEnchantment>(std::move(ActiveWeapon));
                        bHasFire = true;
                        // Display change
                        std::cout << ">> [System] Enchantment Applied! New Name: " << ActiveWeapon->GetName() << "\n";
                        std::cout << ">> [System] New Damage: " << ActiveWeapon->GetDamage() << "\n";
                    }
                }
                else if (Choice == 2)
                {
                    if (bHasIce)
                    {
                        std::cout << "\n>> [Error] Weapon already has Ice!\n";
                    }
                    else
                    {
                        std::cout << "\nApplying 'Ice' Enchantment...\n";
                        ActiveWeapon = std::make_unique<IceEnchantment>(std::move(ActiveWeapon));
                        bHasIce = true;
                        // Display change
                        std::cout << ">> [System] Enchantment Applied! New Name: " << ActiveWeapon->GetName() << "\n";
                        std::cout << ">> [System] New Damage: " << ActiveWeapon->GetDamage() << "\n";
                    }
                }
                else if (Choice == 3)
                {
                    if (bHasPoison)
                    {
                        std::cout << "\n>> [Error] Weapon already has Poison!\n";
                    }
                    else
                    {
                        std::cout << "\nApplying 'Poison' Enchantment...\n";
                        ActiveWeapon = std::make_unique<PoisonEnchantment>(std::move(ActiveWeapon));
                        bHasPoison = true;
                        // Display change
                        std::cout << ">> [System] Enchantment Applied! New Name: " << ActiveWeapon->GetName() << "\n";
                        std::cout << ">> [System] New Damage: " << ActiveWeapon->GetDamage() << "\n";
                    }
                }

                HFL::WaitForInput();
            }
        }

        // --- STEP 5: CONCLUSION ---
        HFL::ClearScreen();
        HFL::PrintHeader("Conclusion");

        std::cout << "Summary:\n\n";
        std::cout << "1. Dynamic Extension:\n";
        std::cout << "   Can add 'Fire', 'Ice', and 'Poison' to the Sword.\n";
        std::cout << "   The 'BasicSword' class was never edited.\n\n";

        std::cout << "2. Stacking (Wrapping Wrappers):\n";
        std::cout << "   'Fire' wrapped 'Sword'.\n";
        std::cout << "   'Ice' wrapped 'FireSword'.\n";
        std::cout << "   The 'Ice' class doesn't need to know about Fire.\n";
        std::cout << "   It just points to what was passed to it.\n\n";

        std::cout << "3. Transparency:\n";
        std::cout << "   The Game Logic (Player) didn't know about the wrapping.\n";
        std::cout << "   It just calls 'GetDamage()'.\n\n";

        std::cout << std::setw(40) << "End of Demo\n";
        HFL::WaitForInput();
    }
}