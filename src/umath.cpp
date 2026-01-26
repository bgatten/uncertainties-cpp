#include <cmath>
#include <stdexcept>
#include "uncertainties/umath.hpp"

namespace uncertainties {

namespace {
    // Threshold for pruning near-zero derivatives
    constexpr double PRUNE_THRESHOLD = 1e-300;

    // Helper to apply chain rule: d(f(g))/dx = f'(g) * (dg/dx)
    udouble::DerivativeMap apply_chain_rule(
        const udouble::DerivativeMap& input_derivs,
        double derivative)
    {
        udouble::DerivativeMap new_derivs;
        for (const auto& [id, deriv] : input_derivs) {
            double new_deriv = derivative * deriv;
            if (std::abs(new_deriv) >= PRUNE_THRESHOLD) {
                new_derivs[id] = new_deriv;
            }
        }
        return new_derivs;
    }
}

// Trigonometric functions

udouble sin(const udouble& x)
{
    double new_nominal = std::sin(x.nominal_value());
    // sin'(x) = cos(x)
    double derivative = std::cos(x.nominal_value());
    return udouble(new_nominal, apply_chain_rule(x.derivatives_, derivative));
}

udouble cos(const udouble& x)
{
    double new_nominal = std::cos(x.nominal_value());
    // cos'(x) = -sin(x)
    double derivative = -std::sin(x.nominal_value());
    return udouble(new_nominal, apply_chain_rule(x.derivatives_, derivative));
}

udouble tan(const udouble& x)
{
    double cos_x = std::cos(x.nominal_value());
    if (cos_x == 0.0) {
        throw std::invalid_argument("Tangent undefined at this value (cos(x) = 0).");
    }
    double new_nominal = std::tan(x.nominal_value());
    // tan'(x) = sec²(x) = 1/cos²(x)
    double derivative = 1.0 / (cos_x * cos_x);
    return udouble(new_nominal, apply_chain_rule(x.derivatives_, derivative));
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
    double derivative = 1.0 / denom;
    return udouble(new_nominal, apply_chain_rule(x.derivatives_, derivative));
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
    double derivative = -1.0 / denom;
    return udouble(new_nominal, apply_chain_rule(x.derivatives_, derivative));
}

udouble atan(const udouble& x)
{
    double val = x.nominal_value();
    double new_nominal = std::atan(val);
    // atan'(x) = 1/(1+x²)
    double derivative = 1.0 / (1.0 + val * val);
    return udouble(new_nominal, apply_chain_rule(x.derivatives_, derivative));
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

    double df_dy = xv / denom;
    double df_dx = -yv / denom;

    udouble::DerivativeMap new_derivs;

    // Apply partial derivatives for y
    for (const auto& [id, deriv] : y.derivatives_) {
        new_derivs[id] += df_dy * deriv;
    }

    // Apply partial derivatives for x
    for (const auto& [id, deriv] : x.derivatives_) {
        new_derivs[id] += df_dx * deriv;
    }

    // Prune near-zero derivatives
    for (auto it = new_derivs.begin(); it != new_derivs.end(); ) {
        if (std::abs(it->second) < PRUNE_THRESHOLD) {
            it = new_derivs.erase(it);
        } else {
            ++it;
        }
    }

    return udouble(new_nominal, std::move(new_derivs));
}

// Hyperbolic functions

udouble sinh(const udouble& x)
{
    double new_nominal = std::sinh(x.nominal_value());
    // sinh'(x) = cosh(x)
    double derivative = std::cosh(x.nominal_value());
    return udouble(new_nominal, apply_chain_rule(x.derivatives_, derivative));
}

udouble cosh(const udouble& x)
{
    double new_nominal = std::cosh(x.nominal_value());
    // cosh'(x) = sinh(x)
    double derivative = std::sinh(x.nominal_value());
    return udouble(new_nominal, apply_chain_rule(x.derivatives_, derivative));
}

udouble tanh(const udouble& x)
{
    double cosh_x = std::cosh(x.nominal_value());
    double new_nominal = std::tanh(x.nominal_value());
    // tanh'(x) = sech²(x) = 1/cosh²(x)
    double derivative = 1.0 / (cosh_x * cosh_x);
    return udouble(new_nominal, apply_chain_rule(x.derivatives_, derivative));
}

// Inverse hyperbolic functions

udouble asinh(const udouble& x)
{
    double val = x.nominal_value();
    double new_nominal = std::asinh(val);
    // asinh'(x) = 1/sqrt(1 + x²)
    double derivative = 1.0 / std::sqrt(1.0 + val * val);
    return udouble(new_nominal, apply_chain_rule(x.derivatives_, derivative));
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
    double derivative = 1.0 / denom;
    return udouble(new_nominal, apply_chain_rule(x.derivatives_, derivative));
}

udouble atanh(const udouble& x)
{
    double val = x.nominal_value();
    if (val <= -1.0 || val >= 1.0) {
        throw std::invalid_argument("atanh input must be in range (-1, 1).");
    }
    double new_nominal = std::atanh(val);
    // atanh'(x) = 1/(1 - x²)
    double derivative = 1.0 / (1.0 - val * val);
    return udouble(new_nominal, apply_chain_rule(x.derivatives_, derivative));
}

// Exponential and logarithmic functions

udouble exp(const udouble& x)
{
    double new_nominal = std::exp(x.nominal_value());
    // exp'(x) = exp(x)
    double derivative = new_nominal;
    return udouble(new_nominal, apply_chain_rule(x.derivatives_, derivative));
}

udouble log(const udouble& x)
{
    if (x.nominal_value() <= 0.0) {
        throw std::invalid_argument("Logarithm input must be greater than zero.");
    }
    double new_nominal = std::log(x.nominal_value());
    // log'(x) = 1/x
    double derivative = 1.0 / x.nominal_value();
    return udouble(new_nominal, apply_chain_rule(x.derivatives_, derivative));
}

udouble log10(const udouble& x)
{
    if (x.nominal_value() <= 0.0) {
        throw std::invalid_argument("log10 input must be greater than zero.");
    }
    double new_nominal = std::log10(x.nominal_value());
    // log10'(x) = 1/(x * ln(10))
    double derivative = 1.0 / (x.nominal_value() * std::log(10.0));
    return udouble(new_nominal, apply_chain_rule(x.derivatives_, derivative));
}

udouble sqrt(const udouble& x)
{
    if (x.nominal_value() <= 0.0) {
        throw std::invalid_argument("sqrt input must be greater than zero.");
    }
    double new_nominal = std::sqrt(x.nominal_value());
    // sqrt'(x) = 1/(2*sqrt(x))
    double derivative = 1.0 / (2.0 * new_nominal);
    return udouble(new_nominal, apply_chain_rule(x.derivatives_, derivative));
}

// Other mathematical functions

udouble abs(const udouble& x)
{
    double val = x.nominal_value();
    double new_nominal = std::abs(val);

    // |x|' = sign(x) for x != 0
    // For x = 0, derivative is undefined but we use 0
    double derivative = (val > 0.0) ? 1.0 : ((val < 0.0) ? -1.0 : 0.0);

    return udouble(new_nominal, apply_chain_rule(x.derivatives_, derivative));
}

udouble hypot(const udouble& x, const udouble& y)
{
    double xv = x.nominal_value();
    double yv = y.nominal_value();
    double new_nominal = std::hypot(xv, yv);

    udouble::DerivativeMap new_derivs;

    if (new_nominal == 0.0) {
        // At origin, derivatives are undefined (0/0)
        // Use a different approach: treat x and y as independent
        // and compute sqrt(σ_x² + σ_y²) using a virtual combination
        // This preserves the original library's behavior at the origin.
        //
        // We create a new result whose stddev() equals sqrt(σ_x² + σ_y²)
        // by combining the derivative maps (as if adding them in quadrature)
        for (const auto& [id, deriv] : x.derivatives_) {
            new_derivs[id] += deriv;
        }
        for (const auto& [id, deriv] : y.derivatives_) {
            new_derivs[id] += deriv;
        }
        return udouble(0.0, std::move(new_derivs));
    }

    // ∂f/∂x = x/f, ∂f/∂y = y/f
    double df_dx = xv / new_nominal;
    double df_dy = yv / new_nominal;

    // Apply partial derivatives for x
    for (const auto& [id, deriv] : x.derivatives_) {
        new_derivs[id] += df_dx * deriv;
    }

    // Apply partial derivatives for y
    for (const auto& [id, deriv] : y.derivatives_) {
        new_derivs[id] += df_dy * deriv;
    }

    // Prune near-zero derivatives
    for (auto it = new_derivs.begin(); it != new_derivs.end(); ) {
        if (std::abs(it->second) < PRUNE_THRESHOLD) {
            it = new_derivs.erase(it);
        } else {
            ++it;
        }
    }

    return udouble(new_nominal, std::move(new_derivs));
}

} // namespace uncertainties
