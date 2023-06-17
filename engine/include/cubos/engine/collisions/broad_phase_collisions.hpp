/// @file
/// @brief Contains the BroadPhaseCollisions resource.

#pragma once

#include <unordered_map>
#include <unordered_set>
#include <vector>

#include <cubos/core/ecs/entity_manager.hpp>

using cubos::core::ecs::Entity;

namespace cubos::engine
{
    /// Used to store data used in broad phase collision detection.
    struct BroadPhaseCollisions
    {
        using Candidate = std::pair<Entity, Entity>; ///< A pair of entities that may collide.

        /// @brief Collision type for each pair of colliders.
        enum class CollisionType
        {
            BoxBox = 0,
            BoxCapsule,
            BoxPlane,
            BoxSimplex,
            CapsuleCapsule,
            CapsulePlane,
            CapsuleSimplex,
            PlanePlane,
            PlaneSimplex,
            SimplexSimplex,

            Count ///< Used to get the amount of collision types.
        };

        /// @brief Marker used for sweep and prune.
        struct SweepMarker
        {
            Entity entity; ///< Entity referenced by the marker.
            bool isMin;    ///< Whether the marker is a min or max marker.
        };

        /// List of ordered sweep markers for each axis. Stores the index of the marker in mMarkers.
        std::vector<SweepMarker> markersPerAxis[3];

        /// Maps of overlapping entities for each axis calculated by sweep and prune.
        /// For each each map, the key is an entity and the value is a list of entities that overlap with the key.
        /// Symmetrical pairs are not stored.
        std::unordered_map<Entity, std::vector<Entity>> sweepOverlapMaps[3];

        /// Set of active entities during sweep for each axis.
        std::unordered_set<Entity> activePerAxis[3];

        /// List of collision candidates for each collision type. The index of the vector is the collision type.
        std::vector<std::vector<Candidate>> candidatesPerType{static_cast<std::size_t>(CollisionType::Count)};

        /// Adds an entity to the list of entities tracked by sweep and prune.
        /// @param entity The entity to add.
        void addEntity(Entity entity);

        /// Removes an entity from the list of entities tracked by sweep and prune.
        /// @param entity The entity to remove.
        void removeEntity(Entity entity);

        /// Clears the list of entities tracked by sweep and prune.
        void clearEntities();

        /// Adds a collision candidate to the list of candidates for a specific collision type.
        /// @param type The collision type.
        /// @param candidate The collision candidate.
        void addCandidate(CollisionType type, Candidate candidate);

        /// @return The collision candidates for a specific collision type.
        /// @param resource The BroadPhaseCollisions resource.
        /// @param type The collision type.
        const std::vector<Candidate>& getCandidates(CollisionType type);
    };
} // namespace cubos::engine