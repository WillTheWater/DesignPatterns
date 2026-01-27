#pragma once

#include "FunctionLibrary/HelperFunctionLibrary.h"

// =========================================================================
// STRUCTURAL DESIGN PATTERN: Flyweight
// =========================================================================
// "Use sharing to support large numbers of fine-grained objects efficiently."
//
// THE GOAL:
// Reduce memory footprint by sharing heavy "intrinsic" data (Meshes/Textures)
// between many lightweight "extrinsic" objects (Soldiers).
//
// THE BENEFIT:
// * Performance: Drastically reduces RAM usage for repetitive objects.
// * Centralization: Heavy assets are managed in one place (The Factory).
// * Scalability: Allows for thousands of instances where only dozens 
//   would have been possible otherwise.
//
// THE EXAMPLE:
// [LegionaryTexture]: The Flyweight. Heavy shared data (The Face image).
// [RomanSoldierBase]: The Context. Lightweight objects that point to the shared data.
// [TextureFactory]: The Manager. Ensures textures are only loaded once.
// =========================================================================

namespace FLY
{
    // =========================================================================
    // THE COMPONENT (The Interface)
    // ROLE: Defines the contract for all soldier types.
    // =========================================================================
    class IRomanSoldier
    {
    public:
        virtual ~IRomanSoldier() = default;
        virtual void Draw() = 0;
        virtual std::string GetSoldierType() const = 0;
    };

    // =========================================================================
    // THE FLYWEIGHT (The Shared Part / Intrinsic State)
    // ROLE: Holds the heavy data that is identical across many instances.
    // =========================================================================
    class LegionaryTexture
    {
    public:
        LegionaryTexture(const std::string& Filename);
        void Render();
        std::string GetFilename() const;

    private:
        std::string Filename;
    };

    // =========================================================================
    // THE CONTEXTS (The Unique Parts / Extrinsic State)
    // ROLE: Lightweight objects that contain unique state but share the Flyweight.
    // =========================================================================

    // Abstract base to handle the shared pointer—maintaining DRY principles.
    class RomanSoldierBase : public IRomanSoldier
    {
    protected:
        std::shared_ptr<LegionaryTexture> MySharedFace;
        RomanSoldierBase(std::shared_ptr<LegionaryTexture> SharedFace)
            : MySharedFace(SharedFace) {
        }
    };

    // ======================== LEGIONARY ========================
    class Legionary : public RomanSoldierBase
    {
    public:
        Legionary(std::shared_ptr<LegionaryTexture> SharedFace);
        void Draw() override;
        std::string GetSoldierType() const override;
    };

    // ======================== AUXILIA ========================
    class Auxilia : public RomanSoldierBase
    {
    public:
        Auxilia(std::shared_ptr<LegionaryTexture> SharedFace);
        void Draw() override;
        std::string GetSoldierType() const override;
    };

    // ======================== PRAETORIAN ========================
    class Praetorian : public RomanSoldierBase
    {
    public:
        Praetorian(std::shared_ptr<LegionaryTexture> SharedFace);
        void Draw() override;
        std::string GetSoldierType() const override;
    };

    // =========================================================================
    // THE FACTORY / MANAGER (The Loader)
    // ROLE: Controls Flyweight creation and ensures assets are reused.
    // =========================================================================
    class TextureFactory
    {
    public:
        // Returns an existing texture if available, or creates a new one if not.
        std::shared_ptr<LegionaryTexture> GetTexture(const std::string& Filename);

    private:
        std::unordered_map<std::string, std::shared_ptr<LegionaryTexture>> TextureCache;
    };

    // =========================================================================
    // DEMO IMPLEMENTATION
    // =========================================================================
    void RunDemo();
}