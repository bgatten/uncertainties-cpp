#ifndef UNCERTAINTIES_udouble_HPP
#define UNCERTAINTIES_udouble_HPP

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
    // friend udouble operator**(const udouble& lhs, const udouble& rhs);

    // friend udouble operator**(const udouble& lhs, const double& rhs);

    // friend udouble operator**(const double& lhs, const udouble& rhs);

};

// Optional: Overload << to print the udouble more easily
inline std::ostream& operator<<(std::ostream& os, const udouble& val)
{
    os << val.nominal_value() << " ± " << val.stddev();
    return os;
}

} // namespace uncertainties

#endif // UNCERTAINTIES_udouble_HPP