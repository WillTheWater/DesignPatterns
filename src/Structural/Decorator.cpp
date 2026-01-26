#include "Structural/Decorator.h"

namespace DEC
{
    // =========================================================================
    // CONCRETE COMPONENT (The Core Item)
    // ROLE: The base object that provides the primary functionality.
    // =========================================================================

    BasicSword::BasicSword(std::string Name, int BaseDamage)
        : Name(Name), BaseDamage(BaseDamage)
    {
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "[System] Enchanting " << Name << " (Base Damage: " << BaseDamage << ")\n";
    }

    // =========================================================================
    // BASE DECORATOR (The Wrapper Contract)
    // ROLE: Standardizes how enchantments wrap weapons.
    // =========================================================================

    IEnchantment::IEnchantment(std::unique_ptr<IWeapon> RawItem)
        : WrappedItem(std::move(RawItem))
    {
    }

    // =========================================================================
    // CONCRETE DECORATORS (The Enchantments)
    // ROLE: Intercept calls to the wrapped object to inject additional logic.
    // =========================================================================

    // ======================== FIRE ENCHANTMENT (+20) ========================
    FireEnchantment::FireEnchantment(std::unique_ptr<IWeapon> RawItem)
        : IEnchantment(std::move(RawItem)) {
    }

    int FireEnchantment::GetDamage() const
    {
        HFL::SetColor(HFL::EColor::Red);
        std::cout << " [Enchantment] Adding Burn (+20)...\n";
        return WrappedItem->GetDamage() + 20;
    }

    // ======================== ICE ENCHANTMENT (+10) ========================
    IceEnchantment::IceEnchantment(std::unique_ptr<IWeapon> RawItem)
        : IEnchantment(std::move(RawItem)) {
    }

    int IceEnchantment::GetDamage() const
    {
        HFL::SetColor(HFL::EColor::Cyan);
        std::cout << " [Enchantment] Adding Frost (+10)...\n";
        return WrappedItem->GetDamage() + 10;
    }

    // ======================== POISON ENCHANTMENT (+15) ========================
    PoisonEnchantment::PoisonEnchantment(std::unique_ptr<IWeapon> RawItem)
        : IEnchantment(std::move(RawItem)) {
    }

    int PoisonEnchantment::GetDamage() const
    {
        HFL::SetColor(HFL::EColor::Green);
        std::cout << " [Enchantment] Adding Poison (+15)...\n";
        return WrappedItem->GetDamage() + 15;
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
        HFL::PrintHeader("DECORATOR DESIGN PATTERN");

        HFL::PrintSection("THE DEFINITION");
        HFL::SetColor(HFL::EColor::White);
        std::cout << "Attach additional responsibilities to an object dynamically.\n"
            << "Decorators provide a flexible alternative to sub-classing for\n"
            << "extending functionality.\n\n";

        HFL::PrintSection("THE GOAL");
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "The Decorator Pattern allows 'wraping' an object inside\n"
            << "another class that shares the same interface. This creates a recursive\n"
            << "structure where each layer adds its own unique behavior.\n\n";

        HFL::PrintSection("THE SCENARIO");
        HFL::SetColor(HFL::EColor::White);
        std::cout << "An RPG Equipment System with dynamic modifiers:\n\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] BASIC SWORD:     ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "The 'Leaf' or Core Item. It provides the base damage stats.\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] ENCHANTMENTS:    ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "The 'Wrappers'. They intercept calls to add elemental damage.\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] DYNAMIC STACK:   ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Enchantments can be applied in any order (Fire + Ice + Poison).\n\n";

        HFL::PrintSection("THE BENEFIT");
        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] OCP COMPLIANCE:  ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Classes are Open for extension but Closed for modification.\n";
        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] NO CLASS BLOAT:  ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Avoids creating 'FireSword', 'IceSword', and 'FireIceSword' classes.\n";
        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] RUNTIME AGILITY: ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Objects can be decorated at runtime based on player choices.\n";

        HFL::WaitForInput();

        // ======================== THE ARCHITECTURE ========================
        HFL::ClearScreen();
        HFL::PrintHeader("THE ARCHITECTURE");

        HFL::PrintSection("STRUCTURAL HIERARCHY");
        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] IWeapon (The Component)\n";
        HFL::SetColor(HFL::EColor::White);
        std::cout << "    INTERFACE:      ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "The common contract for both items and their wrappers.\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] BasicSword (The Concrete Component)\n";
        HFL::SetColor(HFL::EColor::White);
        std::cout << "    ROLE:           ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "The core object that eventually receives the final call.\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] IEnchantment (The Decorator)\n";
        HFL::SetColor(HFL::EColor::White);
        std::cout << "    COMPOSITION:    ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Contains a unique_ptr to the 'Wrapped' IWeapon instance.\n\n";

        HFL::PrintSection("DECORATION PROCESS");
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "When 'GetDamage()' is called on a multi-layered weapon stack:\n\n";

        HFL::SetColor(HFL::EColor::White);
        std::cout << "    [1] OUTER WRAPPER: ";
        HFL::SetColor(HFL::EColor::Red); std::cout << "FireEnchantment";
        HFL::SetColor(HFL::EColor::Gray); std::cout << " calculates +20.\n";

        HFL::SetColor(HFL::EColor::White);
        std::cout << "    [2] INNER WRAPPER: ";
        HFL::SetColor(HFL::EColor::Cyan); std::cout << "IceEnchantment";
        HFL::SetColor(HFL::EColor::Gray); std::cout << " calculates +10.\n";

        HFL::SetColor(HFL::EColor::White);
        std::cout << "    [3] CORE ITEM:     ";
        HFL::SetColor(HFL::EColor::Yellow); std::cout << "BasicSword";
        HFL::SetColor(HFL::EColor::Gray); std::cout << " returns base 10.\n\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "    >> RESULT: 40 Damage returned through the stack.\n";

        HFL::SetColor(HFL::EColor::White);
        HFL::WaitForInput();

        // ======================== INITIALIZATION ========================
        std::unique_ptr<IWeapon> ActiveWeapon = std::make_unique<BasicSword>("Iron Sword", 10);

        bool bHasFire = false, bHasIce = false, bHasPoison = false;

        while (true)
        {
            HFL::ClearScreen();
            HFL::PrintHeader("EQUIPMENT ENCHANTER");

            // ======================== CURRENT STATE ========================
            HFL::PrintSection("ACTIVE WEAPON STATUS");
            HFL::SetColor(HFL::EColor::White);
            std::cout << " NAME:   "; HFL::SetColor(HFL::EColor::Yellow); std::cout << ActiveWeapon->GetName() << "\n";

            int TotalDamage = ActiveWeapon->GetDamage();

            HFL::SetColor(HFL::EColor::White);
            std::cout << " ------------------------------------------\n";
            std::cout << " TOTAL DAMAGE: "; HFL::SetColor(HFL::EColor::Green); std::cout << TotalDamage << "\n\n";

            // ======================== COMMANDS ========================
            HFL::PrintSection("AVAILABLE ENCHANTMENTS");
            HFL::SetColor(HFL::EColor::Green);
            std::cout << " [1] "; HFL::SetColor(HFL::EColor::Red);   std::cout << "ADD FIRE\n";
            HFL::SetColor(HFL::EColor::Green);
            std::cout << " [2] "; HFL::SetColor(HFL::EColor::Cyan);  std::cout << "ADD ICE\n";
            HFL::SetColor(HFL::EColor::Green);
            std::cout << " [3] "; HFL::SetColor(HFL::EColor::Green); std::cout << "ADD POISON\n\n";
            HFL::SetColor(HFL::EColor::Green);
            std::cout << " [0] "; HFL::SetColor(HFL::EColor::White); std::cout << "CONTINUE\n\n";

            int Choice = HFL::GetValidMenuInput(3);
            if (Choice == 0) break;

            HFL::PrintSection("ENCHANTING");
            HFL::SetColor(HFL::EColor::Gray);

            if (Choice == 1 && !bHasFire)
            {
                std::cout << "Applying Fire essence to " << ActiveWeapon->GetName() << "..."; HFL::WaitDots(0.5);
                ActiveWeapon = std::make_unique<FireEnchantment>(std::move(ActiveWeapon));
                bHasFire = true;
            }
            else if (Choice == 2 && !bHasIce)
            {
                std::cout << "Applying Ice essence to " << ActiveWeapon->GetName() << "..."; HFL::WaitDots(0.5);
                ActiveWeapon = std::make_unique<IceEnchantment>(std::move(ActiveWeapon));
                bHasIce = true;
            }
            else if (Choice == 3 && !bHasPoison)
            {
                std::cout << "Applying Poison essence to " << ActiveWeapon->GetName() << "..."; HFL::WaitDots(0.5);
                ActiveWeapon = std::make_unique<PoisonEnchantment>(std::move(ActiveWeapon));
                bHasPoison = true;
            }
            else
            {
                HFL::SetColor(HFL::EColor::Red);
                std::cout << "!! Error: This enchantment is already bound to the weapon.\n";
            }

            HFL::WaitForInput();
        }

        // ======================== CONCLUSION ========================
        HFL::ClearScreen();
        HFL::PrintHeader("CONCLUSION");

        HFL::PrintSection("ARCHITECTURE");
        HFL::SetColor(HFL::EColor::White);
        std::cout << "The implementation of the Decorator Pattern confirms the following:\n\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] DYNAMIC EXTENSION: ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Modify the behavior of 'Iron Sword' at runtime\n"
            << "    without ever touching the BasicSword class code.\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] RECURSIVE COMPOSITION: ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "The decorators form a linked stack. Each layer delegates to the\n"
            << "    wrapped item, allowing for infinite elemental combinations.\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] INTERFACE TRANSPARENCY: ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "The client logic only knows about IWeapon. It is agnostic to\n"
            << "    whether it handles a raw sword or a 10-layer enchantment stack.\n\n";

        HFL::PrintSection("SUMMARY");
        HFL::SetColor(HFL::EColor::White);
        std::cout << "The Decorator Pattern ensures that software is:\n\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] FLEXIBLE:  ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Features can be toggled or stacked dynamically at runtime.\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] SCALABLE:  ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "New enchantments can be added without bloating the inheritance tree.\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] GRANULAR:  ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Complex objects are built by combining small, single-purpose wrappers\n"
            << "    rather than monolithic, multi-purpose subclasses.\n\n";

        HFL::SetColor(HFL::EColor::White);
        HFL::WaitForInput();
    }
}