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
