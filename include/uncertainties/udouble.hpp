#pragma once

#include <cmath>
#include <stdexcept>
#include <ostream>

namespace uncertainties {

class udouble {
private:
    double nominal_;
    double stddev_;
public:
    // Constructors
    udouble() : nominal_(0.0), stddev_(0.0) {}
    udouble(double nominal, double stddev)
        : nominal_(nominal), stddev_(stddev)
    {
        if (stddev < 0.0) {
            throw std::invalid_argument("Standard deviation cannot be negative.");
        }
    }

    // Getters
    double nominal_value() const { return nominal_; }
    double stddev() const { return stddev_; }

    // Setters (optional, if you want to allow changes after construction)
    void set_nominal_value(double value) { nominal_ = value; }
    void set_stddev(double value) {
        if (value < 0.0) {
            throw std::invalid_argument("Standard deviation cannot be negative.");
        }
        stddev_ = value;
    }

    // Operator Overloads
    // ------------------

    // Unary operators
    udouble operator+() const { return *this; }
    udouble operator-() const { return udouble(-nominal_, stddev_); }

    // Addition
    friend udouble operator+(const udouble& lhs, const udouble& rhs);

    // Subtraction
    friend udouble operator-(const udouble& lhs, const udouble& rhs);

    // Multiplication
    friend udouble operator*(const udouble& lhs, const udouble& rhs);
    friend udouble operator*(const double& lhs, const udouble& rhs);
    friend udouble operator*(const udouble& lhs, const double& rhs);

    // Division
    friend udouble operator/(const udouble& lhs, const udouble& rhs);
    friend udouble operator/(const udouble& lhs, const double& rhs);
    friend udouble operator/(const double& lhs, const udouble& rhs);

    // Power
    friend udouble pow(const udouble& base, const udouble& exponent);

    // Compound assignment operators
    udouble& operator+=(const udouble& rhs);
    udouble& operator-=(const udouble& rhs);
    udouble& operator*=(const udouble& rhs);
    udouble& operator/=(const udouble& rhs);
    udouble& operator*=(double rhs);
    udouble& operator/=(double rhs);

    // Comparison operators (compare nominal values)
    friend bool operator==(const udouble& lhs, const udouble& rhs);
    friend bool operator!=(const udouble& lhs, const udouble& rhs);
    friend bool operator<(const udouble& lhs, const udouble& rhs);
    friend bool operator>(const udouble& lhs, const udouble& rhs);
    friend bool operator<=(const udouble& lhs, const udouble& rhs);
    friend bool operator>=(const udouble& lhs, const udouble& rhs);
};

// Optional: Overload << to print the udouble more easily
inline std::ostream& operator<<(std::ostream& os, const udouble& val)
{
    os << val.nominal_value() << " ± " << val.stddev();
    return os;
}

} // namespace uncertainties