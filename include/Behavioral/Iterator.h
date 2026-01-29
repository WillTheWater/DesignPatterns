#pragma once

#include "FunctionLibrary/HelperFunctionLibrary.h"
#include <vector>

// =========================================================================
// BEHAVIORAL DESIGN PATTERN: ITERATOR
// =========================================================================
// "Provide a way to access the elements of an aggregate object sequentially 
// without exposing its underlying representation."
//
// THE GOAL:
// Decouple the traversal logic from the collection itself. This allows 
// for different types of traversal (e.g., Filtering, Reverse, Shuffled) 
// without modifying the container class or exposing its internal structure.
//
// THE BENEFIT:
// * Uniform Interface: The UI can traverse any list using the same methods.
// * Multiple Traversals: You can have several iterators active on one list.
// * Encapsulation: The underlying storage (vector, list, tree) remains hidden.
// * Single Responsibility: The collection manages data; the iterator manages state.
//
// THE EXAMPLE:
// [Quest]: The Data. A simple struct with a name and a status.
// [QuestLog]: The Aggregate. A collection that yields specialized iterators.
// [QuestFilterIterator]: The Smart Remote. Traverses the log while 
//   automatically skipping entries that don't match the desired status.
// =========================================================================

namespace ITR
{
    // =========================================================================
    // THE DATA (The Element)
    // ROLE: The individual object being stored and traversed.
    // =========================================================================
    enum class QuestStatus { Available, Active, Completed, Hidden };

    struct Quest
    {
        std::string Name;
        QuestStatus Status;

        void Display() const;
    };

    // =========================================================================
    // THE ITERATOR INTERFACE
    // ROLE: Defines the contract for traversal. By working with this interface,
    // the UI/Client can iterate through filtered quests without knowing the logic.
    // =========================================================================
    class IQuestIterator
    {
    public:
        virtual ~IQuestIterator() = default;

        virtual bool HasNext() const = 0;
        virtual Quest* Next() = 0;
        virtual Quest* Current() const = 0;
    };

    // =========================================================================
    // THE CONCRETE ITERATOR (Smart Filter Logic)
    // ROLE: Implements the traversal algorithm. It maintains a reference to 
    // the collection and its own current position.
    // =========================================================================
    class QuestFilterIterator : public IQuestIterator
    {
    public:
        // Uses a TargetStatus so this one class can handle ANY filtered view.
        QuestFilterIterator(std::vector<Quest>& QuestList, QuestStatus TargetStatus);

        bool HasNext() const override;
        Quest* Next() override;
        Quest* Current() const override;

    private:
        // Internal logic to jump over quests that don't match filter.
        void AdvanceToNextMatch();

        std::vector<Quest>& Quests;
        QuestStatus Filter;
        size_t Position = 0;
    };

    // =========================================================================
    // THE AGGREGATE (The Container)
    // ROLE: Holds the data and provides "Factory Methods" to create iterators.
    // The UI should never access the 'AllQuests' vector directly.
    // =========================================================================
    class QuestLog
    {
    public:
        void AddQuest(const std::string& Name, QuestStatus Status);

        // ======================== ITERATOR FACTORY METHODS ========================
        // 1. Encapsulation: The UI only interacts with IQuestIterator.
        // 2. Ownership: Returns unique_ptr to ensure the caller manages the "remote".
        // 3. Polymorphism: Different filters use the same return type.
        std::unique_ptr<IQuestIterator> CreateActiveIterator();
        std::unique_ptr<IQuestIterator> CreateCompletedIterator();
        std::unique_ptr<IQuestIterator> CreateHiddenIterator();
        std::unique_ptr<IQuestIterator> CreateAvailableIterator();

        const std::vector<Quest>& GetAllQuestsRaw() const { return AllQuests; }

    private:
        std::vector<Quest> AllQuests;
    };

    // =========================================================================
    // DEMO IMPLEMENTATION
    // =========================================================================
    void RunDemo();
}