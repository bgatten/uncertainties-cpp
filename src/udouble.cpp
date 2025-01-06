#include "uncertainties/udouble.hpp"
#include <cmath>
#include <stdexcept>

namespace uncertainties {

// Addition
udouble operator+(const udouble& lhs, const udouble& rhs)
{
    double new_nominal = lhs.nominal_ + rhs.nominal_;
    // For independent uncertainties: stddev = sqrt(σ₁² + σ₂²)
    double new_stddev = std::sqrt(lhs.stddev_ * lhs.stddev_ + rhs.stddev_ * rhs.stddev_);
    return udouble(new_nominal, new_stddev);
}

// Subtraction
udouble operator-(const udouble& lhs, const udouble& rhs)
{
    double new_nominal = lhs.nominal_ - rhs.nominal_;
    // For independent uncertainties: same as addition
    double new_stddev = std::sqrt(lhs.stddev_ * lhs.stddev_ + rhs.stddev_ * rhs.stddev_);
    return udouble(new_nominal, new_stddev);
}

// Multiplication
udouble operator*(const udouble& lhs, const udouble& rhs)
{
    double new_nominal = lhs.nominal_ * rhs.nominal_;
    // First-order error propagation: f = A * B
    // σ_f = |f| * sqrt((σ_A/A)² + (σ_B/B)²)
    double new_stddev = std::abs(new_nominal) * std::sqrt(
        (lhs.stddev_ / lhs.nominal_) * (lhs.stddev_ / lhs.nominal_) +
        (rhs.stddev_ / rhs.nominal_) * (rhs.stddev_ / rhs.nominal_)
    );
    return udouble(new_nominal, new_stddev);
}

udouble operator*(const udouble& lhs, const double& rhs)
{
    return udouble(lhs.nominal_ * rhs, lhs.stddev_ * std::abs(rhs));
}

udouble operator*(const double& lhs, const udouble& rhs)
{
    return udouble(lhs * rhs.nominal_, abs(lhs) * rhs.stddev_);
}

// Division
udouble operator/(const udouble& lhs, const udouble& rhs)
{
    if (rhs.nominal_ == 0.0) {
        throw std::runtime_error("Division by zero in udouble.");
    }
    double new_nominal = lhs.nominal_ / rhs.nominal_;
    // First-order error propagation: f = A/B
    // σ_f = |f| * sqrt((σ_A/A)² + (σ_B/B)²)
    double new_stddev = std::abs(new_nominal) * std::sqrt(
        (lhs.stddev_ / lhs.nominal_) * (lhs.stddev_ / lhs.nominal_) +
        (rhs.stddev_ / rhs.nominal_) * (rhs.stddev_ / rhs.nominal_)
    );
    return udouble(new_nominal, new_stddev);
}

udouble operator/(const udouble& lhs, const double& rhs)
{
    if (rhs == 0.0) {
        throw std::runtime_error("Division by zero in udouble.");
    }
    return udouble(lhs.nominal_ / rhs, lhs.stddev_ / std::abs(rhs));
}

udouble operator/(const double& lhs, const udouble& rhs)
{
    if (rhs.nominal_ == 0.0) {
        throw std::runtime_error("Division by zero in udouble.");
    }
    return udouble(lhs / rhs.nominal_, abs(lhs) * rhs.stddev_ / (rhs.nominal_ * rhs.nominal_));
}

// // Power
// udouble operator**(const udouble& lhs, const udouble& rhs)
// {
//     double new_nominal = std::pow(lhs.nominal_, rhs.nominal_);
//     double new_stddev = std::abs(new_nominal) * std::pow(lhs.stddev_ / lhs.nominal_, rhs.nominal_);
//     return udouble(new_nominal, new_stddev);
// }

} // namespace uncertainties