/**
 * @file test_arg.cpp
 * @author ashwinn76
 * @brief
 * @version 0.1
 * @date 2021-06-27
 *
 * @copyright Copyright (c) 2021
 *
 */


#include "gtest/gtest.h"

#include <argparse/argparse>

void check_arg( const parse::arg& i_arg,
                std::string i_name,
                std::string i_abbr,
                std::string i_help,
                std::optional<std::string> i_val,
                bool is_empty )
{
    EXPECT_EQ( i_arg.get_name(), i_name );
    EXPECT_EQ( i_arg.get_abbreviation(), i_abbr );
    EXPECT_EQ( i_arg.get_help(), i_help );

    EXPECT_EQ( i_arg.get_val(), i_val );

    EXPECT_EQ( i_arg.is_empty_arg(), is_empty );
}

TEST( ArgTests, EmptyArgTests )
{
    auto arg{ parse::arg{} };

    check_arg( arg, "", "", "", std::nullopt, true );
}


TEST( ArgTests, IntArgTests )
{
    auto arg{ parse::arg{ "arg", "", "argument help" } };

    arg.set( "123" );

    check_arg( arg, "arg", "", "argument help", "123", false );

    EXPECT_EQ( arg.get<int>(), 123 );
    EXPECT_EQ( arg.get<double>(), 123.0 );

    EXPECT_THROW( arg.get<bool>(), std::invalid_argument );
}

TEST( ArgTests, StringArgTests )
{
    auto arg{ parse::arg{ "random_name", "rn", "random argument for testing" } };

    arg.set( "a stupid value" );

    check_arg( arg, "random_name", "rn", "random argument for testing", "a stupid value", false );

    static_assert( std::is_same_v<decltype( arg.get() ), const std::string&> );

    EXPECT_EQ( arg.get(), "a stupid value" );

    EXPECT_THROW( arg.get<int>(), std::invalid_argument );
    EXPECT_THROW( arg.get<double>(), std::invalid_argument );
    EXPECT_THROW( arg.get<bool>(), std::invalid_argument );
}


TEST( ArgTests, DblArgTests )
{
    auto arg{ parse::arg{ "dbl_arg", "da", "another argument" } };

    arg.set( "-234.5712" );

    check_arg( arg, "dbl_arg", "da", "another argument", "-234.5712", false );

    EXPECT_EQ( arg.get<int>(), -234 );
    EXPECT_DOUBLE_EQ( arg.get<double>(), -234.5712 );

    EXPECT_THROW( arg.get<bool>(), std::invalid_argument );
}


TEST( ArgTests, BoolArgTests )
{
    auto arg{ parse::arg{ "bool_arg", "ba", "another argument" } };

    arg.set( "false" );

    check_arg( arg, "bool_arg", "ba", "another argument", "false", false );

    EXPECT_THROW( arg.get<int>(), std::invalid_argument );
    EXPECT_THROW( arg.get<double>(), std::invalid_argument );

    EXPECT_FALSE( arg.get<bool>() );

    arg.set( "true" );

    EXPECT_TRUE( arg.get<bool>() );
}