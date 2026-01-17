#include "Behavioral/Memento.h"

namespace MEM
{
    // =========================================================================
    // 1. ORIGINATOR IMPLEMENTATION (Player)
    // =========================================================================

    Player::Player(PlayerState Baseline) : CurrentState(Baseline) {}

    void Player::TakeDamage(int Amount) { CurrentState.Health -= Amount; }
    void Player::UseMana(int Amount) { CurrentState.Mana -= Amount; }

    void Player::Move(int DistanceX, int DistanceY)
    {
        CurrentState.PosX += DistanceX;
        CurrentState.PosY += DistanceY;
    }

    void Player::SetBuff(std::string Icon) { CurrentState.ActiveBuff = Icon; }

    // --- POLICY-DRIVEN DELTA CAPTURE ---
    std::unique_ptr<PlayerDeltaMemento> Player::CaptureDelta(const PlayerState& Baseline)
    {
        auto Delta = std::make_unique<PlayerDeltaMemento>();

        if (CurrentState.Health != Baseline.Health) Delta->Health = CurrentState.Health;
        if (CurrentState.Mana != Baseline.Mana)   Delta->Mana = CurrentState.Mana;
        if (CurrentState.PosX != Baseline.PosX)   Delta->PosX = CurrentState.PosX;
        if (CurrentState.PosY != Baseline.PosY)   Delta->PosY = CurrentState.PosY;

        // --- SELECTIVE NON-PERSISTENCE (Policy) ---
        // Even if the Buff changed, it is NOT saved here.
        // This demonstrates that Mementos are not automatic mirrors,
        // but explicit snapshots of allowed volatile state.
        // (e.g., Level-specific buffs shouldn't travel to new levels).

        return Delta;
    }

    void Player::ApplyDelta(const PlayerDeltaMemento& Delta)
    {
        if (Delta.Health)     CurrentState.Health = *Delta.Health;
        if (Delta.Mana)       CurrentState.Mana = *Delta.Mana;
        if (Delta.PosX)       CurrentState.PosX = *Delta.PosX;
        if (Delta.PosY)       CurrentState.PosY = *Delta.PosY;
        if (Delta.ActiveBuff) CurrentState.ActiveBuff = *Delta.ActiveBuff;
    }

    void Player::DisplayStats() const
    {
        HFL::SetColor(HFL::EColor::BrightCyan);
        std::cout << "  [ Health: " << CurrentState.Health << " ] ";
        std::cout << "[ Mana: " << CurrentState.Mana << " ] ";
        std::cout << "[ Pos (" << CurrentState.PosX << ", " << CurrentState.PosY << ") ] ";
        std::cout << "[ Buff " << CurrentState.ActiveBuff << " ]\n";
        HFL::SetColor(HFL::EColor::White);
    }

    // =========================================================================
    // 2. DEMO IMPLEMENTATION
    // =========================================================================
    void RunDemo()
    {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        // --- STEP 1: INTRODUCTION ---
        HFL::ClearScreen();
        HFL::PrintHeader("Memento Pattern: Delta Memento");

        std::cout << "Definition:\n";
        std::cout << "Capture and externalize an object's internal state so it can be\n";
        std::cout << "restored later without breaking encapsulation.\n\n";

        std::cout << "The 'Delta' vs 'Regular' Memento:\n";
        std::cout << "- Regular: Saves a full copy. Overwrites everything on restore.\n";
        std::cout << "- Delta: Saves only 'Dirty' fields. The Baseline (Level Defaults)\n";
        std::cout << "  remains authoritative for anything not in the Delta.\n\n";

        std::cout << "In This Demo:\n";
        std::cout << "The Player travels between Level one and Level two. The Player\n";
        std::cout << "object is destroyed, but the 'PlayerInstance' caretaker\n";
        std::cout << "preserves their progress as an opaque memento.\n";

        HFL::WaitForInput();

        // --- STEP 2: THE ROLES ---
        HFL::ClearScreen();
        HFL::PrintHeader("The Architecture");

        std::cout << "1. The Originator (Player):\n";
        std::cout << "   - Defines the 'Capture' policy (What data travels?).\n\n";

        std::cout << "2. The Memento (PlayerDeltaMemento):\n";
        std::cout << "   - A package of 'std::optional' values. If a value is null,\n";
        std::cout << "     it means 'No change from baseline'.\n\n";

        std::cout << "3. The Caretaker (PlayerInstance):\n";
        std::cout << "   - Holds the Memento but CANNOT look inside it.\n";
        std::cout << "   - Lives through level reloads when the Player does not.\n";

        HFL::WaitForInput();

        // --- STEP 3: DEMO ---
        PlayerInstance SessionCaretaker;
        PlayerState L1_Baseline = { 100, 50, 0, 0, "None" };
        PlayerState L2_Baseline = { 100, 50, 500, 500, "None" };

        int CurrentLevel = 1;
        PlayerState* CurrentBaseline = &L1_Baseline;
        auto ActivePlayer = std::make_unique<Player>(*CurrentBaseline);

        while (true)
        {
            HFL::ClearScreen();
            HFL::PrintHeader("Location: Level " + std::to_string(CurrentLevel));

            std::cout << "Player State: " << CurrentBaseline->PosX << "," << CurrentBaseline->PosY << "\n";
            std::cout << "Current Stats:\n";
            ActivePlayer->DisplayStats();
            std::cout << "--------------------------------------------------\n\n";

            std::cout << "1. Change State (Persistent)\n";
            std::cout << "2. Cast Buff (Non-Persistent)\n";
            std::cout << "3. Change Level\n";
            std::cout << "0. Exit\n\n";

            int Choice = HFL::GetValidMenuInput(3);
            if (Choice == 0) break;

            if (Choice == 1)
            {
                ActivePlayer->TakeDamage(15);
                ActivePlayer->Move(20, 10);
                ActivePlayer->UseMana(10);
                std::cout << ">> Action: Modified Player State.\n";
            }
            else if (Choice == 2)
            {
                ActivePlayer->SetBuff("+5 Str");
                std::cout << ">> Action: Cast Strength Buff.\n";
            }
            else if (Choice == 3)
            {
                std::cout << "\n--- TRANSITION LOG ---\n";

                // 1. Capture
                auto NewDelta = ActivePlayer->CaptureDelta(*CurrentBaseline);

                // 2. Opacity & Efficiency Check
                if (NewDelta->IsEmpty())
                {
                    std::cout << ">> System: Delta is EMPTY. No changes from baseline detected.\n";
                    std::cout << ">> Performance: Saving 0 bytes (Snapshot skipped).\n";
                }
                else 
                {
                    std::cout << ">> System: Delta Captured (Health, Mana and Position).\n";
                    std::cout << ">> Performance: Saving only changed data.\n";
                }

                // 3. Caretaker Storage
                std::cout << ">> Caretaker: Storing opaque memento package...\n";
                SessionCaretaker.StoreDelta(std::move(NewDelta));

                // 4. Lifecycle
                ActivePlayer.reset();
                CurrentLevel = (CurrentLevel == 1) ? 2 : 1;
                CurrentBaseline = (CurrentLevel == 1) ? &L1_Baseline : &L2_Baseline;
                std::cout << ">> System: Loading Level " << CurrentLevel << " (Previous Player Deleted).\n";

                // 5. Restoration
                ActivePlayer = std::make_unique<Player>(*CurrentBaseline);
                const PlayerDeltaMemento* SavedData = SessionCaretaker.GetDelta();

                if (SavedData && !SavedData->IsEmpty()) 
                {
                    std::cout << ">> Caretaker: Handing memento to new Player (contents unknown).\n";
                    ActivePlayer->ApplyDelta(*SavedData);
                }
                else 
                {
                    std::cout << ">> System: No valid delta found. Using Level Baselines only.\n";
                }

                std::cout << "----------------------\n";
            }
            HFL::WaitForInput();
        }

        // --- STEP 4: CONCLUSION ---
        HFL::ClearScreen();
        HFL::PrintHeader("Conclusion: The Delta Advantage");

        std::cout << "1. Baseline Authority:\n";
        std::cout << "   The baseline is authoritative for non-volatile states. If it\n";
        std::cout << "   doesn't save a field, the NEW level's defaults take over. This\n";
        std::cout << "   prevents 'Ghost Data' from old levels corrupting new ones.\n\n";

        std::cout << "2. Policy Over Automation:\n";
        std::cout << "   The Buff [ +5 Str ] never traveled. The Memento is a policy\n";
        std::cout << "   contract, not raw memory.\n\n";

        std::cout << "3. Snapshot Cost vs. Delta Cost:\n";
        std::cout << "   - Full Snapshot: Overwrites everything.\n";
        std::cout << "   - Delta Snapshot: Only overwrites the 'Player's State'.\n\n";

        std::cout << std::setw(40) << "End of Demo\n";
        HFL::WaitForInput();
    }
}