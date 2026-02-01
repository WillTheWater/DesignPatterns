#pragma once

#include "FunctionLibrary/HelperFunctionLibrary.h"

// =========================================================================
// BEHAVIORAL DESIGN PATTERN: Visitor
// =========================================================================
// "Represent an operation to be performed on the elements of an object 
// structure. Visitor lets you define a new operation without changing the 
// classes of the elements on which it operates."
//
// THE GOAL:
// Decouple complex logic (like combat damage formulas) from the data objects 
// (Player/Enemy). This allows the targets to remain "pure" data containers 
// while the calculation logic can grow and change independently.
//
// THE BENEFIT:
// * Extensibility: Add new operations (Magic, True Damage, Debuffs) by 
//   simply creating a new Visitor class—no changes to Target classes needed.
// * Double Dispatch: Resolves the correct logic based on both the type of 
//   the Attack (Visitor) and the type of the Target (Element) at runtime.
// * Clean Code: Prevents Target classes from becoming bloated with "switch" 
//   statements or complex branching logic for every possible interaction.
//
// THE EXAMPLE:
// [DamageContext]: The Configuration. Defines crit chances and variance.
// [ITarget]: The Element Interface. Entities that "Accept" visitors.
// [IAttackVisitor]: The Visitor Interface. Defines how to "Visit" specific types.
// [Physical/MagicAttack]: Concrete Visitors. House the actual math formulas.
// =========================================================================

namespace VIS
{
    // =========================================================================
    // THE CONFIGURATION
    // ROLE: Carries metadata for the calculation to avoid "Magic Numbers."
    // =========================================================================
    struct DamageContext
    {
        float CritChance = 0.0f;
        float CritMultiplier = 1.5f;
        float Variance = 0.2f;
    };

    // =========================================================================
    // THE VISITOR INTERFACE (The Strategy)
    // Why specific overloads instead of a single 'Visit(ITarget&)'?
    // This enables "Double Dispatch." By having explicit overloads, the formula
    // can resolve the correct calculation at runtime without needing 
    // dynamic_cast or 'switch' statements based on type IDs.
    // =========================================================================
    class IAttackVisitor
    {
    public:
        virtual ~IAttackVisitor() = default;

        // These allow the Visitor to see the "Concrete" type of the target
        virtual void Visit(class Player& Target) = 0;
        virtual void Visit(class Enemy& Target) = 0;
    };

    // =========================================================================
    // THE ELEMENT INTERFACE (The "Passive" Container)
    // Why use the 'Accept' method?
    // This is the first half of the Double Dispatch. It allows the Target
    // to "hand over control" to a Visitor. This keeps the Target classes 
    // focused on state (Health, Def) rather than logic (Damage Formulas).
    // =========================================================================
    class ITarget
    {
    public:
        virtual ~ITarget() = default;

        // The Entry Point for the Pattern.
        // Implementations simply call: Visitor.Visit(*this);
        virtual void Accept(IAttackVisitor& Visitor) = 0;

        // ======================== Data Accessors ========================
        // Visitors use these to "pull" data out to perform calculations.
        virtual std::string GetName() const = 0;
        virtual int GetLevel() const = 0;
        virtual int GetDefense() const = 0;
        virtual float GetResistance() const = 0;

        // ======================== State Mutation ========================
        // While the Visitor calculates the number, the Target remains
        // responsible for its own state (Health reduction).
        void ApplyDamage(int Amount)
        {
            Health = (Amount >= Health) ? 0 : Health - Amount;
        }

        int GetHealth() const { return Health; }
        bool IsDead() const { return Health <= 0; }

    protected:
        int Health = 100;
    };

    // =========================================================================
    // CONCRETE ELEMENTS (The Targets)
    // ROLE: Specific entities that contain unique stats and implement 'Accept'.
    // =========================================================================

    class Player : public ITarget
    {
    public:
        Player(int Lvl, int Def, float Res) : Level(Lvl), Defense(Def), Resistance(Res) { Health = 100; }

        void Accept(IAttackVisitor& Visitor) override;

        std::string GetName() const override { return "Hero"; }
        int GetLevel() const override { return Level; }
        int GetDefense() const override { return Defense; }
        float GetResistance() const override { return Resistance; }

    private:
        int Level, Defense;
        float Resistance;
    };

    class Enemy : public ITarget
    {
    public:
        Enemy(std::string Name, int Lvl, int Def, float Res, int BaseHP)
            : TypeName(Name), Level(Lvl), Defense(Def), Resistance(Res) {
            Health = BaseHP;
        }

        void Accept(IAttackVisitor& Visitor) override;

        std::string GetName() const override { return TypeName; }
        int GetLevel() const override { return Level; }
        int GetDefense() const override { return Defense; }
        float GetResistance() const override { return Resistance; }

    private:
        std::string TypeName;
        int Level, Defense;
        float Resistance;
    };

    // =========================================================================
    // CONCRETE VISITORS (The Operations)
    // ROLE: Specialized formulas that treat different targets uniquely.
    // =========================================================================

    // ======================== PHYSICAL ATTACK ========================
    class PhysicalAttack : public IAttackVisitor
    {
    public:
        PhysicalAttack(int Power, DamageContext Context) : BasePower(Power), Ctx(Context) {}

        void Visit(Player& Target) override;
        void Visit(Enemy& Target) override;

    private:
        int BasePower;
        DamageContext Ctx;
    };

    // ======================== MAGIC ATTACK ========================
    class MagicAttack : public IAttackVisitor
    {
    public:
        MagicAttack(int Power, DamageContext Context) : BasePower(Power), Ctx(Context) {}

        void Visit(Player& Target) override;
        void Visit(Enemy& Target) override;

    private:
        int BasePower;
        DamageContext Ctx;
    };

    // =========================================================================
    // DEMO IMPLEMENTATION
    // =========================================================================
    void RunDemo();
}