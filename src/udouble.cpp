#include "uncertainties/udouble.hpp"
#include <cmath>
#include <stdexcept>

namespace uncertainties {

namespace {
    // Threshold for pruning near-zero derivatives
    constexpr double PRUNE_THRESHOLD = 1e-300;

    // Helper to prune near-zero derivatives from a map
    void prune_derivatives(udouble::DerivativeMap& derivs) {
        for (auto it = derivs.begin(); it != derivs.end(); ) {
            if (std::abs(it->second) < PRUNE_THRESHOLD) {
                it = derivs.erase(it);
            } else {
                ++it;
            }
        }
    }
}

// Addition: d(a+b)/dx = da/dx + db/dx
udouble operator+(const udouble& lhs, const udouble& rhs)
{
    double new_nominal = lhs.nominal_ + rhs.nominal_;

    // Merge derivative maps
    udouble::DerivativeMap new_derivs = lhs.derivatives_;
    for (const auto& [id, deriv] : rhs.derivatives_) {
        new_derivs[id] += deriv;
    }

    prune_derivatives(new_derivs);
    return udouble(new_nominal, std::move(new_derivs));
}

// Subtraction: d(a-b)/dx = da/dx - db/dx
udouble operator-(const udouble& lhs, const udouble& rhs)
{
    double new_nominal = lhs.nominal_ - rhs.nominal_;

    // Merge derivative maps with subtraction
    udouble::DerivativeMap new_derivs = lhs.derivatives_;
    for (const auto& [id, deriv] : rhs.derivatives_) {
        new_derivs[id] -= deriv;
    }

    prune_derivatives(new_derivs);
    return udouble(new_nominal, std::move(new_derivs));
}

// Multiplication: d(a*b)/dx = b*(da/dx) + a*(db/dx)
udouble operator*(const udouble& lhs, const udouble& rhs)
{
    double new_nominal = lhs.nominal_ * rhs.nominal_;

    udouble::DerivativeMap new_derivs;

    // b * (da/dx)
    for (const auto& [id, deriv] : lhs.derivatives_) {
        new_derivs[id] += rhs.nominal_ * deriv;
    }

    // a * (db/dx)
    for (const auto& [id, deriv] : rhs.derivatives_) {
        new_derivs[id] += lhs.nominal_ * deriv;
    }

    prune_derivatives(new_derivs);
    return udouble(new_nominal, std::move(new_derivs));
}

// Scalar multiplication: d(c*a)/dx = c * (da/dx)
udouble operator*(const udouble& lhs, const double& rhs)
{
    double new_nominal = lhs.nominal_ * rhs;

    udouble::DerivativeMap new_derivs;
    for (const auto& [id, deriv] : lhs.derivatives_) {
        new_derivs[id] = rhs * deriv;
    }

    prune_derivatives(new_derivs);
    return udouble(new_nominal, std::move(new_derivs));
}

udouble operator*(const double& lhs, const udouble& rhs)
{
    return rhs * lhs;
}

// Division: d(a/b)/dx = (1/b)*(da/dx) - (a/b²)*(db/dx)
udouble operator/(const udouble& lhs, const udouble& rhs)
{
    if (rhs.nominal_ == 0.0) {
        throw std::runtime_error("Division by zero in udouble.");
    }

    double new_nominal = lhs.nominal_ / rhs.nominal_;
    double inv_b = 1.0 / rhs.nominal_;
    double a_over_b_sq = lhs.nominal_ / (rhs.nominal_ * rhs.nominal_);

    udouble::DerivativeMap new_derivs;

    // (1/b) * (da/dx)
    for (const auto& [id, deriv] : lhs.derivatives_) {
        new_derivs[id] += inv_b * deriv;
    }

    // -(a/b²) * (db/dx)
    for (const auto& [id, deriv] : rhs.derivatives_) {
        new_derivs[id] -= a_over_b_sq * deriv;
    }

    prune_derivatives(new_derivs);
    return udouble(new_nominal, std::move(new_derivs));
}

// Scalar division: d(a/c)/dx = (1/c) * (da/dx)
udouble operator/(const udouble& lhs, const double& rhs)
{
    if (rhs == 0.0) {
        throw std::runtime_error("Division by zero in udouble.");
    }

    double new_nominal = lhs.nominal_ / rhs;
    double inv_rhs = 1.0 / rhs;

    udouble::DerivativeMap new_derivs;
    for (const auto& [id, deriv] : lhs.derivatives_) {
        new_derivs[id] = inv_rhs * deriv;
    }

    prune_derivatives(new_derivs);
    return udouble(new_nominal, std::move(new_derivs));
}

// Constant divided by udouble: d(c/b)/dx = -c/b² * (db/dx)
udouble operator/(const double& lhs, const udouble& rhs)
{
    if (rhs.nominal_ == 0.0) {
        throw std::runtime_error("Division by zero in udouble.");
    }

    double new_nominal = lhs / rhs.nominal_;
    double coef = -lhs / (rhs.nominal_ * rhs.nominal_);

    udouble::DerivativeMap new_derivs;
    for (const auto& [id, deriv] : rhs.derivatives_) {
        new_derivs[id] = coef * deriv;
    }

    prune_derivatives(new_derivs);
    return udouble(new_nominal, std::move(new_derivs));
}

// Power: d(a^b)/dx = a^b * (b/a * da/dx + ln(a) * db/dx)
udouble pow(const udouble& base, const udouble& exp)
{
    if (base.nominal_ <= 0.0) {
        throw std::runtime_error("Base of exponentiation (base) must be positive.");
    }

    double new_nominal = std::pow(base.nominal_, exp.nominal_);
    double coef_base = new_nominal * exp.nominal_ / base.nominal_;
    double coef_exp = new_nominal * std::log(base.nominal_);

    udouble::DerivativeMap new_derivs;

    // a^b * (b/a) * (da/dx)
    for (const auto& [id, deriv] : base.derivatives_) {
        new_derivs[id] += coef_base * deriv;
    }

    // a^b * ln(a) * (db/dx)
    for (const auto& [id, deriv] : exp.derivatives_) {
        new_derivs[id] += coef_exp * deriv;
    }

    prune_derivatives(new_derivs);
    return udouble(new_nominal, std::move(new_derivs));
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
