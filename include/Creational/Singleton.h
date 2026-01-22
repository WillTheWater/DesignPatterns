#pragma once

#include "FunctionLibrary/HelperFunctionLibrary.h"

// =========================================================================
// CREATIONAL DESIGN PATTERNS: Singleton Pattern
// =========================================================================
// "Ensure a class has only one instance, and provide a global point of access to it."
//
// THE GOAL:
// The Singleton pattern is used to coordinate a unique, centralized resource 
// that must exist exactly once throughout the lifetime of the application. 
// It prevents "Multiple Source of Truth" errors by strictly controlling 
// instantiation.
//
// THE EXAMPLE:
// This implementation features a Modern Thread-Safe Asset Manager that 
// utilizes Template Specialization to handle distinct asset types:
// 1. [TEXTURES]: Managed in a dedicated internal hash map.
// 2. [SOUNDS]: Managed separately to prevent type-pollution.
// 3. [FONTS]: Demonstrates how the same manager handles diverse resources.
//
// THE BENEFIT:
// * Resource Safety: Guarantees that global states (like GPU memory or File 
//   Handles) aren't duplicated or overwritten.
// * Simplified Access: Systems like UI, Physics, or Audio can all reach 
//   the same manager without passing complex pointers through constructors.
// * Type Safety: Using templates allows a single manager to provide specialized 
//   logic for different data types.
// =========================================================================

namespace SGT
{
    // ------------------------------------------------------------------------
    // ASSET TYPES (MOCK)
    // ROLE: Type-safe tags used to differentiate internal storage logic.
    // ------------------------------------------------------------------------
    struct Texture {};
    struct Sound {};
    struct Font {};

    // =========================================================================
    // THE SINGLETON CLASS: AssetManager
    // ROLE: The unique authority for loading and caching game resources.
    // =========================================================================
    class AssetManager
    {
    public:
        // ------------------------------------------------------------------------
        // THE GLOBAL ACCESS POINT (Meyers' Singleton)
        // ------------------------------------------------------------------------
        // Static method to access the single instance. This approach ensures 
        // the instance is created lazily (only when first called) and is 
        // thread-safe by default in C++11 and later.
        // ------------------------------------------------------------------------
        static AssetManager& GetInstance();

        // ------------------------------------------------------------------------
        // PUBLIC INTERFACE: LoadAsset
        // ------------------------------------------------------------------------
        // 'typename T' allows this function to accept Texture, Sound, or Font.
        // The implementation uses Template Specialization to 
        // ensure the correct cache is updated based on the type provided.
        // ------------------------------------------------------------------------
        template <typename T>
        void LoadAsset(const std::string& AssetPath);

    private:
        // =========================================================================
        // SINGLETON ENFORCEMENT RULES
        // =========================================================================

        // 1. PRIVATE CONSTRUCTOR:
        // Prevents external code from creating new instances via 'new' or stack 
        // allocation. The manager can only be instantiated inside GetInstance().
        AssetManager();

        // 2. DELETE COPY CONSTRUCTOR:
        // Prevents the creation of a second instance via: AssetManager MyMgr = GetInstance();
        AssetManager(const AssetManager&) = delete;

        // 3. DELETE ASSIGNMENT OPERATOR:
        // Prevents overwriting the singleton via: AssetManagerInstance = OtherManager;
        AssetManager& operator=(const AssetManager&) = delete;

        // 4. DESTRUCTOR (Implicitly Private):
        // Handled automatically by the static scope at application exit.

        // -------------------------------------------------------------------------
        // INTERNAL RESOURCE CACHES
        // SCOPE: Private storage to prevent external direct manipulation.
        // -------------------------------------------------------------------------
        std::unordered_map<std::string, int> TextureCache;
        std::unordered_map<std::string, int> SoundCache;
        std::unordered_map<std::string, int> FontCache;
    };

    // =========================================================================
    // DEMO IMPLEMENTATION
    // =========================================================================
    void RunDemo();
}