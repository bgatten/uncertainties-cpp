#pragma once

#include <cmath>
#include <stdexcept>
#include <ostream>
#include <sstream>
#include <iomanip>
#include <string>

namespace uncertainties {

class udouble {
private:
    double nominal_;
    double stddev_;
public:
    // Constructors
    constexpr udouble() noexcept : nominal_(0.0), stddev_(0.0) {}

    // Implicit conversion from double (zero uncertainty)
    constexpr udouble(double nominal) noexcept : nominal_(nominal), stddev_(0.0) {}

    constexpr udouble(double nominal, double stddev)
        : nominal_(nominal), stddev_(stddev)
    {
        if (stddev < 0.0) {
            throw std::invalid_argument("Standard deviation cannot be negative.");
        }
    }

    // Getters
    constexpr double nominal_value() const noexcept { return nominal_; }
    constexpr double stddev() const noexcept { return stddev_; }

    // Setters (optional, if you want to allow changes after construction)
    constexpr void set_nominal_value(double value) noexcept { nominal_ = value; }
    void set_stddev(double value) {
        if (value < 0.0) {
            throw std::invalid_argument("Standard deviation cannot be negative.");
        }
        stddev_ = value;
    }

    // Operator Overloads
    // ------------------

    // Unary operators
    constexpr udouble operator+() const noexcept { return *this; }
    constexpr udouble operator-() const noexcept { return udouble(-nominal_, stddev_); }

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

    // Formatting methods

    // Format with specified precision: "1.234 ± 0.056"
    std::string to_string(int precision = 6) const {
        std::ostringstream oss;
        oss << std::setprecision(precision) << nominal_ << " ± " << stddev_;
        return oss.str();
    }

    // Format in scientific notation: "1.234e+00 ± 5.600e-02"
    std::string to_scientific(int precision = 3) const {
        std::ostringstream oss;
        oss << std::scientific << std::setprecision(precision)
            << nominal_ << " ± " << stddev_;
        return oss.str();
    }

    // Format in compact notation: "1.234(56)" where digits in parentheses
    // represent uncertainty in the last digits
    std::string to_compact(int significant_digits = 2) const {
        if (stddev_ == 0.0) {
            std::ostringstream oss;
            oss << nominal_;
            return oss.str();
        }

        // Determine the order of magnitude of the uncertainty
        int uncert_exp = static_cast<int>(std::floor(std::log10(std::abs(stddev_))));
        int display_exp = uncert_exp - significant_digits + 1;

        // Round uncertainty to significant digits
        double scale = std::pow(10.0, -display_exp);
        long long uncert_int = static_cast<long long>(std::round(stddev_ * scale));

        // Round nominal to same decimal place
        double rounded_nominal = std::round(nominal_ * scale) / scale;

        // Format the output
        std::ostringstream oss;
        int decimal_places = (display_exp < 0) ? -display_exp : 0;
        oss << std::fixed << std::setprecision(decimal_places) << rounded_nominal
            << "(" << uncert_int << ")";
        return oss.str();
    }
};

// Stream output operator
inline std::ostream& operator<<(std::ostream& os, const udouble& val)
{
    os << val.nominal_value() << " ± " << val.stddev();
    return os;
}

} // namespace uncertainties