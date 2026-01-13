#include "Behavioral/Observer.h"

namespace OBS
{
    // =========================================================================
    // SUBJECT LOGIC
    // =========================================================================

    void StatsComponent::TakeDamage(float Amount) {
        Health = std::max(0.0f, Health - Amount);
        OnHealthChanged.Broadcast(Health, MaxHealth);
    }

    void StatsComponent::Heal(float Amount) {
        Health = std::min(MaxHealth, Health + Amount);
        OnHealthChanged.Broadcast(Health, MaxHealth);
    }

    void StatsComponent::GainXP(float Amount) {
        CurrentXP += Amount;
        if (CurrentXP >= XPToLevel) {
            LevelUp();
        }
        else {
            OnXPChanged.Broadcast(CurrentXP, XPToLevel);
        }
    }

    void StatsComponent::LevelUp() {
        Level++;
        Health = MaxHealth; // Level up heals to full
        CurrentXP = 0.0f;   // Reset XP

        OnLevelUp.Broadcast(Level);
        OnHealthChanged.Broadcast(Health, MaxHealth);
        OnXPChanged.Broadcast(CurrentXP, XPToLevel);
    }

    // =========================================================================
    // OBSERVER LOGIC (UI Drawing)
    // =========================================================================

    void HUDWidget::UpdateHealth(float Current, float Max) {
        LastHealth = Current;
        LastMaxHealth = Max;
    }

    void HUDWidget::UpdateXP(float Current, float Max) {
        LastXP = Current;
        LastMaxXP = Max;
    }

    void HUDWidget::UpdateLevel(int NewLevel) {
        LastLevel = NewLevel;
        std::cout << "\n   [HUD EVENT] Level Up detected! Now Level " << NewLevel << "\n";
    }

    // FIXED: Added HUDWidget:: scope
    void HUDWidget::DrawHUD() {
        int Segments = 20;

        std::cout << "   Character Level: " << LastLevel << "\n";

        // Health Bar
        int HPFilled = static_cast<int>((LastHealth / LastMaxHealth) * Segments);
        std::cout << "   HP: [";
        for (int i = 0; i < Segments; ++i) std::cout << (i < HPFilled ? "|" : " ");
        std::cout << "] " << (int)LastHealth << "/" << (int)LastMaxHealth << "\n";

        // XP Bar
        int XPFilled = static_cast<int>((LastXP / LastMaxXP) * Segments);
        std::cout << "   XP: [";
        for (int i = 0; i < Segments; ++i) std::cout << (i < XPFilled ? "-" : " ");
        std::cout << "] " << (int)LastXP << "%\n";
    }

    // =========================================================================
    // DEMO IMPLEMENTATION
    // =========================================================================
    void RunDemo()
    {
        // Clear input buffer
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        // --- STEP 1: INTRODUCTION ---
        HFL::ClearScreen();
        HFL::PrintHeader("Observer Pattern (Delegates)");

        std::cout << "Definition:\n";
        std::cout << "Define a one-to-many dependency so that when one object changes state,\n";
        std::cout << "all its dependents are notified automatically.\n\n";

        std::cout << "In This Demo:\n";
        std::cout << "StatsComponent: The Subject (Source of Truth for Health/XP).\n";
        std::cout << "HUDWidget: The Observer (Draws bars based on notifications).\n";
        std::cout << "The StatsComponent has ZERO knowledge that the HUD exists.\n";

        HFL::WaitForInput();

        // --- STEP 2: THE ROLES ---
        HFL::ClearScreen();
        HFL::PrintHeader("Step 1: The Roles");

        std::cout << "There are three distinct roles in this system:\n\n";

        std::cout << "1. The Subject (StatsComponent):\n";
        std::cout << "   - Holds the 'Raw Data' (Health, XP, Level).\n";
        std::cout << "   - Owns the Broadcasters (Delegates).\n";
        std::cout << "   - It broadcasts: 'Health changed to 80!'\n\n";

        std::cout << "2. The Observer (HUDWidget):\n";
        std::cout << "   - Holds a 'Cache' of the data it needs to display.\n";
        std::cout << "   - It doesn't modify the data; it only listens and draws.\n\n";

        std::cout << "3. The Delegate (The Bridge):\n";
        std::cout << "   - Manages the list of listeners.\n";
        std::cout << "   - Uses weak_ptr to ensure that if the HUD is deleted,\n";
        std::cout << "     the system won't crash when broadcasting.\n";

        HFL::WaitForInput();

        // --- STEP 3: THE WIRING ---
        HFL::ClearScreen();
        HFL::PrintHeader("Step 2: The Wiring");

        std::cout << "Connecting the systems. This is the only time they meet.\n\n";

        // Setup lifetimes
        auto PlayerStats = std::make_shared<StatsComponent>();
        auto PlayerHUD = std::make_shared<HUDWidget>();

        std::cout << "ACTION: Binding HUD methods to Stats Delegates...\n";

        // THE BINDING Logic:
        // We tell the PlayerStats: "When Health changes, call PlayerHUD->UpdateHealth"
        PlayerStats->OnHealthChanged.Bind(PlayerHUD, &HUDWidget::UpdateHealth);
        PlayerStats->OnXPChanged.Bind(PlayerHUD, &HUDWidget::UpdateXP);
        PlayerStats->OnLevelUp.Bind(PlayerHUD, &HUDWidget::UpdateLevel);

        std::cout << "SUCCESS: Systems are now decoupled but communicating.\n";

        HFL::WaitForInput();

        // --- STEP 4: INTERACTIVE SIMULATION ---
        while (true)
        {
            HFL::ClearScreen();
            HFL::PrintHeader("Game HUD Simulation");

            // UI Section
            // Note: We don't ask PlayerStats for data here. 
            // We ask the HUD what it has 'observed'.
            std::cout << "--------------------------------------------------\n";
            PlayerHUD->DrawHUD();
            std::cout << "--------------------------------------------------\n\n";

            std::cout << "1. Take 20 Damage\n";
            std::cout << "2. Heal 20 Health\n";
            std::cout << "3. Gain 35 XP\n";
            std::cout << "0. Exit\n\n";

            int Choice = HFL::GetValidMenuInput(3);
            if (Choice == 0) break;

            std::cout << "\n--- BROADCAST LOG ---\n";

            if (Choice == 1) {
                std::cout << ">> Action: Triggering Damage logic in StatsComponent...\n";
                PlayerStats->TakeDamage(20.0f);
            }
            else if (Choice == 2) {
                std::cout << ">> Action: Triggering Heal logic in StatsComponent...\n";
                PlayerStats->Heal(20.0f);
            }
            else if (Choice == 3) {
                std::cout << ">> Action: Triggering XP Gain logic in StatsComponent...\n";
                PlayerStats->GainXP(35.0f);
            }

            std::cout << "---------------------\n";
            HFL::WaitForInput();
        }

        // --- STEP 5: CONCLUSION ---
        HFL::ClearScreen();
        HFL::PrintHeader("Conclusion");

        std::cout << "Summary of Observer Pattern:\n\n";

        std::cout << "1. Loose Coupling:\n";
        std::cout << "   The StatsComponent logic is 100% independent.\n";
        std::cout << "   You could add a Sound System or Achievement System\n";
        std::cout << "   listening to the same events without changing Stats.\n\n";



        std::cout << "2. Memory Safety:\n";
        std::cout << "   Because we used weak pointers in our Delegate Bindings,\n";
        std::cout << "   destroying the HUD object will not cause a null-pointer\n";
        std::cout << "   crash during the next broadcast.\n\n";

        std::cout << "3. Real-Time Sync:\n";
        std::cout << "   The UI is never 'stale'. It updates the exact millisecond\n";
        std::cout << "   the underlying data changes.\n\n";

        std::cout << std::setw(40) << "End of Demo\n";
        HFL::WaitForInput();
    }
}