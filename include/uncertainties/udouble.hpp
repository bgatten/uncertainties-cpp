#pragma once

/**
 * @file udouble.hpp
 * @brief Core class for values with uncertainties and correlation tracking.
 *
 * This implementation tracks correlations between variables by storing
 * partial derivatives with respect to original atomic variables. This allows
 * correct uncertainty propagation for expressions like x - x = 0 ± 0.
 */

#include <cmath>
#include <stdexcept>
#include <ostream>
#include <sstream>
#include <iomanip>
#include <string>
#include <unordered_map>
#include <cstdint>

#include "uncertainties/variable_registry.hpp"

namespace uncertainties {

/**
 * @class udouble
 * @brief A double-precision floating-point value with associated uncertainty.
 *
 * The udouble class represents a value with an associated standard deviation
 * (uncertainty). It supports automatic error propagation through arithmetic
 * operations using first-order Taylor expansion (linear approximation).
 *
 * This implementation tracks correlations between variables, so operations
 * like x - x correctly yield 0 ± 0, and x + x yields 2x ± 2σ.
 *
 * Example usage:
 * @code
 * uncertainties::udouble x(10.0, 0.5);  // 10.0 ± 0.5
 * uncertainties::udouble y(20.0, 1.0);  // 20.0 ± 1.0
 * uncertainties::udouble z = x + y;     // Automatic error propagation
 * std::cout << z << std::endl;          // Output: 30 ± 1.11803
 *
 * // Correlation tracking:
 * uncertainties::udouble w = x - x;     // 0 ± 0 (not 0 ± 0.707)
 * @endcode
 */
class udouble {
public:
    /// Type alias for the derivative map
    using DerivativeMap = std::unordered_map<uint64_t, double>;

private:
    double nominal_;           ///< The nominal (central) value
    DerivativeMap derivatives_; ///< Partial derivatives w.r.t. atomic variables

    /**
     * @brief Private constructor for derived values.
     * @param nominal The nominal value
     * @param derivatives The derivative map
     */
    udouble(double nominal, DerivativeMap derivatives)
        : nominal_(nominal), derivatives_(std::move(derivatives)) {}

    // Allow operators to use private constructor
    friend udouble operator+(const udouble& lhs, const udouble& rhs);
    friend udouble operator-(const udouble& lhs, const udouble& rhs);
    friend udouble operator*(const udouble& lhs, const udouble& rhs);
    friend udouble operator*(const double& lhs, const udouble& rhs);
    friend udouble operator*(const udouble& lhs, const double& rhs);
    friend udouble operator/(const udouble& lhs, const udouble& rhs);
    friend udouble operator/(const udouble& lhs, const double& rhs);
    friend udouble operator/(const double& lhs, const udouble& rhs);
    friend udouble pow(const udouble& base, const udouble& exponent);

    // Math functions need access too
    friend udouble sin(const udouble& x);
    friend udouble cos(const udouble& x);
    friend udouble tan(const udouble& x);
    friend udouble asin(const udouble& x);
    friend udouble acos(const udouble& x);
    friend udouble atan(const udouble& x);
    friend udouble atan2(const udouble& y, const udouble& x);
    friend udouble sinh(const udouble& x);
    friend udouble cosh(const udouble& x);
    friend udouble tanh(const udouble& x);
    friend udouble asinh(const udouble& x);
    friend udouble acosh(const udouble& x);
    friend udouble atanh(const udouble& x);
    friend udouble exp(const udouble& x);
    friend udouble log(const udouble& x);
    friend udouble log10(const udouble& x);
    friend udouble sqrt(const udouble& x);
    friend udouble abs(const udouble& x);
    friend udouble hypot(const udouble& x, const udouble& y);

public:
    /// @name Constructors
    /// @{

    /**
     * @brief Default constructor. Initializes to 0 ± 0.
     */
    udouble() noexcept : nominal_(0.0) {}

    /**
     * @brief Implicit conversion from double with zero uncertainty.
     * @param nominal The nominal value
     *
     * Creates a constant with no uncertainty (empty derivative map).
     */
    udouble(double nominal) noexcept : nominal_(nominal) {}

    /**
     * @brief Construct an atomic udouble with specified nominal value and uncertainty.
     * @param nominal The nominal (central) value
     * @param stddev The standard deviation (must be non-negative)
     * @throws std::invalid_argument if stddev is negative
     *
     * Creates an "atomic" variable that is registered with the global registry.
     * This variable will be tracked through all subsequent operations.
     */
    udouble(double nominal, double stddev)
        : nominal_(nominal)
    {
        if (stddev < 0.0) {
            throw std::invalid_argument("Standard deviation cannot be negative.");
        }
        if (stddev > 0.0) {
            uint64_t id = detail::VariableRegistry::instance().register_variable(stddev);
            derivatives_[id] = 1.0;
        }
        // If stddev == 0, derivatives_ remains empty (constant)
    }

    /// @}

    /// @name Accessors
    /// @{

    /**
     * @brief Get the nominal (central) value.
     * @return The nominal value
     */
    double nominal_value() const noexcept { return nominal_; }

    /**
     * @brief Get the standard deviation (uncertainty).
     * @return The standard deviation, computed from derivatives
     *
     * The uncertainty is computed as:
     * σ = sqrt(Σ (∂f/∂xi)² * σi²)
     * where xi are the original atomic variables.
     */
    double stddev() const {
        if (derivatives_.empty()) {
            return 0.0;
        }

        double variance = 0.0;
        const auto& registry = detail::VariableRegistry::instance();

        for (const auto& [id, deriv] : derivatives_) {
            double original_stddev = registry.get_stddev(id);
            variance += deriv * deriv * original_stddev * original_stddev;
        }

        return std::sqrt(variance);
    }

    /**
     * @brief Get the derivative map.
     * @return Reference to the map of variable IDs to partial derivatives
     */
    const DerivativeMap& derivatives() const noexcept { return derivatives_; }

    /**
     * @brief Get the number of contributing atomic variables.
     * @return Number of variables in the derivative map
     */
    size_t num_variables() const noexcept { return derivatives_.size(); }

    /**
     * @brief Check if this is an atomic variable (created with explicit stddev).
     * @return true if this variable has exactly one derivative entry with value 1.0
     */
    bool is_atomic() const noexcept {
        return derivatives_.size() == 1 &&
               derivatives_.begin()->second == 1.0;
    }

    /**
     * @brief Create an independent copy with the same nominal value and uncertainty.
     * @return A new udouble with a new unique ID
     *
     * This creates a new atomic variable that is statistically independent
     * from the original, even though it has the same nominal value and stddev.
     */
    udouble independent_copy() const {
        return udouble(nominal_, stddev());
    }

    /**
     * @brief Set the nominal value.
     * @param value The new nominal value
     */
    void set_nominal_value(double value) noexcept { nominal_ = value; }

    /**
     * @brief Set the standard deviation (creates a new atomic variable).
     * @param value The new standard deviation (must be non-negative)
     * @throws std::invalid_argument if value is negative
     *
     * @note This creates a new atomic variable, discarding any existing
     * correlation information.
     */
    void set_stddev(double value) {
        if (value < 0.0) {
            throw std::invalid_argument("Standard deviation cannot be negative.");
        }
        derivatives_.clear();
        if (value > 0.0) {
            uint64_t id = detail::VariableRegistry::instance().register_variable(value);
            derivatives_[id] = 1.0;
        }
    }

    /// @}

    /// @name Unary Operators
    /// @{

    /** @brief Unary plus (returns a copy) */
    udouble operator+() const { return *this; }

    /** @brief Unary negation (negates nominal value and derivatives) */
    udouble operator-() const {
        DerivativeMap neg_derivs;
        for (const auto& [id, deriv] : derivatives_) {
            neg_derivs[id] = -deriv;
        }
        return udouble(-nominal_, std::move(neg_derivs));
    }

    /// @}

    /// @name Compound Assignment Operators
    /// @{

    udouble& operator+=(const udouble& rhs);
    udouble& operator-=(const udouble& rhs);
    udouble& operator*=(const udouble& rhs);
    udouble& operator/=(const udouble& rhs);
    udouble& operator*=(double rhs);
    udouble& operator/=(double rhs);

    /// @}

    /// @name Comparison Operators
    /// @{
    /// @note Comparisons are based on nominal values only.

    friend bool operator==(const udouble& lhs, const udouble& rhs);
    friend bool operator!=(const udouble& lhs, const udouble& rhs);
    friend bool operator<(const udouble& lhs, const udouble& rhs);
    friend bool operator>(const udouble& lhs, const udouble& rhs);
    friend bool operator<=(const udouble& lhs, const udouble& rhs);
    friend bool operator>=(const udouble& lhs, const udouble& rhs);

    /// @}

    /// @name Formatting Methods
    /// @{

    /**
     * @brief Format as "value ± uncertainty" with specified precision.
     * @param precision Number of significant digits (default: 6)
     * @return Formatted string
     */
    std::string to_string(int precision = 6) const {
        std::ostringstream oss;
        oss << std::setprecision(precision) << nominal_ << " ± " << stddev();
        return oss.str();
    }

    /**
     * @brief Format in scientific notation.
     * @param precision Number of decimal places (default: 3)
     * @return Formatted string like "1.234e+00 ± 5.600e-02"
     */
    std::string to_scientific(int precision = 3) const {
        std::ostringstream oss;
        oss << std::scientific << std::setprecision(precision)
            << nominal_ << " ± " << stddev();
        return oss.str();
    }

    /**
     * @brief Format in compact notation with uncertainty in parentheses.
     * @param significant_digits Number of significant digits for uncertainty (default: 2)
     * @return Formatted string like "1.234(56)" where 56 is the uncertainty in the last digits
     */
    std::string to_compact(int significant_digits = 2) const {
        double s = stddev();
        if (s == 0.0) {
            std::ostringstream oss;
            oss << nominal_;
            return oss.str();
        }

        // Determine the order of magnitude of the uncertainty
        int uncert_exp = static_cast<int>(std::floor(std::log10(std::abs(s))));
        int display_exp = uncert_exp - significant_digits + 1;

        // Round uncertainty to significant digits
        double scale = std::pow(10.0, -display_exp);
        long long uncert_int = static_cast<long long>(std::round(s * scale));

        // Round nominal to same decimal place
        double rounded_nominal = std::round(nominal_ * scale) / scale;

        // Format the output
        std::ostringstream oss;
        int decimal_places = (display_exp < 0) ? -display_exp : 0;
        oss << std::fixed << std::setprecision(decimal_places) << rounded_nominal
            << "(" << uncert_int << ")";
        return oss.str();
    }

    /// @}
};

/**
 * @brief Stream output operator.
 * @param os Output stream
 * @param val The udouble to output
 * @return Reference to the output stream
 *
 * Outputs in the format "value ± uncertainty".
 */
inline std::ostream& operator<<(std::ostream& os, const udouble& val)
{
    os << val.nominal_value() << " ± " << val.stddev();
    return os;
}

} // namespace uncertainties
