#ifndef UNCERTAINTIES_UFLOAT_HPP
#define UNCERTAINTIES_UFLOAT_HPP

#include <cmath>
#include <stdexcept>
#include <ostream>

namespace uncertainties {

class ufloat {
public:
    // Constructors
    ufloat() : nominal_(0.0), stddev_(0.0) {}
    ufloat(double nominal, double stddev)
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
    friend ufloat operator+(const ufloat& lhs, const ufloat& rhs);

    // Subtraction
    friend ufloat operator-(const ufloat& lhs, const ufloat& rhs);

    // Multiplication
    friend ufloat operator*(const ufloat& lhs, const ufloat& rhs);

    // Division
    friend ufloat operator/(const ufloat& lhs, const ufloat& rhs);

private:
    double nominal_;
    double stddev_;
};

// Optional: Overload << to print the ufloat more easily
inline std::ostream& operator<<(std::ostream& os, const ufloat& val)
{
    os << val.nominal_value() << " ± " << val.stddev();
    return os;
}

} // namespace uncertainties

#endif // UNCERTAINTIES_UFLOAT_HPP