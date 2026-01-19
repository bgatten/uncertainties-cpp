#pragma once

/**
 * @file udouble.hpp
 * @brief Core class for values with uncertainties
 */

#include <cmath>
#include <stdexcept>
#include <ostream>
#include <sstream>
#include <iomanip>
#include <string>

namespace uncertainties {

/**
 * @class udouble
 * @brief A double-precision floating-point value with associated uncertainty.
 *
 * The udouble class represents a value with an associated standard deviation
 * (uncertainty). It supports automatic error propagation through arithmetic
 * operations using first-order Taylor expansion (linear approximation).
 *
 * @note Uncertainties are assumed to be independent (uncorrelated).
 *
 * Example usage:
 * @code
 * uncertainties::udouble x(10.0, 0.5);  // 10.0 ± 0.5
 * uncertainties::udouble y(20.0, 1.0);  // 20.0 ± 1.0
 * uncertainties::udouble z = x + y;     // Automatic error propagation
 * std::cout << z << std::endl;          // Output: 30 ± 1.11803
 * @endcode
 */
class udouble {
private:
    double nominal_;  ///< The nominal (central) value
    double stddev_;   ///< The standard deviation (uncertainty)

public:
    /// @name Constructors
    /// @{

    /**
     * @brief Default constructor. Initializes to 0 ± 0.
     */
    constexpr udouble() noexcept : nominal_(0.0), stddev_(0.0) {}

    /**
     * @brief Implicit conversion from double with zero uncertainty.
     * @param nominal The nominal value
     */
    constexpr udouble(double nominal) noexcept : nominal_(nominal), stddev_(0.0) {}

    /**
     * @brief Construct a udouble with specified nominal value and uncertainty.
     * @param nominal The nominal (central) value
     * @param stddev The standard deviation (must be non-negative)
     * @throws std::invalid_argument if stddev is negative
     */
    constexpr udouble(double nominal, double stddev)
        : nominal_(nominal), stddev_(stddev)
    {
        if (stddev < 0.0) {
            throw std::invalid_argument("Standard deviation cannot be negative.");
        }
    }

    /// @}

    /// @name Accessors
    /// @{

    /**
     * @brief Get the nominal (central) value.
     * @return The nominal value
     */
    constexpr double nominal_value() const noexcept { return nominal_; }

    /**
     * @brief Get the standard deviation (uncertainty).
     * @return The standard deviation
     */
    constexpr double stddev() const noexcept { return stddev_; }

    /**
     * @brief Set the nominal value.
     * @param value The new nominal value
     */
    constexpr void set_nominal_value(double value) noexcept { nominal_ = value; }

    /**
     * @brief Set the standard deviation.
     * @param value The new standard deviation (must be non-negative)
     * @throws std::invalid_argument if value is negative
     */
    void set_stddev(double value) {
        if (value < 0.0) {
            throw std::invalid_argument("Standard deviation cannot be negative.");
        }
        stddev_ = value;
    }

    /// @}

    /// @name Unary Operators
    /// @{

    /** @brief Unary plus (returns a copy) */
    constexpr udouble operator+() const noexcept { return *this; }

    /** @brief Unary negation (negates nominal value, preserves uncertainty) */
    constexpr udouble operator-() const noexcept { return udouble(-nominal_, stddev_); }

    /// @}

    /// @name Arithmetic Operators
    /// @{

    friend udouble operator+(const udouble& lhs, const udouble& rhs);
    friend udouble operator-(const udouble& lhs, const udouble& rhs);
    friend udouble operator*(const udouble& lhs, const udouble& rhs);
    friend udouble operator*(const double& lhs, const udouble& rhs);
    friend udouble operator*(const udouble& lhs, const double& rhs);
    friend udouble operator/(const udouble& lhs, const udouble& rhs);
    friend udouble operator/(const udouble& lhs, const double& rhs);
    friend udouble operator/(const double& lhs, const udouble& rhs);

    /**
     * @brief Raise base to a power with uncertainty propagation.
     * @param base The base (must be positive)
     * @param exponent The exponent
     * @return base^exponent with propagated uncertainty
     * @throws std::runtime_error if base is not positive
     */
    friend udouble pow(const udouble& base, const udouble& exponent);

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
        oss << std::setprecision(precision) << nominal_ << " ± " << stddev_;
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
            << nominal_ << " ± " << stddev_;
        return oss.str();
    }

    /**
     * @brief Format in compact notation with uncertainty in parentheses.
     * @param significant_digits Number of significant digits for uncertainty (default: 2)
     * @return Formatted string like "1.234(56)" where 56 is the uncertainty in the last digits
     */
    std::string to_compact(int significant_digits = 2) const {
        if (stddev_ == 0.0) {
            std::ostringstream oss;
            oss << nominal_;
            return oss.str();
        }

        // Determine the order of magnitude of the uncertainty
        int uncert_exp = static_cast<int>(std::floor(std::log10(std::abs(stddev_))));
        int display_exp = uncert_exp - significant_digits + 1;

        // Round uncertainty to significant digits
        double scale = std::pow(10.0, -display_exp);
        long long uncert_int = static_cast<long long>(std::round(stddev_ * scale));

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