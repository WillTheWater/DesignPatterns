#pragma once

#include "FunctionLibrary/HelperFunctionLibrary.h"

// =========================================================================
// BEHAVIORAL DESIGN PATTERNS: Strategy
// =========================================================================
// "Define a family of algorithms, encapsulate each one, and make them interchangeable."
//
// THE GOAL:
// Decoupling of pathfinding logic from the Navigation system. 
// Enables the mini-map to calculate routes based on varying terrain constraints 
// (Walking vs. Riding vs. Flying) at runtime.
//
// THE EXAMPLE:
// Fantasy World Navigation / Mini-Map.
// 1. The Strategy Interface (IPathStrategy): Defines the 'CalculateRoute' contract.
// 2. Concrete Strategies (Walking, Mount, Flying): Implement specific traversal rules.
// 3. The Context (MapNavigator): Maintains the active strategy and executes it.
//
// THE SCENARIO:
// A character switches between travel modes. 
// Walking: Limited by roads and terrain.
// Riding: Can leap over small obstacles (Fences/Streams).
// Flying: Direct line-of-sight pathing, ignoring all ground obstacles.
// =========================================================================

namespace STR
{
    // ------------------------------------------------------------------------
    // 1. THE STRATEGY INTERFACE (The Contract)
    // ------------------------------------------------------------------------
    // This defines a common "language" for all travel modes.
    // By interacting only with this interface, the MapNavigator remains 
    // agnostic of the specific math or rules used by different mounts.
    // ------------------------------------------------------------------------
    class IPathStrategy
    {
    public:
        virtual ~IPathStrategy() = default;
        virtual void CalculatePath(const std::string& Destination) = 0;
        virtual std::string GetModeName() const = 0;
    };

    // ------------------------------------------------------------------------
    // 2. CONCRETE STRATEGIES (Encapsulated Algorithms)
    // ------------------------------------------------------------------------
    // Each class represents a single, isolated algorithm.
    // Putting "Walking" and "Flying" in their own classes prevents a single 
    // "Pathfinder" class from becoming a massive, unmaintainable mess of 
    // 'if-else' statements.
    // ------------------------------------------------------------------------
    class WalkingPath : public IPathStrategy
    {
    public:
        void CalculatePath(const std::string& Destination) override;
        std::string GetModeName() const override { return "On Foot"; }
    };

    class MountPath : public IPathStrategy
    {
    public:
        void CalculatePath(const std::string& Destination) override;
        std::string GetModeName() const override { return "Bear Mount"; }
    };

    class FlyingPath : public IPathStrategy
    {
    public:
        void CalculatePath(const std::string& Destination) override;
        std::string GetModeName() const override { return "Griffon Flying Mount"; }
    };

    // ------------------------------------------------------------------------
    // 3. THE CONTEXT (The Consumer)
    // ------------------------------------------------------------------------
    // The MapNavigator is the "Brain" that uses the tools.
    // It does not implement pathfinding logic itself; it delegates that task
    // to the active strategy. This satisfies the Open-Closed Principle:
    // New mounts can be added to the game without ever touching this class.
    // ------------------------------------------------------------------------
    class MapNavigator
    {
    public:
        // Dependency is injected, ensuring the Navigator always has a valid behavior.
        MapNavigator(std::shared_ptr<IPathStrategy> InitialStrategy);

        // Delegation: The Navigator simply calls the current strategy.
        void UpdateMiniMap(const std::string& Goal);

        // Runtime Flexibility: Behavior can be swapped instantly (e.g., when a player mounts).
        void ChangeTravelMode(std::shared_ptr<IPathStrategy> NewStrategy);

        std::string GetCurrentMode() const { return ActiveStrategy->GetModeName(); }

    private:
        // The "Strategy" pointer allows for polymorphic behavior at runtime.
        std::shared_ptr<IPathStrategy> ActiveStrategy;
    };

    void RunDemo();
}