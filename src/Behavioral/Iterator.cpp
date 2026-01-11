#include "Behavioral/Iterator.h"

namespace ITR
{
    void Quest::Display() const
    {
        std::string StatusStr;
        switch (Status) {
        case QuestStatus::Available: StatusStr = "[ AVAILABLE ]"; break;
        case QuestStatus::Active:    StatusStr = "[ ACTIVE ]"; break;
        case QuestStatus::Completed: StatusStr = "[ COMPLETE ]"; break;
        case QuestStatus::Hidden:    StatusStr = "[ HIDDEN ]"; break;
        }
        std::cout << "   " << StatusStr << " " << Name << "\n";
    }

    // =========================================================================
    // ITERATOR LOGIC: The "Smart" Traversal
    // =========================================================================

    QuestFilterIterator::QuestFilterIterator(std::vector<Quest>& QuestList, QuestStatus TargetStatus)
        : Quests(QuestList), Filter(TargetStatus), Position(0)
    {
        // Immediately find the first match so HasNext() is accurate on start
        if (Position < Quests.size() && Quests[Position].Status != Filter)
        {
            AdvanceToNextMatch();
        }
    }

    void QuestFilterIterator::AdvanceToNextMatch()
    {
        while (Position < Quests.size())
        {
            Position++;
            if (Position < Quests.size() && Quests[Position].Status == Filter)
            {
                break;
            }
        }
    }

    bool QuestFilterIterator::HasNext() const { return Position < Quests.size(); }

    Quest* QuestFilterIterator::Current() const
    {
        return (Position < Quests.size()) ? const_cast<Quest*>(&Quests[Position]) : nullptr;
    }

    Quest* QuestFilterIterator::Next()
    {
        Quest* CurrentQuest = Current();
        AdvanceToNextMatch();
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
        return std::make_unique<QuestFilterIterator>(AllQuests, QuestStatus::Active); 
    }

    std::unique_ptr<IQuestIterator> QuestLog::CreateCompletedIterator() 
    { 
        return std::make_unique<QuestFilterIterator>(AllQuests, QuestStatus::Completed); 
    }

    std::unique_ptr<IQuestIterator> QuestLog::CreateHiddenIterator() 
    { 
        return std::make_unique<QuestFilterIterator>(AllQuests, QuestStatus::Hidden); 
    }

    std::unique_ptr<IQuestIterator> QuestLog::CreateAvailableIterator() 
    {
        return std::make_unique<QuestFilterIterator>(AllQuests, QuestStatus::Available); 
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
        HFL::PrintHeader("Iterator Pattern");

        std::cout << "Definition:\n";
        std::cout << "Provide a way to access elements of a collection sequentially\n";
        std::cout << "without exposing the underlying structure (Vector, List, Tree, etc.).\n\n";

        std::cout << "In This Demo:\n";
        std::cout << "A 'Quest Log' contains different types of quests (Active, Complete, Hidden).\n";
        std::cout << "Instead of the UI checking every quest, it asks for a specific\n";
        std::cout << "Iterator that filters the data automatically.\n";

        HFL::WaitForInput();

        // --- STEP 2: THE ROLES ---
        HFL::ClearScreen();
        HFL::PrintHeader("Step 1: The Roles");

        std::cout << "1. The Aggregate (QuestLog):\n";
        std::cout << "   - Holds the raw vector of quests.\n";
        std::cout << "   - Factory methods create specific Iterators (Active, Hidden, etc.).\n\n";

        std::cout << "2. The Iterator Interface (IQuestIterator):\n";
        std::cout << "   - Standardizes how to move: HasNext() and Next().\n\n";

        std::cout << "3. The Concrete Iterator (QuestFilterIterator):\n";
        std::cout << "   - Encapsulates the filtering logic. The UI never sees 'noise'.\n";

        HFL::WaitForInput();

        QuestLog PlayerLog;
        PlayerLog.AddQuest("Kill 10 Rats", QuestStatus::Completed);
        PlayerLog.AddQuest("Find the Lost Ring", QuestStatus::Active);
        PlayerLog.AddQuest("Secret: Traitor's Note", QuestStatus::Hidden);
        PlayerLog.AddQuest("Talk to the King", QuestStatus::Active);
        PlayerLog.AddQuest("Escort the Merchant", QuestStatus::Available);
        PlayerLog.AddQuest("Slay the Dragon", QuestStatus::Active);
        PlayerLog.AddQuest("Collect 5 Herbs", QuestStatus::Completed);

        // --- STEP 3: INTERACTIVE SIMULATION ---
        while (true)
        {
            HFL::ClearScreen();
            HFL::PrintHeader("Quest Log Interface");

            std::cout << "Select View Mode:\n";
            std::cout << "1. All Quests\n";
            std::cout << "2. Active Quests\n";
            std::cout << "3. Completed Quests\n";
            std::cout << "4. Hidden Quests\n";
            std::cout << "0. Exit\n\n";

            int Choice = HFL::GetValidMenuInput(4);
            if (Choice == 0) break;

            std::cout << "\n--- EXECUTION LOG ---\n";

            if (Choice == 1)
            {
                std::cout << "[System] Direct access: UI is manually looping through 'std::vector<Quest>'.\n";
                std::cout << "[System] Logic: No filtering, just raw dumping.\n\n";

                // This demonstrates why direct access is "fragile":
                // If the vector changes to a map, this code breaks.
                for (const auto& Q : PlayerLog.GetAllQuestsRaw())
                {
                    Q.Display();
                }
            }
            else
            {
                std::unique_ptr<IQuestIterator> It = nullptr;

                // OBTAIN: Choose the right filter.
                if (Choice == 2) It = PlayerLog.CreateActiveIterator();
                else if (Choice == 3) It = PlayerLog.CreateCompletedIterator();
                else if (Choice == 4) It = PlayerLog.CreateHiddenIterator();

                std::cout << "[System] Using IQuestIterator to filter results...\n\n";

                // TRAVERSE: This loop is UNIFORM. It works for any filter!
                while (It && It->HasNext())
                {
                    Quest* q = It->Next();
                    if (q) q->Display();
                }
            }
            std::cout << "----------------------\n";
            HFL::WaitForInput();
        }

        // --- STEP 4: CONCLUSION ---
        HFL::ClearScreen();
        HFL::PrintHeader("Conclusion");

        std::cout << "Summary:\n\n";
        std::cout << "1. Logic Encapsulation:\n";
        std::cout << "   The filtering logic is hidden inside the Iterator class.\n\n";

        std::cout << "2. Clean UI Code:\n";
        std::cout << "   The display loop remains simple and doesn't change when adding filters.\n\n";

        std::cout << "3. Multiple Views:\n";
        std::cout << "   The same data can be viewed in many ways by swapping Iterators.\n";

        std::cout << std::setw(40) << "End of Demo\n";
        HFL::WaitForInput();
    }
}