#pragma once

#include "FunctionLibrary/HelperFunctionLibrary.h"

// =========================================================================
// STRUCTURAL DESIGN PATTERNS: Flyweight
// =========================================================================
// "Use sharing to support large numbers of fine-grained objects efficiently."
//
// THE GOAL:
// Reduce memory footprint by sharing heavy intrinsic data (Meshes/Textures)
// between many lightweight extrinsic objects (Soldiers).
//
// THE EXAMPLE:
// A "Giant Roman Army" Simulation.
// 1. The Flyweight (LegionaryTexture): The heavy shared data (Face image).
// 2. The Contexts (Legionary, Auxilia, Praetorian): The lightweight objects (Soldiers).
//    They do NOT own the texture. They just POINT to it.
// 3. The Factory (TextureFactory): Loads the heavy texture once and gives it to soldiers.
//
// THE SCENARIO:
// We want 1000 Roman Soldiers.
// If every soldier loads a "Legionary.png" texture, we use 1GB of RAM (Waste).
// If every soldier points to ONE "LegionaryTexture", we use only 10KB (Efficient).
//
// BENEFIT:
// "Look at this army! 1000 soldiers, but we only loaded 1 texture!"
// =========================================================================

namespace FLY
{
    // ------------------------------------------------------------------------
    // 1. THE COMPONENT (The Interface)
    // ------------------------------------------------------------------------
    class IRomanSoldier
    {
    public:
        virtual ~IRomanSoldier() = default;
        virtual void Draw() = 0;
        virtual std::string GetSoldierType() const = 0;
    };

    // ------------------------------------------------------------------------
    // 2. THE FLYWEIGHT (The Shared Part / Intrinsic State)
    // ------------------------------------------------------------------------
    class LegionaryTexture
    {
    public:
        LegionaryTexture(const std::string& Filename);
        void Render();
        std::string GetFilename() const;

    private:
        std::string Filename;
    };

    // ------------------------------------------------------------------------
    // 3. THE CONTEXTS (The Unique Parts / Extrinsic State)
    // ------------------------------------------------------------------------

    // Abstract base to handle the shared pointer—avoiding your previous DRY violation.
    class RomanSoldierBase : public IRomanSoldier
    {
    protected:
        std::shared_ptr<LegionaryTexture> MySharedFace;
        RomanSoldierBase(std::shared_ptr<LegionaryTexture> SharedFace) : MySharedFace(SharedFace) {}
    };

    class Legionary : public RomanSoldierBase
    {
    public:
        Legionary(std::shared_ptr<LegionaryTexture> SharedFace);
        void Draw() override;
        std::string GetSoldierType() const override;
    };

    class Auxilia : public RomanSoldierBase
    {
    public:
        Auxilia(std::shared_ptr<LegionaryTexture> SharedFace);
        void Draw() override;
        std::string GetSoldierType() const override;
    };

    class Praetorian : public RomanSoldierBase
    {
    public:
        Praetorian(std::shared_ptr<LegionaryTexture> SharedFace);
        void Draw() override;
        std::string GetSoldierType() const override;
    };

    // ------------------------------------------------------------------------
    // 4. THE FACTORY / MANAGER (The Loader)
    // ------------------------------------------------------------------------
    class TextureFactory
    {
    public:
        // A real factory uses a map to manage resources, not a single static pointer.
        std::shared_ptr<LegionaryTexture> GetTexture(const std::string& Filename);
    private:
        std::unordered_map<std::string, std::shared_ptr<LegionaryTexture>> TextureCache;
    };

    void RunDemo();
}