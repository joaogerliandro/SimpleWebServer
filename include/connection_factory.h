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

                std::string query = "SELECT * FROM User";
                boost::mysql::results result;

                con->query(query, result);

                int32_t row_index = 0;
                
                for(boost::mysql::row_view row : result.rows())
                {
                    std::string field_str = "";

                    for(boost::mysql::field_view field : row)
                    {
                        // Get the field string
                    }

                    row_index++;

                    std::cout << "Row #" << row_index << " -" << field_str << std::endl;
                }
            }

            boost::mysql::tcp_ssl_connection* get_connection()
            {
                if(con != nullptr)
                    return con;
                else
                    std::cout << "[WARNING]: Could not return a connection. A connection with host server is needed first !" << std::endl;
            }

            void disconnect()
            {
                con->close();
                delete con;
            }

        private:
            boost::mysql::tcp_ssl_connection *con;
    };
}