/**
 * @file macro_tests.cpp
 * @author ashwinn76
 * @brief
 * @version 0.1
 * @date 2020-10-10
 *
 * @copyright Copyright (c) 2020
 *
 */

#include "gtest/gtest.h"

#include <general/macro_utils>


TEST( MacroTests, NumericConverterTests_64bit )
{
    static_assert( std::is_same_v<decltype( 2_ui64 ), std::uint64_t> );
    static_assert( std::is_same_v<decltype( 1_i64 ), std::int64_t> );
}


TEST( MacroTests, NumericConverterTests_32bit )
{
    static_assert( std::is_same_v<decltype( 34_ui32 ), std::uint32_t> );
    static_assert( std::is_same_v<decltype( 23_i32 ), std::int32_t> );
}


TEST( MacroTests, NumericConverterTests_16bit )
{
    static_assert( std::is_same_v<decltype( 123_ui16 ), std::uint16_t> );
    static_assert( std::is_same_v<decltype( 543_i16 ), std::int16_t> );
}


TEST( MacroTests, NumericConverterTests_8bit )
{
    static_assert( std::is_same_v<decltype( 11_ui8 ), std::uint8_t> );
    static_assert( std::is_same_v<decltype( 12_i8 ), std::int8_t> );
}