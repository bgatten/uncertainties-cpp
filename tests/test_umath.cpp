#include <gtest/gtest.h>
#include "uncertainties/udouble.hpp"
#include "uncertainties/umath.hpp"

// Trigonometric functions

TEST(umathTest, Sin) {
    uncertainties::udouble x(1.0, 0.1);

    uncertainties::udouble result = uncertainties::sin(x);

    EXPECT_NEAR(result.nominal_value(), std::sin(1.0), 1e-12);
    EXPECT_NEAR(result.stddev(), std::abs(std::cos(1.0)) * 0.1, 1e-6);
}

TEST(umathTest, Cos) {
    uncertainties::udouble x(1.0, 0.1);

    uncertainties::udouble result = uncertainties::cos(x);

    EXPECT_NEAR(result.nominal_value(), std::cos(1.0), 1e-12);
    EXPECT_NEAR(result.stddev(), std::abs(std::sin(1.0)) * 0.1, 1e-6);
}

TEST(umathTest, Tan) {
    uncertainties::udouble x(0.5, 0.1);

    uncertainties::udouble result = uncertainties::tan(x);

    double cos_x = std::cos(0.5);
    EXPECT_NEAR(result.nominal_value(), std::tan(0.5), 1e-12);
    EXPECT_NEAR(result.stddev(), 0.1 / (cos_x * cos_x), 1e-6);
}

// Inverse trigonometric functions

TEST(umathTest, Asin) {
    uncertainties::udouble x(0.5, 0.1);

    uncertainties::udouble result = uncertainties::asin(x);

    double denom = std::sqrt(1.0 - 0.5 * 0.5);
    EXPECT_NEAR(result.nominal_value(), std::asin(0.5), 1e-12);
    EXPECT_NEAR(result.stddev(), 0.1 / denom, 1e-6);
}

TEST(umathTest, Acos) {
    uncertainties::udouble x(0.5, 0.1);

    uncertainties::udouble result = uncertainties::acos(x);

    double denom = std::sqrt(1.0 - 0.5 * 0.5);
    EXPECT_NEAR(result.nominal_value(), std::acos(0.5), 1e-12);
    EXPECT_NEAR(result.stddev(), 0.1 / denom, 1e-6);
}

TEST(umathTest, Atan) {
    uncertainties::udouble x(1.0, 0.1);

    uncertainties::udouble result = uncertainties::atan(x);

    EXPECT_NEAR(result.nominal_value(), std::atan(1.0), 1e-12);
    EXPECT_NEAR(result.stddev(), 0.1 / (1.0 + 1.0 * 1.0), 1e-6);
}

TEST(umathTest, Atan2) {
    uncertainties::udouble y(3.0, 0.1);
    uncertainties::udouble x(4.0, 0.2);

    uncertainties::udouble result = uncertainties::atan2(y, x);

    double xv = 4.0, yv = 3.0;
    double denom = xv * xv + yv * yv;  // 25
    double expected_stddev = std::sqrt(xv * xv * 0.1 * 0.1 + yv * yv * 0.2 * 0.2) / denom;

    EXPECT_NEAR(result.nominal_value(), std::atan2(3.0, 4.0), 1e-12);
    EXPECT_NEAR(result.stddev(), expected_stddev, 1e-6);
}

// Hyperbolic functions

TEST(umathTest, Sinh) {
    uncertainties::udouble x(1.0, 0.1);

    uncertainties::udouble result = uncertainties::sinh(x);

    EXPECT_NEAR(result.nominal_value(), std::sinh(1.0), 1e-12);
    EXPECT_NEAR(result.stddev(), std::cosh(1.0) * 0.1, 1e-6);
}

TEST(umathTest, Cosh) {
    uncertainties::udouble x(1.0, 0.1);

    uncertainties::udouble result = uncertainties::cosh(x);

    EXPECT_NEAR(result.nominal_value(), std::cosh(1.0), 1e-12);
    EXPECT_NEAR(result.stddev(), std::abs(std::sinh(1.0)) * 0.1, 1e-6);
}

TEST(umathTest, Tanh) {
    uncertainties::udouble x(1.0, 0.1);

    uncertainties::udouble result = uncertainties::tanh(x);

    double cosh_x = std::cosh(1.0);
    EXPECT_NEAR(result.nominal_value(), std::tanh(1.0), 1e-12);
    EXPECT_NEAR(result.stddev(), 0.1 / (cosh_x * cosh_x), 1e-6);
}

// Inverse hyperbolic functions

TEST(umathTest, Asinh) {
    uncertainties::udouble x(1.0, 0.1);

    uncertainties::udouble result = uncertainties::asinh(x);

    EXPECT_NEAR(result.nominal_value(), std::asinh(1.0), 1e-12);
    EXPECT_NEAR(result.stddev(), 0.1 / std::sqrt(1.0 + 1.0 * 1.0), 1e-6);
}

TEST(umathTest, Acosh) {
    uncertainties::udouble x(2.0, 0.1);

    uncertainties::udouble result = uncertainties::acosh(x);

    EXPECT_NEAR(result.nominal_value(), std::acosh(2.0), 1e-12);
    EXPECT_NEAR(result.stddev(), 0.1 / std::sqrt(2.0 * 2.0 - 1.0), 1e-6);
}

TEST(umathTest, Atanh) {
    uncertainties::udouble x(0.5, 0.1);

    uncertainties::udouble result = uncertainties::atanh(x);

    EXPECT_NEAR(result.nominal_value(), std::atanh(0.5), 1e-12);
    EXPECT_NEAR(result.stddev(), 0.1 / (1.0 - 0.5 * 0.5), 1e-6);
}

// Exponential and logarithmic functions

TEST(umathTest, Exp) {
    uncertainties::udouble x(1.0, 0.1);

    uncertainties::udouble result = uncertainties::exp(x);

    EXPECT_NEAR(result.nominal_value(), std::exp(1.0), 1e-12);
    EXPECT_NEAR(result.stddev(), std::exp(1.0) * 0.1, 1e-6);
}

TEST(umathTest, Log) {
    uncertainties::udouble x(2.0, 0.1);

    uncertainties::udouble result = uncertainties::log(x);

    EXPECT_NEAR(result.nominal_value(), std::log(2.0), 1e-12);
    EXPECT_NEAR(result.stddev(), 0.1 / 2.0, 1e-6);
}

TEST(umathTest, Log10) {
    uncertainties::udouble x(10.0, 0.1);

    uncertainties::udouble result = uncertainties::log10(x);

    EXPECT_NEAR(result.nominal_value(), std::log10(10.0), 1e-12);
    EXPECT_NEAR(result.stddev(), 0.1 / (10.0 * std::log(10.0)), 1e-6);
}

TEST(umathTest, Sqrt) {
    uncertainties::udouble x(4.0, 0.1);

    uncertainties::udouble result = uncertainties::sqrt(x);

    EXPECT_NEAR(result.nominal_value(), 2.0, 1e-12);
    EXPECT_NEAR(result.stddev(), 0.1 / (2.0 * 2.0), 1e-6);
}

// Invalid input tests

TEST(umathTest, LogWithInvalidInput) {
    uncertainties::udouble x(-1.0, 0.1);

    EXPECT_THROW({
        uncertainties::log(x);
    }, std::invalid_argument);
}

TEST(umathTest, Log10WithInvalidInput) {
    uncertainties::udouble x(-1.0, 0.1);

    EXPECT_THROW({
        uncertainties::log10(x);
    }, std::invalid_argument);
}

TEST(umathTest, SqrtWithInvalidInput) {
    uncertainties::udouble x(-1.0, 0.1);

    EXPECT_THROW({
        uncertainties::sqrt(x);
    }, std::invalid_argument);
}

TEST(umathTest, AsinWithInvalidInput) {
    uncertainties::udouble x(2.0, 0.1);  // Outside [-1, 1]

    EXPECT_THROW({
        uncertainties::asin(x);
    }, std::invalid_argument);
}

TEST(umathTest, AcosWithInvalidInput) {
    uncertainties::udouble x(2.0, 0.1);  // Outside [-1, 1]

    EXPECT_THROW({
        uncertainties::acos(x);
    }, std::invalid_argument);
}

TEST(umathTest, AcoshWithInvalidInput) {
    uncertainties::udouble x(0.5, 0.1);  // Must be >= 1

    EXPECT_THROW({
        uncertainties::acosh(x);
    }, std::invalid_argument);
}

TEST(umathTest, AtanhWithInvalidInput) {
    uncertainties::udouble x(1.5, 0.1);  // Must be in (-1, 1)

    EXPECT_THROW({
        uncertainties::atanh(x);
    }, std::invalid_argument);
}

TEST(umathTest, Atan2AtOrigin) {
    uncertainties::udouble x(0.0, 0.1);
    uncertainties::udouble y(0.0, 0.1);

    EXPECT_THROW({
        uncertainties::atan2(y, x);
    }, std::invalid_argument);
}

// Other mathematical functions

TEST(umathTest, Abs) {
    uncertainties::udouble x(-3.0, 0.1);

    uncertainties::udouble result = uncertainties::abs(x);

    EXPECT_NEAR(result.nominal_value(), 3.0, 1e-12);
    EXPECT_NEAR(result.stddev(), 0.1, 1e-12);
}

TEST(umathTest, AbsPositive) {
    uncertainties::udouble x(3.0, 0.1);

    uncertainties::udouble result = uncertainties::abs(x);

    EXPECT_NEAR(result.nominal_value(), 3.0, 1e-12);
    EXPECT_NEAR(result.stddev(), 0.1, 1e-12);
}

TEST(umathTest, Hypot) {
    uncertainties::udouble x(3.0, 0.1);
    uncertainties::udouble y(4.0, 0.2);

    uncertainties::udouble result = uncertainties::hypot(x, y);

    double expected_nominal = 5.0;  // sqrt(9 + 16)
    double expected_stddev = std::sqrt(9.0 * 0.01 + 16.0 * 0.04) / 5.0;

    EXPECT_NEAR(result.nominal_value(), expected_nominal, 1e-12);
    EXPECT_NEAR(result.stddev(), expected_stddev, 1e-6);
}

TEST(umathTest, HypotAtOrigin) {
    uncertainties::udouble x(0.0, 0.1);
    uncertainties::udouble y(0.0, 0.2);

    uncertainties::udouble result = uncertainties::hypot(x, y);

    EXPECT_NEAR(result.nominal_value(), 0.0, 1e-12);
    EXPECT_NEAR(result.stddev(), std::sqrt(0.01 + 0.04), 1e-6);
}
