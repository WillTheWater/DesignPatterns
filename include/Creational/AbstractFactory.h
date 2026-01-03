#pragma once

#include "FunctionLibrary/HelperFunctionLibrary.h"

// =========================================================================
// CREATIONAL DESIGN PATTERNS: Abstract Factory
// =========================================================================
// "Provide an interface for creating families of related objects
// without specifying their concrete classes."
//
// THE GOAL:
// Demonstrate how Abstract Factory models *NPC ecosystems*,
// where multiple related objects must remain compatible.
//
// GAME CONTEXT:
// NPCs spawned in different BIOMES (Forest, Desert, Swampland).
// Each biome defines a coherent "family" of NPC components.
//
// BENEFIT:
// - Forest NPCs should NOT use Swamp attacks
// - Desert NPCs should NOT drop forest herbs
// - The Spawner should NOT care about biome-specific rules
//
// ABSTRACT FACTORY ENCAPSULATES:
// - "What belongs together in each unique biome?"
// =========================================================================

namespace AFT
{
    // ------------------------------------------------------------------------
    // 1. BASE NPC INTERFACE (What the game knows)
    // ------------------------------------------------------------------------
    // The Game Level and Spawner only know about INPC.
    // They do NOT know what biome the NPCs came from.
    class INPC
    {
    public:
        virtual ~INPC() = default;

        virtual std::string GetName() const = 0;
    };

    // ------------------------------------------------------------------------
    // 2. BIOME-SPECIFIC ROLE INTERFACES
    // ------------------------------------------------------------------------
    // These interfaces represent *roles* inside a biome.
    // Each biome must provide compatible implementations.

    // --- Combat Behavior ---
    class ICombatBehavior
    {
    public:
        virtual ~ICombatBehavior() = default;
        virtual void Attack() const = 0;
    };

    // --- Movement / AI Behavior ---
    class IAIBehavior
    {
    public:
        virtual ~IAIBehavior() = default;
        virtual void Behavior() const = 0;
    };

    // --- Drop Table ---
    class ILootTable
    {
    public:
        virtual ~ILootTable() = default;
        virtual void DropLoot() const = 0;
    };

    // ------------------------------------------------------------------------
    // 3. THE ABSTRACT BIOME FACTORY
    // ------------------------------------------------------------------------
    // This is the CORE of the pattern.
    //
    // Each Biome Factory guarantees that:
    // - Combat
    // -  AI Behavior
    // - Loot
    // belong together and are compatible.
    //
    // The Spawner never asks:
    // "Should this NPC attack with poison or fire?"
    // That decision is defined by the factory.
    class IBiomeNPCFactory
    {
    public:
        virtual ~IBiomeNPCFactory() = default;

        virtual INPC* CreateNPC() = 0;
        virtual ICombatBehavior* CreateCombat() = 0;
        virtual IAIBehavior* CreateBehavior() = 0;
        virtual ILootTable* CreateLootTable() = 0;
    };

    // ------------------------------------------------------------------------
    // 4. CONCRETE PRODUCTS — FOREST BIOME
    // ------------------------------------------------------------------------
    // These classes are NEVER mixed with other biomes.

    class ForestNPC : public INPC
    {
    public:
        std::string GetName() const override { return "Forest Imp"; }
    };

    class ForestCombat : public ICombatBehavior
    {
    public:
        void Attack() const override
        {
            std::cout << "Attacks with sticks and stones.\n";
        }
    };

    class ForestBehavior : public IAIBehavior
    {
    public:
        void Behavior() const override
        {
            std::cout << "Hides behind trees and wait to ambush.\n";
        }
    };

    class ForestLoot : public ILootTable
    {
    public:
        void DropLoot() const override
        {
            std::cout << "Drops healing herbs, magic stones and enchanted wood.\n";
        }
    };

    // ------------------------------------------------------------------------
    // 5. CONCRETE PRODUCTS — DESERT BIOME
    // ------------------------------------------------------------------------

    class DesertNPC : public INPC
    {
    public:
        std::string GetName() const override { return "Desert Raider"; }
    };

    class DesertCombat : public ICombatBehavior
    {
    public:
        void Attack() const override
        {
            std::cout << "Throws fire and sand.\n";
        }
    };

    class DesertBehavior : public IAIBehavior
    {
    public:
        void Behavior() const override
        {
            std::cout << "Uses hit-and-run tactics.\n";
        }
    };

    class DesertLoot : public ILootTable
    {
    public:
        void DropLoot() const override
        {
            std::cout << "Drops minerals and cloth.\n";
        }
    };

    // ------------------------------------------------------------------------
    // 6. EXTENDABILITY EXAMPLE
    // ------------------------------------------------------------------------
    // Creating a new biome factory follows the Dependency Injection Principle &
    // the Open-Closed Principle.
    //
    // Step 1. Create a new Biome Type eg Swamp.

    class SwampNPC : public INPC
    {
    public:
        std::string GetName() const override { return "Swamp Gator"; }
    };

    // Step 2. Create the unique Attack, AI Behavior and Loot Table.

    class SwampCombat : public ICombatBehavior
    {
    public:
        void Attack() const override
        {
            std::cout << "Spits poison and bites.\n";
        }
    };

    class SwampBehavior : public IAIBehavior
    {
    public:
        void Behavior() const override
        {
            std::cout << "Chases endlessly in packs.\n";
        }
    };

    class SwampLoot : public ILootTable
    {
    public:
        void DropLoot() const override
        {
            std::cout << "Drops swamp scales and teeth.\n";
        }
    };

    // Step 3. Create the Sub-factory

    class SwampBiomeFactory : public IBiomeNPCFactory
    {
    public:
        INPC* CreateNPC() override;
        ICombatBehavior* CreateCombat() override;
        IAIBehavior* CreateBehavior() override;
        ILootTable* CreateLootTable() override;
    };

    // All of this plugs right into the spawner.

    // ------------------------------------------------------------------------
    // 7. CONCRETE BIOME FACTORIES
    // ------------------------------------------------------------------------
    // Each factory ENFORCES consistency.
    // No invalid combinations are possible.

    class ForestBiomeFactory : public IBiomeNPCFactory
    {
    public:
        INPC* CreateNPC() override;
        ICombatBehavior* CreateCombat() override;
        IAIBehavior* CreateBehavior() override;
        ILootTable* CreateLootTable() override;
    };

    class DesertBiomeFactory : public IBiomeNPCFactory
    {
    public:
        INPC* CreateNPC() override;
        ICombatBehavior* CreateCombat() override;
        IAIBehavior* CreateBehavior() override;
        ILootTable* CreateLootTable() override;
    };

    // ------------------------------------------------------------------------
    // 8. THE CLIENT — NPC SPAWNER
    // ------------------------------------------------------------------------
    // The Spawner depends ONLY on the abstract factory.
    //
    // It does not:
    // - Know what biome is active
    // - Know what combat style is used
    // - Know what loot drops
    //
    // This is Dependency Inversion in practice.
    class NPCSpawner
    {
    public:
        void SetBiomeFactory(IBiomeNPCFactory* NewFactory) { CurrentFactory = NewFactory; }
        void SpawnNPC();

    private:
        IBiomeNPCFactory* CurrentFactory = nullptr;
    };

    // ------------------------------------------------------------------------
    // 9. DEMO
    // ------------------------------------------------------------------------
    // Switching biomes automatically switches:
    // - NPC visuals
    // - Combat behavior
    // - AI behavior
    // - Loot tables
    //
    // WITHOUT modifying the Spawner.
    void RunDemo();
}
