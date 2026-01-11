#pragma once

#include "FunctionLibrary/HelperFunctionLibrary.h"
#include <vector>

// =========================================================================
// BEHAVIORAL DESIGN PATTERNS: Iterator
// =========================================================================
// "Provide a way to access the elements of an aggregate object sequentially 
// without exposing its underlying representation."
//
// THE GOAL:
// Decouple the traversal logic from the collection itself. This allows 
// for different types of traversal (e.g., Filtering, Reverse, Shuffled) 
// without modifying the container class.
//
// THE EXAMPLE:
// Smart Quest Log.
// 1. The Data (Quest): A simple struct with a name and a status.
// 2. The Aggregate (QuestLog): A collection of many quests.
// 3. The Iterator (ActiveQuestIterator): A "smart" remote that skips 
//    completed/hidden quests and only shows what the player needs to do.
// =========================================================================

namespace ITR
{
    enum class QuestStatus { Available, Active, Completed, Hidden };

    struct Quest
    {
        std::string Name;
        QuestStatus Status;

        void Display() const;
    };

    // ------------------------------------------------------------------------
    // 1. THE ITERATOR INTERFACE
    // ------------------------------------------------------------------------
    // By defining a generic interface, the UI can work with any 
    // iterator (Active, Completed, or Daily quests) without knowing the rules.
    class IQuestIterator
    {
    public:
        virtual ~IQuestIterator() = default;
        virtual bool HasNext() const = 0;
        virtual Quest* Next() = 0;
        virtual Quest* Current() const = 0;
    };

    // ------------------------------------------------------------------------
    // 2. THE CONCRETE ITERATOR (Smart Filter Logic)
    // ------------------------------------------------------------------------
    class QuestFilterIterator : public IQuestIterator
    {
    public:
        // TargetStatus so this one class can handle ANY filter.
        QuestFilterIterator(std::vector<Quest>& QuestList, QuestStatus TargetStatus);

        bool HasNext() const override;
        Quest* Next() override;
        Quest* Current() const override;

    private:
        void AdvanceToNextMatch();

        std::vector<Quest>& Quests;
        QuestStatus Filter;
        size_t Position = 0;
    };

    // ------------------------------------------------------------------------
    // 3. THE AGGREGATE (The Container)
    // ------------------------------------------------------------------------
    class QuestLog
    {
    public:
        void AddQuest(const std::string& Name, QuestStatus Status);

        // --- THE ITERATOR FACTORY METHOD ---
        // This is the ONLY way the outside classes should get an iterator.
        // 1. Encapsulation: The UI doesn't need to know 'ActiveQuestIterator' exists.
        //    It only cares about the 'IQuestIterator' interface.
        // 2. Ownership: Returning a unique_ptr make sure that the caller 
        //    now owns this "remote control" and is responsible for its lifetime.
        // 3. Polymorphism: 'CreateCompletedIterator()' can be added later;
        //    the UI would treat it exactly the same way.
        std::unique_ptr<IQuestIterator> CreateActiveIterator();
        std::unique_ptr<IQuestIterator> CreateCompletedIterator();
        std::unique_ptr<IQuestIterator> CreateHiddenIterator();
        std::unique_ptr<IQuestIterator> CreateAvailableIterator();
        const std::vector<Quest>& GetAllQuestsRaw() const { return AllQuests; }

    private:
        std::vector<Quest> AllQuests;
    };

    void RunDemo();
}