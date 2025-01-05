#include "uncertainties/ufloat.hpp"
#include <cmath>
#include <stdexcept>

namespace uncertainties {

// Addition
ufloat operator+(const ufloat& lhs, const ufloat& rhs)
{
    double new_nominal = lhs.nominal_ + rhs.nominal_;
    // For independent uncertainties: stddev = sqrt(σ₁² + σ₂²)
    double new_stddev = std::sqrt(lhs.stddev_ * lhs.stddev_ + rhs.stddev_ * rhs.stddev_);
    return ufloat(new_nominal, new_stddev);
}

// Subtraction
ufloat operator-(const ufloat& lhs, const ufloat& rhs)
{
    double new_nominal = lhs.nominal_ - rhs.nominal_;
    // For independent uncertainties: same as addition
    double new_stddev = std::sqrt(lhs.stddev_ * lhs.stddev_ + rhs.stddev_ * rhs.stddev_);
    return ufloat(new_nominal, new_stddev);
}

// Multiplication
ufloat operator*(const ufloat& lhs, const ufloat& rhs)
{
    double new_nominal = lhs.nominal_ * rhs.nominal_;
    // First-order error propagation: f = x * y
    // σ_f² = (y*σ_x)² + (x*σ_y)²
    double new_stddev = std::sqrt(
        (rhs.nominal_ * lhs.stddev_) * (rhs.nominal_ * lhs.stddev_) +
        (lhs.nominal_ * rhs.stddev_) * (lhs.nominal_ * rhs.stddev_)
    );
    return ufloat(new_nominal, new_stddev);
}

// Division
ufloat operator/(const ufloat& lhs, const ufloat& rhs)
{
    if (rhs.nominal_ == 0.0) {
        throw std::runtime_error("Division by zero in ufloat.");
    }
    double new_nominal = lhs.nominal_ / rhs.nominal_;
    // First-order error propagation: f = x / y
    // σ_f² = (σ_x / y)² + (x*σ_y / y²)²
    double new_stddev = std::sqrt(
        (lhs.stddev_ / rhs.nominal_) * (lhs.stddev_ / rhs.nominal_) +
        (lhs.nominal_ * rhs.stddev_ / (rhs.nominal_ * rhs.nominal_)) *
            (lhs.nominal_ * rhs.stddev_ / (rhs.nominal_ * rhs.nominal_))
    );
    return ufloat(new_nominal, new_stddev);
}

} // namespace uncertainties