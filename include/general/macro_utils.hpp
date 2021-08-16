/**
 * @file macro_utils.hpp
 * @author ashwinn76
 * @brief Macros used in the repo
 * @version 0.1
 * @date 2020-08-20
 *
 * @copyright Copyright (c) 2020
 *
 */

#pragma once

#include <cstdint>

///////////////////// Defining consteval operator

#ifdef __cpp_consteval
#    define __CONSTEVAL consteval
#else
#    define __CONSTEVAL constexpr
#endif


///////////////////// Defining concepts

#if( defined __cpp_concepts && defined __cpp_lib_concepts )
#    define __CONCEPTS
#endif


///////////////////// Defining arithmetic operators

#define UNSIGNED_CONVERTER( x ) \
    __CONSTEVAL auto operator"" _ui##x( unsigned long long __n ) noexcept \
    { \
        return static_cast<uint##x##_t>( __n ); \
    }
#define SIGNED_CONVERTER( x ) \
    __CONSTEVAL auto operator"" _i##x( unsigned long long __n ) noexcept \
    { \
        return static_cast<int##x##_t>( __n ); \
    }

#define CONVERTER( x ) UNSIGNED_CONVERTER( x ) SIGNED_CONVERTER( x )

CONVERTER( 8 );
CONVERTER( 16 );
CONVERTER( 32 );
CONVERTER( 64 );

__CONSTEVAL std::size_t operator"" _sz( unsigned long long __n ) noexcept
{
    return static_cast<std::size_t>( __n );
}


///////////////////// Useful macros

#define RAW( X ) std::remove_reference_t<std::remove_cv_t<X>>


#define NO_COPY_CLASS( x ) \
    x( const x& ) = delete; \
    x& operator=( const x& ) = delete;

#define NO_MOVE_CLASS( x ) \
    x( x&& ) = delete; \
    x& operator=( x&& ) = delete;

#define NO_COPY_NO_MOVE_CLASS( x ) NO_COPY_CLASS( x ) NO_MOVE_CLASS( x )
