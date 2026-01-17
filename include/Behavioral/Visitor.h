#pragma once

#include "FunctionLibrary/HelperFunctionLibrary.h"

namespace VIS
{
    struct DamageContext
    {
        float CritChance = 0.0f;
        float CritMultiplier = 1.5f;
        float Variance = 0.2f;
    };

    
    // THE VISITOR INTERFACE
    // Why specific overloads instead of a single 'Visit(ITarget&)'?
    // This enables "Double Dispatch". If a base class pointer were used,
    // it loses the specific identity of the target. By having explicit
    // Player and Enemy overloads, so the formula can resolve the correct
    // calculation at runtime without needing dynamic_cast or 'switch' statements.
    
    class IAttackVisitor
    {
    public:
        virtual ~IAttackVisitor() = default;

        // These allow the Visitor to see the "Concrete" type of the target
        virtual void Visit(class Player& Target) = 0;
        virtual void Visit(class Enemy& Target) = 0;
    };

    
    // THE ELEMENT INTERFACE (The "Passive" Container)
    // Why use the 'Accept' method?
    // This is the first half of the Double Dispatch. It allows the Target
    // to "hand over control" to a Visitor. This keeps the Target classes
    // pure: they hold data (Health, Def) but don't contain any logic
    // regarding HOW damage is calculated.
    
    class ITarget
    {
    public:
        virtual ~ITarget() = default;

         
        // The Entry Point for the Pattern.
        // Implementations of this simply call: Visitor.Visit(*this);
        
        virtual void Accept(IAttackVisitor& Visitor) = 0;

        // --- Data Accessors ---
        // Visitors use these to "pull" data out to perform calculations.
        virtual std::string GetName() const = 0;
        virtual int GetLevel() const = 0;
        virtual int GetDefense() const = 0;
        virtual float GetResistance() const = 0;

        
        // Mutation Logic:
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

    // --- CONCRETE ELEMENTS ---

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

    // --- CONCRETE VISITORS ---

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

    void RunDemo();
}