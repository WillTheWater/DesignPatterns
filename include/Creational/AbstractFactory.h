#pragma once

#include "FunctionLibrary/HelperFunctionLibrary.h"

// =========================================================================
// CREATIONAL DESIGN PATTERN: Abstract Factory
// =========================================================================
// "Provide an interface for creating families of related objects 
// without specifying their concrete classes."
//
// THE GOAL:
// Demonstrate how Abstract Factory models "NPC ecosystems," where multiple 
// related objects (NPC, Combat, AI, Loot) must remain compatible based 
// on a specific theme or biome.
//
// THE BENEFIT:
// * Consistency: Ensures Forest NPCs don't use Swamp attacks or drop 
//   Desert loot. Related objects are always created as a coherent "family."
// * Decoupling: The NPCSpawner remains "biome-blind," interacting only 
//   with high-level interfaces.
// * Scalability: Adding a "Volcano Biome" only requires a new factory and 
//   product set; the core spawning logic remains untouched.
//
// THE EXAMPLE:
// [IBiomeNPCFactory]: The Abstract Factory. Defines the creation contract.
// [Forest/Desert/Swamp]: Concrete Factories. They enforce biome consistency.
// [INPC/ICombat/IAI/ILoot]: Abstract Products defining the NPC's traits.
// [NPCSpawner]: The Client. Uses the factory to populate the world.
// =========================================================================

namespace AFT
{
    // =========================================================================
    // ABSTRACT PRODUCTS (The Interfaces)
    // These define the "roles" within our NPC ecosystem.
    // =========================================================================

    class INPC
    {
    public:
        virtual ~INPC() = default;
        virtual std::string GetName() const = 0;
    };

    class ICombatBehavior
    {
    public:
        virtual ~ICombatBehavior() = default;
        virtual void Attack() const = 0;
    };

    class IAIBehavior
    {
    public:
        virtual ~IAIBehavior() = default;
        virtual void Behavior() const = 0;
    };

    class ILootTable
    {
    public:
        virtual ~ILootTable() = default;
        virtual void DropLoot() const = 0;
    };

    // =========================================================================
    // THE ABSTRACT FACTORY
    // The "Contract" that ensures every biome provides a complete NPC family.
    // =========================================================================

    class IBiomeNPCFactory
    {
    public:
        virtual ~IBiomeNPCFactory() = default;

        virtual std::unique_ptr<INPC> CreateNPC() = 0;
        virtual std::unique_ptr<ICombatBehavior> CreateCombat() = 0;
        virtual std::unique_ptr<IAIBehavior> CreateBehavior() = 0;
        virtual std::unique_ptr<ILootTable> CreateLootTable() = 0;
    };

    // =========================================================================
    // CONCRETE PRODUCTS: FOREST BIOME
    // =========================================================================

    class ForestNPC : public INPC
    {
    public:
        std::string GetName() const override { return "Forest Imp"; }
    };

    class ForestCombat : public ICombatBehavior
    {
    public:
        void Attack() const override;
    };

    class ForestBehavior : public IAIBehavior
    {
    public:
        void Behavior() const override;
    };

    class ForestLoot : public ILootTable
    {
    public:
        void DropLoot() const override;
    };

    // =========================================================================
    // CONCRETE PRODUCTS: DESERT BIOME
    // =========================================================================

    class DesertNPC : public INPC
    {
    public:
        std::string GetName() const override { return "Desert Raider"; }
    };

    class DesertCombat : public ICombatBehavior
    {
    public:
        void Attack() const override;
    };

    class DesertBehavior : public IAIBehavior
    {
    public:
        void Behavior() const override;
    };

    class DesertLoot : public ILootTable
    {
    public:
        void DropLoot() const override;
    };

    // =========================================================================
    // CONCRETE PRODUCTS: SWAMP BIOME (Extensibility Example)
    // =========================================================================

    class SwampNPC : public INPC
    {
    public:
        std::string GetName() const override { return "Swamp Gator"; }
    };

    class SwampCombat : public ICombatBehavior
    {
    public:
        void Attack() const override;
    };

    class SwampBehavior : public IAIBehavior
    {
    public:
        void Behavior() const override;
    };

    class SwampLoot : public ILootTable
    {
    public:
        void DropLoot() const override;
    };

    // =========================================================================
    // CONCRETE FACTORIES
    // These classes implement the creation logic for specific biomes.
    // =========================================================================

    class ForestBiomeFactory : public IBiomeNPCFactory
    {
    public:
        std::unique_ptr<INPC> CreateNPC() override;
        std::unique_ptr<ICombatBehavior> CreateCombat() override;
        std::unique_ptr<IAIBehavior> CreateBehavior() override;
        std::unique_ptr<ILootTable> CreateLootTable() override;
    };

    class DesertBiomeFactory : public IBiomeNPCFactory
    {
    public:
        std::unique_ptr<INPC> CreateNPC() override;
        std::unique_ptr<ICombatBehavior> CreateCombat() override;
        std::unique_ptr<IAIBehavior> CreateBehavior() override;
        std::unique_ptr<ILootTable> CreateLootTable() override;
    };

    class SwampBiomeFactory : public IBiomeNPCFactory
    {
    public:
        std::unique_ptr<INPC> CreateNPC() override;
        std::unique_ptr<ICombatBehavior> CreateCombat() override;
        std::unique_ptr<IAIBehavior> CreateBehavior() override;
        std::unique_ptr<ILootTable> CreateLootTable() override;
    };

    // =========================================================================
    // HIGH LEVEL MODULE: NPC SPAWNER
    // The Client that operates on the Abstract Factory interface.
    // =========================================================================

    class NPCSpawner
    {
    public:
        NPCSpawner() = default;

        // INJECTION POINT: Swaps the entire ecosystem at runtime.
        void SetBiomeFactory(IBiomeNPCFactory* NewFactory) { CurrentFactory = NewFactory; }

        void SpawnNPC(); // Logic to create a consistent set of objects

    private:
        IBiomeNPCFactory* CurrentFactory = nullptr;
    };

    // =========================================================================
    // DEMO
    // =========================================================================
    void RunDemo();
}