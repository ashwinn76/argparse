/**
 * @file curl_helper.hpp
 * @author ashwinn76
 * @brief
 * @version 0.1
 * @date 2021-08-16
 *
 * @copyright Copyright (c) 2021
 *
 */

#pragma once

#ifndef CURL_STATICLIB
#    error "Please make sure you have defined CURL_INCLUDE_DIR, CURL_LIBRARY and CURL_STATICLIB in your project"
#endif

#include <memory>

#include <curl/curl.h>

#include <general/macro_utils.hpp>

namespace stdh::internet
{
/**
 * @brief Handler to ensure proper initialization and cleanup of cURL
 *
 */
class curl_handler
{
public:
    curl_handler()
    {
        curl_global_init( CURL_GLOBAL_DEFAULT );
    }

    ~curl_handler()
    {
        curl_global_cleanup();
    }
};

using curl_handle_ptr = std::unique_ptr<CURL, decltype( &curl_easy_cleanup )>;
using curl_list_ptr = std::unique_ptr<curl_slist, decltype( &curl_slist_free_all )>;

/**
 * @brief handler class for collecting the response from curl operation
 *
 */
class curl_response
{
private:
    /**
     * @brief writer function for curl
     *
     * @param i_ptr incoming new data
     * @param i_size size of character
     * @param i_nchars number of characters
     * @param i_data_ptr data to write to
     * @return amount of data written
     */
    static std::size_t handler( void* i_ptr, std::size_t i_size, std::size_t i_nchars, void* i_data_ptr )
    {
        if( i_data_ptr != nullptr )
        {
            reinterpret_cast<std::string*>( i_data_ptr )->append( reinterpret_cast<char*>( i_ptr ), i_size * i_nchars );
        }

        return i_size * i_nchars;
    }

public:
    NO_COPY_NO_MOVE_CLASS( curl_response );

    std::string m_header{};
    std::string m_response{};

    /**
     * @brief Construct a new curl response object
     *
     * @param i_curl_ptr incoming curl handle
     * @param i_response whether to collect response
     * @param i_header whether to collect header
     */
    curl_response( CURL* i_pcurl, bool i_response = false, bool i_header = false )
    {
        curl_easy_setopt( i_pcurl, CURLOPT_WRITEFUNCTION, &curl_response::handler );

        auto presponse = i_response ? std::addressof( m_response ) : nullptr;
        curl_easy_setopt( i_pcurl, CURLOPT_WRITEDATA, presponse );

        auto pheader = i_header ? std::addressof( m_header ) : nullptr;
        curl_easy_setopt( i_pcurl, CURLOPT_WRITEHEADER, pheader );
    }
};
}