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
    // σ_f = sqrt(B² * σ_A² + A² * σ_B²)
    double new_stddev = std::sqrt(
        rhs.nominal_ * rhs.nominal_ * lhs.stddev_ * lhs.stddev_ +
        lhs.nominal_ * lhs.nominal_ * rhs.stddev_ * rhs.stddev_
    );
    return udouble(new_nominal, new_stddev);
}

udouble operator*(const udouble& lhs, const double& rhs)
{
    return udouble(lhs.nominal_ * rhs, lhs.stddev_ * std::abs(rhs));
}

udouble operator*(const double& lhs, const udouble& rhs)
{
    return udouble(lhs * rhs.nominal_, std::abs(lhs) * rhs.stddev_);
}

// Division
udouble operator/(const udouble& lhs, const udouble& rhs)
{
    if (rhs.nominal_ == 0.0) {
        throw std::runtime_error("Division by zero in udouble.");
    }
    double new_nominal = lhs.nominal_ / rhs.nominal_;
    // First-order error propagation: f = A/B
    // σ_f = (1/|B|) * sqrt(σ_A² + (A/B)² * σ_B²)
    double ratio = new_nominal;
    double new_stddev = std::sqrt(
        lhs.stddev_ * lhs.stddev_ + ratio * ratio * rhs.stddev_ * rhs.stddev_
    ) / std::abs(rhs.nominal_);
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
    return udouble(lhs / rhs.nominal_, std::abs(lhs) * rhs.stddev_ / (rhs.nominal_ * rhs.nominal_));
}

// Power
udouble pow(const udouble& base, const udouble& exp)
{
    if (base.nominal_ <= 0.0) {
        throw std::runtime_error("Base of exponentiation (base) must be positive.");
    }
    double new_nominal = std::pow(base.nominal_, exp.nominal_);
    double new_stddev = std::abs(new_nominal) * std::sqrt(std::pow(exp.nominal_/base.nominal_*base.stddev_, 2)
                                                         + std::pow(std::log(base.nominal_) * exp.stddev_, 2));
    return udouble(new_nominal, new_stddev);
}

// Compound assignment operators
udouble& udouble::operator+=(const udouble& rhs)
{
    *this = *this + rhs;
    return *this;
}

udouble& udouble::operator-=(const udouble& rhs)
{
    *this = *this - rhs;
    return *this;
}

udouble& udouble::operator*=(const udouble& rhs)
{
    *this = *this * rhs;
    return *this;
}

udouble& udouble::operator/=(const udouble& rhs)
{
    *this = *this / rhs;
    return *this;
}

udouble& udouble::operator*=(double rhs)
{
    *this = *this * rhs;
    return *this;
}

udouble& udouble::operator/=(double rhs)
{
    *this = *this / rhs;
    return *this;
}

// Comparison operators (compare nominal values)
bool operator==(const udouble& lhs, const udouble& rhs)
{
    return lhs.nominal_ == rhs.nominal_;
}

bool operator!=(const udouble& lhs, const udouble& rhs)
{
    return lhs.nominal_ != rhs.nominal_;
}

bool operator<(const udouble& lhs, const udouble& rhs)
{
    return lhs.nominal_ < rhs.nominal_;
}

bool operator>(const udouble& lhs, const udouble& rhs)
{
    return lhs.nominal_ > rhs.nominal_;
}

bool operator<=(const udouble& lhs, const udouble& rhs)
{
    return lhs.nominal_ <= rhs.nominal_;
}

bool operator>=(const udouble& lhs, const udouble& rhs)
{
    return lhs.nominal_ >= rhs.nominal_;
}

} // namespace uncertainties