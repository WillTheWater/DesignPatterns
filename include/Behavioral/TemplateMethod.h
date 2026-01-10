#pragma once

#include "FunctionLibrary/HelperFunctionLibrary.h"

// =========================================================================
// BEHAVIORAL DESIGN PATTERNS: Template Method
// =========================================================================
// "Define the skeleton of an algorithm in an operation, deferring some steps to subclasses."
//
// THE GOAL:
// Fix the structure of a multi-step process while allowing specific steps 
// to be customized. This prevents logic duplication and enforces a strict 
// order of operations.
//
// THE EXAMPLE:
// Fantasy Crafting System.
// 1. Abstract Class (CraftingProcess): Defines the Template Method 'CraftItem'.
// 2. Concrete Primitives: Steps like 'PrepareMaterials' and 'ShapeMaterial'.
// 3. Hooks: Optional steps like 'ApplyHeat' or 'ApplyTreatment'.
// 4. Control Hooks: Booleans like 'RequiresHeat' to toggle algorithm branches.
//
// THE SCENARIO:
// Different professions follow the same "Gather -> Process -> Finish" workflow.
// Without Template Method: Each class duplicates the sequencing logic, leading to errors.
// With Template Method: The base class enforces the sequence. Subclasses only 
// provide the specific "how-to" for the steps relevant to their craft.
// =========================================================================

namespace TMP
{
    // ------------------------------------------------------------------------
    // 1. THE ABSTRACT CLASS (The Blueprint)
    // ------------------------------------------------------------------------
    class CraftingProcess
    {
    public:
        virtual ~CraftingProcess() = default;

        // --- THE TEMPLATE METHOD ---
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

    // ------------------------------------------------------------------------
    // 2. CONCRETE IMPLEMENTATIONS (The Variations)
    // ------------------------------------------------------------------------

    // Blacksmith: Requires Heat and Cooling (Treatment).
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

    // Woodworker: No Heat. Requires Varnishing (Treatment).
    class Woodworking : public CraftingProcess
    {
    protected:
        void PrepareMaterials() override;
        void ShapeMaterial() override;
        bool RequiresTreatment() override { return true; }
        void ApplyTreatment() override;
    };

    // Alchemist: Requires Heat. No Treatment (Cooling not needed).
    class Alchemy : public CraftingProcess
    {
    protected:
        void PrepareMaterials() override;
        void ShapeMaterial() override;
        bool RequiresHeat() override { return true; }
        void ApplyHeat() override;
    };

    // Tailor: No Heat. No Treatment. Just raw preparation and shaping.
    class Tailoring : public CraftingProcess
    {
    protected:
        void PrepareMaterials() override;
        void ShapeMaterial() override;
    };

    void RunDemo();
}