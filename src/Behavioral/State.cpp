#include "Behavioral/State.h"

namespace STA
{
    // =========================================================================
    // THE CONTEXT (Player Implementation)
    // ROLE: Manages the lifecycle of the current "Brain" (State).
    // =========================================================================

    Player::Player()
    {
        // Start in the Idle state - initialization is silent
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
    // CONCRETE STATE: IDLE
    // =========================================================================

    void IdleState::Enter(Player& P) { (void)P; }
    void IdleState::Update(Player& P)
    {
        (void)P;
        std::cout << "    [Anim] Character is standing breathing idle...\n";
    }
    void IdleState::Exit(Player& P) { (void)P; }

    void IdleState::HandleInput(Player& P, int Input)
    {
        if (Input == 1) P.SetState(std::make_shared<WalkState>());
        if (Input == 3) P.SetState(std::make_shared<AttackState>());
        if (Input == 4) P.SetState(std::make_shared<JumpState>());
    }

    // =========================================================================
    // CONCRETE STATE: WALK
    // =========================================================================

    void WalkState::Enter(Player& P) { (void)P; }
    void WalkState::Update(Player& P)
    {
        (void)P;
        std::cout << "    [Anim] Character is walking forward (Looping)...\n";
    }
    void WalkState::Exit(Player& P) { (void)P; }

    void WalkState::HandleInput(Player& P, int Input)
    {
        if (Input == 2) P.SetState(std::make_shared<IdleState>());
        if (Input == 3) P.SetState(std::make_shared<AttackState>());
        if (Input == 4) P.SetState(std::make_shared<JumpState>());
    }

    // =========================================================================
    // CONCRETE STATE: ATTACK (Self-Transitioning)
    // =========================================================================

    void AttackState::Enter(Player& P)
    {
        HFL::ClearScreen();
        HFL::PrintHeader("ANIMATION STATE MACHINE");
        HFL::SetColor(HFL::EColor::White);
        std::cout << "CURRENT STATE: [ ATTACKING ]\n";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "--------------------------------------------------\n";

        std::cout << "    (Anim) Character swings sword!\n";
        HFL::Wait(1.5f);

        std::cout << "    (Combat) Critical Hit detected!\n";
        std::cout << "    (Combat) The monster flees...\n";
        HFL::Wait(1.5f);

        // AUTO-TRANSITION: Return to Idle once the action is complete
        P.SetState(std::make_shared<IdleState>());
    }

    void AttackState::Update(Player& P) { (void)P; }
    void AttackState::Exit(Player& P) { (void)P; }
    void AttackState::HandleInput(Player& P, int Input) { (void)P; (void)Input; }

    // =========================================================================
    // CONCRETE STATE: JUMP (Self-Transitioning)
    // =========================================================================

    void JumpState::Enter(Player& P)
    {
        HFL::ClearScreen();
        HFL::PrintHeader("ANIMATION STATE MACHINE");
        HFL::SetColor(HFL::EColor::White);
        std::cout << "CURRENT STATE: [ JUMPING ]\n";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "--------------------------------------------------\n";

        std::cout << "    (Anim) Character jumps!\n";
        HFL::Wait(1.0f);

        std::cout << "    (Anim) Character lands!\n";
        HFL::Wait(1.5f);

        P.SetState(std::make_shared<IdleState>());
    }

    void JumpState::Update(Player& P) { (void)P; }
    void JumpState::Exit(Player& P) { (void)P; }
    void JumpState::HandleInput(Player& P, int Input) { (void)P; (void)Input; }

    // =========================================================================
    // DEMO IMPLEMENTATION
    // =========================================================================

    void RunDemo()
    {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        // ======================== INTRODUCTION ========================
        HFL::ClearScreen();
        HFL::PrintHeader("STATE DESIGN PATTERN");

        HFL::PrintSection("THE DEFINITION");
        HFL::SetColor(HFL::EColor::White);
        std::cout << "Allow an object to alter its behavior when its internal state changes.\n"
            << "The object will appear to change its class.\n\n";

        HFL::PrintSection("THE GOAL");
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "The State Pattern is about 'Encapsulated Polymorphism'.\n"
            << "Instead of one character class containing 500 lines of if-else\n"
            << "statements for Idle, Walk, and Attack, we move that logic into\n"
            << "separate classes that the character swaps between at runtime.\n\n";

        HFL::PrintSection("THE EXAMPLE");
        HFL::SetColor(HFL::EColor::White);
        std::cout << "This demonstration features an Animation State Machine (ASM):\n\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] THE IDLE STATE:    ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Standard waiting behavior. Allows transitions to all states.\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] THE ATTACK STATE:  ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Locks movement and input until the swing animation ends.\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] THE CONTEXT:       ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "The Player class. It is just a vessel for the current state.\n\n";

        HFL::PrintSection("THE BENEFIT");
        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] CLEAN UPDATE:    ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "The Player's Update() is always one line: CurrentState->Update().\n";
        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] INPUT FILTERING: ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "The Attack state simply chooses not to implement movement input.\n";
        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] SCALABILITY:     ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Adding 'Swimming' or 'Dying' is just adding a new class.\n";

        HFL::WaitForInput();

        // ======================== THE ARCHITECTURE ========================
        HFL::ClearScreen();
        HFL::PrintHeader("THE ARCHITECTURE");

        HFL::PrintSection("THE 'BRAIN SWAP'");
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Unlike a standard Finite State Machine, transitions are 'pushed' by the states themselves.\n"
            << "When the Player is in 'WalkState' and the user stops moving, the\n"
            << "WalkState calls 'SetState(Idle)' on the Player.\n\n";

        HFL::PrintSection("IMPLEMENTATION");

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] IPlayerState (The Interface)\n";
        HFL::SetColor(HFL::EColor::White);
        std::cout << "    ROLE:           ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "The Contract. Defines Enter, Update, HandleInput, and Exit.\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] Player (The Context)\n";
        HFL::SetColor(HFL::EColor::White);
        std::cout << "    ROLE:           ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "The Owner. Holds the shared_ptr to the current state.\n\n";

        HFL::SetColor(HFL::EColor::White);
        HFL::WaitForInput();

        // ======================== INITIALIZATION ========================
        Player MyPlayer;

        // ======================== GAME LOOP ========================
        while (true)
        {
            HFL::ClearScreen();
            HFL::PrintHeader("ANIMATION STATE MACHINE");

            HFL::PrintSection("CURRENT STATUS");
            HFL::SetColor(HFL::EColor::White);
            std::cout << "  ACTIVE STATE:  ";
            HFL::SetColor(HFL::EColor::Green);
            std::cout << "[ " << MyPlayer.GetCurrentStateName() << " ]\n";
            HFL::SetColor(HFL::EColor::Gray);
            std::cout << "  --------------------------------------------------\n";
            MyPlayer.Update();
            HFL::SetColor(HFL::EColor::Gray);
            std::cout << "  --------------------------------------------------\n\n";

            HFL::PrintSection("CONTROLS");
            HFL::SetColor(HFL::EColor::Green);
            std::cout << " [1] "; HFL::SetColor(HFL::EColor::White); std::cout << "WALK\n";
            HFL::SetColor(HFL::EColor::Green);
            std::cout << " [2] "; HFL::SetColor(HFL::EColor::White); std::cout << "IDLE\n";
            HFL::SetColor(HFL::EColor::Green);
            std::cout << " [3] "; HFL::SetColor(HFL::EColor::White); std::cout << "ATTACK\n";
            HFL::SetColor(HFL::EColor::Green);
            std::cout << " [4] "; HFL::SetColor(HFL::EColor::White); std::cout << "JUMP\n\n";
            HFL::SetColor(HFL::EColor::Green);
            std::cout << " [0] "; HFL::SetColor(HFL::EColor::White); std::cout << "CONTINUE\n\n";

            int Choice = HFL::GetValidMenuInput(4);
            if (Choice == 0) break;

            MyPlayer.HandleInput(Choice);
        }

        // ======================== CONCLUSION ========================
        HFL::ClearScreen();
        HFL::PrintHeader("CONCLUSION");

        HFL::PrintSection("ARCHITECTURE");
        HFL::SetColor(HFL::EColor::White);
        std::cout << "The implementation of the State Pattern confirms the following:\n\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] ENCAPSULATED LOGIC:      ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "The Walking logic doesn't 'pollute' the Idle code. Each state\n"
            << "    is a clean, isolated class with its own responsibilities.\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] EXPLICIT TRANSITIONS:    ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "State changes are intentional. The code tells a story of how\n"
            << "    the character moves from one behavior to the next.\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] INPUT FILTERING:         ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Because the Attack state overrides HandleInput to do nothing,\n"
            << "    we guarantee the player cannot move mid-swing.\n\n";

        HFL::PrintSection("SUMMARY");
        HFL::SetColor(HFL::EColor::White);
        std::cout << "The State Pattern ensures that software is:\n\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] MODULAR:   ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Logic is split into manageable, bite-sized objects.\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] RELIABLE:  ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Illegal transitions are impossible if not defined in the state.\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] SCALABLE:  ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Adding new behaviors (like 'Dead' or 'Swimming') requires zero\n"
            << "    changes to the core Player class.\n\n";

        HFL::SetColor(HFL::EColor::White);
        HFL::WaitForInput();
    }
}