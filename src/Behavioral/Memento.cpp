#include "Behavioral/Memento.h"

namespace MEM
{
    // =========================================================================
    // ORIGINATOR IMPLEMENTATION (Player)
    // ROLE: The object whose state needs to be protected. It handles the 
    // internal logic for creating and consuming its own mementos.
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

    // ======================== POLICY-DRIVEN DELTA CAPTURE ========================
    std::unique_ptr<PlayerDeltaMemento> Player::CaptureDelta(const PlayerState& Baseline)
    {
        auto Delta = std::make_unique<PlayerDeltaMemento>();

        if (CurrentState.Health != Baseline.Health) Delta->Health = CurrentState.Health;
        if (CurrentState.Mana != Baseline.Mana)   Delta->Mana = CurrentState.Mana;
        if (CurrentState.PosX != Baseline.PosX)   Delta->PosX = CurrentState.PosX;
        if (CurrentState.PosY != Baseline.PosY)   Delta->PosY = CurrentState.PosY;

        // ======================== SELECTIVE NON-PERSISTENCE ========================
        // Even if the Buff changed, it is NOT saved here.
        // This demonstrates that Mementos are not automatic mirrors, but explicit 
        // snapshots of allowed volatile state (e.g., buffs shouldn't travel to new levels).
        // =========================================================================

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
    // DEMO IMPLEMENTATION
    // =========================================================================
    void RunDemo()
    {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        // ======================== INTRODUCTION ========================
        HFL::ClearScreen();
        HFL::PrintHeader("MEMENTO DESIGN PATTERN");

        HFL::PrintSection("THE DEFINITION");
        HFL::SetColor(HFL::EColor::White);
        std::cout << "Without violating encapsulation, capture and externalize an object's\n"
            << "internal state so that the object can be restored to this state later.\n\n";

        HFL::PrintSection("THE GOAL");
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "The Memento Pattern provides a 'Checkpoint' system. The Delta variant\n"
            << "optimizes this by only tracking fields that differ from a baseline state,\n"
            << "minimizing memory footprint while maintaining strict encapsulation.\n\n";

        HFL::PrintSection("THE EXAMPLE");
        HFL::SetColor(HFL::EColor::White);
        std::cout << "This demonstration simulates a Level Transition System with three roles:\n\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] THE ORIGINATOR:      ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "The Player object. It creates and consumes the mementos.\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] THE MEMENTO:        ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "The Delta package. An opaque box of optional state data.\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] THE CARETAKER:      ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "The PlayerInstance. It stores the box but cannot open it.\n\n";

        HFL::PrintSection("THE BENEFIT");
        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] ENCAPSULATION:   ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "The storage manager never sees the Player's private variables.\n";
        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] DELTA EFFICIENCY:";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Only 'dirty' fields are saved, reducing data overhead significantly.\n";
        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] LIFECYCLE SAFETY:";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "State survives object destruction during level streaming/loading.\n";

        HFL::WaitForInput();

        // ======================== THE ARCHITECTURE ========================
        HFL::ClearScreen();
        HFL::PrintHeader("THE ARCHITECTURE");

        HFL::PrintSection("THE 'RESTORE' PIPELINE");
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "In a Delta Memento system, it compares the current state against a known\n"
            << "Baseline (Level Defaults). Only the differences are packed into the Memento.\n"
            << "This allows new levels to define their own defaults while keeping player progress.\n\n";

        HFL::PrintSection("IMPLEMENTATION");

        // ======================== ROLES ========================
        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[1] THE ORIGINATOR (Player)\n";
        HFL::SetColor(HFL::EColor::White);
        std::cout << "    ROLE:           ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "The source of truth. It defines the 'Capture' and 'Apply' logic.\n";
        HFL::SetColor(HFL::EColor::White);
        std::cout << "    POLICY:         ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Decides which stats are persistent and which are level-specific.\n\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[2] THE MEMENTO (PlayerDeltaMemento)\n";
        HFL::SetColor(HFL::EColor::White);
        std::cout << "    ROLE:           ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "A lightweight data container using std::optional for null-checking.\n";
        HFL::SetColor(HFL::EColor::White);
        std::cout << "    VISIBILITY:     ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Opaque to the Caretaker; only accessible by the Originator.\n\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[3] THE CARETAKER (PlayerInstance)\n";
        HFL::SetColor(HFL::EColor::White);
        std::cout << "    ROLE:           ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "The Warehouse. It manages the lifecycle of the memento package.\n";
        HFL::SetColor(HFL::EColor::White);
        std::cout << "    PERSISTENCE:    ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Acts as a bridge across destruction boundaries (Level Unload).\n\n";

        HFL::SetColor(HFL::EColor::White);
        HFL::WaitForInput();

        // ======================== INITIALIZATION ========================
        PlayerInstance SessionCaretaker;
        PlayerState L1_Baseline = { 100, 50, 0, 0, "None" };
        PlayerState L2_Baseline = { 100, 50, 500, 500, "None" };

        int CurrentLevel = 1;
        PlayerState* CurrentBaseline = &L1_Baseline;
        auto ActivePlayer = std::make_unique<Player>(*CurrentBaseline);

        // ======================== GAME LOOP ========================
        while (true)
        {
            HFL::ClearScreen();
            HFL::PrintHeader("LOCATION: LEVEL " + std::to_string(CurrentLevel));

            HFL::PrintSection("CURRENT STATE");
            HFL::SetColor(HFL::EColor::Gray);
            std::cout << " Level Default Origin: [" << CurrentBaseline->PosX << "," << CurrentBaseline->PosY << "]\n";
            ActivePlayer->DisplayStats();

            HFL::PrintSection("COMMANDS");
            HFL::SetColor(HFL::EColor::Green);
            std::cout << " [1] "; HFL::SetColor(HFL::EColor::White); std::cout << "CHANGE STATE (Persistent)\n";
            HFL::SetColor(HFL::EColor::Green);
            std::cout << " [2] "; HFL::SetColor(HFL::EColor::White); std::cout << "CAST BUFF (Non-Persistent)\n";
            HFL::SetColor(HFL::EColor::Green);
            std::cout << " [3] "; HFL::SetColor(HFL::EColor::White); std::cout << "CHANGE LEVEL\n";
            HFL::SetColor(HFL::EColor::Green);
            std::cout << " [0] "; HFL::SetColor(HFL::EColor::White); std::cout << "CONTINUE\n\n";

            int Choice = HFL::GetValidMenuInput(3);
            if (Choice == 0) break;

            if (Choice == 1)
            {
                ActivePlayer->TakeDamage(15);
                ActivePlayer->Move(20, 10);
                ActivePlayer->UseMana(10);
                std::cout << ">> Action: Modified Health, Mana, and Position.\n";
            }
            else if (Choice == 2)
            {
                ActivePlayer->SetBuff("+5 Str");
                std::cout << ">> Action: Cast Strength Buff (Not in persistence policy).\n";
            }
            else if (Choice == 3)
            {
                HFL::PrintSection("TRANSITION LOG");
                HFL::SetColor(HFL::EColor::Gray);

                // Capture
                auto NewDelta = ActivePlayer->CaptureDelta(*CurrentBaseline);

                // Opacity & Efficiency Check
                if (NewDelta->IsEmpty())
                {
                    std::cout << "[System] Delta is EMPTY. No changes from baseline detected.\n";
                    std::cout << "[System] Performance: Snapshot skipped (0 bytes).\n";
                }
                else
                {
                    std::cout << "[System] Delta Captured (Health, Mana, and Position stored).\n";
                    std::cout << "[System] Performance: Saving only changed data.\n";
                }

                // Caretaker Storage
                std::cout << "[Caretaker] Storing opaque memento package...\n";
                SessionCaretaker.StoreDelta(std::move(NewDelta));

                // Lifecycle management
                ActivePlayer.reset();
                CurrentLevel = (CurrentLevel == 1) ? 2 : 1;
                CurrentBaseline = (CurrentLevel == 1) ? &L1_Baseline : &L2_Baseline;
                std::cout << "[System] Loading Level " << CurrentLevel << " (Previous Player destroyed).\n";

                // Restoration
                ActivePlayer = std::make_unique<Player>(*CurrentBaseline);
                const PlayerDeltaMemento* SavedData = SessionCaretaker.GetDelta();

                if (SavedData && !SavedData->IsEmpty())
                {
                    std::cout << "[Caretaker] Handing memento to new Player (contents unknown).\n";
                    ActivePlayer->ApplyDelta(*SavedData);
                }
                else
                {
                    std::cout << "[System] No valid delta found. Using Level Baselines only.\n";
                }
            }
            HFL::WaitForInput();
        }

        // ======================== CONCLUSION ========================
        HFL::ClearScreen();
        HFL::PrintHeader("CONCLUSION: THE MEMENTO PATTERN");

        HFL::PrintSection("ARCHITECTURE");
        HFL::SetColor(HFL::EColor::White);
        std::cout << "The implementation of the Delta Memento confirms the following:\n\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] BASELINE AUTHORITY:      ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "The baseline is authoritative for non-volatile states. If a field\n"
            << "    is not in the Delta, the NEW level's defaults take over. This\n"
            << "    prevents 'Ghost Data' from old levels corrupting new ones.\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] POLICY OVER AUTOMATION:  ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "The Memento is a strategic contract, not a raw memory dump.\n"
            << "    As seen with the Buff [+5 Str], it never traveled because the\n"
            << "    Originator's policy excluded it from the snapshot.\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] ENCAPSULATION INTEGRITY: ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "The Caretaker (PlayerInstance) manages the lifetime of the state\n"
            << "    without ever knowing what the PlayerState struct actually contains.\n\n";

        HFL::PrintSection("SUMMARY");
        HFL::SetColor(HFL::EColor::White);
        std::cout << "The Delta Memento ensures that software is:\n\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] EFFICIENT: ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Only 'dirty' (changed) fields are stored, reducing I/O and memory.\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] ROBUST:    ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Object state survives destruction boundaries like level loads.\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] FLEXIBLE:  ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Restoration is additive; it only patches what is necessary,\n"
            << "               leaving level-specific defaults untouched.\n\n";

        HFL::SetColor(HFL::EColor::White);
        HFL::WaitForInput();
    }
}