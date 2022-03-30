/**
 * @file TemplatesTests.cpp
 * @author ashwinn76
 * @brief File with tests for generic algorithms, concepts and type traits
 * @version 0.1
 * @date 2020-08-23
 *
 * @copyright Copyright (c) 2020
 *
 */


#include "gtest/gtest.h"

#include <general/macro_utils>
#include <general/type_trait_utils>
#include <math/bound_value>
#include <math/matrix>


TEST( MatrixTests, MatrixTemplatesTests )
{
    using product_t_1 = matrix_product_t<matrix<2, 6>, matrix<6, 3>>;

    static_assert( std::is_same_v<product_t_1, matrix<2, 3>> );

    static_assert( std::is_same_v<matrix_transpose_t<matrix<2, 3>>, matrix<3, 2>> );
}


TEST( MatrixTests, MatrixEqualityTests )
{
    constexpr auto firstMatrix = matrix<2, 2>{ -1, -4, 6, 9 };

    constexpr auto sameAsFirstMatrix = firstMatrix;

    static_assert( firstMatrix == sameAsFirstMatrix );

    constexpr auto defaultMatrix = matrix<2, 2>{ false };

    static_assert( firstMatrix != defaultMatrix );

    auto mat = matrix<3, 3>{ -2, -3, 1, 4, -6, 6, 0, 3, 7 };

    EXPECT_TRUE( mat[0][0] == -2 );
    EXPECT_TRUE( mat[1][2] == 6 );
    EXPECT_TRUE( mat[1][1] == -6 );
    EXPECT_TRUE( mat[0][2] == 1 );
    EXPECT_TRUE( mat[2][1] == 3 );
}


TEST( MatrixTests, MatrixArithmeticTests )
{
    constexpr auto firstMatrix = Matrix2x2{ -1, -4, 6, 9 };

    constexpr auto sameAsFirstMatrix = firstMatrix;

    constexpr auto defaultMatrix = Matrix2x2{ false };

    constexpr auto shouldBeAllZerosMatrix = firstMatrix - sameAsFirstMatrix;

    static_assert( defaultMatrix == shouldBeAllZerosMatrix );

    constexpr auto shouldBeSameAsFirstMatrix = firstMatrix + decltype( firstMatrix ){ false };

    static_assert( firstMatrix == shouldBeSameAsFirstMatrix );


    auto firstMat = Matrix3x3{ -2, -3, 1, 4, -6, 6, 0, 3, 7 };

    auto sameAsFirstMat = firstMat;

    firstMat += decltype( firstMat ){ false };

    EXPECT_TRUE( firstMat == sameAsFirstMat );

    firstMat -= sameAsFirstMat;

    EXPECT_TRUE( firstMat == decltype( firstMat ){ false } );
}


TEST( MatrixTests, MatrixMultiplicationTests )
{
    constexpr auto matrix1 = matrix<4, 3>{
        1, 2, 3, 53, 6, 45, 3, 43, 3, 2, 32, 3,
    };

    constexpr auto matrix2 = matrix<3, 6>{
        23, 3, 4, 54, 236, 8, 56, 4, 3, 78, 711, 8, 6, 6, 6, 6, 41, 64,
    };

    constexpr auto product = matrix1 * matrix2;

    constexpr auto expected = matrix_product_t<decltype( matrix1 ), decltype( matrix2 )>{
        153,  29,  28,  228,  1781,  216, 1825, 453, 500, 3600, 18619, 3352,
        2495, 199, 159, 3534, 31404, 560, 1856, 152, 122, 2622, 23347, 464,
    };

    static_assert( product == expected );


    auto mat1 = Matrix2x2{
        3,
        3,
        5,
        3,
    };

    auto mat2 = Matrix2x2{
        42,
        46,
        54,
        68,
    };

    mat1 *= mat2;

    auto exp = Matrix2x2{
        288,
        342,
        372,
        434,
    };

    EXPECT_TRUE( exp == mat1 );
}


TEST( MatrixTests, MatrixTransposeTests )
{
    constexpr auto matrix1 = matrix<3, 2>{ -1.0, 3.0, 12.9, -12.78, -0.9, 900.8 };

    constexpr auto transpose = ~matrix1;

    constexpr auto expected = matrix_transpose_t<decltype( matrix1 )>{ -1.0, 12.9, -0.9, 3.0, -12.78, 900.8 };

    static_assert( transpose == expected );
}


TEST( MatrixTests, MatrixLeftoverElementsTests )
{
    constexpr auto matrix1 = Matrix3x3{ -1.0, 3.0, 12.9, -12.78, -0.9, 900.8, 23.4, 0.0, 69.8 };

    constexpr auto leftover = matrix1.leftover_elements( 0_ui64, 0_ui64 );

    constexpr auto expected_leftover = Matrix2x2{ -0.9, 900.8, 0.0, 69.8 };

    static_assert( leftover == expected_leftover );

    static_assert( expected_leftover.determinant() == -62.82 );

    static_assert( abs( matrix1.determinant() - 66246.786L ) < 1e-5 );
}


TEST( MatrixTests, MatrixTests )
{
    static_assert( IsMatrix<matrix<2, 3>> );

    static_assert( !IsMatrix<int> );

    static_assert( matrix<10, 10>::IsSquare() );

    static_assert( !matrix<2, 3>::IsSquare() );
}


TEST( MatrixTests, MatrixAdjointTests )
{
    constexpr auto matrix1 = Matrix4x4{
        5, -2, 2, 7, 1, 0, 0, 3, -3, 1, 5, 0, 3, -1, -9, 4,
    };

    constexpr auto adjoint{ matrix1.adjoint() };

    constexpr auto expected_adjoint = Matrix4x4{
        -12, 76, -60, -36, -56, 208, -82, -58, 4, 4, -2, -10, 4, 4, 20, 12,
    };

    static_assert( adjoint == expected_adjoint );
}


TEST( MatrixTests, IdentityMatrixTests )
{
    static_assert( IdentityMatrix<1>.determinant() == 1.0L );
    static_assert( IdentityMatrix<2>.determinant() == 1.0L );
    static_assert( IdentityMatrix<3>.determinant() == 1.0L );
}


TEST( MatrixTests, InverseTests )
{
    static_assert( IdentityMatrix<2>.inverse() == IdentityMatrix<2> );
    static_assert( IdentityMatrix<3>.inverse() == IdentityMatrix<3> );

    constexpr auto matrix1 = Matrix4x4{
        5, -2, 2, 7, 1, 0, 0, 3, -3, 1, 5, 0, 3, -1, -9, 4,
    };

    auto mat{ matrix1.adjoint() };

    constexpr auto det{ matrix1.determinant() };

    for( auto&& row : mat )
    {
        for( auto&& ele : row )
        {
            ele /= det;
        }
    }

    EXPECT_EQ( mat, matrix1.inverse() );
}
