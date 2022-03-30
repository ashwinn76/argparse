/**
 * @file json_helper.hpp
 * @author ashwinn76
 * @brief
 * @version 0.1
 * @date 2021-08-16
 *
 * @copyright Copyright (c) 2021
 *
 */

#pragma once

#include <sstream>
#include <string>

#ifdef _MSC_VER
#    define _snprintf snprintf
#endif

#include <json/json.hpp>

using namespace nlohmann;


namespace std
{
inline json stojson( const std::string& i_str )
{
    auto js{ json{} };
    if( auto stream{ std::stringstream{ i_str } }; stream.good() )
    {
        stream >> js;
    }

    return js;
}

inline std::string to_string( const json& i_json )
{
    auto stream{ std::ostringstream{} };

    stream << i_json;

    return stream.str();
}
}
