#pragma once

#include "FunctionLibrary/HelperFunctionLibrary.h"

// =========================================================================
// BEHAVIORAL DESIGN PATTERN: Interpreter
// =========================================================================
// "Given a language, define a representation for its grammar along with an 
// interpreter that uses the representation to interpret sentences."
//
// THE GOAL:
// Turn a string or data-structure into a tree of objects that represents 
// logic. This allows defining complex rules (like Quests) as data 
// rather than hard-coded logic.
//
// THE BENEFIT:
// * Extensibility: Adding new grammar rules is as simple as adding a class.
// * Flexibility: Complex conditions can be changed at runtime via JSON/Scripts.
// * Readability: Logic trees can describe themselves in plain English via ToString().
//
// THE EXAMPLE:
// [Context]: The World State. Holds the Player's level, gold, and kill history.
// [TerminalExpression]: The base facts (e.g., "Is Level 10?").
// [NonTerminalExpression]: The logic gates (e.g., AND, OR, NOT) that combine facts.
// =========================================================================

namespace INT
{
    // =========================================================================
    // THE CONTEXT
    // ROLE: The 'Global State' or 'Database' the interpreter reads from.
    // =========================================================================
    struct Context
    {
        int Level = 1;
        int Gold = 0;
        std::unordered_map<std::string, int> KillCount;
        std::unordered_map<std::string, bool> CompletedQuests;

        void AddKill(const std::string& Type);
        void MarkComplete(const std::string& QuestName);
        bool IsQuestDone(const std::string& Name) const;
    };

    // =========================================================================
    // ABSTRACT EXPRESSION (The Interface)
    // ROLE: The common contract for every node in the logic tree.
    // =========================================================================
    class Expression
    {
    public:
        virtual ~Expression() = default;

        // Returns true/false based on the provided game context.
        virtual bool Interpret(const Context& Ctx) = 0;

        // Allows the logic tree to describe itself (e.g., for Quest Logs).
        virtual std::string ToString() const = 0;
    };

    // =========================================================================
    // TERMINAL EXPRESSIONS (The Leaves)
    // ROLE: Performs the actual checks against the Context. 
    // They are the "end of the line" for the tree.
    // =========================================================================

    // ======================== LEVEL CHECK ========================
    class LevelExpression : public Expression
    {
    public:
        LevelExpression(int Lvl) : RequiredLevel(Lvl) {}
        bool Interpret(const Context& Ctx) override { return Ctx.Level >= RequiredLevel; }
        std::string ToString() const override { return "Level >= " + std::to_string(RequiredLevel); }

    private:
        int RequiredLevel;
    };

    // ======================== GOLD CHECK ========================-
    class GoldExpression : public Expression
    {
    public:
        GoldExpression(int Amt) : RequiredGold(Amt) {}
        bool Interpret(const Context& Ctx) override { return Ctx.Gold >= RequiredGold; }
        std::string ToString() const override { return "Gold >= " + std::to_string(RequiredGold); }

    private:
        int RequiredGold;
    };

    // ======================== KILL CHECK ========================-
    class KillExpression : public Expression
    {
    public:
        KillExpression(const std::string& Type, int Count) : TargetType(Type), RequiredCount(Count) {}
        bool Interpret(const Context& Ctx) override
        {
            auto it = Ctx.KillCount.find(TargetType);
            return (it != Ctx.KillCount.end()) && (it->second >= RequiredCount);
        }
        std::string ToString() const override { return "Kill " + std::to_string(RequiredCount) + " " + TargetType + "s"; }

    private:
        std::string TargetType;
        int RequiredCount;
    };

    // ======================== QUEST CHECK ========================
    class QuestExpression : public Expression
    {
    public:
        QuestExpression(const std::string& Name) : RequiredQuest(Name) {}
        bool Interpret(const Context& Ctx) override { return Ctx.IsQuestDone(RequiredQuest); }
        std::string ToString() const override { return "Completed: '" + RequiredQuest + "'"; }

    private:
        std::string RequiredQuest;
    };

    // =========================================================================
    // NON-TERMINAL EXPRESSIONS (The Branches)
    // ROLE: The "Grammar" that combines other expressions recursively.
    // =========================================================================

    // ======================== AND GATE ========================
    class AndExpression : public Expression
    {
    public:
        AndExpression(std::unique_ptr<Expression> L, std::unique_ptr<Expression> R)
            : Left(std::move(L)), Right(std::move(R)) {
        }

        bool Interpret(const Context& Ctx) override {
            return Left->Interpret(Ctx) && Right->Interpret(Ctx);
        }
        std::string ToString() const override {
            return "(" + Left->ToString() + " AND " + Right->ToString() + ")";
        }

    private:
        std::unique_ptr<Expression> Left, Right;
    };

    // ======================== OR GATE ========================
    class OrExpression : public Expression
    {
    public:
        OrExpression(std::unique_ptr<Expression> L, std::unique_ptr<Expression> R)
            : Left(std::move(L)), Right(std::move(R)) {
        }

        bool Interpret(const Context& Ctx) override {
            return Left->Interpret(Ctx) || Right->Interpret(Ctx);
        }
        std::string ToString() const override {
            return "(" + Left->ToString() + " OR " + Right->ToString() + ")";
        }

    private:
        std::unique_ptr<Expression> Left, Right;
    };

    // ======================== NOT GATE ========================
    class NotExpression : public Expression
    {
    public:
        NotExpression(std::unique_ptr<Expression> E) : Expr(std::move(E)) {}

        bool Interpret(const Context& Ctx) override { return !Expr->Interpret(Ctx); }
        std::string ToString() const override { return "(NOT " + Expr->ToString() + ")"; }

    private:
        std::unique_ptr<Expression> Expr;
    };

    // =========================================================================
    // DEMO IMPLEMENTATION
    // =========================================================================
    void RunDemo();
}