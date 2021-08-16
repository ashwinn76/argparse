/**
 * @file algo_utils.hpp
 * @author ashwinn76
 * @brief Utility algorithms
 * @version 0.1
 * @date 2020-08-22
 *
 * @copyright Copyright (c) 2020
 *
 */

#pragma once

#include <random>

#include <general/type_trait_utils.hpp>

/**
 * @brief Get the char address of the object
 *
 * @tparam _T template type
 * @param object object for retrieving char address
 * @return char address of the object
 */
template<typename _T>
auto get_char_address( _T&& object )
{
    return (char*)std::addressof( object );
}

#define IO_INFORMATION( x ) get_char_address( x ), sizeof( std::remove_cv_t<std::remove_reference_t<decltype( x )>> )


/**
 * @brief check if given value is within provided range
 *
 * @tparam _T template type
 * @param value value to check
 * @param value1 first bound
 * @param value2 second bound
 * @return whether value is in range
 */
template<typename _T>
constexpr auto in_range( const _T& value, const _T& value1, const _T& value2 )
{
    return std::min( value1, value2 ) <= value && value <= std::max( value, value2 );
}


/**
 * @brief Get a random value in the specified range
 *
 * @tparam _T type of random value
 * @param i_min first bound of the range
 * @param i_max second bound of the range
 * @return random value
 */
template<typename _T>
auto get_random_value( _T i_min, _T i_max )
{
    static_assert( std::is_integral_v<_T> || std::is_enum_v<_T>, "Only integral or enum types are allowed!" );
    using my_gen = std::mt19937_64;

    auto generator{ my_gen{ static_cast<my_gen::result_type>( std::rand() ) } };

    auto min{ static_cast<long long>( i_min ) };
    auto max{ static_cast<long long>( i_max ) };

    auto distributor{ std::uniform_int_distribution{ min, max } };

    return static_cast<decltype( i_min )>( distributor( generator ) );
}


/**
 * @brief Get randomized value within the limits set by the type of the input parameter.
 *
 * @tparam T Template type constrained by IsBound concept.
 */
template<typename _T>
auto get_random_value()
{
    static_assert( IsBound<_T>, "Type has to be bound!" );
    return _T{ get_random_value( _T::min(), _T::max() ) };
}
