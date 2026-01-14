#include "Behavioral/State.h"

namespace STA
{
    // =========================================================================
    // CONTEXT IMPLEMENTATION (SILENT)
    // =========================================================================

    Player::Player() 
    {
        // Start in the Idle state - No log here
        CurrentState = std::make_shared<IdleState>();
        CurrentState->Enter(*this);
    }

    void Player::SetState(std::shared_ptr<IPlayerState> NewState) 
    {
        if (CurrentState) 
        {
            CurrentState->Exit(*this);
        }

        CurrentState = NewState;
        CurrentState->Enter(*this);
    }

    void Player::HandleInput(int Input) 
    {
        CurrentState->HandleInput(*this, Input);
    }

    void Player::Update() 
    {
        CurrentState->Update(*this);
    }

    // =========================================================================
    // STATE LOGIC: IDLE
    // =========================================================================

    void IdleState::Enter(Player& P) { P;/* Silent */ }
    void IdleState::Update(Player& P) { P; std::cout << "   [Anim] Character is standing idle...\n"; }
    void IdleState::Exit(Player& P) { P;/* Silent */ }

    void IdleState::HandleInput(Player& P, int Input) 
    {
        if (Input == 1) P.SetState(std::make_shared<WalkState>());
        if (Input == 3) P.SetState(std::make_shared<AttackState>());
        if (Input == 4) P.SetState(std::make_shared<JumpState>());
    }

    // =========================================================================
    // STATE LOGIC: WALK
    // =========================================================================

    void WalkState::Enter(Player& P) { P;/* Silent */ }
    void WalkState::Update(Player& P) { P; std::cout << "   [Anim] Character is walking forward...\n"; }
    void WalkState::Exit(Player& P) { P;/* Silent */ }

    void WalkState::HandleInput(Player& P, int Input) 
    {
        if (Input == 2) P.SetState(std::make_shared<IdleState>());
        if (Input == 3) P.SetState(std::make_shared<AttackState>());
        if (Input == 4) P.SetState(std::make_shared<JumpState>());
    }

    // =========================================================================
    // STATE LOGIC: ATTACK (Self-Transitioning)
    // =========================================================================

    void AttackState::Enter(Player& P) 
    {
        // 1. Immediate Visual Feedback
        HFL::ClearScreen();
        HFL::PrintHeader("Animation State Machine");
        std::cout << "CURRENT STATE: [ ATTACKING ]\n";
        std::cout << "--------------------------------------------------\n";
        std::cout << "   (Anim) Character swings swoard!\n";

        // 2. Real-Time Wait
        HFL::Wait(1.5f);

        std::cout << "   (Combat) The enemy is hit!\n";
        std::cout << "   (Combat) The monster flees...\n";
        HFL::Wait(2.0f);

        // 3. AUTO-TRANSITION
        // This effectively "pops" the state back to Idle before the loop continues
        P.SetState(std::make_shared<IdleState>());
    }

    void AttackState::Update(Player& P) { P;/* Ignore */ }
    void AttackState::Exit(Player& P) { P;/* Ignore */}
    void AttackState::HandleInput(Player& P, int Input) { P; Input;/* Ignore */ }

    // =========================================================================
    // STATE LOGIC: JUMP (Self-Transitioning)
    // =========================================================================

    void JumpState::Enter(Player& P)
    {
        // 1. Immediate Visual Feedback
        HFL::ClearScreen();
        HFL::PrintHeader("Animation State Machine");
        std::cout << "CURRENT STATE: [ JUMPING ]\n";
        std::cout << "--------------------------------------------------\n";
        std::cout << "   (Anim) Character jumps!\n";

        // 2. Real-Time Wait
        HFL::Wait(1.f);

        std::cout << "   (Anim) Character lands!\n";
        std::cout << "   And recovers...\n";
        HFL::Wait(2.0f);

        // 3. AUTO-TRANSITION
        P.SetState(std::make_shared<IdleState>());
    }

    void JumpState::Update(Player& P) { P;/* Ignore */ }
    void JumpState::Exit(Player& P) { P;/* Ignore */ }
    void JumpState::HandleInput(Player& P, int Input) { P; Input;/* Ignore */ }

    // =========================================================================
    // DEMO MAIN
    // =========================================================================
    void RunDemo()
    {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        // --- STEP 1: INTRODUCTION ---
        HFL::ClearScreen();
        HFL::PrintHeader("State Pattern");

        std::cout << "Definition:\n";
        std::cout << "Allow an object to alter its behavior when its internal state changes.\n";
        std::cout << "The object will appear to change its class.\n\n";

        std::cout << "In This Demo:\n";
        std::cout << "It simulates a Character with 3 (States):\n";
        std::cout << "- Idle: Waiting for input.\n";
        std::cout << "- Walking: Moving logic enabled.\n";
        std::cout << "- Attacking: Inputs locked until animation finishes.\n";

        HFL::WaitForInput();

        // --- STEP 2: THE BRAIN SWAP ---
        HFL::ClearScreen();
        HFL::PrintHeader("Step 1: The Transition Logic");

        std::cout << "Unlike a massive Switch statement, transitions are handled BY states.\n\n";
        std::cout << "1. IdleState detects 'Move' -> It tells Player: 'SetState(Walk)'.\n";
        std::cout << "2. The Player exits Idle, enters Walk.\n";
        std::cout << "3. The Player's Update() now only runs Walk code.\n\n";



        HFL::WaitForInput();

        // --- STEP 3: INTERACTIVE SIMULATION ---
        Player MyPlayer;

        while (true)
        {
            HFL::ClearScreen();
            HFL::PrintHeader("Animation State Machine");

            // Display current state info
            std::cout << "CURRENT STATE: [ " << MyPlayer.GetCurrentStateName() << " ]\n";
            std::cout << "--------------------------------------------------\n";
            MyPlayer.Update();
            std::cout << "--------------------------------------------------\n\n";

            // Controls
            std::cout << "Controls:\n";
            std::cout << "1. Walk\n";
            std::cout << "2. Stop/Idle\n";
            std::cout << "3. Attack\n";
            std::cout << "4. Jump\n";
            std::cout << "0. Exit Demo\n";

            int Choice = HFL::GetValidMenuInput(4);
            if (Choice == 0) break;

            // Handle Input - If choice is '2', this will block for 1.5s 
            // and then return to Idle before the next loop iteration.
            MyPlayer.HandleInput(Choice);
        }

        // --- STEP 4: CONCLUSION ---
        HFL::ClearScreen();
        HFL::PrintHeader("Conclusion");

        std::cout << "Summary of State Pattern:\n\n";
        std::cout << "1. Encapsulation:\n";
        std::cout << "   Walking logic doesn't 'pollute' the Idle code.\n\n";

        std::cout << "2. Input Filtering:\n";
        std::cout << "   The Attack state explicitly ignores movement inputs,\n";
        std::cout << "   making the code much easier to reason about.\n\n";

        std::cout << "3. Scalability:\n";
        std::cout << "   To add 'Swimming', you just create a SwimState class\n";
        std::cout << "   and add a transition from Walk -> Swim.\n\n";

        std::cout << std::setw(40) << "End of Demo\n";
        HFL::WaitForInput();
    }
}