/**
 * @file argparse.hpp
 * @author ashwinn76
 * @brief
 * @version 0.1
 * @date 2021-06-21
 *
 * @copyright Copyright (c) 2021
 *
 */

#include <algorithm>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

using namespace std::literals;

constexpr auto operator""_sz( unsigned long long __n ) noexcept
{
    return static_cast<std::size_t>( __n );
}


namespace parse
{
class arg
{
private:
    std::string m_name{};
    std::string m_abbr{};
    std::string m_help{};
    std::optional<std::string> m_val{};


public:
    arg() noexcept = default;

    arg( std::string i_name, std::string i_abbr, std::string i_help ) noexcept :
        m_name{ std::move( i_name ) }, m_abbr{ std::move( i_abbr ) }, m_help{ std::move( i_help ) }
    {
    }


    auto is_empty_arg() const noexcept
    {
        return !m_val.has_value();
    }


    void set( std::string i_val ) noexcept
    {
        m_val = std::move( i_val );
    }


    void clear() noexcept
    {
        m_val.reset();
    }

    decltype( auto ) get_name() const noexcept
    {
        return ( m_name );
    }


    decltype( auto ) get_abbreviation() const noexcept
    {
        return ( m_abbr );
    }


    decltype( auto ) get_help() const noexcept
    {
        return ( m_help );
    }


    decltype( auto ) get_val() const noexcept
    {
        return ( m_val );
    }


    template<typename _Tp = std::string>
    decltype( auto ) get() const
    {
        if constexpr( std::is_same_v<_Tp, int> )
        {
            return std::stoi( m_val.value() );
        }
        else if constexpr( std::is_same_v<_Tp, double> )
        {
            return std::stod( m_val.value() );
        }
        else if constexpr( std::is_same_v<_Tp, bool> )
        {
            if( m_val == "false" )
            {
                return false;
            }
            else if( m_val == "true" )
            {
                return true;
            }
            else
            {
                throw std::invalid_argument{ "unable to parse a boolean out of the string!" };
            }
        }
        else if constexpr( std::is_same_v<_Tp, std::string> )
        {
            return ( m_val.value() );
        }
    }
};


class arg_parser
{
private:
    std::string m_application_name{};
    std::vector<arg> m_required_args{};
    std::vector<arg> m_optional_args{};


    auto find_arg( std::string_view i_str ) const
    {
        auto check_arg = [&]( auto&& arg ) { return arg.get_name() == i_str; };

        auto iter{ std::find_if( m_required_args.begin(), m_required_args.end(), check_arg ) };

        if( iter != m_required_args.end() )
        {
            return std::make_pair( iter, true );
        }

        iter = std::find_if( m_optional_args.begin(), m_optional_args.end(), check_arg );

        if( iter != m_optional_args.end() )
        {
            return std::make_pair( iter, true );
        }

        return std::make_pair( iter, false );
    }


    auto parse( std::string i_str )
    {
        auto required{ true };

        if( i_str[0] == '-' && i_str[1] == '-' )
        {
            required = false;
            i_str = i_str.substr( 2_sz );
        }

        auto pos{ i_str.find( '=' ) };

        auto arg_name{ i_str.substr( 0_sz, pos ) };
        auto arg_val{ "true"s };  // Assign default value of true to argument so that we can use it without issues

        if( pos != std::string::npos )
        {
            arg_val = i_str.substr( pos + 1_sz );
        }

        return std::make_tuple( required, std::move( arg_name ), std::move( arg_val ) );
    }

    void check_required_arguments_parsed( std::vector<arg>::iterator* i_iterator_ptr = nullptr )
    {
        auto end_iter{ i_iterator_ptr != nullptr ? *i_iterator_ptr : m_required_args.end() };

        auto all_parsed{ true };

        for( auto iter{ m_required_args.begin() }; iter != end_iter; ++iter )
        {
            all_parsed &= !iter->is_empty_arg();

            if( !all_parsed )
            {
                break;
            }
        }

        if( !all_parsed )
        {
            if( i_iterator_ptr != nullptr )
            {
                throw std::logic_error{ "A required argument was missed!" };
            }
            throw std::logic_error{ "All required arguments need to be placed before optional arguments!" };
        }
    }

public:
    arg_parser( std::string_view i_application_name ) noexcept : m_application_name{ std::move( i_application_name ) }
    {
    }


    void add_required_arg( std::string_view i_str, std::string_view i_abbr_str, std::string_view i_help_str )
    {
        if( m_optional_args.empty() )
        {
            m_required_args.emplace_back( std::string{ std::move( i_str ) },
                                          std::string{ std::move( i_abbr_str ) },
                                          std::string{ std::move( i_help_str ) } );
        }
        else
        {
            throw std::invalid_argument{ "You cannot add required argument after adding a positional argument!" };
        }
    }


    void add_optional_arg( std::string_view i_str, std::string_view i_abbr_str, std::string_view i_help_str )
    {
        m_optional_args.emplace_back( std::string{ std::move( i_str ) },
                                      std::string{ std::move( i_abbr_str ) },
                                      std::string{ std::move( i_help_str ) } );
    }


    auto size() const noexcept
    {
        return m_required_args.size() + m_optional_args.size();
    }


    std::string operator[]( std::string_view i_str ) const
    {
        if( auto [iter, found] = find_arg( std::move( i_str ) ); found )
        {
            if( auto& val{ iter->get_val() }; val.has_value() )
            {
                return val.value();
            }

            throw std::invalid_argument{ "Value not set yet!" };
        }

        throw std::out_of_range{ "Invalid key!" };
    }


    template<typename _Tp = std::string>
    decltype( auto ) get( std::string_view i_str )
    {
        if( auto [iter, found] = find_arg( std::move( i_str ) ); found )
        {
            return iter->get<_Tp>();
        }
        else
        {
            throw std::invalid_argument{ "Key not found!" };
        }
    }


    bool has_argument( std::string_view i_str )
    {
        auto [_, found] = find_arg( std::move( i_str ) );

        return found;
    }


    void parse( int i_argc, const char* i_argv[] )
    {
        for( auto i{ 1 }; i < i_argc; i++ )
        {
            auto [required, arg_name, arg_val] = parse( std::string{ i_argv[i] } );

            auto& container{ required ? m_required_args : m_optional_args };

            auto iter = std::find_if(
                container.begin(), container.end(), [&]( auto&& i_arg ) { return i_arg.get_name() == arg_name; } );

            if( iter != container.end() )
            {
                check_required_arguments_parsed( required ? std::addressof( iter ) : nullptr );

                iter->set( std::move( arg_val ) );
            }
        }

        check_required_arguments_parsed();

        for( auto&& arg : m_optional_args )
        {
            if( arg.is_empty_arg() )
            {
                arg.set( "false"s );
            }
        }
    }

    void clear()
    {
        for( auto&& arg : m_required_args )
        {
            arg.clear();
        }

        for( auto&& arg : m_optional_args )
        {
            arg.clear();
        }
    }
};

}
