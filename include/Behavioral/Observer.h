#pragma once

#include "FunctionLibrary/HelperFunctionLibrary.h"
#include <memory>
#include <vector>
#include <functional>

// =========================================================================
// BEHAVIORAL DESIGN PATTERN: Observer (via Delegates)
// =========================================================================
// "Define a one-to-many dependency between objects so that when one object 
// changes state, all its dependents are notified automatically."
//
// THE GOAL:
// Decouple the 'Source of Truth' (Game Logic) from the 'Listeners' (UI/FX).
// This allows the Game Logic to broadcast events without needing to know 
// which (or how many) systems are actually listening.
//
// THE BENEFIT:
// * Zero Knowledge: The StatsComponent has no dependency on the HUD or UI headers.
// * Memory Safety: Using weak_ptr prevents "dangling pointers" if an Observer 
//   is destroyed while the Subject is still active.
// * Extensibility: You can add unlimited listeners (Achievement systems, Sound 
//   engines, VFX) without changing a single line of the core logic.
//
// THE EXAMPLE:
// [StatsComponent]: The Subject. Tracks Health/XP. Triggers events when they change.
// [HUDWidget]: The Observer. Watches the Subject and updates visual bars.
// [Delegate]: The Bridge. A template-based manager that handles the subscription list.
// =========================================================================

namespace OBS
{
    // =========================================================================
    // THE BASE OBJECT
    // ROLE: Provides 'shared_from_this' capability, allowing objects to safely 
    // generate weak pointers to themselves for the Delegate system.
    // =========================================================================
    class Object : public std::enable_shared_from_this<Object>
    {
    public:
        virtual ~Object() = default;
    };

    // =========================================================================
    // THE DELEGATE SYSTEM (The Broadcaster)
    // ROLE: The 'Subscription Manager'. It maintains a list of callbacks and 
    // handles the logic of notifying them while cleaning up "dead" listeners.
    // =========================================================================
    template<typename... Args>
    class Delegate
    {
    public:
        // Registers a new listener. Requires a weak pointer to ensure 
        // it doesn't try to notify a 'deleted' object.
        template<typename ClassName>
        void Bind(std::weak_ptr<Object> Obj, void(ClassName::* Callback)(Args...))
        {
            std::function<bool(Args...)> CallbackFunc = [Obj, Callback](Args... args) -> bool
                {
                    if (!Obj.expired())
                    {
                        (static_cast<ClassName*>(Obj.lock().get())->*Callback)(args...);
                        return true;
                    }
                    return false; // Tells the broadcaster this listener is dead
                };
            DelegateCallbacks.push_back(CallbackFunc);
        }

        // Iterates through listeners and executes their logic.
        void Broadcast(Args... args)
        {
            for (auto i = DelegateCallbacks.begin(); i != DelegateCallbacks.end();)
            {
                if ((*i)(args...)) { ++i; }
                else { i = DelegateCallbacks.erase(i); } // Cleanup expired pointers
            }
        }

    private:
        std::vector<std::function<bool(Args...)>> DelegateCallbacks;
    };

    // =========================================================================
    // THE SUBJECT (The Source of Truth)
    // ROLE: Owns the actual data. It triggers Delegates whenever data changes, 
    // regardless of who is listening.
    // =========================================================================
    class StatsComponent : public Object
    {
    public:
        // Broadcasters
        Delegate<float, float> OnHealthChanged;
        Delegate<float, float> OnXPChanged;
        Delegate<int>          OnLevelUp;

        void TakeDamage(float Amount);
        void Heal(float Amount);
        void GainXP(float Amount);

        int GetLevel() const { return Level; }

    private:
        void LevelUp();

        float Health = 100.0f;
        float MaxHealth = 100.0f;
        float CurrentXP = 0.0f;
        float XPToLevel = 100.0f;
        int Level = 1;
    };

    // =========================================================================
    // THE OBSERVER (The Listener)
    // ROLE: A 'passive' system that watches the Subject. It updates its local 
    // cache only when notified, keeping the UI sync efficient.
    // =========================================================================
    class HUDWidget : public Object
    {
    public:
        // Callbacks: Triggered by Subject Delegates
        void UpdateHealth(float Current, float Max);
        void UpdateXP(float Current, float Max);
        void UpdateLevel(int NewLevel);

        // Visual Logic: Renders bars using cached values.
        void DrawHUD();

    private:
        // Local cache of the Subject's data
        float LastHealth = 100.0f;
        float LastMaxHealth = 100.0f;
        float LastXP = 0.0f;
        float LastMaxXP = 100.0f;
        int LastLevel = 1;
    };

    // =========================================================================
    // DEMO IMPLEMENTATION
    // =========================================================================
    void RunDemo();
}