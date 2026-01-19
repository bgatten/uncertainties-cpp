#pragma once

/**
 * @file umath.hpp
 * @brief Mathematical functions for udouble with automatic error propagation.
 *
 * All functions implement first-order error propagation using the derivative
 * of the function. For a function f(x), the uncertainty is computed as:
 * σ_f = |f'(x)| * σ_x
 *
 * For functions of multiple variables, partial derivatives are used:
 * σ_f = sqrt((∂f/∂x)² * σ_x² + (∂f/∂y)² * σ_y²)
 */

#include "uncertainties/udouble.hpp"

namespace uncertainties {

/// @name Trigonometric Functions
/// @{

/** @brief Compute sine with uncertainty propagation. */
udouble sin(const udouble& x);

/** @brief Compute cosine with uncertainty propagation. */
udouble cos(const udouble& x);

/**
 * @brief Compute tangent with uncertainty propagation.
 * @throws std::invalid_argument if cos(x) = 0
 */
udouble tan(const udouble& x);

/// @}

/// @name Inverse Trigonometric Functions
/// @{

/**
 * @brief Compute arc sine with uncertainty propagation.
 * @param x Value in range [-1, 1]
 * @throws std::invalid_argument if x is outside [-1, 1] or at ±1
 */
udouble asin(const udouble& x);

/**
 * @brief Compute arc cosine with uncertainty propagation.
 * @param x Value in range [-1, 1]
 * @throws std::invalid_argument if x is outside [-1, 1] or at ±1
 */
udouble acos(const udouble& x);

/** @brief Compute arc tangent with uncertainty propagation. */
udouble atan(const udouble& x);

/**
 * @brief Compute two-argument arc tangent with uncertainty propagation.
 * @param y The y-coordinate
 * @param x The x-coordinate
 * @return Angle in radians between -π and π
 * @throws std::invalid_argument if both x and y are zero
 */
udouble atan2(const udouble& y, const udouble& x);

/// @}

/// @name Hyperbolic Functions
/// @{

/** @brief Compute hyperbolic sine with uncertainty propagation. */
udouble sinh(const udouble& x);

/** @brief Compute hyperbolic cosine with uncertainty propagation. */
udouble cosh(const udouble& x);

/** @brief Compute hyperbolic tangent with uncertainty propagation. */
udouble tanh(const udouble& x);

/// @}

/// @name Inverse Hyperbolic Functions
/// @{

/** @brief Compute inverse hyperbolic sine with uncertainty propagation. */
udouble asinh(const udouble& x);

/**
 * @brief Compute inverse hyperbolic cosine with uncertainty propagation.
 * @param x Value >= 1
 * @throws std::invalid_argument if x < 1 or x = 1
 */
udouble acosh(const udouble& x);

/**
 * @brief Compute inverse hyperbolic tangent with uncertainty propagation.
 * @param x Value in range (-1, 1)
 * @throws std::invalid_argument if x is outside (-1, 1)
 */
udouble atanh(const udouble& x);

/// @}

/// @name Exponential and Logarithmic Functions
/// @{

/** @brief Compute exponential (e^x) with uncertainty propagation. */
udouble exp(const udouble& x);

/**
 * @brief Compute natural logarithm with uncertainty propagation.
 * @param x Value > 0
 * @throws std::invalid_argument if x <= 0
 */
udouble log(const udouble& x);

/**
 * @brief Compute base-10 logarithm with uncertainty propagation.
 * @param x Value > 0
 * @throws std::invalid_argument if x <= 0
 */
udouble log10(const udouble& x);

/**
 * @brief Compute square root with uncertainty propagation.
 * @param x Value > 0
 * @throws std::invalid_argument if x <= 0
 */
udouble sqrt(const udouble& x);

/// @}

/// @name Other Mathematical Functions
/// @{

/** @brief Compute absolute value with uncertainty propagation. */
udouble abs(const udouble& x);

/**
 * @brief Compute Euclidean distance sqrt(x² + y²) with uncertainty propagation.
 * @param x First value
 * @param y Second value
 * @return sqrt(x² + y²) with propagated uncertainty
 */
udouble hypot(const udouble& x, const udouble& y);

/// @}

} // namespace uncertainties