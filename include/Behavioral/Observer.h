#pragma once

#include "FunctionLibrary/HelperFunctionLibrary.h"
#include <memory>
#include <vector>
#include <functional>

// =========================================================================
// BEHAVIORAL DESIGN PATTERNS: Observer (via Delegates)
// =========================================================================
// "Define a one-to-many dependency between objects so that when one object 
// changes state, all its dependents are notified automatically."
//
// THE GOAL:
// Decouple the 'Source of Truth' (Game Logic) from the 'Listeners' (UI/FX).
// This allows the Game Logic to broadcast events without needing to know 
// which (or how many) systems are actually listening.
//
// THE EXAMPLE:
// Game HUD & Player Stats.
// 1. Subject (StatsComponent): Tracks Health and XP. Broadcasts when they change.
// 2. Observer (HUDWidget): Listens. Caches data and draws UI.
// 3. Bridge (Delegate): Manages the list of listeners safely.
//
// THE SCENARIO:
// When a player takes damage, the StatsComponent simply triggers a broadcast.
// The HUD catches that broadcast and updates its internal bars.
//
// BENEFIT:
// 1. Zero Knowledge: StatsComponent.cpp never includes HUDWidget.h.
// 2. Memory Safety: Using weak_ptr prevents crashing if a UI element is deleted.
// 3. Extensibility: You can add 10 more observers without changing one line 
//    of Player code.
// =========================================================================

namespace OBS
{
    // ------------------------------------------------------------------------
    // 1. THE BASE OBJECT
    // ------------------------------------------------------------------------
    // Provides 'shared_from_this' capability, allowing objects to safely 
    // generate weak pointers to themselves for the Delegate system.
    class Object : public std::enable_shared_from_this<Object> {
    public:
        virtual ~Object() = default;
    };

    // ------------------------------------------------------------------------
    // 2. THE DELEGATE SYSTEM (The "Broadcaster")
    // ------------------------------------------------------------------------
    // This template acts as the 'Subscription Manager'. 
    // It maintains a list of callbacks and handles the logic of notifying them.
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

    // ------------------------------------------------------------------------
    // 3. THE SUBJECT (StatsComponent)
    // ------------------------------------------------------------------------
    // This is the "Source of Truth". It holds the actual data.
    // It owns several Delegates that it triggers whenever data changes.
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

    // ------------------------------------------------------------------------
    // 4. THE OBSERVER (HUDWidget)
    // ------------------------------------------------------------------------
    // This is a "Listener". It has no authority over the data.
    // It simply 'watches' the Subject and updates its internal cache 
    // so it knows what to draw on screen.
    class HUDWidget : public Object
    {
    public:
        // Callback: Triggered by OnHealthChanged
        void UpdateHealth(float Current, float Max);

        // Callback: Triggered by OnXPChanged
        void UpdateXP(float Current, float Max);

        // Callback: Triggered by OnLevelUp
        void UpdateLevel(int NewLevel);

        // Visual Logic: Uses the cached values to draw the bars.
        void DrawHUD();

    private:
        // The Observer's local cache of the Subject's data
        float LastHealth = 100.0f;
        float LastMaxHealth = 100.0f;
        float LastXP = 0.0f;
        float LastMaxXP = 100.0f;
        int LastLevel = 1;
    };

    // ------------------------------------------------------------------------
    // 5. DEMO
    // ------------------------------------------------------------------------
    void RunDemo();
}