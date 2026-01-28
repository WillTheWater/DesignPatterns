#pragma once

#include "FunctionLibrary/HelperFunctionLibrary.h"

// =========================================================================
// BEHAVIORAL DESIGN PATTERNS: STRATEGY
// =========================================================================
// "Define a family of algorithms, encapsulate each one, and make them interchangeable."
//
// THE GOAL:
// Decoupling of pathfinding logic from the Navigation system. 
// Enables the mini-map to calculate routes based on varying terrain constraints 
// (Walking vs. Riding vs. Flying) at runtime.
//
// THE BENEFIT:
// [*] INTERCHANGEABLE: Change travel modes at runtime without conditional logic.
// [*] ENCAPSULATION:   Individual pathing rules are isolated in their own classes.
// [*] SCALABILITY:     Add new travel modes (e.g., Swimming) without modifying the Navigator.
//
// THE EXAMPLE:
// [IPathStrategy]: The Interface. Defines the 'CalculateRoute' contract.
// [Walking/Flying]: The Strategies. Isolated algorithms for different terrain.
// [MapNavigator]: The Context. The user of the tools. Executes the active logic.
// =========================================================================

namespace STR
{
    // =========================================================================
    // THE STRATEGY INTERFACE (The Shared Contract)
    // ROLE: Defines the common "language" for all travel modes. By interacting 
    // only with this interface, the MapNavigator remains agnostic of the 
    // specific math or rules used by different mounts.
    // =========================================================================
    class IPathStrategy
    {
    public:
        virtual ~IPathStrategy() = default;
        virtual void CalculatePath(const std::string& Destination) = 0;
        virtual std::string GetModeName() const = 0;
    };

    // =========================================================================
    // THE CONCRETE STRATEGIES (Encapsulated Algorithms)
    // ROLE: Specialized workers that represent a single, isolated algorithm.
    // Separating these prevents the Navigator from becoming a massive, 
    // unmaintainable mess of 'if-else' statements.
    // =========================================================================

    // ======================== WALKING (Ground Logic) ========================
    class WalkingPath : public IPathStrategy
    {
    public:
        void CalculatePath(const std::string& Destination) override;
        std::string GetModeName() const override { return "On Foot"; }
    };

    // ======================== MOUNT (Agile Logic) ========================
    class MountPath : public IPathStrategy
    {
    public:
        void CalculatePath(const std::string& Destination) override;
        std::string GetModeName() const override { return "Bear Mount"; }
    };

    // ======================== FLYING (Direct Logic) ========================
    class FlyingPath : public IPathStrategy
    {
    public:
        void CalculatePath(const std::string& Destination) override;
        std::string GetModeName() const override { return "Griffon Flying Mount"; }
    };

    // =========================================================================
    // THE CONTEXT (The Consumer)
    // ROLE: The high-level Navigator. It does not implement pathfinding logic 
    // itself; it delegates that task to the active strategy. It supports 
    // runtime flexibility via dependency injection.
    // =========================================================================
    class MapNavigator
    {
    public:
        // Dependency is injected, ensuring the Navigator always has a valid behavior.
        MapNavigator(std::shared_ptr<IPathStrategy> InitialStrategy);

        // Delegation: The Navigator simply calls the current strategy.
        void UpdateMiniMap(const std::string& Goal);

        // Runtime Flexibility: Behavior can be swapped instantly.
        void ChangeTravelMode(std::shared_ptr<IPathStrategy> NewStrategy);

        std::string GetCurrentMode() const { return ActiveStrategy->GetModeName(); }

    private:
        // The "Strategy" pointer allows for polymorphic behavior at runtime.
        std::shared_ptr<IPathStrategy> ActiveStrategy;
    };

    // =========================================================================
    // DEMO IMPLEMENTATION
    // =========================================================================
    void RunDemo();
}