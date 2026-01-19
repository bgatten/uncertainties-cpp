#include <gtest/gtest.h>
#include <Eigen/Dense>
#include "uncertainties/eigen_support.hpp"

using uncertainties::udouble;

// Type aliases for convenience
using Matrix2u = Eigen::Matrix<udouble, 2, 2>;
using Matrix3u = Eigen::Matrix<udouble, 3, 3>;
using Vector2u = Eigen::Vector<udouble, 2>;
using Vector3u = Eigen::Vector<udouble, 3>;
using MatrixXu = Eigen::Matrix<udouble, Eigen::Dynamic, Eigen::Dynamic>;
using VectorXu = Eigen::Vector<udouble, Eigen::Dynamic>;

// Basic matrix operations

TEST(EigenTest, MatrixConstruction) {
    Matrix2u A;
    A << udouble(1.0, 0.1), udouble(2.0, 0.2),
         udouble(3.0, 0.3), udouble(4.0, 0.4);

    EXPECT_NEAR(A(0, 0).nominal_value(), 1.0, 1e-12);
    EXPECT_NEAR(A(0, 0).stddev(), 0.1, 1e-12);
    EXPECT_NEAR(A(1, 1).nominal_value(), 4.0, 1e-12);
    EXPECT_NEAR(A(1, 1).stddev(), 0.4, 1e-12);
}

TEST(EigenTest, VectorConstruction) {
    Vector3u v;
    v << udouble(1.0, 0.1), udouble(2.0, 0.2), udouble(3.0, 0.3);

    EXPECT_NEAR(v(0).nominal_value(), 1.0, 1e-12);
    EXPECT_NEAR(v(2).stddev(), 0.3, 1e-12);
}

TEST(EigenTest, MatrixAddition) {
    Matrix2u A, B;
    A << udouble(1.0, 0.1), udouble(2.0, 0.1),
         udouble(3.0, 0.1), udouble(4.0, 0.1);
    B << udouble(5.0, 0.2), udouble(6.0, 0.2),
         udouble(7.0, 0.2), udouble(8.0, 0.2);

    Matrix2u C = A + B;

    EXPECT_NEAR(C(0, 0).nominal_value(), 6.0, 1e-12);
    // Uncertainty: sqrt(0.1² + 0.2²) ≈ 0.2236
    EXPECT_NEAR(C(0, 0).stddev(), std::sqrt(0.01 + 0.04), 1e-6);
}

TEST(EigenTest, MatrixScalarMultiplication) {
    Matrix2u A;
    A << udouble(1.0, 0.1), udouble(2.0, 0.2),
         udouble(3.0, 0.3), udouble(4.0, 0.4);

    Matrix2u B = A * udouble(2.0, 0.0);

    EXPECT_NEAR(B(0, 0).nominal_value(), 2.0, 1e-12);
    EXPECT_NEAR(B(0, 0).stddev(), 0.2, 1e-6);
    EXPECT_NEAR(B(1, 1).nominal_value(), 8.0, 1e-12);
    EXPECT_NEAR(B(1, 1).stddev(), 0.8, 1e-6);
}

TEST(EigenTest, MatrixVectorMultiplication) {
    Matrix2u A;
    A << udouble(1.0, 0.0), udouble(2.0, 0.0),
         udouble(3.0, 0.0), udouble(4.0, 0.0);

    Vector2u v;
    v << udouble(1.0, 0.1), udouble(2.0, 0.2);

    Vector2u result = A * v;

    // result[0] = 1*1 + 2*2 = 5
    // result[1] = 3*1 + 4*2 = 11
    EXPECT_NEAR(result(0).nominal_value(), 5.0, 1e-12);
    EXPECT_NEAR(result(1).nominal_value(), 11.0, 1e-12);
}

TEST(EigenTest, MatrixMatrixMultiplication) {
    Matrix2u A, B;
    A << udouble(1.0, 0.1), udouble(2.0, 0.0),
         udouble(3.0, 0.0), udouble(4.0, 0.1);
    B << udouble(5.0, 0.0), udouble(6.0, 0.0),
         udouble(7.0, 0.0), udouble(8.0, 0.0);

    Matrix2u C = A * B;

    // C(0,0) = 1*5 + 2*7 = 19
    // C(0,1) = 1*6 + 2*8 = 22
    // C(1,0) = 3*5 + 4*7 = 43
    // C(1,1) = 3*6 + 4*8 = 50
    EXPECT_NEAR(C(0, 0).nominal_value(), 19.0, 1e-12);
    EXPECT_NEAR(C(0, 1).nominal_value(), 22.0, 1e-12);
    EXPECT_NEAR(C(1, 0).nominal_value(), 43.0, 1e-12);
    EXPECT_NEAR(C(1, 1).nominal_value(), 50.0, 1e-12);
}

TEST(EigenTest, DotProduct) {
    Vector3u a, b;
    a << udouble(1.0, 0.1), udouble(2.0, 0.1), udouble(3.0, 0.1);
    b << udouble(4.0, 0.2), udouble(5.0, 0.2), udouble(6.0, 0.2);

    udouble result = a.dot(b);

    // dot = 1*4 + 2*5 + 3*6 = 32
    EXPECT_NEAR(result.nominal_value(), 32.0, 1e-12);
    // Uncertainty propagates through the dot product
    EXPECT_GT(result.stddev(), 0.0);
}

TEST(EigenTest, CrossProduct) {
    Vector3u a, b;
    a << udouble(1.0, 0.0), udouble(0.0, 0.0), udouble(0.0, 0.0);
    b << udouble(0.0, 0.0), udouble(1.0, 0.0), udouble(0.0, 0.0);

    Vector3u result = a.cross(b);

    // cross([1,0,0], [0,1,0]) = [0,0,1]
    EXPECT_NEAR(result(0).nominal_value(), 0.0, 1e-12);
    EXPECT_NEAR(result(1).nominal_value(), 0.0, 1e-12);
    EXPECT_NEAR(result(2).nominal_value(), 1.0, 1e-12);
}

TEST(EigenTest, Transpose) {
    Matrix2u A;
    A << udouble(1.0, 0.1), udouble(2.0, 0.2),
         udouble(3.0, 0.3), udouble(4.0, 0.4);

    Matrix2u B = A.transpose();

    EXPECT_NEAR(B(0, 1).nominal_value(), 3.0, 1e-12);
    EXPECT_NEAR(B(0, 1).stddev(), 0.3, 1e-12);
    EXPECT_NEAR(B(1, 0).nominal_value(), 2.0, 1e-12);
    EXPECT_NEAR(B(1, 0).stddev(), 0.2, 1e-12);
}

TEST(EigenTest, DynamicMatrix) {
    MatrixXu A(2, 3);
    A << udouble(1.0, 0.1), udouble(2.0, 0.1), udouble(3.0, 0.1),
         udouble(4.0, 0.1), udouble(5.0, 0.1), udouble(6.0, 0.1);

    EXPECT_EQ(A.rows(), 2);
    EXPECT_EQ(A.cols(), 3);
    EXPECT_NEAR(A(1, 2).nominal_value(), 6.0, 1e-12);
}

TEST(EigenTest, Identity) {
    Matrix3u I = Matrix3u::Identity();

    EXPECT_NEAR(I(0, 0).nominal_value(), 1.0, 1e-12);
    EXPECT_NEAR(I(0, 0).stddev(), 0.0, 1e-12);
    EXPECT_NEAR(I(0, 1).nominal_value(), 0.0, 1e-12);
    EXPECT_NEAR(I(1, 1).nominal_value(), 1.0, 1e-12);
}

TEST(EigenTest, Zero) {
    Matrix2u Z = Matrix2u::Zero();

    EXPECT_NEAR(Z(0, 0).nominal_value(), 0.0, 1e-12);
    EXPECT_NEAR(Z(0, 0).stddev(), 0.0, 1e-12);
}

TEST(EigenTest, ImplicitConversionFromDouble) {
    // Test that matrices can be initialized with plain doubles
    Matrix2u A;
    A << 1.0, 2.0,
         3.0, 4.0;

    EXPECT_NEAR(A(0, 0).nominal_value(), 1.0, 1e-12);
    EXPECT_NEAR(A(0, 0).stddev(), 0.0, 1e-12);
}

// Numerical operations that require NumTraits

TEST(EigenTest, Norm) {
    Vector3u v;
    v << udouble(3.0, 0.0), udouble(4.0, 0.0), udouble(0.0, 0.0);

    // This tests that sqrt works with udouble via Eigen
    udouble n = v.norm();

    EXPECT_NEAR(n.nominal_value(), 5.0, 1e-12);
}

TEST(EigenTest, Determinant2x2) {
    Matrix2u A;
    A << udouble(1.0, 0.1), udouble(2.0, 0.0),
         udouble(3.0, 0.0), udouble(4.0, 0.1);

    udouble det = A.determinant();

    // det = 1*4 - 2*3 = -2
    EXPECT_NEAR(det.nominal_value(), -2.0, 1e-12);
    // Uncertainty propagates
    EXPECT_GT(det.stddev(), 0.0);
}

TEST(EigenTest, Trace) {
    Matrix3u A;
    A << udouble(1.0, 0.1), udouble(0.0, 0.0), udouble(0.0, 0.0),
         udouble(0.0, 0.0), udouble(2.0, 0.2), udouble(0.0, 0.0),
         udouble(0.0, 0.0), udouble(0.0, 0.0), udouble(3.0, 0.3);

    udouble tr = A.trace();

    EXPECT_NEAR(tr.nominal_value(), 6.0, 1e-12);
    // trace uncertainty: sqrt(0.1² + 0.2² + 0.3²) ≈ 0.374
    EXPECT_NEAR(tr.stddev(), std::sqrt(0.01 + 0.04 + 0.09), 1e-6);
}
