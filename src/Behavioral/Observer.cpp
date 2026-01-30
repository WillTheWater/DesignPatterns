#include "Behavioral/Observer.h"

namespace OBS
{
    // =========================================================================
    // SUBJECT LOGIC (The Source of Truth)
    // ROLE: Perform internal state changes and broadcast results via Delegates.
    // =========================================================================

    void StatsComponent::TakeDamage(float Amount)
    {
        Health = std::max(0.0f, Health - Amount);

        std::cout << "[Subject] Health reduced. Broadcasting to listeners...\n";
        OnHealthChanged.Broadcast(Health, MaxHealth);
    }

    void StatsComponent::Heal(float Amount)
    {
        Health = std::min(MaxHealth, Health + Amount);

        std::cout << "[Subject] Health restored. Broadcasting to listeners...\n";
        OnHealthChanged.Broadcast(Health, MaxHealth);
    }

    void StatsComponent::GainXP(float Amount)
    {
        CurrentXP += Amount;
        if (CurrentXP >= XPToLevel)
        {
            LevelUp();
        }
        else
        {
            std::cout << "[Subject] XP Gained. Broadcasting to listeners...\n";
            OnXPChanged.Broadcast(CurrentXP, XPToLevel);
        }
    }

    void StatsComponent::LevelUp()
    {
        Level++;
        Health = MaxHealth; // Level up heals to full
        CurrentXP = 0.0f;   // Reset XP

        std::cout << "[Subject] LEVEL UP! Broadcasting multiple events...\n";
        OnLevelUp.Broadcast(Level);
        OnHealthChanged.Broadcast(Health, MaxHealth);
        OnXPChanged.Broadcast(CurrentXP, XPToLevel);
    }

    // =========================================================================
    // OBSERVER LOGIC (The UI Listener)
    // ROLE: Update a local cache of data to keep the UI in sync without 
    // constantly polling the Subject for information.
    // =========================================================================

    void HUDWidget::UpdateHealth(float Current, float Max)
    {
        LastHealth = Current;
        LastMaxHealth = Max;
        std::cout << "[Observer] HUD Cache updated: Health.\n";
    }

    void HUDWidget::UpdateXP(float Current, float Max)
    {
        LastXP = Current;
        LastMaxXP = Max;
        std::cout << "[Observer] HUD Cache updated: XP.\n";
    }

    void HUDWidget::UpdateLevel(int NewLevel)
    {
        LastLevel = NewLevel;
        HFL::SetColor(HFL::EColor::Cyan);
        std::cout << "[Observer] HUD EVENT: Level Up detected! Now Level " << NewLevel << "\n";
        HFL::SetColor(HFL::EColor::Gray);
    }

    void HUDWidget::DrawHUD()
    {
        int Segments = 20;

        std::cout << "   Character Level: ";
        HFL::SetColor(HFL::EColor::Green);
        std::cout << LastLevel << "\n";
        HFL::SetColor(HFL::EColor::White);

        // Health Bar
        int HPFilled = static_cast<int>((LastHealth / LastMaxHealth) * Segments);
        std::cout << "   HP: [";
        HFL::SetColor(HFL::EColor::BrightRed);
        for (int i = 0; i < Segments; ++i) std::cout << (i < HPFilled ? "|" : " ");
        HFL::SetColor(HFL::EColor::White);
        std::cout << "] " << (int)LastHealth << "/" << (int)LastMaxHealth << "\n";

        // XP Bar
        int XPFilled = static_cast<int>((LastXP / LastMaxXP) * Segments);
        std::cout << "   XP: [";
        HFL::SetColor(HFL::EColor::BrightYellow);
        for (int i = 0; i < Segments; ++i) std::cout << (i < XPFilled ? "-" : " ");
        HFL::SetColor(HFL::EColor::White);
        std::cout << "] " << (int)LastXP << "%\n";
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
        HFL::PrintHeader("OBSERVER DESIGN PATTERN");

        HFL::PrintSection("THE DEFINITION");
        HFL::SetColor(HFL::EColor::White);
        std::cout << "Define a one-to-many dependency between objects so that when one\n"
            << "object changes state, all its dependents are notified automatically.\n\n";

        HFL::PrintSection("THE GOAL");
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "The Observer Pattern is about 'Event-Driven Decoupling'.\n"
            << "It allows a 'Subject' to broadcast state changes to an unknown number\n"
            << "of 'Observers' (UI, VFX, SFX) without creating hard dependencies.\n"
            << "The Subject triggers an event; the Observers decide how to react.\n\n";

        HFL::PrintSection("THE EXAMPLE");
        HFL::SetColor(HFL::EColor::White);
        std::cout << "This demonstration features a Player Stats system and a decoupled HUD:\n\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] THE SUBJECT:  ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "StatsComponent. Source of truth for Health, XP, and Level.\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] THE OBSERVER: ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "HUDWidget. A passive listener that draws the visual interface.\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] THE DELEGATE: ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "The subscription manager that connects Subject to Observer.\n\n";

        HFL::PrintSection("THE BENEFIT");
        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] LOOSE COUPLING:  ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "The Player logic never needs to #include the UI or Sound headers.\n";
        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] MEMORY SAFETY:   ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Using weak_ptr prevents crashing if a listener is destroyed mid-game.\n";
        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] EXTENSIBILITY:   ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "New systems (Achievements, Analytics) can listen without changing code.\n";

        HFL::WaitForInput();

        // ======================== THE ARCHITECTURE ========================
        HFL::ClearScreen();
        HFL::PrintHeader("THE ARCHITECTURE");

        HFL::PrintSection("THE 'EVENT BRIDGE'");
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Without an Observer, the Player must manually tell the HUD to update.\n"
            << "With an Observer, the Player simply shouts 'I changed!' into a Delegate,\n"
            << "and whoever is subscribed handles the rest automatically.\n\n";

        HFL::PrintSection("IMPLEMENTATION");

        // ======================== THE SUBJECT ========================
        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] StatsComponent (The Subject)\n";
        HFL::SetColor(HFL::EColor::White);
        std::cout << "    ROLE:           ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "The Broadcaster. It owns the Delegates and triggers 'Broadcast()'.\n";
        HFL::SetColor(HFL::EColor::White);
        std::cout << "    KNOWLEDGE:      ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Zero. It does not know who (if anyone) is listening.\n\n";

        // ======================== THE OBSERVER ========================
        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] HUDWidget (The Observer)\n";
        HFL::SetColor(HFL::EColor::White);
        std::cout << "    ROLE:           ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "The Listener. It 'Binds' its functions to the Subject's Delegates.\n";
        HFL::SetColor(HFL::EColor::White);
        std::cout << "    LIFECYCLE:      ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Safe. It uses weak_ptr to ensure the Subject doesn't call a dead object.\n\n";

        HFL::SetColor(HFL::EColor::White);
        HFL::WaitForInput();

        // ======================== INITIALIZATION ========================
        auto PlayerStats = std::make_shared<StatsComponent>();
        auto PlayerHUD = std::make_shared<HUDWidget>();

        HFL::SetColor(HFL::EColor::White);
        std::cout << "ACTION: Binding HUD methods to Stats Delegates...\n";
        HFL::WaitDots(0.5f);

        PlayerStats->OnHealthChanged.Bind(PlayerHUD, &HUDWidget::UpdateHealth);
        PlayerStats->OnXPChanged.Bind(PlayerHUD, &HUDWidget::UpdateXP);
        PlayerStats->OnLevelUp.Bind(PlayerHUD, &HUDWidget::UpdateLevel);

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "SUCCESS: Systems are now decoupled but communicating.\n";
        HFL::SetColor(HFL::EColor::White);

        HFL::WaitForInput();

        // ======================== GAME LOOP ========================
        while (true)
        {
            HFL::ClearScreen();
            HFL::PrintHeader("GAME HUD SIMULATION");

            // UI Section: We ask the HUD what it has 'observed'.
            HFL::PrintSection("HUD RENDERING");
            PlayerHUD->DrawHUD();
            std::cout << "\n";

            HFL::PrintSection("COMMANDS");
            HFL::SetColor(HFL::EColor::Green);
            std::cout << " [1] "; HFL::SetColor(HFL::EColor::White); std::cout << "TAKE DAMAGE\n";
            HFL::SetColor(HFL::EColor::Green);
            std::cout << " [2] "; HFL::SetColor(HFL::EColor::White); std::cout << "HEAL HEALTH\n";
            HFL::SetColor(HFL::EColor::Green);
            std::cout << " [3] "; HFL::SetColor(HFL::EColor::White); std::cout << "GAIN XP\n\n";
            HFL::SetColor(HFL::EColor::Green);
            std::cout << " [0] "; HFL::SetColor(HFL::EColor::White); std::cout << "CONTINUE\n\n";

            int Choice = HFL::GetValidMenuInput(3);
            if (Choice == 0) break;

            HFL::PrintSection("BROADCAST LOG");
            HFL::SetColor(HFL::EColor::Gray);

            if (Choice == 1) {
                std::cout << ">> Triggering Damage logic in StatsComponent...\n";
                PlayerStats->TakeDamage(HFL::GetRandom(2.f, 20.f));
            }
            else if (Choice == 2) {
                std::cout << ">> Triggering Heal logic in StatsComponent...\n";
                PlayerStats->Heal(HFL::GetRandom(2.f, 20.f));
            }
            else if (Choice == 3) {
                std::cout << ">> Triggering XP Gain logic in StatsComponent...\n";
                PlayerStats->GainXP(HFL::GetRandom(5.f, 40.f));
            }

            HFL::SetColor(HFL::EColor::White);
            HFL::WaitForInput();
        }

        // ======================== CONCLUSION ========================
        HFL::ClearScreen();
        HFL::PrintHeader("CONCLUSION");

        HFL::PrintSection("ARCHITECTURE");
        HFL::SetColor(HFL::EColor::White);
        std::cout << "The implementation of the Observer Pattern confirms the following:\n\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] EVENT-DRIVEN DECOUPLING: ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "The 'StatsComponent' acts as a pure data source. It functions\n"
            << "    perfectly whether 0 or 100 observers are listening.\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] SMART SUBSCRIPTIONS:     ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "The 'Delegate' template manages the complexity of function pointers\n"
            << "    and argument passing, providing a clean API for the Subject.\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] ZERO HEADER POLLUTION:   ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "The gameplay logic remains pure. It has no knowledge of UI classes,\n"
            << "    rendering pipelines, or sound engine headers.\n\n";

        HFL::PrintSection("SUMMARY");
        HFL::SetColor(HFL::EColor::White);
        std::cout << "The Observer Pattern ensures that software is:\n\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] SCALABLE:  ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Add new features (Achievements, VFX) by simply binding to events.\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] SECURE:    ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Memory leaks and null-pointer crashes are mitigated via weak_ptr.\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] EFFICIENT: ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Observers only update when data actually changes, avoiding expensive\n"
            << "    per-frame 'polling' or checking variables in the Game Loop.\n\n";

        HFL::SetColor(HFL::EColor::White);
        HFL::WaitForInput();
    }
}