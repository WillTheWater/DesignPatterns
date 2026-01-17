#include "Behavioral/Visitor.h"
#include <iomanip>

namespace VIS
{
    void Player::Accept(IAttackVisitor& Visitor) { Visitor.Visit(*this); }
    void Enemy::Accept(IAttackVisitor& Visitor) { Visitor.Visit(*this); }

    // =========================================================================
     // VISITOR LOGIC: PHYSICAL ATTACK (Logical Output)
     // =========================================================================
    void PhysicalAttack::Visit(Player& Target)
    {
        float Range = BasePower * Ctx.Variance;
        float Roll = HFL::GetRandom((float)BasePower - Range, (float)BasePower + Range);

        bool bIsCrit = HFL::GetRandom(0.0f, 1.0f) <= Ctx.CritChance;
        float Multiplier = bIsCrit ? Ctx.CritMultiplier : 1.00f;

        int Raw = static_cast<int>(Roll * Multiplier);
        int DefenseScale = Target.GetDefense() + (Target.GetLevel() * 1);
        int Final = std::max(5, Raw - DefenseScale);

        std::cout << (bIsCrit ? " CRITICAL HIT! " : " PHYSICAL STRIKE ") << "\n";
        std::cout << "  - Base Power:    " << BasePower << " (+/- " << (int)(Ctx.Variance * 100) << "% Variance)\n";
        std::cout << "  - Attack Roll:   " << (int)Roll << "\n";

        if (bIsCrit)
        {
            std::cout << "  - Critical:      " << (int)Roll << " * " << Multiplier << " Multiplier\n";
        }

        std::cout << "  - Raw Damage:    " << Raw << "\n";
        std::cout << "  - Mitigation:    -" << DefenseScale << " (Defense " << Target.GetDefense() << " + Lvl " << Target.GetLevel() << ")\n";

        HFL::SetColor(HFL::EColor::BrightRed);
        std::cout << "  >> Final Damage: " << Final << " dealt to " << Target.GetName() << "\n";
        HFL::SetColor(HFL::EColor::White);

        Target.ApplyDamage(Final);
    }

    void PhysicalAttack::Visit(Enemy& Target)
    {
        float Range = BasePower * Ctx.Variance;
        float Roll = HFL::GetRandom((float)BasePower - Range, (float)BasePower + Range);

        bool bIsCrit = HFL::GetRandom(0.0f, 1.0f) <= Ctx.CritChance;
        float Multiplier = bIsCrit ? Ctx.CritMultiplier : 1.00f;

        int Raw = static_cast<int>(Roll * Multiplier);
        int DefenseScale = Target.GetDefense() / 3;
        int Final = std::max(1, Raw - DefenseScale);

        std::cout << (bIsCrit ? " CRITICAL HIT! " : " PHYSICAL STRIKE ") << "\n";
        std::cout << "  - Base Power:    " << BasePower << " (+/- " << (int)(Ctx.Variance * 100) << "% Variance)\n";
        std::cout << "  - Attack Roll:   " << (int)Roll << "\n";

        if (bIsCrit)
        {
            std::cout << "  - Critical:      " << (int)Roll << " * " << Multiplier << " Multiplier\n";
        }

        std::cout << "  - Raw Damage:    " << Raw << "\n";
        std::cout << "  - Mitigation:    -" << DefenseScale << " (Armor Pierced)\n";

        HFL::SetColor(HFL::EColor::BrightRed);
        std::cout << "  >> Final Damage: " << Final << " dealt to " << Target.GetName() << "\n";
        HFL::SetColor(HFL::EColor::White);

        Target.ApplyDamage(Final);
    }

    // =========================================================================
    // VISITOR LOGIC: MAGIC ATTACK
    // =========================================================================
    void MagicAttack::Visit(Player& Target)
    {
        float Roll = HFL::GetRandom((float)BasePower * 0.9f, (float)BasePower * 1.1f);
        float Resist = Target.GetResistance();
        int Final = static_cast<int>(Roll * (1.0f - Resist));

        std::cout << " ARCANE BLAST \n";
        std::cout << "  - Base Power:  " << BasePower << " (Roll: " << (int)Roll << ")\n";
        std::cout << "  - Raw Damage:  " << (int)Roll << " [Magic]\n";
        std::cout << "  - Mitigation:  " << (int)(Resist * 100) << "% [Resistance]\n";

        HFL::SetColor(HFL::EColor::BrightCyan);
        std::cout << "  >> Result:     " << Final << " Damage dealt to " << Target.GetName() << "\n";
        HFL::SetColor(HFL::EColor::White);

        Target.ApplyDamage(Final);
    }

    void MagicAttack::Visit(Enemy& Target)
    {
        int Final = static_cast<int>(BasePower * 1.5f);

        std::cout << " ARCANE BLAST \n";
        std::cout << "  - Base Power:  " << BasePower << "\n";
        std::cout << "  - Raw Damage:  " << Final << " [Elemental Weakness]\n";
        std::cout << "  - Mitigation:  0 [Bypassed Defense]\n";

        HFL::SetColor(HFL::EColor::BrightCyan);
        std::cout << "  >> Result:     " << Final << " Damage dealt to " << Target.GetName() << "\n";
        HFL::SetColor(HFL::EColor::White);

        Target.ApplyDamage(Final);
    }

    // =========================================================================
    // DEMO IMPLEMENTATION
    // =========================================================================
    void RunDemo()
    {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        // --- STEP 1: INTRODUCTION ---
        HFL::ClearScreen();
        HFL::PrintHeader("Visitor Pattern: Combat Visitors");

        std::cout << "Definition:\n";
        std::cout << "Represent an operation to be performed on the elements of an object structure.\n";
        std::cout << "Lets you define a new operation without changing the classes of the elements\n";
        std::cout << "on which it operates.\n\n";

        std::cout << "In This Demo:\n";
        std::cout << "Targets (Player/Enemy) are 'Passive' data containers holding stats.\n";
        std::cout << "Attacks (Physical/Magic) are 'Active' Visitors containing math formulas.\n";
        std::cout << "The Target 'Accepts' an Attack, which then executes the correct calculation\n";
        std::cout << "based on the specific type of both the Attack and the Target.\n";

        HFL::WaitForInput();

        // --- STEP 2: THE ROLES ---
        HFL::ClearScreen();
        HFL::PrintHeader("The Architecture");

        std::cout << "1. The Visitor (IAttackVisitor):\n";
        std::cout << "   - Defines the interface for the operations (Visit functions).\n";
        std::cout << "   - PhysicalAttack and MagicAttack implement the actual RPG math.\n\n";

        std::cout << "2. The Elements (Player & Enemy):\n";
        std::cout << "   - Contain an 'Accept' method that takes a Visitor.\n";
        std::cout << "   - They do not know HOW damage is calculated; they only provide\n";
        std::cout << "     their stats to the Visitor when asked.\n\n";

        std::cout << "3. Double Dispatch:\n";
        std::cout << "   - The 'Accept' call triggers the correct logic at runtime.\n";
        std::cout << "   - Adding a 'Poison' or 'Lifesteal' attack requires zero changes\n";
        std::cout << "     to the Player or Enemy classes.\n\n";

        HFL::WaitForInput();

        auto Hero = std::make_unique<Player>(10, 15, 0.15f);
        auto Orc = std::make_unique<Enemy>("Orc", 8, 45, 0.05f, 150);
        int OrcsSlain = 0;

        while (true)
        {
            HFL::ClearScreen();
            HFL::PrintHeader("Active Combat - Slain: " + std::to_string(OrcsSlain));

            // -- STATUS DISPLAY --
            HFL::SetColor(HFL::EColor::BrightCyan);
            std::cout << " [ PLAYER ] " << Hero->GetName() << " [Lvl " << Hero->GetLevel() << "]\n";
            std::cout << "    HP: " << Hero->GetHealth() << " | Def: " << Hero->GetDefense() << " | Res: " << Hero->GetResistance() << "\n\n";

            HFL::SetColor(HFL::EColor::BrightRed);
            std::cout << " [ ENEMY  ] " << Orc->GetName() << " [Lvl " << Orc->GetLevel() << "]\n";
            std::cout << "    HP: " << Orc->GetHealth() << " | Def: " << Orc->GetDefense() << " | Res: " << Orc->GetResistance() << "\n";
            HFL::SetColor(HFL::EColor::White);
            std::cout << "--------------------------------------------------\n\n";

            std::cout << "Choose Action:\n";
            std::cout << " 1. Heavy Swing   (High Power | Low Crit)\n";
            std::cout << " 2. Quick Stab    (Low Power  | High Crit)\n";
            std::cout << " 3. Arcane Blast  (Bypasses Armor)\n";
            std::cout << " 0. Continue\n\n";

            int Choice = HFL::GetValidMenuInput(3);
            if (Choice == 0) break;
            HFL::ClearScreen();

            std::cout << "\n --- COMBAT LOG ---\n";

            // Player Turn
            if (Choice == 1) {
                PhysicalAttack Heavy(HFL::GetRandom(55, 75), { 0.10f, 2.0f, 0.2f });
                Orc->Accept(Heavy);
            }
            else if (Choice == 2) {
                PhysicalAttack Dagger(HFL::GetRandom(30, 40), { 0.50f, 3.5f, 0.05f });
                Orc->Accept(Dagger);
            }
            else if (Choice == 3) {
                MagicAttack Spell(HFL::GetRandom(45, 55), { 0.0f, 1.0f, 0.15f });
                Orc->Accept(Spell);
            }

            // Orc Death Handling
            if (Orc->IsDead())
            {
                HFL::SetColor(HFL::EColor::Yellow);
                std::cout << "\n >> " << Orc->GetName() << " vanquished!\n";
                OrcsSlain++;
                HFL::SetColor(HFL::EColor::White);
                HFL::WaitForInput();

                HFL::SetColor(HFL::EColor::Yellow);
                std::cout << " >> Respawning stronger enemy...\n";
                Orc = std::make_unique<Enemy>("Orc", HFL::GetRandom(8, 15), HFL::GetRandom(30, 50), 0.05f, 150 + (OrcsSlain * 20));
                HFL::SetColor(HFL::EColor::White);
            }
            else
            {
                // Enemy Turn
                std::cout << "\n >> The Orc strikes back!\n";
                PhysicalAttack EnemyBash(HFL::GetRandom(45, 65), { 0.15f, 1.5f, 0.1f });
                Hero->Accept(EnemyBash);
            }

            // Player Death Handling
            if (Hero->IsDead())
            {
                HFL::SetColor(HFL::EColor::BrightRed);
                std::cout << "\n >> PLAYER DIED AFTER SLAYING " << OrcsSlain << " ENEMIES.\n";
                HFL::SetColor(HFL::EColor::White);
                std::cout << " 1. Respawn\n";
                std::cout << " 2. End Session\n";

                if (HFL::GetValidMenuInput(2) == 1) {
                    Hero = std::make_unique<Player>(10, 15, 0.15f);
                    std::cout << " >> A new Hero arises!\n";
                }
                else break;
            }

            std::cout << " --------------------\n";
            HFL::WaitForInput();
        }

        // --- STEP 4: CONCLUSION ---
        HFL::ClearScreen();
        HFL::PrintHeader("Conclusion: The Visitor Pattern");

        std::cout << "1. Open/Closed Principle:\n";
        std::cout << "   The Target classes (Player/Enemy) are 'Closed' for modification\n";
        std::cout << "   but the system is 'Open' for new operations. We can add 'Lava'\n";
        std::cout << "   or 'Ice' damage without touching the Target code.\n\n";

        std::cout << "2. Clean Data Containers:\n";
        std::cout << "   The Player and Enemy classes stay 'Lean'. They only manage\n";
        std::cout << "   health and stats, leaving complex combat math to specialized\n";
        std::cout << "   Visitor classes.\n\n";

        std::cout << "3. Double Dispatch Power:\n";
        std::cout << "   The system automatically resolves the correct math formula\n";
        std::cout << "   based on both the Attack type and the Target type, allowing\n";
        std::cout << "   for deep contextual interactions (like Magic vs. Orcs).\n\n";

        std::cout << std::setw(40) << "End of Demo\n";
        HFL::WaitForInput();
    }
}