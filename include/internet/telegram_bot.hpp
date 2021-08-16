/**
 * @file telegram_bot.hpp
 * @author ashwinn76
 * @brief
 * @version 0.1
 * @date 2021-08-16
 *
 * @copyright Copyright (c) 2021
 *
 */

#pragma once

#include <cstdint>
#include <mutex>
#include <string>

#include <general/fs_include.hpp>

#include <internet/curl_helper.hpp>

#include <json/json_helper.hpp>

namespace stdh::internet
{
class telegram_bot
{
private:
    std::mutex m_mtx;

    fs::path m_credentials{};
    std::string m_url{ "https://api.telegram.org/bot" };

    bool m_logMessages{ true };

    json m_document{};

    /**
     * @brief Process the json response to extract message info
     *
     * @param i_json incoming json
     * @param i_user_id user id that needs to be parsed for
     * @return last update id and a list of commands
     */
    auto process_updates( const json& i_json, std::int64_t i_user_id )
    {
        auto latest_update_id{ 0_i64 };

        auto cmds{ std::vector<std::string>{} };
        cmds.reserve( i_json["result"].size() );

        for( auto&& update : i_json["result"] )
        {
            latest_update_id = update["update_id"].get<std::int64_t>();

            if( update.contains( "message" ) )
            {
                auto& message{ update["message"] };

                auto current_user_id{ message["chat"]["id"].get<std::int64_t>() };

                if( current_user_id == i_user_id )
                {
                    cmds.emplace_back( message["text"].get<std::string>() );
                }
            }
        }

        return std::make_pair( latest_update_id, std::move( cmds ) );
    }

public:
    /**
     * @brief Construct a new telegram bot object
     *
     * @param i_path file path for credentials
     * @param i_logMessages whether to log messages through telegram app
     */
    telegram_bot( fs::path i_path, bool i_logMessages );

    /**
     * @brief Destroy the telegram bot object
     *
     */
    virtual telegram_bot::~telegram_bot();

    /**
     * @brief Send message using app
     *
     * @param i_message incoming message
     */
    void send_message( const std::string& i_message ) const;

    /**
     * @brief Process updates from Telegram
     *
     * @param i_create_tasks Whether to create tasks from updates
     */
    void process_updates( bool i_create_tasks = true );

    /**
     * @brief Add task to main manager
     *
     * @param i_cmd incoming command
     */
    virtual void add_task( std::string i_cmd ){};
};

telegram_bot::telegram_bot( fs::path i_path, bool i_logMessages ) :
    m_credentials{ std::move( i_path ) }, m_logMessages{ i_logMessages }
{
    if( auto stream{ std::ifstream{ this->m_credentials } }; stream.good() )
    {
        stream >> this->m_document;

        this->m_url.append( this->m_document["bot_key"].get<std::string>() ).append( "/" );
    }

    this->process_updates( false );

    if( this->m_logMessages )
    {
        this->send_message( "Bot has started" );
    }
}


telegram_bot::~telegram_bot()
{
    if( this->m_logMessages )
    {
        this->send_message( "Bot is shutting down" );
    }

    if( auto stream{ std::ofstream{ this->m_credentials } }; stream.good() )
    {
        stream << this->m_document;
    }
}


void telegram_bot::send_message( const std::string& i_message ) const
{
    std::lock_guard lk{ this->m_mtx };

    auto complete_url{ this->m_url + "sendMessage" };

    json js;
    js["chat_id"] = this->m_document["user_id"].get<std::int64_t>();
    js["text"] = i_message;

    auto str{ std::to_string( js ) };

    auto curl_list_ptr{ curl_list_ptr{ curl_slist_append( nullptr, "Content-Type: application/json" ),
                                       curl_slist_free_all } };

    auto curl_ptr{ curl_handle_ptr{ curl_easy_init(), curl_easy_cleanup } };

    curl_easy_setopt( curl_ptr.get(), CURLOPT_URL, complete_url.c_str() );

    curl_easy_setopt( curl_ptr.get(), CURLOPT_POSTFIELDS, str.c_str() );
    curl_easy_setopt( curl_ptr.get(), CURLOPT_POSTFIELDSIZE, str.size() );
    curl_easy_setopt( curl_ptr.get(), CURLOPT_POST, 1 );

    curl_easy_setopt( curl_ptr.get(), CURLOPT_HTTPHEADER, curl_list_ptr.get() );

    auto response{ curl_response{ curl_ptr.get() } };

    curl_easy_perform( curl_ptr.get() );
}


void telegram_bot::process_updates( bool i_create_tasks /*= true*/ )
{
    std::lock_guard lk{ this->m_mtx };

    auto full_url{ this->m_url + "getUpdates?offset=" +
                   std::to_string( this->m_document["last_update_id"].get<std::int64_t>() + 1_i64 ) };

    auto curl_ptr{ curl_handle_ptr{ curl_easy_init(), curl_easy_cleanup } };

    curl_easy_setopt( curl_ptr.get(), CURLOPT_URL, full_url.c_str() );

    auto response{ curl_response{ curl_ptr.get(), true } };

    curl_easy_perform( curl_ptr.get() );

    auto js = std::stojson( response.m_response );

    auto [new_update_id, messages] = ::process_updates( js, this->m_document["user_id"].get<std::int64_t>() );

    if( new_update_id != 0_i64 )
    {
        this->m_document["last_update_id"] = new_update_id;
    }

    if( i_create_tasks )
    {
        for( auto&& message : messages )
        {
            this->add_task( std::move( message ), m_manager );
        }
    }
}
}
