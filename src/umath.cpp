#include <cmath>
#include <stdexcept>
#include "uncertainties/umath.hpp"

namespace uncertainties {

// Trigonometric functions

udouble sin(const udouble& x)
{
    double new_nominal = std::sin(x.nominal_value());
    double new_stddev = std::abs(std::cos(x.nominal_value())) * x.stddev();
    return udouble(new_nominal, new_stddev);
}

udouble cos(const udouble& x)
{
    double new_nominal = std::cos(x.nominal_value());
    double new_stddev = std::abs(std::sin(x.nominal_value())) * x.stddev();
    return udouble(new_nominal, new_stddev);
}

udouble tan(const udouble& x)
{
    double cos_x = std::cos(x.nominal_value());
    if (cos_x == 0.0) {
        throw std::invalid_argument("Tangent undefined at this value (cos(x) = 0).");
    }
    double new_nominal = std::tan(x.nominal_value());
    // tan'(x) = sec²(x) = 1/cos²(x)
    double new_stddev = x.stddev() / (cos_x * cos_x);
    return udouble(new_nominal, new_stddev);
}

// Inverse trigonometric functions

udouble asin(const udouble& x)
{
    double val = x.nominal_value();
    if (val < -1.0 || val > 1.0) {
        throw std::invalid_argument("asin input must be in range [-1, 1].");
    }
    double new_nominal = std::asin(val);
    // asin'(x) = 1/sqrt(1-x²)
    double denom = std::sqrt(1.0 - val * val);
    if (denom == 0.0) {
        throw std::invalid_argument("asin derivative undefined at x = ±1.");
    }
    double new_stddev = x.stddev() / denom;
    return udouble(new_nominal, new_stddev);
}

udouble acos(const udouble& x)
{
    double val = x.nominal_value();
    if (val < -1.0 || val > 1.0) {
        throw std::invalid_argument("acos input must be in range [-1, 1].");
    }
    double new_nominal = std::acos(val);
    // acos'(x) = -1/sqrt(1-x²)
    double denom = std::sqrt(1.0 - val * val);
    if (denom == 0.0) {
        throw std::invalid_argument("acos derivative undefined at x = ±1.");
    }
    double new_stddev = x.stddev() / denom;
    return udouble(new_nominal, new_stddev);
}

udouble atan(const udouble& x)
{
    double val = x.nominal_value();
    double new_nominal = std::atan(val);
    // atan'(x) = 1/(1+x²)
    double new_stddev = x.stddev() / (1.0 + val * val);
    return udouble(new_nominal, new_stddev);
}

udouble atan2(const udouble& y, const udouble& x)
{
    double xv = x.nominal_value();
    double yv = y.nominal_value();
    double new_nominal = std::atan2(yv, xv);
    // ∂f/∂y = x / (x² + y²), ∂f/∂x = -y / (x² + y²)
    double denom = xv * xv + yv * yv;
    if (denom == 0.0) {
        throw std::invalid_argument("atan2 undefined at origin (0, 0).");
    }
    double new_stddev = std::sqrt(
        xv * xv * y.stddev() * y.stddev() +
        yv * yv * x.stddev() * x.stddev()
    ) / denom;
    return udouble(new_nominal, new_stddev);
}

// Hyperbolic functions

udouble sinh(const udouble& x)
{
    double new_nominal = std::sinh(x.nominal_value());
    // sinh'(x) = cosh(x)
    double new_stddev = std::cosh(x.nominal_value()) * x.stddev();
    return udouble(new_nominal, new_stddev);
}

udouble cosh(const udouble& x)
{
    double new_nominal = std::cosh(x.nominal_value());
    // cosh'(x) = sinh(x)
    double new_stddev = std::abs(std::sinh(x.nominal_value())) * x.stddev();
    return udouble(new_nominal, new_stddev);
}

udouble tanh(const udouble& x)
{
    double cosh_x = std::cosh(x.nominal_value());
    double new_nominal = std::tanh(x.nominal_value());
    // tanh'(x) = sech²(x) = 1/cosh²(x)
    double new_stddev = x.stddev() / (cosh_x * cosh_x);
    return udouble(new_nominal, new_stddev);
}

// Inverse hyperbolic functions

udouble asinh(const udouble& x)
{
    double val = x.nominal_value();
    double new_nominal = std::asinh(val);
    // asinh'(x) = 1/sqrt(1 + x²)
    double new_stddev = x.stddev() / std::sqrt(1.0 + val * val);
    return udouble(new_nominal, new_stddev);
}

udouble acosh(const udouble& x)
{
    double val = x.nominal_value();
    if (val < 1.0) {
        throw std::invalid_argument("acosh input must be >= 1.");
    }
    double new_nominal = std::acosh(val);
    // acosh'(x) = 1/sqrt(x² - 1)
    double denom = std::sqrt(val * val - 1.0);
    if (denom == 0.0) {
        throw std::invalid_argument("acosh derivative undefined at x = 1.");
    }
    double new_stddev = x.stddev() / denom;
    return udouble(new_nominal, new_stddev);
}

udouble atanh(const udouble& x)
{
    double val = x.nominal_value();
    if (val <= -1.0 || val >= 1.0) {
        throw std::invalid_argument("atanh input must be in range (-1, 1).");
    }
    double new_nominal = std::atanh(val);
    // atanh'(x) = 1/(1 - x²)
    double new_stddev = x.stddev() / (1.0 - val * val);
    return udouble(new_nominal, new_stddev);
}

// Exponential and logarithmic functions

udouble exp(const udouble& x)
{
    double new_nominal = std::exp(x.nominal_value());
    // exp'(x) = exp(x)
    double new_stddev = new_nominal * x.stddev();
    return udouble(new_nominal, new_stddev);
}

udouble log(const udouble& x)
{
    if (x.nominal_value() <= 0.0) {
        throw std::invalid_argument("Logarithm input must be greater than zero.");
    }
    double new_nominal = std::log(x.nominal_value());
    // log'(x) = 1/x
    double new_stddev = x.stddev() / x.nominal_value();
    return udouble(new_nominal, new_stddev);
}

udouble log10(const udouble& x)
{
    if (x.nominal_value() <= 0.0) {
        throw std::invalid_argument("log10 input must be greater than zero.");
    }
    double new_nominal = std::log10(x.nominal_value());
    // log10'(x) = 1/(x * ln(10))
    double new_stddev = x.stddev() / (x.nominal_value() * std::log(10.0));
    return udouble(new_nominal, new_stddev);
}

udouble sqrt(const udouble& x)
{
    if (x.nominal_value() <= 0.0) {
        throw std::invalid_argument("sqrt input must be greater than zero.");
    }
    double new_nominal = std::sqrt(x.nominal_value());
    // sqrt'(x) = 1/(2*sqrt(x))
    double new_stddev = x.stddev() / (2.0 * new_nominal);
    return udouble(new_nominal, new_stddev);
}

// Other mathematical functions

udouble abs(const udouble& x)
{
    double new_nominal = std::abs(x.nominal_value());
    // |x|' = sign(x), but uncertainty magnitude is preserved
    return udouble(new_nominal, x.stddev());
}

udouble hypot(const udouble& x, const udouble& y)
{
    double xv = x.nominal_value();
    double yv = y.nominal_value();
    double new_nominal = std::hypot(xv, yv);
    if (new_nominal == 0.0) {
        // At origin, uncertainty is sqrt(σ_x² + σ_y²)
        double new_stddev = std::sqrt(x.stddev() * x.stddev() + y.stddev() * y.stddev());
        return udouble(0.0, new_stddev);
    }
    // ∂f/∂x = x/f, ∂f/∂y = y/f
    double new_stddev = std::sqrt(
        xv * xv * x.stddev() * x.stddev() +
        yv * yv * y.stddev() * y.stddev()
    ) / new_nominal;
    return udouble(new_nominal, new_stddev);
}

} // namespace uncertainties
