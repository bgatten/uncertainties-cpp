#pragma once

/**
 * @file variable_registry.hpp
 * @brief Global registry for tracking atomic variable uncertainties.
 *
 * This registry stores the original standard deviations of atomic uncertain
 * variables, indexed by unique IDs. This enables correlation tracking by
 * allowing derived values to store partial derivatives with respect to
 * original variables rather than accumulated uncertainties.
 */

#include <atomic>
#include <cstdint>
#include <mutex>
#include <shared_mutex>
#include <stdexcept>
#include <unordered_map>

namespace uncertainties {
namespace detail {

/**
 * @class VariableRegistry
 * @brief Thread-safe singleton registry for atomic variable uncertainties.
 *
 * Each atomic udouble (one created with an explicit stddev) is assigned a
 * unique ID and its original stddev is stored in this registry. Derived
 * values store partial derivatives with respect to these atomic variables,
 * and compute their final uncertainty by combining derivatives with the
 * original stddevs from this registry.
 */
class VariableRegistry {
public:
    /**
     * @brief Get the singleton instance.
     * @return Reference to the global registry
     */
    static VariableRegistry& instance() {
        static VariableRegistry registry;
        return registry;
    }

    /**
     * @brief Register a new atomic variable.
     * @param stddev The standard deviation of the variable
     * @return Unique ID for this variable
     */
    uint64_t register_variable(double stddev) {
        uint64_t id = next_id_.fetch_add(1, std::memory_order_relaxed);
        std::unique_lock lock(mutex_);
        stddevs_[id] = stddev;
        return id;
    }

    /**
     * @brief Get the original stddev for a variable ID.
     * @param id The variable ID
     * @return The original standard deviation
     * @throws std::runtime_error if ID is not found
     */
    double get_stddev(uint64_t id) const {
        std::shared_lock lock(mutex_);
        auto it = stddevs_.find(id);
        if (it == stddevs_.end()) {
            throw std::runtime_error("Unknown variable ID in registry");
        }
        return it->second;
    }

    /**
     * @brief Clear all registrations (for testing purposes).
     */
    void clear() {
        std::unique_lock lock(mutex_);
        stddevs_.clear();
        next_id_.store(1, std::memory_order_relaxed);
    }

    /**
     * @brief Get the number of registered variables.
     * @return Number of variables in the registry
     */
    size_t size() const {
        std::shared_lock lock(mutex_);
        return stddevs_.size();
    }

    // Prevent copying
    VariableRegistry(const VariableRegistry&) = delete;
    VariableRegistry& operator=(const VariableRegistry&) = delete;

private:
    VariableRegistry() = default;

    std::atomic<uint64_t> next_id_{1};  ///< Next available ID (0 reserved)
    mutable std::shared_mutex mutex_;    ///< Protects stddevs_ map
    std::unordered_map<uint64_t, double> stddevs_;  ///< ID -> original stddev
};

} // namespace detail
} // namespace uncertainties
