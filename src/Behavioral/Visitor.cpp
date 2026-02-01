#include "Behavioral/Visitor.h"
#include <iomanip>

namespace VIS
{
    void Player::Accept(IAttackVisitor& Visitor) { Visitor.Visit(*this); }
    void Enemy::Accept(IAttackVisitor& Visitor) { Visitor.Visit(*this); }

    // =========================================================================
    // VISITOR LOGIC: PHYSICAL ATTACK (Logical Output)
    // ROLE: Implements the physics-based damage formula.
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
            std::cout << "  - Critical:       " << (int)Roll << " * " << Multiplier << " Multiplier\n";
        }

        std::cout << "  - Raw Damage:     " << Raw << "\n";
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
            std::cout << "  - Critical:       " << (int)Roll << " * " << Multiplier << " Multiplier\n";
        }

        std::cout << "  - Raw Damage:     " << Raw << "\n";
        std::cout << "  - Mitigation:    -" << DefenseScale << " (Armor Pierced)\n";

        HFL::SetColor(HFL::EColor::BrightRed);
        std::cout << "  >> Final Damage: " << Final << " dealt to " << Target.GetName() << "\n";
        HFL::SetColor(HFL::EColor::White);

        Target.ApplyDamage(Final);
    }

    // =========================================================================
    // VISITOR LOGIC: MAGIC ATTACK
    // ROLE: Implements elemental/arcane damage logic.
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
        std::cout << "  >> Result:      " << Final << " Damage dealt to " << Target.GetName() << "\n";
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
        std::cout << "  >> Result:      " << Final << " Damage dealt to " << Target.GetName() << "\n";
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

        // ======================== INTRODUCTION ========================
        HFL::ClearScreen();
        HFL::PrintHeader("VISITOR DESIGN PATTERN");

        HFL::PrintSection("THE DEFINITION");
        HFL::SetColor(HFL::EColor::White);
        std::cout << "Represent an operation to be performed on the elements of an object structure.\n"
            << "Visitor allows defining new operations without changing the classes\n"
            << "of the elements on which it operates.\n\n";

        HFL::PrintSection("THE GOAL");
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "The Visitor Pattern is about 'Separating Algorithm from Data Structure'.\n"
            << "Targets (Player/Enemy) are passive containers holding stats, while\n"
            << "Attacks (Physical/Magic) are active Visitors containing the formulas.\n"
            << "This makes adding new damage types easier without touching core entities.\n\n";

        HFL::PrintSection("THE EXAMPLE");
        HFL::SetColor(HFL::EColor::White);
        std::cout << "This demonstration features a Combat System utilizing Double Dispatch:\n\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] THE ELEMENT:         ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Player and Enemy. They 'Accept' visitors to initiate logic.\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] THE VISITOR:         ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "The Attack logic. It 'Visits' concrete types to apply math.\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] DOUBLE DISPATCH:     ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Resolves the correct formula based on both Attack AND Target types.\n\n";

        HFL::PrintSection("THE BENEFIT");
        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] EXTENSIBILITY:   ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Adding 'Poison' or 'True' damage requires zero changes to Player/Enemy.\n";
        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] CLEAN DATA:      ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "The Player class remains lean, focusing only on state management.\n";
        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] CONTEXTUAL MATH: ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Magic can behave differently against Orcs than against Players automatically.\n";

        HFL::WaitForInput();

        // ======================== THE ARCHITECTURE ========================
        HFL::ClearScreen();
        HFL::PrintHeader("THE ARCHITECTURE");

        HFL::PrintSection("DOUBLE DISPATCH MECHANICS");
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "The system uses two virtual calls: first, the Target's 'Accept' method\n"
            << "is called. The Target then calls the Visitor's 'Visit' method, passing\n"
            << "itself (*this) to ensure the specific concrete type is recognized.\n\n";

        HFL::PrintSection("IMPLEMENTATION");

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] The Visitor (IAttackVisitor)\n";
        HFL::SetColor(HFL::EColor::White);
        std::cout << "    ROLE:           ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Defines the interface for operations. Holds the combat math.\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] The Elements (Player & Enemy)\n";
        HFL::SetColor(HFL::EColor::White);
        std::cout << "    ROLE:           ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "The Data. They grant Visitors permission to access their stats.\n\n";

        HFL::SetColor(HFL::EColor::White);
        HFL::WaitForInput();

        // ======================== INITIALIZATION ========================
        auto Hero = std::make_unique<Player>(10, 15, 0.15f);
        auto Orc = std::make_unique<Enemy>("Orc", 8, 45, 0.05f, 150);
        int OrcsSlain = 0;

        // ======================== Game LOOP ========================
        while (true)
        {
            HFL::ClearScreen();
            HFL::PrintHeader("ACTIVE COMBAT - SLAIN: " + std::to_string(OrcsSlain));

            // ======================== DATA DISPLAY ========================
            HFL::PrintSection("BATTLEFIELD STATUS");
            HFL::SetColor(HFL::EColor::BrightCyan);
            std::cout << " [ PLAYER ] " << Hero->GetName() << " [Lvl " << Hero->GetLevel() << "]\n";
            std::cout << "    HP: " << Hero->GetHealth() << " | Def: " << Hero->GetDefense() << " | Res: " << Hero->GetResistance() << "\n\n";

            HFL::SetColor(HFL::EColor::BrightRed);
            std::cout << " [ ENEMY  ] " << Orc->GetName() << " [Lvl " << Orc->GetLevel() << "]\n";
            std::cout << "    HP: " << Orc->GetHealth() << " | Def: " << Orc->GetDefense() << " | Res: " << Orc->GetResistance() << "\n";
            HFL::SetColor(HFL::EColor::Gray);
            std::cout << " --------------------------------------------------\n\n";

            HFL::PrintSection("CHOOSE ACTION");
            HFL::SetColor(HFL::EColor::Green);
            std::cout << " [1] "; HFL::SetColor(HFL::EColor::White); std::cout << "HEAVY SWING  (High Power | Low Crit)\n";
            HFL::SetColor(HFL::EColor::Green);
            std::cout << " [2] "; HFL::SetColor(HFL::EColor::White); std::cout << "QUICK STAB   (Low Power  | High Crit)\n";
            HFL::SetColor(HFL::EColor::Green);
            std::cout << " [3] "; HFL::SetColor(HFL::EColor::White); std::cout << "ARCANE BLAST (Bypasses Armor)\n";
            HFL::SetColor(HFL::EColor::Green);
            std::cout << " [0] "; HFL::SetColor(HFL::EColor::White); std::cout << "CONTINUE\n\n";

            int Choice = HFL::GetValidMenuInput(3);
            if (Choice == 0) break;

            HFL::ClearScreen();
            HFL::PrintSection("COMBAT LOG");
            HFL::SetColor(HFL::EColor::Gray);

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
            }
            else
            {
                std::cout << "\n >> The Orc strikes back!\n";
                PhysicalAttack EnemyBash(HFL::GetRandom(45, 65), { 0.15f, 1.5f, 0.1f });
                Hero->Accept(EnemyBash);
            }

            if (Hero->IsDead())
            {
                HFL::SetColor(HFL::EColor::BrightRed);
                std::cout << "\n >> PLAYER DIED AFTER SLAYING " << OrcsSlain << " ENEMIES.\n";
                HFL::SetColor(HFL::EColor::White);
                std::cout << " [1] Respawn\n";
                std::cout << " [2] End Session\n";

                if (HFL::GetValidMenuInput(2) == 1) {
                    Hero = std::make_unique<Player>(10, 15, 0.15f);
                    std::cout << " >> A new Hero arises!\n";
                }
                else break;
            }

            HFL::SetColor(HFL::EColor::Gray);
            std::cout << " --------------------\n";
            HFL::WaitForInput();
        }

        // ======================== CONCLUSION ========================
        HFL::ClearScreen();
        HFL::PrintHeader("CONCLUSION");

        HFL::PrintSection("ARCHITECTURE");
        HFL::SetColor(HFL::EColor::White);
        std::cout << "The implementation of the Visitor Pattern confirms the following:\n\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] OPEN/CLOSED PRINCIPLE:   ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "The Target classes are 'Closed' for modification but the system\n"
            << "    is 'Open' for new operations (Attacks) without changing core code.\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] CLEAN DATA CONTAINERS:   ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Player and Enemy stay 'Lean'. They only manage state and stats,\n"
            << "    leaving complex combat math to specialized Visitor classes.\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] DOUBLE DISPATCH POWER:   ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "The system automatically resolves the correct math formula\n"
            << "    based on both the Attack type and the Target type contextually.\n\n";

        HFL::PrintSection("SUMMARY");
        HFL::SetColor(HFL::EColor::White);
        std::cout << "The Visitor Pattern ensures that software is:\n\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] EXTENSIBLE:";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << " Add new behaviors as external classes.\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] DECOUPLED: ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << " Data structures and logic stay completely separate.\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] CONTEXTUAL:";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << " Logic can behave differently based on specific concrete types.\n\n";

        HFL::SetColor(HFL::EColor::White);
        HFL::WaitForInput();
    }
}