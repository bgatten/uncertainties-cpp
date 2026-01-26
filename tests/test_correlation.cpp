#include <gtest/gtest.h>
#include <cmath>
#include "uncertainties/udouble.hpp"
#include "uncertainties/umath.hpp"

using uncertainties::udouble;

// Helper to clear registry between tests for isolation
class CorrelationTest : public ::testing::Test {
protected:
    void SetUp() override {
        uncertainties::detail::VariableRegistry::instance().clear();
    }
};

// Basic correlation tests

TEST_F(CorrelationTest, SelfSubtraction) {
    udouble x(10.0, 0.5);
    udouble result = x - x;

    EXPECT_NEAR(result.nominal_value(), 0.0, 1e-12);
    EXPECT_NEAR(result.stddev(), 0.0, 1e-12);
}

TEST_F(CorrelationTest, SelfAddition) {
    udouble x(10.0, 0.5);
    udouble result = x + x;

    EXPECT_NEAR(result.nominal_value(), 20.0, 1e-12);
    EXPECT_NEAR(result.stddev(), 1.0, 1e-12);  // 2 * 0.5
}

TEST_F(CorrelationTest, SelfMultiplication) {
    udouble x(3.0, 0.1);
    udouble result = x * x;  // x^2

    EXPECT_NEAR(result.nominal_value(), 9.0, 1e-12);
    // d(x²)/dx = 2x, so sigma = 2*3*0.1 = 0.6
    EXPECT_NEAR(result.stddev(), 0.6, 1e-12);
}

TEST_F(CorrelationTest, SelfDivision) {
    udouble x(10.0, 0.5);
    udouble result = x / x;

    EXPECT_NEAR(result.nominal_value(), 1.0, 1e-12);
    EXPECT_NEAR(result.stddev(), 0.0, 1e-12);
}

// Independent variables still combine in quadrature

TEST_F(CorrelationTest, IndependentAddition) {
    udouble x(1.0, 0.1);
    udouble y(2.0, 0.2);
    udouble result = x + y;

    EXPECT_NEAR(result.nominal_value(), 3.0, 1e-12);
    EXPECT_NEAR(result.stddev(), std::sqrt(0.01 + 0.04), 1e-6);
}

TEST_F(CorrelationTest, IndependentSubtraction) {
    udouble x(5.0, 0.3);
    udouble y(2.0, 0.4);
    udouble result = x - y;

    EXPECT_NEAR(result.nominal_value(), 3.0, 1e-12);
    EXPECT_NEAR(result.stddev(), std::sqrt(0.09 + 0.16), 1e-6);
}

TEST_F(CorrelationTest, IndependentMultiplication) {
    udouble x(2.0, 0.1);
    udouble y(3.0, 0.2);
    udouble result = x * y;

    EXPECT_NEAR(result.nominal_value(), 6.0, 1e-12);
    // σ = sqrt(y² * σ_x² + x² * σ_y²) = sqrt(9*0.01 + 4*0.04) = sqrt(0.25) = 0.5
    EXPECT_NEAR(result.stddev(), 0.5, 1e-6);
}

// Independent copy creates uncorrelated variable

TEST_F(CorrelationTest, IndependentCopy) {
    udouble x(10.0, 0.5);
    udouble y = x.independent_copy();
    udouble result = x - y;

    EXPECT_NEAR(result.nominal_value(), 0.0, 1e-12);
    // Independent: sqrt(0.5² + 0.5²) = sqrt(0.5) ≈ 0.707
    EXPECT_NEAR(result.stddev(), std::sqrt(0.5), 1e-12);
}

// Complex expressions

TEST_F(CorrelationTest, ComplexExpressionXPlusYMinusX) {
    udouble x(5.0, 0.1);
    udouble y(3.0, 0.2);
    udouble result = (x + y) - x;

    EXPECT_NEAR(result.nominal_value(), 3.0, 1e-12);
    EXPECT_NEAR(result.stddev(), 0.2, 1e-12);  // Only y's uncertainty
}

TEST_F(CorrelationTest, ComplexExpressionXTimesYDividedByX) {
    udouble x(4.0, 0.2);
    udouble y(3.0, 0.3);
    udouble result = (x * y) / x;

    EXPECT_NEAR(result.nominal_value(), 3.0, 1e-12);
    EXPECT_NEAR(result.stddev(), 0.3, 1e-12);  // Only y's uncertainty
}

TEST_F(CorrelationTest, ChainedOperations) {
    udouble x(2.0, 0.1);
    udouble a = x + x;      // 4.0, deriv = 2
    udouble b = a + x;      // 6.0, deriv = 3
    udouble c = b - x;      // 4.0, deriv = 2

    EXPECT_NEAR(c.nominal_value(), 4.0, 1e-12);
    EXPECT_NEAR(c.stddev(), 0.2, 1e-12);  // 2 * 0.1
}

// Scalar operations

TEST_F(CorrelationTest, ScalarMultiplication) {
    udouble x(5.0, 0.1);
    udouble result = 2.0 * x;

    EXPECT_NEAR(result.nominal_value(), 10.0, 1e-12);
    EXPECT_NEAR(result.stddev(), 0.2, 1e-12);
}

TEST_F(CorrelationTest, ScalarDivision) {
    udouble x(10.0, 0.4);
    udouble result = x / 2.0;

    EXPECT_NEAR(result.nominal_value(), 5.0, 1e-12);
    EXPECT_NEAR(result.stddev(), 0.2, 1e-12);
}

TEST_F(CorrelationTest, MultiplicationVsAddition) {
    udouble x(5.0, 0.1);
    udouble mult_result = x * 2.0;
    udouble add_result = x + x;

    EXPECT_NEAR(mult_result.stddev(), add_result.stddev(), 1e-12);
}

// Math function correlation tests

TEST_F(CorrelationTest, SinMinusSin) {
    udouble x(1.0, 0.1);
    udouble s1 = uncertainties::sin(x);
    udouble s2 = uncertainties::sin(x);
    udouble result = s1 - s2;

    EXPECT_NEAR(result.nominal_value(), 0.0, 1e-12);
    EXPECT_NEAR(result.stddev(), 0.0, 1e-12);
}

TEST_F(CorrelationTest, ExpDividedByExp) {
    udouble x(1.0, 0.1);
    udouble e1 = uncertainties::exp(x);
    udouble e2 = uncertainties::exp(x);
    udouble result = e1 / e2;

    EXPECT_NEAR(result.nominal_value(), 1.0, 1e-12);
    EXPECT_NEAR(result.stddev(), 0.0, 1e-12);
}

TEST_F(CorrelationTest, SinSquaredPlusCosSquared) {
    udouble x(0.5, 0.1);
    udouble s = uncertainties::sin(x);
    udouble c = uncertainties::cos(x);
    udouble result = s * s + c * c;

    // sin²(x) + cos²(x) = 1, and the derivatives should cancel
    EXPECT_NEAR(result.nominal_value(), 1.0, 1e-12);
    // d/dx(sin²x + cos²x) = 2sinx*cosx - 2cosx*sinx = 0
    EXPECT_NEAR(result.stddev(), 0.0, 1e-10);
}

TEST_F(CorrelationTest, LogOfExp) {
    udouble x(2.0, 0.1);
    udouble result = uncertainties::log(uncertainties::exp(x));

    EXPECT_NEAR(result.nominal_value(), 2.0, 1e-12);
    EXPECT_NEAR(result.stddev(), 0.1, 1e-12);  // Derivative is 1
}

TEST_F(CorrelationTest, SqrtOfSquare) {
    udouble x(3.0, 0.1);
    udouble result = uncertainties::sqrt(x * x);

    EXPECT_NEAR(result.nominal_value(), 3.0, 1e-12);
    // d/dx(sqrt(x²)) = x/|x| = 1 for x > 0
    EXPECT_NEAR(result.stddev(), 0.1, 1e-12);
}

// Constant handling

TEST_F(CorrelationTest, ConstantHasNoDerivatives) {
    udouble c = 5.0;  // Implicit conversion

    EXPECT_EQ(c.num_variables(), 0u);
    EXPECT_NEAR(c.stddev(), 0.0, 1e-12);
}

TEST_F(CorrelationTest, ZeroStddevNoDerivatives) {
    udouble x(5.0, 0.0);

    EXPECT_EQ(x.num_variables(), 0u);
    EXPECT_NEAR(x.stddev(), 0.0, 1e-12);
}

TEST_F(CorrelationTest, ConstantPlusVariable) {
    udouble x(3.0, 0.1);
    udouble c = 2.0;
    udouble result = x + c;

    EXPECT_NEAR(result.nominal_value(), 5.0, 1e-12);
    EXPECT_NEAR(result.stddev(), 0.1, 1e-12);
}

// Variable tracking

TEST_F(CorrelationTest, AtomicVariableIsAtomic) {
    udouble x(10.0, 0.5);

    EXPECT_TRUE(x.is_atomic());
    EXPECT_EQ(x.num_variables(), 1u);
}

TEST_F(CorrelationTest, DerivedVariableNotAtomic) {
    udouble x(10.0, 0.5);
    udouble y = x + x;

    EXPECT_FALSE(y.is_atomic());
    EXPECT_EQ(y.num_variables(), 1u);  // Still only depends on x
}

TEST_F(CorrelationTest, TwoVariables) {
    udouble x(1.0, 0.1);
    udouble y(2.0, 0.2);
    udouble z = x + y;

    EXPECT_EQ(z.num_variables(), 2u);
}

// Power function correlation

TEST_F(CorrelationTest, PowerSelfExponent) {
    udouble x(2.0, 0.1);
    udouble result = pow(x, x);  // x^x at x=2 is 4

    EXPECT_NEAR(result.nominal_value(), 4.0, 1e-12);
    // d/dx(x^x) = x^x * (1 + ln(x)) at x=2 = 4 * (1 + ln(2)) ≈ 6.77
    double expected_deriv = 4.0 * (1.0 + std::log(2.0));
    EXPECT_NEAR(result.stddev(), std::abs(expected_deriv) * 0.1, 1e-10);
}

// Unary operators

TEST_F(CorrelationTest, UnaryNegation) {
    udouble x(5.0, 0.3);
    udouble neg_x = -x;
    udouble result = x + neg_x;

    EXPECT_NEAR(result.nominal_value(), 0.0, 1e-12);
    EXPECT_NEAR(result.stddev(), 0.0, 1e-12);
}

TEST_F(CorrelationTest, UnaryPlus) {
    udouble x(5.0, 0.3);
    udouble pos_x = +x;
    udouble result = x - pos_x;

    EXPECT_NEAR(result.nominal_value(), 0.0, 1e-12);
    EXPECT_NEAR(result.stddev(), 0.0, 1e-12);
}

// Compound assignment

TEST_F(CorrelationTest, CompoundAddition) {
    udouble x(3.0, 0.1);
    udouble y = x;
    y += x;

    EXPECT_NEAR(y.nominal_value(), 6.0, 1e-12);
    EXPECT_NEAR(y.stddev(), 0.2, 1e-12);  // 2 * 0.1
}

TEST_F(CorrelationTest, CompoundSubtraction) {
    udouble x(3.0, 0.1);
    udouble y = x;
    y -= x;

    EXPECT_NEAR(y.nominal_value(), 0.0, 1e-12);
    EXPECT_NEAR(y.stddev(), 0.0, 1e-12);
}

// Two-argument functions with correlation

TEST_F(CorrelationTest, Atan2SameVariable) {
    udouble x(1.0, 0.1);
    // atan2(x, x) = atan2(1, 1) = pi/4
    udouble result = uncertainties::atan2(x, x);

    EXPECT_NEAR(result.nominal_value(), M_PI / 4.0, 1e-12);
    // d/dx(atan2(x,x)) = (x*1 - x*1) / (x² + x²) = 0
    EXPECT_NEAR(result.stddev(), 0.0, 1e-10);
}

TEST_F(CorrelationTest, HypotSameVariable) {
    udouble x(3.0, 0.1);
    // hypot(x, x) = sqrt(x² + x²) = x * sqrt(2)
    udouble result = uncertainties::hypot(x, x);

    EXPECT_NEAR(result.nominal_value(), 3.0 * std::sqrt(2.0), 1e-12);
    // d/dx(sqrt(2)*x) = sqrt(2)
    EXPECT_NEAR(result.stddev(), std::sqrt(2.0) * 0.1, 1e-10);
}
