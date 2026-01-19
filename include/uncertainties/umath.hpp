#pragma once

#include "uncertainties/udouble.hpp"

namespace uncertainties {
    // Trigonometric functions
    udouble sin(const udouble& x);
    udouble cos(const udouble& x);
    udouble tan(const udouble& x);

    // Inverse trigonometric functions
    udouble asin(const udouble& x);
    udouble acos(const udouble& x);
    udouble atan(const udouble& x);
    udouble atan2(const udouble& y, const udouble& x);

    // Hyperbolic functions
    udouble sinh(const udouble& x);
    udouble cosh(const udouble& x);
    udouble tanh(const udouble& x);

    // Inverse hyperbolic functions
    udouble asinh(const udouble& x);
    udouble acosh(const udouble& x);
    udouble atanh(const udouble& x);

    // Exponential and logarithmic functions
    udouble exp(const udouble& x);
    udouble log(const udouble& x);
    udouble log10(const udouble& x);
    udouble sqrt(const udouble& x);

    // Other mathematical functions
    udouble abs(const udouble& x);
    udouble hypot(const udouble& x, const udouble& y);
}