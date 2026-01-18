#pragma once

#include "FunctionLibrary/HelperFunctionLibrary.h"

// =========================================================================
// BEHAVIORAL DESIGN PATTERNS: Interpreter
// =========================================================================
// "Given a language, define a representation for its grammar along with an 
// interpreter that uses the representation to interpret sentences in the 
// language."
//
// THE GOAL:
// Turn a string or data-structure (like JSON) into a tree of objects that 
// represents logic. This allows you to define complex rules as data.
//
// THE EXAMPLE:
// Game Quest & Achievement System.
// 1. The Context (PlayerCtx): The 'Global State' the interpreter reads from 
//    (Level, Gold, KillCount).
// 2. The Abstract Expression (IExpression): Defines the 'Interpret' method.
// 3. Terminal Expressions (LevelExpr, GoldExpr): The 'Leaf' nodes that 
//    perform the actual checks against the Context.
// 4. Non-Terminal Expressions (AndExpr, OrExpr, NotExpr): The 'Branches' 
//    that combine other expressions to build complex logic trees.
// 5. The Parser: The 'Factory' that reads raw JSON and assembles the 
//    recursive object tree.
// =========================================================================

namespace INT
{
    // =========================================================================
    // THE CONTEXT
    // Holds the global state required for evaluation.
    // The Interpreter reads from this to make decisions.
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
    // =========================================================================
    // This is the base class for the entire tree. By using a single interface, 
    // the Parser can treat a single Level check and a complex AND/OR/NOT 
    // structure exactly the same way.

    class Expression
    {
    public:
        virtual ~Expression() = default;

        // Context is passed by reference to allow the expression to "poll" 
        // the current game state without owning it.
        virtual bool Interpret(const Context& Ctx) = 0;

        // ToString allows the logic tree to describe itself for UI/Debugging.
        virtual std::string ToString() const = 0;
    };

    // =========================================================================
    // TERMINAL EXPRESSIONS (The Leaves)
    // =========================================================================
    // Terminal expressions represent the actual facts we want to check.
    // They are called "Terminal" because they do not have any child nodes; 
    // they are the end-points of the logic tree.

    class LevelExpression : public Expression
    {
    public:
        LevelExpression(int Lvl) : RequiredLevel(Lvl) {}
        bool Interpret(const Context& Ctx) override { return Ctx.Level >= RequiredLevel; }
        std::string ToString() const override { return "Level >= " + std::to_string(RequiredLevel); }

    private:
        int RequiredLevel;
    };

    class GoldExpression : public Expression
    {
    public:
        GoldExpression(int Amt) : RequiredGold(Amt) {}
        bool Interpret(const Context& Ctx) override { return Ctx.Gold >= RequiredGold; }
        std::string ToString() const override { return "Gold >= " + std::to_string(RequiredGold); }

    private:
        int RequiredGold;
    };

    class KillExpression : public Expression
    {
    public:
        KillExpression(const std::string& Type, int Count) : TargetType(Type), RequiredCount(Count) {}
        bool Interpret(const Context& Ctx) override
        {
            // Terminal nodes often interact with Maps or Collections inside the Context.
            auto it = Ctx.KillCount.find(TargetType);
            return (it != Ctx.KillCount.end()) && (it->second >= RequiredCount);
        }
        std::string ToString() const override { return "Kill " + std::to_string(RequiredCount) + " " + TargetType + "s"; }
    private:
        std::string TargetType;
        int RequiredCount;
    };

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
    // =========================================================================
    // Non-Terminal expressions represent the "Grammar" or "Rules" of the language.
    // They contain other Expressions (children), allowing for recursive nesting.

    class AndExpression : public Expression
    {
    public:
        // By taking unique_ptrs, we ensure the tree owns its children.
        AndExpression(std::unique_ptr<Expression> L, std::unique_ptr<Expression> R)
            : Left(std::move(L)), Right(std::move(R)) {
        }

        bool Interpret(const Context& Ctx) override {
            // Recursive evaluation: This node only returns true if its children do.
            return Left->Interpret(Ctx) && Right->Interpret(Ctx);
        }
        std::string ToString() const override {
            return "(" + Left->ToString() + " AND " + Right->ToString() + ")";
        }
    private:
        std::unique_ptr<Expression> Left, Right;
    };

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

    class NotExpression : public Expression
    {
    public:
        NotExpression(std::unique_ptr<Expression> E) : Expr(std::move(E)) {}

        bool Interpret(const Context& Ctx) override {
            // Unary operator: Inverts whatever the child expression returns.
            return !Expr->Interpret(Ctx);
        }

        std::string ToString() const override {
            return "(NOT " + Expr->ToString() + ")";
        }
    private:
        std::unique_ptr<Expression> Expr;
    };

    void RunDemo();
}