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

// Unary operators

TEST(udoubleTest, UnaryPlus) {
    uncertainties::udouble a(1.5, 0.1);
    uncertainties::udouble b = +a;

    EXPECT_NEAR(b.nominal_value(), 1.5, 1e-12);
    EXPECT_NEAR(b.stddev(), 0.1, 1e-12);
}

TEST(udoubleTest, UnaryNegation) {
    uncertainties::udouble a(1.5, 0.1);
    uncertainties::udouble b = -a;

    EXPECT_NEAR(b.nominal_value(), -1.5, 1e-12);
    EXPECT_NEAR(b.stddev(), 0.1, 1e-12);
}

// Compound assignment operators

TEST(udoubleTest, CompoundAddition) {
    uncertainties::udouble a(1.0, 0.1);
    uncertainties::udouble b(2.0, 0.2);

    a += b;

    EXPECT_NEAR(a.nominal_value(), 3.0, 1e-12);
    EXPECT_NEAR(a.stddev(), 0.223606, 1e-6);
}

TEST(udoubleTest, CompoundSubtraction) {
    uncertainties::udouble a(1.0, 0.1);
    uncertainties::udouble b(2.0, 0.2);

    a -= b;

    EXPECT_NEAR(a.nominal_value(), -1.0, 1e-12);
    EXPECT_NEAR(a.stddev(), 0.223606, 1e-6);
}

TEST(udoubleTest, CompoundMultiplication) {
    uncertainties::udouble a(1.0, 0.1);
    uncertainties::udouble b(2.0, 0.2);

    a *= b;

    EXPECT_NEAR(a.nominal_value(), 2.0, 1e-12);
    EXPECT_NEAR(a.stddev(), 0.282843, 1e-6);
}

TEST(udoubleTest, CompoundMultiplicationWithDouble) {
    uncertainties::udouble a(1.0, 0.1);

    a *= 2.0;

    EXPECT_NEAR(a.nominal_value(), 2.0, 1e-12);
    EXPECT_NEAR(a.stddev(), 0.2, 1e-6);
}

TEST(udoubleTest, CompoundDivision) {
    uncertainties::udouble a(1.0, 0.1);
    uncertainties::udouble b(2.0, 0.2);

    a /= b;

    EXPECT_NEAR(a.nominal_value(), 0.5, 1e-12);
    EXPECT_NEAR(a.stddev(), 0.070711, 1e-6);
}

TEST(udoubleTest, CompoundDivisionWithDouble) {
    uncertainties::udouble a(1.0, 0.1);

    a /= 2.0;

    EXPECT_NEAR(a.nominal_value(), 0.5, 1e-12);
    EXPECT_NEAR(a.stddev(), 0.05, 1e-6);
}

// Comparison operators

TEST(udoubleTest, ComparisonEqual) {
    uncertainties::udouble a(1.0, 0.1);
    uncertainties::udouble b(1.0, 0.2);
    uncertainties::udouble c(2.0, 0.1);

    EXPECT_TRUE(a == b);  // Same nominal, different stddev
    EXPECT_FALSE(a == c);
}

TEST(udoubleTest, ComparisonNotEqual) {
    uncertainties::udouble a(1.0, 0.1);
    uncertainties::udouble b(1.0, 0.2);
    uncertainties::udouble c(2.0, 0.1);

    EXPECT_FALSE(a != b);
    EXPECT_TRUE(a != c);
}

TEST(udoubleTest, ComparisonLess) {
    uncertainties::udouble a(1.0, 0.1);
    uncertainties::udouble b(2.0, 0.1);

    EXPECT_TRUE(a < b);
    EXPECT_FALSE(b < a);
    EXPECT_FALSE(a < a);
}

TEST(udoubleTest, ComparisonGreater) {
    uncertainties::udouble a(1.0, 0.1);
    uncertainties::udouble b(2.0, 0.1);

    EXPECT_FALSE(a > b);
    EXPECT_TRUE(b > a);
    EXPECT_FALSE(a > a);
}

TEST(udoubleTest, ComparisonLessEqual) {
    uncertainties::udouble a(1.0, 0.1);
    uncertainties::udouble b(2.0, 0.1);
    uncertainties::udouble c(1.0, 0.2);

    EXPECT_TRUE(a <= b);
    EXPECT_FALSE(b <= a);
    EXPECT_TRUE(a <= c);  // Equal nominals
}

TEST(udoubleTest, ComparisonGreaterEqual) {
    uncertainties::udouble a(1.0, 0.1);
    uncertainties::udouble b(2.0, 0.1);
    uncertainties::udouble c(1.0, 0.2);

    EXPECT_FALSE(a >= b);
    EXPECT_TRUE(b >= a);
    EXPECT_TRUE(a >= c);  // Equal nominals
}

// Edge cases

TEST(udoubleTest, MultiplicationWithZeroNominal) {
    uncertainties::udouble a(0.0, 0.1);
    uncertainties::udouble b(2.0, 0.2);

    uncertainties::udouble c = a * b;

    EXPECT_NEAR(c.nominal_value(), 0.0, 1e-12);
    // σ = sqrt(2² * 0.1² + 0² * 0.2²) = sqrt(0.04) = 0.2
    EXPECT_NEAR(c.stddev(), 0.2, 1e-6);
}

TEST(udoubleTest, DivisionByZeroThrows) {
    uncertainties::udouble a(1.0, 0.1);
    uncertainties::udouble b(0.0, 0.1);

    EXPECT_THROW({
        a / b;
    }, std::runtime_error);
}

TEST(udoubleTest, DivisionByZeroDoubleThrows) {
    uncertainties::udouble a(1.0, 0.1);

    EXPECT_THROW({
        a / 0.0;
    }, std::runtime_error);
}

TEST(udoubleTest, NegativeStddevThrows) {
    EXPECT_THROW({
        uncertainties::udouble a(1.0, -0.1);
    }, std::invalid_argument);
}

TEST(udoubleTest, SetNegativeStddevThrows) {
    uncertainties::udouble a(1.0, 0.1);

    EXPECT_THROW({
        a.set_stddev(-0.1);
    }, std::invalid_argument);
}

TEST(udoubleTest, DefaultConstructor) {
    uncertainties::udouble a;

    EXPECT_NEAR(a.nominal_value(), 0.0, 1e-12);
    EXPECT_NEAR(a.stddev(), 0.0, 1e-12);
}

// Implicit conversion from double

TEST(udoubleTest, ImplicitConversionFromDouble) {
    uncertainties::udouble a = 3.14;

    EXPECT_NEAR(a.nominal_value(), 3.14, 1e-12);
    EXPECT_NEAR(a.stddev(), 0.0, 1e-12);
}

TEST(udoubleTest, ImplicitConversionInArithmetic) {
    uncertainties::udouble a(1.0, 0.1);

    // Adding a plain double should work via implicit conversion
    uncertainties::udouble b = a + 2.0;

    EXPECT_NEAR(b.nominal_value(), 3.0, 1e-12);
    EXPECT_NEAR(b.stddev(), 0.1, 1e-6);  // Only a has uncertainty
}

TEST(udoubleTest, ImplicitConversionInFunction) {
    // This tests that functions accepting udouble can take double
    auto square = [](const uncertainties::udouble& x) {
        return x * x;
    };

    uncertainties::udouble result = square(3.0);

    EXPECT_NEAR(result.nominal_value(), 9.0, 1e-12);
    EXPECT_NEAR(result.stddev(), 0.0, 1e-12);
}

// Note: constexpr tests removed - udouble is no longer a literal type
// due to the derivative map used for correlation tracking.

// Formatting tests

TEST(udoubleTest, ToStringDefault) {
    uncertainties::udouble a(1.23456789, 0.00123456);

    std::string result = a.to_string();

    // Default precision is 6
    EXPECT_TRUE(result.find("1.23457") != std::string::npos ||
                result.find("1.2345") != std::string::npos);
    EXPECT_TRUE(result.find("±") != std::string::npos);
}

TEST(udoubleTest, ToStringPrecision) {
    uncertainties::udouble a(1.23456789, 0.00123456);

    std::string result = a.to_string(3);

    EXPECT_TRUE(result.find("1.23") != std::string::npos);
}

TEST(udoubleTest, ToScientific) {
    uncertainties::udouble a(12345.0, 67.0);

    std::string result = a.to_scientific(2);

    EXPECT_TRUE(result.find("e+") != std::string::npos || result.find("E+") != std::string::npos);
}

TEST(udoubleTest, ToCompact) {
    uncertainties::udouble a(1.234, 0.056);

    std::string result = a.to_compact(2);

    // Should produce something like "1.234(56)" or "1.23(6)"
    EXPECT_TRUE(result.find("(") != std::string::npos);
    EXPECT_TRUE(result.find(")") != std::string::npos);
}

TEST(udoubleTest, ToCompactZeroUncertainty) {
    uncertainties::udouble a(1.234, 0.0);

    std::string result = a.to_compact();

    // No parentheses when uncertainty is zero
    EXPECT_TRUE(result.find("(") == std::string::npos);
}