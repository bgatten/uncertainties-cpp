#include <gtest/gtest.h>
#include "uncertainties/ufloat.hpp"

TEST(UFloatTest, Addition) {
    uncertainties::ufloat a(1.0, 0.1);
    uncertainties::ufloat b(2.0, 0.2);

    uncertainties::ufloat c = a + b;

    EXPECT_NEAR(c.nominal_value(), 3.0, 1e-12); // Check nominal value
    EXPECT_NEAR(c.stddev(), 0.223606, 1e-6);    // Check standard deviation
}

TEST(UFloatTest, Multiplication) {
    uncertainties::ufloat a(1.0, 0.1);
    uncertainties::ufloat b(2.0, 0.2);

    uncertainties::ufloat d = a * b;

    EXPECT_NEAR(d.nominal_value(), 2.0, 1e-12); // Check nominal value
    EXPECT_NEAR(d.stddev(), 0.282843, 1e-6);    // Check standard deviation
}
