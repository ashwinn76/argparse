/**
 * @file test_arg_parse.cpp
 * @author ashwinn76
 * @brief
 * @version 0.1
 * @date 2021-06-23
 *
 * @copyright Copyright (c) 2021
 *
 */

#include "gtest/gtest.h"

#include <argparse/argparse>

#define ARGS_AND_N_ARGS( ... ) \
    const char* args[] = { __VA_ARGS__ }; \
    [[maybe_unused]] auto n_args{ sizeof( args ) / sizeof( args[0] ) };

TEST( ArgParseTests, SimpleParseTest )
{
    ARGS_AND_N_ARGS( "parse_tester", "first", "second=random", "--third=53" );

    auto parser{ parse::arg_parser{ args[0] } };

    parser.add_required_arg( "first", "", "this is the first argument" );
    parser.add_required_arg( "second", "", "this is the second argument" );
    parser.add_optional_arg( "third", "", "this is the third argument" );

    EXPECT_EQ( parser.size(), 3_sz );

    parser.parse( n_args, args );

    EXPECT_TRUE( parser.has_argument( "first" ) );

    EXPECT_TRUE( parser.has_argument( "second" ) );
    EXPECT_EQ( parser["second"], "random" );

    EXPECT_TRUE( parser.has_argument( "third" ) );
    EXPECT_EQ( parser["third"], "53" );
    EXPECT_EQ( parser.get<int>( "third" ), 53 );

    EXPECT_THROW( parser["fourth"], std::out_of_range );
}


TEST( ArgParseTests, ParseErrorTests )
{
    ARGS_AND_N_ARGS( "parse_tester", "second=random", "--third=53" );

    auto parser{ parse::arg_parser{ args[0] } };

    parser.add_required_arg( "first", "", "this is the first argument" );
    parser.add_required_arg( "second", "", "this is the second argument" );
    parser.add_optional_arg( "third", "", "this is the third argument" );

    EXPECT_THROW( parser.parse( n_args, args ), std::logic_error );
}


TEST( ArgParseTests, OptionalArgTests )
{
    ARGS_AND_N_ARGS( "parse_tester", "first", "--third=random", "--second=53" );

    auto parser{ parse::arg_parser{ args[0] } };

    parser.add_required_arg( "first", "", "this is the first" );
    parser.add_optional_arg( "second", "", "this is the second argument" );
    parser.add_optional_arg( "third", "", "this is the third argument" );

    parser.parse( n_args, args );

    EXPECT_TRUE( parser.has_argument( "second" ) );
    EXPECT_TRUE( parser.has_argument( "third" ) );

    EXPECT_EQ( parser["third"], "random" );
    EXPECT_EQ( parser.get<int>( "second" ), 53 );
}


TEST( ArgParseTests, ErrorIfRequiredArgumentsNotProvidedTests )
{
    ARGS_AND_N_ARGS( "parse_tester", "first", "second=53", "third=stupid" );

    auto parser{ parse::arg_parser{ args[0] } };

    parser.add_required_arg( "first", "", "this is the first" );
    parser.add_required_arg( "second", "", "this is the second argument" );
    parser.add_required_arg( "third", "", "this is the third argument" );

    EXPECT_THROW( parser.parse( 3, args ), std::logic_error );

    parser.parse( n_args, args );

    EXPECT_EQ( parser["third"], "stupid" );
}


TEST( ArgParseTests, ArgumentValuesSetToTrueIfNoneProvidedTests )
{
    ARGS_AND_N_ARGS( "parse_tester", "first", "--second=53", "--third" );

    auto parser{ parse::arg_parser{ args[0] } };

    parser.add_required_arg( "first", "", "this is the first" );
    parser.add_optional_arg( "second", "", "this is the second argument" );
    parser.add_optional_arg( "third", "", "this is the third argument" );

    parser.parse( 3, args );

    EXPECT_TRUE( parser.get<bool>( "first" ) );
    EXPECT_EQ( parser.get<int>( "second" ), 53 );
    EXPECT_FALSE( parser.get<bool>( "third" ) );

    parser.parse( 2, args + 2 );

    EXPECT_TRUE( parser.get<bool>( "third" ) );
}

TEST( ArgParseTests, OptionalArgumentsDefaultToFalseIfNotProvidedTests )
{
    ARGS_AND_N_ARGS( "parse_tester", "first", "--second=53", "--third=stupid" );

    auto parser{ parse::arg_parser{ args[0] } };

    parser.add_required_arg( "first", "", "this is the first" );
    parser.add_optional_arg( "second", "", "this is the second argument" );
    parser.add_optional_arg( "third", "", "this is the third argument" );

    parser.parse( 3, args );
    EXPECT_FALSE( parser.get<bool>( "third" ) );

    parser.clear();

    parser.parse( n_args, args );
    EXPECT_EQ( parser["third"], "stupid" );
}