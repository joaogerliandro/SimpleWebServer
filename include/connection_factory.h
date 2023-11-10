#pragma once

#pragma once

#include <common.h>

namespace Database
{
    class ConnectionFactory
    {
        public:
            void connect(std::string hostname, std::string database, std::string username, std::string password)
            {
                boost::asio::io_context io_context;

                boost::asio::ssl::context ssl_ctx(boost::asio::ssl::context::tls_client);

                con = new boost::mysql::tcp_ssl_connection(io_context.get_executor(), ssl_ctx);

                boost::asio::ip::tcp::resolver resolver(io_context.get_executor());

                auto endpoints = resolver.resolve(hostname, boost::mysql::default_port_string);

                boost::mysql::handshake_params params(username, password, database);

                con->connect(*endpoints.begin(), params);
            }

            boost::mysql::tcp_ssl_connection* get_connection()
            {
                if(con != nullptr)
                    return con;
                else
                    std::cout << "[WARNING]: Could not return a connection. A connection with host server is needed first !" << std::endl;
            }

            std::vector<std::string> execute_query(std::string query)
            {
                std::vector<std::string> query_result;

                boost::mysql::results result_set;

                con->query(query, result_set);
                
                for(boost::mysql::row_view row : result_set.rows())
                {
                    std::string field_str = "";

                    for(boost::mysql::field_view field : row.as_vector())
                    {
                        field_str += (std::string(" ") + process_field(field));
                    }

                    query_result.push_back(field_str);
                }

                return query_result;
            }

            void disconnect()
            {
                con->close();
                delete con;
            }

        private:
            boost::mysql::tcp_ssl_connection *con;

            std::string process_field(boost::mysql::field_view field)
            {
                std::string result = "";

                if(field.is_null())
                {
                    result = std::string("NULL");
                }
                if(field.is_int64() || field.is_uint64())
                {
                    if(field.is_int64())
                        result = std::to_string(field.get_int64());
                    else
                        result = std::to_string(field.get_uint64());
                }
                else if(field.is_string())
                {
                    result = std::string(field.get_string().data());
                }
                else if(field.is_double() || field.is_float())
                {
                    if(field.is_double())
                        result = std::to_string(field.get_double());
                    else
                        result = std::to_string(field.get_float());
                }
                else if(field.is_date() || field.is_datetime())
                {
                    if(field.is_date())
                    {
                        boost::mysql::date date = field.get_date();
                        result = std::to_string(date.day()) + "/" + std::to_string(date.month()) + "/" + std::to_string(date.year());
                    }
                    else
                    {
                        boost::mysql::datetime datetime = field.get_datetime();
                        result = std::to_string(datetime.day()) + "/" + std::to_string(datetime.month()) + "/" + std::to_string(datetime.year())
                        + " - " + std::to_string(datetime.hour()) + ":" + std::to_string(datetime.minute()) 
                        + ":" + std::to_string(datetime.second()) + std::to_string(datetime.microsecond());
                    }
                }

                return result;
            }
    };
}