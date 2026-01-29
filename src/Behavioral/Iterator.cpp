#include "Behavioral/Iterator.h"

namespace ITR
{
    // =========================================================================
    // DATA REPRESENTATION
    // =========================================================================

    void Quest::Display() const
    {
        std::string StatusStr;
        HFL::EColor Color = HFL::EColor::Gray;

        switch (Status)
        {
        case QuestStatus::Available: StatusStr = "[ AVAILABLE ]"; Color = HFL::EColor::White; break;
        case QuestStatus::Active:    StatusStr = "[  ACTIVE   ]"; Color = HFL::EColor::Cyan; break;
        case QuestStatus::Completed: StatusStr = "[  COMPLETE  ]"; Color = HFL::EColor::Green; break;
        case QuestStatus::Hidden:    StatusStr = "[  HIDDEN    ]"; Color = HFL::EColor::Red; break;
        }

        HFL::SetColor(Color);
        std::cout << "    " << std::left << std::setw(15) << StatusStr;
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << Name << "\n";
    }

    // =========================================================================
    // CONCRETE ITERATOR LOGIC: The "Smart" Traversal
    // ROLE: Encapsulates the filtering algorithm so the UI stays "clean".
    // =========================================================================

    QuestFilterIterator::QuestFilterIterator(std::vector<Quest>& QuestList, QuestStatus TargetStatus)
        : Quests(QuestList), Filter(TargetStatus), Position(0)
    {
        // Immediately find the first match so HasNext() is accurate upon creation
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

    bool QuestFilterIterator::HasNext() const
    {
        return Position < Quests.size();
    }

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
    // AGGREGATE LOGIC: QuestLog
    // ROLE: The collection that yields specialized iterators via Factory Methods.
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

        // ======================== INTRODUCTION ========================
        HFL::ClearScreen();
        HFL::PrintHeader("ITERATOR DESIGN PATTERN");

        HFL::PrintSection("THE DEFINITION");
        HFL::SetColor(HFL::EColor::White);
        std::cout << "Provide a way to access the elements of an aggregate object sequentially\n"
            << "without exposing its underlying representation.\n\n";

        HFL::PrintSection("THE GOAL");
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "The Iterator Pattern is about 'Separation of Traversal'. It allows\n"
            << "looping through complex collections (Quest Logs, Inventories, Skill Trees)\n"
            << "using a uniform interface, regardless of how the data is actually stored.\n\n";

        HFL::PrintSection("THE EXAMPLE");
        HFL::SetColor(HFL::EColor::White);
        std::cout << "This demonstration features a Quest Log with three specialized components:\n\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] THE AGGREGATE:       ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "The QuestLog. It owns the raw std::vector of data.\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] THE INTERFACE:       ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "IQuestIterator. Defines the standard 'Remote Control' buttons.\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] THE SMART FILTER:    ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "A Concrete Iterator that skips irrelevant quests automatically.\n\n";

        HFL::PrintSection("THE BENEFIT");
        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] UNIFORM ACCESS:  ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "The UI uses the same 'while' loop for Active, Completed, or Hidden quests.\n";
        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] ENCAPSULATION:   ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "The UI never sees the underlying std::vector or filtering logic.\n";
        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] FLEXIBILITY:     ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Could change the storage to a Map or Linked List without breaking the UI.\n";

        HFL::WaitForInput();

        // ======================== THE ARCHITECTURE ========================
        HFL::ClearScreen();
        HFL::PrintHeader("THE ARCHITECTURE");

        HFL::PrintSection("THE 'SMART REMOTE'");
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Without an Iterator, the UI must manually check 'if (Status == Active)'\n"
            << "inside every loop. With an Iterator, that complexity is 'pushed' into\n"
            << "a dedicated object, leaving the UI code clean and declarative.\n\n";

        HFL::PrintSection("IMPLEMENTATION");

        // ======================== THE AGGREGATE ========================
        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] QuestLog (The Aggregate)\n";
        HFL::SetColor(HFL::EColor::White);
        std::cout << "    ROLE:           ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "The Container. It is responsible for data storage and ownership.\n";
        HFL::SetColor(HFL::EColor::White);
        std::cout << "    FACTORY:        ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Creates specific iterators (Active/Complete) on demand.\n\n";

        // ======================== THE ITERATOR ========================
        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] QuestFilterIterator (The Concrete Iterator)\n";
        HFL::SetColor(HFL::EColor::White);
        std::cout << "    ROLE:           ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "The Traversal logic. It knows how to 'AdvanceToNextMatch'.\n";
        HFL::SetColor(HFL::EColor::White);
        std::cout << "    SIMPLICITY:     ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Exposes only HasNext(), Next(), and Current().\n";
        HFL::SetColor(HFL::EColor::White);
        std::cout << "    DECOUPLING:     ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "The UI only knows the Interface, not the Filter logic.\n\n";

        HFL::SetColor(HFL::EColor::White);
        HFL::WaitForInput();

        // ======================== INITIALIZATION ========================
        QuestLog PlayerLog;
        PlayerLog.AddQuest("Kill 10 Rats", QuestStatus::Completed);
        PlayerLog.AddQuest("Find the Lost Ring", QuestStatus::Active);
        PlayerLog.AddQuest("Secret: Traitor's Note", QuestStatus::Hidden);
        PlayerLog.AddQuest("Talk to the King", QuestStatus::Active);
        PlayerLog.AddQuest("Escort the Merchant", QuestStatus::Available);
        PlayerLog.AddQuest("Slay the Dragon", QuestStatus::Active);
        PlayerLog.AddQuest("Collect 5 Herbs", QuestStatus::Completed);

        // ======================== INTERACTIVE LOOP ========================
        while (true)
        {
            HFL::ClearScreen();
            HFL::PrintHeader("QUEST LOG INTERFACE");

            HFL::PrintSection("VIEW MODES");
            HFL::SetColor(HFL::EColor::Green);
            std::cout << " [1] "; HFL::SetColor(HFL::EColor::White); std::cout << "ALL QUESTS\n";
            HFL::SetColor(HFL::EColor::Green);
            std::cout << " [2] "; HFL::SetColor(HFL::EColor::White); std::cout << "ACTIVE QUESTS\n";
            HFL::SetColor(HFL::EColor::Green);
            std::cout << " [3] "; HFL::SetColor(HFL::EColor::White); std::cout << "COMPLETED QUESTS\n";
            HFL::SetColor(HFL::EColor::Green);
            std::cout << " [4] "; HFL::SetColor(HFL::EColor::White); std::cout << "HIDDEN QUESTS\n\n";
            HFL::SetColor(HFL::EColor::Green);
            std::cout << " [0] "; HFL::SetColor(HFL::EColor::White); std::cout << "CONTINUE\n\n";

            int Choice = HFL::GetValidMenuInput(4);
            if (Choice == 0) break;

            HFL::PrintSection("EXECUTION LOG");
            HFL::SetColor(HFL::EColor::Gray);

            if (Choice == 1)
            {
                std::cout << "[System] Direct access: UI is manually looping 'std::vector<Quest>'.\n";
                std::cout << "[System] Logic: No filtering applied.\n\n";

                for (const auto& Q : PlayerLog.GetAllQuestsRaw())
                {
                    Q.Display();
                }
            }
            else
            {
                std::unique_ptr<IQuestIterator> It = nullptr;

                // OBTAIN: The Facade-style Factory creates the specific worker
                if (Choice == 2)      It = PlayerLog.CreateActiveIterator();
                else if (Choice == 3) It = PlayerLog.CreateCompletedIterator();
                else if (Choice == 4) It = PlayerLog.CreateHiddenIterator();

                std::cout << "[System] Using IQuestIterator to filter results...\n";
                std::cout << "[System] UI loop remains identical regardless of filter type.\n\n";

                // TRAVERSE: The Client loop is identical for ANY iterator type (Polymorphism)
                while (It && It->HasNext())
                {
                    Quest* q = It->Next();
                    if (q) q->Display();
                }
            }

            HFL::SetColor(HFL::EColor::Gray);
            std::cout << "\n--------------------------------------------------\n";
            HFL::WaitForInput();
        }

        // ======================== CONCLUSION ========================
        HFL::ClearScreen();
        HFL::PrintHeader("CONCLUSION");

        HFL::PrintSection("ARCHITECTURE");
        HFL::SetColor(HFL::EColor::White);
        std::cout << "The implementation of the Iterator Pattern confirms the following:\n\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] LOGIC ENCAPSULATION:     ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "The UI no longer contains 'if (Status == X)' checks. The filtering\n"
            << "    logic is hidden entirely inside the QuestFilterIterator.\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] UNIFORM TRAVERSAL:       ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "The UI loop is identical for Active, Completed, or Hidden views.\n"
            << "    It only knows how to call 'Next()' on an interface.\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] FACTORY ABSTRACTION:     ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "The QuestLog handles the creation and configuration of iterators,\n"
            << "    returning a unique_ptr to shield the UI from lifecycle management.\n\n";

        HFL::PrintSection("SUMMARY");
        HFL::SetColor(HFL::EColor::White);
        std::cout << "The Iterator Pattern ensures that software is:\n\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] CLEAN:     ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "The presentation layer is separated from the traversal algorithm.\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] FLEXIBLE:  ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "New filters (e.g. DailyQuests) can be added without touching the UI.\n";

        HFL::SetColor(HFL::EColor::Green);
        std::cout << "[*] DECOUPLED: ";
        HFL::SetColor(HFL::EColor::Gray);
        std::cout << "Swapping a 'std::vector' for a 'std::map' only requires changing the\n"
            << "    Iterator, leaving the character's Quest Log UI 100% untouched.\n\n";

        HFL::SetColor(HFL::EColor::White);
        HFL::WaitForInput();
    }
}