#pragma once

#include "FunctionLibrary/HelperFunctionLibrary.h"

// =========================================================================
// CREATIONAL DESIGN PATTERNS: Singleton Pattern
// =========================================================================
// "Ensure a class has only one instance, and provide a global point of access to it."
//
// THE GOAL:
// Prevent multiple instantiations of a class that represents a unique resource.
//
// THE EXAMPLE:
// A generic Asset Manager using Template Specialization.
// 1. Singleton Class: Holds separate reference counts for Textures, Sounds, and Fonts.
// 2. Template Function: LoadAsset<Texture>(), LoadAsset<Sound>(), LoadAsset<Font>().
//    Which could be extended to load any asset type.
//
// BENEFIT:
// One global manager handles multiple asset types using clean, specific code blocks for each type.
// =========================================================================

namespace SGT
{
    // ------------------------------------------------------------------------
    // ASSET TYPES (FAKE)
    // ------------------------------------------------------------------------
    // Sample types as 'Tags' to tell the manager what cache to use.
    struct Texture {};
    struct Sound {};
    struct Font {};

    // ------------------------------------------------------------------------
    // THE SINGLETON CLASS
    // ------------------------------------------------------------------------
    class AssetManager
    {
    public:
        // THE GLOBAL ACCESS POINT
        // Static method to access the single instance.
        static AssetManager& GetInstance();

        // ------------------------------------------------------------------------
        // PUBLIC TEMPLATE DECLARATION
        // ------------------------------------------------------------------------
        // 'typename T' allows this function to accept 'Texture', 'Sound' or 'Font'.
        // The compiler generates separate versions of this function.
        template <typename T>
        void LoadAsset(const std::string& AssetPath);

    private:
        // -------------------------------------------------------------------------
        // SINGLETON RULES (The Enforcements)
        // -------------------------------------------------------------------------

        // 1. PRIVATE CONSTRUCTOR:
        // By making this private, the compiler prevents code like:
        // AssetManager* Mgr = new AssetManager();
        // It forces the use of GetInstance() to access the object.
        AssetManager();

        // 2. DESTRUCTOR (Defaulted):
        // Since we this setup uses Modern C++ approach (static reference), the object is destroyed
        // automatically when the application exits. We don't need a manual Destroy() method.

        // 3. DELETE COPY CONSTRUCTOR:
        // Prevents copying the Singleton via:
        // AssetManager mgr2 = GetInstance();
        AssetManager(const AssetManager&) = delete;

        // 4. DELETE ASSIGNMENT:
        // Prevents assignment via:
        // AssetManager mgr2 = GetInstance();
        // This ensures the singleton cannot be overwritten or copied.
        AssetManager& operator=(const AssetManager&) = delete;

        // -------------------------------------------------------------------------
        // SPECIFIC CACHES
        // -------------------------------------------------------------------------
        // Separate maps for type safety and organization.
        std::unordered_map<std::string, int> TextureCache;
        std::unordered_map<std::string, int> SoundCache;
        std::unordered_map<std::string, int> FontCache;
    };

    // ------------------------------------------------------------------------
    // DEMO
    // ------------------------------------------------------------------------
    void RunDemo();
}