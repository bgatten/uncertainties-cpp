#include <gtest/gtest.h>
#include "uncertainties/udouble.hpp"

TEST(udoubleTest, Addition) {
    uncertainties::udouble a(1.0, 0.1);
    uncertainties::udouble b(2.0, 0.2);

    uncertainties::udouble c = a + b;

    EXPECT_NEAR(c.nominal_value(), 3.0, 1e-12); // Check nominal value
    EXPECT_NEAR(c.stddev(), 0.223606, 1e-6);    // Check standard deviation
}

TEST(udoubleTest, Multiplication) {
    uncertainties::udouble a(1.0, 0.1);
    uncertainties::udouble b(2.0, 0.2);

    uncertainties::udouble d = a * b;

    EXPECT_NEAR(d.nominal_value(), 2.0, 1e-12); // Check nominal value
    EXPECT_NEAR(d.stddev(), 0.282843, 1e-6);    // Check standard deviation
}

TEST(udoubleTest, MultiplicationWithDouble) {
    uncertainties::udouble a(1.0, 0.1);
    double b = 2.0;

    uncertainties::udouble d = a * b;
    uncertainties::udouble e = b * a;

    EXPECT_NEAR(d.nominal_value(), 2.0, 1e-12); // Check nominal value
    EXPECT_NEAR(d.stddev(), 0.2, 1e-6);    // Check standard deviation
    EXPECT_NEAR(e.nominal_value(), 2.0, 1e-12); // Check nominal value
    EXPECT_NEAR(e.stddev(), 0.2, 1e-6);    // Check standard deviation
}

TEST(udoubleTest, Division) {
    uncertainties::udouble a(1.0, 0.1);
    uncertainties::udouble b(2.0, 0.2);

    uncertainties::udouble e = a / b;

    EXPECT_NEAR(e.nominal_value(), 0.5, 1e-12); // Check nominal value
    EXPECT_NEAR(e.stddev(), 0.070711, 1e-6);    // Check standard deviation
}

TEST(udoubleTest, DivisionWithDouble) {
    uncertainties::udouble a(1.0, 0.1);
    double b = 2.0;

    uncertainties::udouble e = a / b;
    uncertainties::udouble f = b / a;

    EXPECT_NEAR(e.nominal_value(), 0.5, 1e-12); // Check nominal value
    EXPECT_NEAR(e.stddev(), 0.05, 1e-6);    // Check standard deviation
    EXPECT_NEAR(f.nominal_value(), 2.0, 1e-12); // Check nominal value
    EXPECT_NEAR(f.stddev(), 0.2, 1e-6);    // Check standard deviation
}

TEST(udoubleTest, Subtraction) {
    uncertainties::udouble a(1.0, 0.1);
    uncertainties::udouble b(2.0, 0.2);

    uncertainties::udouble c = a - b;

    EXPECT_NEAR(c.nominal_value(), -1.0, 1e-12); // Check nominal value
    EXPECT_NEAR(c.stddev(), 0.223606, 1e-6);    // Check standard deviation
}

TEST(udoubleTest, Power) {
    uncertainties::udouble a(3.0, 0.1);
    uncertainties::udouble b(2.0, 0.2);

    uncertainties::udouble c = pow(a,b); //a ^ b

    EXPECT_NEAR(c.nominal_value(), 9.0, 1e-12);
    EXPECT_NEAR(c.stddev(), 2.0665223, 1e-6);
}