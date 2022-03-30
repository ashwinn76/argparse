/**
 * @file bound_value_tests.cpp
 * @author ashwinn76
 * @brief
 * @version 0.1
 * @date 2020-10-10
 *
 * @copyright Copyright (c) 2020
 *
 */


#include "gtest/gtest.h"

#include <math/bound_value>

TEST( BoundValueTests, BoundValueTemplateTests )
{
    static_assert( IsBound<bound_value<1, 2>> );
}


TEST( BoundValueTests, NotInRangeTests )
{
    static_assert( !in_range( 1, 2, 3 ) );
    static_assert( !in_range( 1, 3, 2 ) );
}


TEST( BoundValueTests, InRangeTests )
{
    static_assert( in_range( 1, 0, 2 ) );
    static_assert( in_range( 1, 2, 0 ) );

    static_assert( in_range( 23, -90, 89 ) );
    static_assert( in_range( 23, 89, -90 ) );

    static_assert( in_range( -1, 0, -2 ) );
    static_assert( in_range( -1, -2, 0 ) );
}


TEST( BoundValueTests, BoundValueMinMaxTests )
{
    static_assert( bound_value<11, 22>::min() == 11 );
    static_assert( bound_value<23, 45>::max() == 45 );

    static_assert( bound_value<-23, -12>::min() == -23 );
    static_assert( bound_value<-11, -10>::max() == -10 );
}


TEST( BoundValueTests, BoundValueEqualityTests )
{
    using sample_bound = bound_value<12, 23>;

    static_assert( 12 == sample_bound{ 12 } );
    static_assert( sample_bound{ 12 } == 12 );

    static_assert( sample_bound{ 23 } == sample_bound{ 23 } );
    static_assert( !( sample_bound{ 20 } == sample_bound{ 21 } ) );

    EXPECT_THROW( sample_bound{ 1 }, std::out_of_range );
    EXPECT_NO_THROW( sample_bound{ 22 } );
}


TEST( BoundValueTests, RandomValueTests )
{
    constexpr auto min = -2;
    constexpr auto max = 101;

    using bound_type = bound_value<min, max>;

    auto val3{ get_random_value<bound_type>() };

    EXPECT_TRUE( in_range( val3.value(), bound_type::min(), bound_type::max() ) );
}
