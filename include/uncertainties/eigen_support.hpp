#pragma once

/**
 * @file eigen_support.hpp
 * @brief Eigen integration for udouble type.
 *
 * Include this header to use udouble as a scalar type in Eigen matrices and vectors.
 *
 * Example:
 * @code
 * #include <Eigen/Dense>
 * #include "uncertainties/eigen_support.hpp"
 *
 * Eigen::Matrix<uncertainties::udouble, 3, 3> A;
 * Eigen::Vector<uncertainties::udouble, 3> b;
 * auto x = A.inverse() * b;  // Uncertainty propagates through matrix operations
 * @endcode
 *
 * @note This header must be included AFTER Eigen headers.
 */

#include <Eigen/Core>
#include "uncertainties/udouble.hpp"
#include "uncertainties/umath.hpp"

namespace Eigen {

/**
 * @brief NumTraits specialization for uncertainties::udouble.
 *
 * This tells Eigen how to work with udouble as a scalar type.
 */
template<>
struct NumTraits<uncertainties::udouble> : NumTraits<double> {
    using Real = uncertainties::udouble;
    using NonInteger = uncertainties::udouble;
    using Literal = uncertainties::udouble;
    using Nested = uncertainties::udouble;

    enum {
        IsComplex = 0,
        IsInteger = 0,
        IsSigned = 1,
        RequireInitialization = 1,  // Derivative map needs initialization
        ReadCost = 10,     // Nominal + derivative map access
        AddCost = 20,      // Map merge operations
        MulCost = 30       // Map operations with multiplication
    };

    static inline Real epsilon() {
        return uncertainties::udouble(NumTraits<double>::epsilon(), 0.0);
    }

    static inline Real dummy_precision() {
        return uncertainties::udouble(NumTraits<double>::dummy_precision(), 0.0);
    }

    static inline int digits10() {
        return NumTraits<double>::digits10();
    }

    static inline Real highest() {
        return uncertainties::udouble(NumTraits<double>::highest(), 0.0);
    }

    static inline Real lowest() {
        return uncertainties::udouble(NumTraits<double>::lowest(), 0.0);
    }

    static inline Real infinity() {
        return uncertainties::udouble(NumTraits<double>::infinity(), 0.0);
    }

    static inline Real quiet_NaN() {
        return uncertainties::udouble(NumTraits<double>::quiet_NaN(), 0.0);
    }
};

} // namespace Eigen

// ADL (Argument-Dependent Lookup) support
// These functions allow Eigen to find math functions via unqualified lookup
namespace uncertainties {

/**
 * @brief Returns the real part (nominal value) for Eigen compatibility.
 */
inline double real(const udouble& x) {
    return x.nominal_value();
}

/**
 * @brief Returns zero (imaginary part) for Eigen compatibility.
 */
inline double imag(const udouble& x) {
    (void)x;
    return 0.0;
}

/**
 * @brief Conjugate (identity for real types) for Eigen compatibility.
 */
inline udouble conj(const udouble& x) {
    return x;
}

/**
 * @brief Absolute value squared for Eigen compatibility.
 */
inline udouble abs2(const udouble& x) {
    return x * x;
}

/**
 * @brief Check if value is finite.
 */
inline bool isfinite(const udouble& x) {
    return std::isfinite(x.nominal_value()) && std::isfinite(x.stddev());
}

/**
 * @brief Check if value is NaN.
 */
inline bool isnan(const udouble& x) {
    return std::isnan(x.nominal_value()) || std::isnan(x.stddev());
}

/**
 * @brief Check if value is infinite.
 */
inline bool isinf(const udouble& x) {
    return std::isinf(x.nominal_value()) || std::isinf(x.stddev());
}

} // namespace uncertainties
