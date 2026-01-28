#pragma once

#include "FunctionLibrary/HelperFunctionLibrary.h"

// =========================================================================
// BEHAVIORAL DESIGN PATTERNS: TEMPLATE METHOD
// =========================================================================
// "Define the skeleton of an algorithm in an operation, deferring some steps to subclasses."
//
// THE GOAL:
// Fix the structure of a multi-step process while allowing specific steps 
// to be customized. This prevents logic duplication and enforces a strict 
// order of operations across various implementations.
//
// THE BENEFIT:
// [*] CODE REUSE:     Common algorithm steps are moved into the base class.
// [*] INVERSION:      The base class calls subclass methods, not the other way around.
// [*] ENFORCEMENT:    The sequence of execution is locked via a non-virtual method.
//
// THE EXAMPLE:
// [CraftingProcess]: The Template. Defines the 'CraftItem' sequence.
// [Primitives]:      Required steps (Prepare/Shape) that every craft must implement.
// [Hooks]:           Optional steps (Heat/Treatment) that crafts can toggle or ignore.
// [Concrete Crafts]: Blacksmith, Alchemist, etc., that provide the "how-to".
// =========================================================================

namespace TMP
{
    // =========================================================================
    // THE ABSTRACT CLASS (The Blueprint)
    // ROLE: Defines the 'Template Method' which is a non-virtual function 
    // containing the immutable skeleton. It provides default behavior for 
    // hooks and requires implementation for primitives.
    // =========================================================================
    class CraftingProcess
    {
    public:
        virtual ~CraftingProcess() = default;

        // THE TEMPLATE METHOD
        // Definition of the immutable skeleton. It is non-virtual to prevent 
        // subclasses from altering the order of the crafting stages.
        void CraftItem()
        {
            PrepareMaterials();

            if (RequiresHeat())
            {
                ApplyHeat();
            }

            ShapeMaterial();

            if (RequiresTreatment())
            {
                ApplyTreatment();
            }

            Complete();
        }

    protected:
        // REQUIRED PRIMITIVES: Every craft must gather and shape.
        virtual void PrepareMaterials() = 0;
        virtual void ShapeMaterial() = 0;

        // OPTIONAL HOOKS: Default behavior is provided (No-Op).
        virtual void ApplyHeat() {}
        virtual void ApplyTreatment() {}

        // CONTROL HOOKS: Subclasses override these to toggle algorithm logic.
        virtual bool RequiresHeat() { return false; }
        virtual bool RequiresTreatment() { return false; }

        void Complete() { std::cout << "   [Step] Crafting sequence finished successfully.\n"; }
    };

    // =========================================================================
    // THE CONCRETE IMPLEMENTATIONS (The Variations)
    // ROLE: Specialized workers that fill in the "blanks" of the algorithm.
    // Each class focuses only on its specific domain logic without worrying 
    // about the overall workflow.
    // =========================================================================

    // ======================== BLACKSMITHING (Heavy Metal) ========================
    class Blacksmithing : public CraftingProcess
    {
    protected:
        void PrepareMaterials() override;
        void ShapeMaterial() override;
        bool RequiresHeat() override { return true; }
        void ApplyHeat() override;
        bool RequiresTreatment() override { return true; }
        void ApplyTreatment() override;
    };

    // ======================== WOODWORKING (Natural) ========================
    class Woodworking : public CraftingProcess
    {
    protected:
        void PrepareMaterials() override;
        void ShapeMaterial() override;
        bool RequiresTreatment() override { return true; }
        void ApplyTreatment() override;
    };

    // ======================== ALCHEMY (Chemical) ========================
    class Alchemy : public CraftingProcess
    {
    protected:
        void PrepareMaterials() override;
        void ShapeMaterial() override;
        bool RequiresHeat() override { return true; }
        void ApplyHeat() override;
    };

    // ======================== TAILORING (Fabric) ========================
    class Tailoring : public CraftingProcess
    {
    protected:
        void PrepareMaterials() override;
        void ShapeMaterial() override;
    };

    // =========================================================================
    // DEMO IMPLEMENTATION
    // =========================================================================
    void RunDemo();
}