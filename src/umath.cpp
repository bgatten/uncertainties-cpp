#include <cmath>
#include <stdexcept>
#include "uncertainties/umath.hpp"

namespace uncertainties {

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

udouble exp(const udouble& x)
{
    double new_nominal = std::exp(x.nominal_value());
    double new_stddev = new_nominal * x.stddev(); // exp'(x) = exp(x)
    return udouble(new_nominal, new_stddev);
}

udouble log(const udouble& x)
{
    if (x.nominal_value() <= 0.0) {
        throw std::invalid_argument("Logarithm input must be greater than zero.");
    }
    double new_nominal = std::log(x.nominal_value());
    double new_stddev = x.stddev() / x.nominal_value(); // log'(x) = 1/x
    return udouble(new_nominal, new_stddev);
}

} // namespace uncertainties
