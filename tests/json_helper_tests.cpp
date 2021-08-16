/**
 * @file json_helper_tests.cpp
 * @author ashwinn76
 * @brief
 * @version 0.1
 * @date 2021-08-16
 *
 * @copyright Copyright (c) 2021
 *
 */

#include "gtest/gtest.h"

#include <fstream>

#include <general/fs_include.hpp>

#include <json/json_helper.hpp>

TEST( JsonHelperTests, JsonToString )
{
    json js1 = std::stojson( "{ \"first\": \"trial\", \"second\": 2 }" );

    auto str{ std::to_string( js1 ) };

    auto js2 = std::stojson( str );

    EXPECT_EQ( js1["first"].get<std::string>(), js2["first"].get<std::string>() );
    EXPECT_EQ( js1["second"].get<int>(), js2["second"].get<int>() );
}


TEST( JsonHelperTests, StringToJson )
{
    std::string str{ "{ \"first\": \"trial\", \"second\": 2 }" };

    auto json = std::stojson( str );

    EXPECT_EQ( json["first"].get<std::string>(), "trial" );
    EXPECT_EQ( json["second"].get<int>(), 2 );
}