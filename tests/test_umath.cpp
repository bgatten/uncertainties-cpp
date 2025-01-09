#include <gtest/gtest.h>
#include "uncertainties/udouble.hpp"
#include "uncertainties/umath.hpp"

TEST(umathTest, Sin) {
    uncertainties::udouble x(1.0, 0.1);

    uncertainties::udouble result = uncertainties::sin(x);

    EXPECT_NEAR(result.nominal_value(), std::sin(1.0), 1e-12); // Nominal value
    EXPECT_NEAR(result.stddev(), std::abs(std::cos(1.0) * 0.1), 1e-6); // Stddev
}

TEST(umathTest, Cos) {
    uncertainties::udouble x(1.0, 0.1);

    uncertainties::udouble result = uncertainties::cos(x);

    EXPECT_NEAR(result.nominal_value(), std::cos(1.0), 1e-12); // Nominal value
    EXPECT_NEAR(result.stddev(), std::abs(std::sin(1.0) * 0.1), 1e-6); // Stddev
}

TEST(umathTest, Exp) {
    uncertainties::udouble x(1.0, 0.1);

    uncertainties::udouble result = uncertainties::exp(x);

    EXPECT_NEAR(result.nominal_value(), std::exp(1.0), 1e-12); // Nominal value
    EXPECT_NEAR(result.stddev(), std::exp(1.0) * 0.1, 1e-6); // Stddev
}

TEST(umathTest, Log) {
    uncertainties::udouble x(2.0, 0.1);

    uncertainties::udouble result = uncertainties::log(x);

    EXPECT_NEAR(result.nominal_value(), std::log(2.0), 1e-12); // Nominal value
    EXPECT_NEAR(result.stddev(), 0.1 / 2.0, 1e-6); // Stddev
}

TEST(umathTest, LogWithInvalidInput) {
    uncertainties::udouble x(-1.0, 0.1);

    EXPECT_THROW({
        uncertainties::log(x);
    }, std::invalid_argument); // Logarithm of negative value should throw
}
