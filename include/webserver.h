#pragma once

#include <config.h>
#include <entities.h>

using boost::asio::ip::tcp;

namespace WebServer
{
    class RequestHandler
    {
        public:
            RequestHandler() 
            {
                web_server_config = new Util::Config();

                web_server_config->connect_to_database();

                user_dao = new Database::DAO::UserDAO(web_server_config->get_connection_factory());
            }

            void process_request(const std::string& request, tcp::socket *socket)
            {
                boost::system::error_code ignored_error;

                if (request.find("GET") != std::string::npos) 
                {
                    size_t first_space = request.find(' ');
                    size_t second_space = request.find(' ', first_space + 1);
                    std::string requested_file = request.substr(first_space + 1, second_space - first_space - 1);
                    
                    if (requested_file == "/") 
                    {
                        requested_file = "index.html";
                    }
                    else
                        requested_file = requested_file.substr(1);

                    std::string response = generate_response(requested_file);
                    boost::asio::write(*socket, boost::asio::buffer(response), ignored_error);
                }
            }

        private:
            Util::Config *web_server_config;
            Database::DAO::UserDAO *user_dao;

            std::string generate_response(const std::string& filename) 
            {
                std::ifstream file(filename, std::ios::binary);
                if (!file.is_open()) 
                {
                    return "HTTP/1.1 404 Not Found\r\n\r\nFile Not Found";
                }

                std::string file_extension = filename.substr(filename.find_last_of(".") + 1);
                std::string content_type = map_content_type(file_extension);

                std::string response = "HTTP/1.1 200 OK\r\n";
                response += "Content-Type: " + content_type + "\r\n\r\n";
                std::string file_content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
                response += file_content;

                return response;
            }

            std::string map_content_type(const std::string& file_extension) 
            {
                if (file_extension == "html") 
                {
                    return "text/html";
                } 
                else if (file_extension == "css") 
                {
                    return "text/css";
                } 
                else if (file_extension == "js") 
                {
                    return "application/javascript";
                } 
                else if (file_extension == "jpg" || file_extension == "jpeg") 
                {
                    return "image/jpeg";
                } 
                else if (file_extension == "png") 
                {
                    return "image/png";
                } 
                else if (file_extension == "gif") 
                {
                    return "image/gif";
                }
                else if (file_extension == "svg") 
                {
                    return "image/svg+xml";
                }

                return "application/octet-stream";
            }
    };
}