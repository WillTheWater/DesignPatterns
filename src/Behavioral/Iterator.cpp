#include "Behavioral/Iterator.h"

namespace ITR
{
    void Quest::Display() const
    {
        std::string StatusStr;
        switch (Status) {
        case QuestStatus::Available: StatusStr = "[Available]"; break;
        case QuestStatus::Active:    StatusStr = "[ ACTIVE  ]"; break;
        case QuestStatus::Completed: StatusStr = "[ DONE    ]"; break;
        case QuestStatus::Hidden:    StatusStr = "[ HIDDEN  ]"; break;
        }
        std::cout << "   " << StatusStr << " " << Name << "\n";
    }

    // =========================================================================
    // ITERATOR LOGIC: The "Smart" Traversal
    // =========================================================================

    ActiveQuestIterator::ActiveQuestIterator(std::vector<Quest>& QuestList)
        : Quests(QuestList), Position(0)
    {
        // Start at the first valid active quest immediately
        if (Position < Quests.size() && Quests[Position].Status != QuestStatus::Active)
        {
            AdvanceToNextActive();
        }
    }

    void ActiveQuestIterator::AdvanceToNextActive()
    {
        // This is the core of the pattern: hidden logic that decides 
        // what the user sees next.
        while (Position < Quests.size())
        {
            Position++;
            if (Position < Quests.size() && Quests[Position].Status == QuestStatus::Active)
            {
                break;
            }
        }
    }

    bool ActiveQuestIterator::HasNext() const { return Position < Quests.size(); }

    Quest* ActiveQuestIterator::Current() const
    {
        return (Position < Quests.size()) ? const_cast<Quest*>(&Quests[Position]) : nullptr;
    }

    Quest* ActiveQuestIterator::Next()
    {
        Quest* CurrentQuest = Current();
        AdvanceToNextActive();
        return CurrentQuest;
    }

    // =========================================================================
    // AGGREGATE LOGIC
    // =========================================================================

    void QuestLog::AddQuest(const std::string& Name, QuestStatus Status)
    {
        AllQuests.push_back({ Name, Status });
    }

    std::unique_ptr<IQuestIterator> QuestLog::CreateActiveIterator()
    {
        return std::make_unique<ActiveQuestIterator>(AllQuests);
    }

    // =========================================================================
    // DEMO IMPLEMENTATION
    // =========================================================================
    void RunDemo()
    {
        // Clear buffer
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        // --- STEP 1: INTRODUCTION ---
        HFL::ClearScreen();
        HFL::PrintHeader("Iterator Pattern");

        std::cout << "Definition:\n";
        std::cout << "Provide a way to access elements of a collection sequentially\n";
        std::cout << "without exposing the underlying structure (Vector, List, Tree, etc.).\n\n";

        std::cout << "In This Demo:\n";
        std::cout << "We have a 'Quest Log' containing many types of quests.\n";
        std::cout << "The UI shouldn't have to filter through raw data manually.\n";
        std::cout << "We use a 'Smart Iterator' to provide a clean stream of Active Quests.\n";

        HFL::WaitForInput();

        // --- STEP 2: THE ROLES ---
        HFL::ClearScreen();
        HFL::PrintHeader("Step 1: The Roles");

        std::cout << "The Iterator Pattern involves 3 key components:\n\n";

        std::cout << "1. The Aggregate (QuestLog):\n";
        std::cout << "   - The container that holds the raw data (std::vector).\n";
        std::cout << "   - It creates the iterator for the user.\n\n";



        std::cout << "2. The Iterator Interface (IQuestIterator):\n";
        std::cout << "   - Defines the 'Next()' and 'HasNext()' contract.\n";
        std::cout << "   - This allows the UI to remain agnostic of the collection type.\n\n";

        std::cout << "3. The Concrete Iterator (ActiveQuestIterator):\n";
        std::cout << "   - The 'Smart Remote' that knows the filtering rules.\n";
        std::cout << "   - It handles the logic of skipping finished or hidden quests.\n";

        HFL::WaitForInput();

        // Setup the Data
        QuestLog PlayerLog;
        PlayerLog.AddQuest("Kill 10 Rats", QuestStatus::Completed);
        PlayerLog.AddQuest("Find the Lost Ring", QuestStatus::Active);
        PlayerLog.AddQuest("Secret: Traitor's Note", QuestStatus::Hidden);
        PlayerLog.AddQuest("Talk to the King", QuestStatus::Active);
        PlayerLog.AddQuest("Escort the Merchant", QuestStatus::Available);
        PlayerLog.AddQuest("Slay the Dragon", QuestStatus::Active);

        // --- STEP 3: INTERACTIVE SIMULATION ---
        while (true)
        {
            HFL::ClearScreen();
            HFL::PrintHeader("Quest Log");

            std::cout << "The QuestLog contains 6 total entries (Active, Done, and Hidden).\n\n";

            std::cout << "1. All Quests (Show everything, no filter)\n";
            std::cout << "2. Active Quests (Use Smart Filtered Iterator)\n";
            std::cout << "0. Exit\n\n";

            int Choice = HFL::GetValidMenuInput(2);
            if (Choice == 0) break;

            std::cout << "\n--- EXECUTION LOG ---\n";
            if (Choice == 1)
            {
                std::cout << "[System] Direct access: Iterating through AllQuests[0...N]\n";
                std::cout << "[System] Logic: Show all quests.\n\n";

                std::cout << "   [Completed] Kill 10 Rats\n";
                std::cout << "   [Active]    Find the Lost Ring\n";
                std::cout << "   [Hidden]    Secret: Traitor's Note\n";
                std::cout << "   [Active]    Talk to the King\n";
                std::cout << "   [Available] Escort the Merchant\n";
                std::cout << "   [Active]    Slay the Dragon\n";
            }
            else if (Choice == 2)
            {
                std::cout << "[System] Using IQuestIterator::Next();\n";
                std::cout << "[System] Logic: Iterator is internally skipping noise.\n\n";

                // 1. OBTAIN: Asking the container for a "remote control." 
                // Don't specify the concrete class name here.
                auto It = PlayerLog.CreateActiveIterator();

                // 2. TRAVERSE: The 'while' loop is clean and readable. 
                // It doesn't contain any 'if (status == Active)' logic. 
                // The "Smart Iterator" handles that filtering.
                while (It->HasNext())
                {
                    // 3. ACCESS: Next() both returns the current item AND 
                    // advances the internal pointer to the next valid entry.
                    Quest* q = It->Next();

                    // 4. EXECUTE: Blindly call the display logic.
                    if (q) q->Display();
                }

                // Result: The UI remains simple, even if the filtering logic 
                // inside the iterator becomes incredibly complex later.
            }
            std::cout << "----------------------\n";

            HFL::WaitForInput();
        }

        // --- STEP 4: CONCLUSION ---
        HFL::ClearScreen();
        HFL::PrintHeader("Conclusion");

        std::cout << "Summary:\n\n";

        std::cout << "1. Encapsulated Traversal:\n";
        std::cout << "   The UI code doesn't need 'if(quest.isActive)' checks.\n";
        std::cout << "   The Iterator provides only what is needed.\n\n";



        std::cout << "2. Single Responsibility:\n";
        std::cout << "   QuestLog manages storage; ActiveQuestIterator manages navigation.\n";
        std::cout << "   This keeps both classes clean and easy to test.\n\n";

        std::cout << "3. Flexibility:\n";
        std::cout << "   Want a 'Completed Quest' view? Just swap the Iterator class.\n";
        std::cout << "   The UI and the QuestLog remain completely unchanged.\n\n";

        std::cout << std::setw(40) << "End of Demo\n";
        HFL::WaitForInput();
    }
}