#pragma once

#include <common.h>

namespace http = boost::beast::http;

using namespace nlohmann;

namespace WebServer
{
    class RequestHandler
    {
        public:
            RequestHandler() 
            {
            }

            void process_request(const http::request<http::string_body>& req, boost::asio::ip::tcp::socket& socket)
            {
                if (req.method() == http::verb::get) 
                {
                    std::string requested_file(req.target().data(), req.target().size());

                    if (requested_file == "/") {
                        requested_file = "index.html";
                    } else {
                        requested_file = requested_file.substr(1);
                    }

                    boost::beast::http::response<boost::beast::http::string_body> response = generate_response(requested_file, req);

                    boost::beast::http::write(socket, response);
                }
                else if (req.method() == boost::beast::http::verb::post) 
                {
                    std::string post_data = req.body();  // Extrai os dados do corpo da solicitação POST

                    // Aqui você pode processar os dados POST conforme necessário
                    std::cout << "Dados POST recebidos: " << post_data << std::endl;

                    std::string post_response = "Solicitação POST recebida com sucesso!";
                    boost::beast::http::response<boost::beast::http::string_body> http_response{boost::beast::http::status::ok, req.version()};
                    http_response.set(boost::beast::http::field::server, "Boost Beast Server");
                    http_response.set(boost::beast::http::field::content_type, "text/plain");
                    http_response.keep_alive(req.keep_alive());
                    http_response.body() = post_response;
                    http_response.prepare_payload();

                    boost::beast::http::write(socket, http_response);
                }
            }

        private:
            boost::beast::http::response<boost::beast::http::string_body> generate_response(const std::string& filename, const http::request<http::string_body>& req) 
            {
                std::ifstream file(filename, std::ios::binary);
                if (!file.is_open()) 
                {
                    boost::beast::http::response<boost::beast::http::string_body> http_response{http::status::not_found, req.version()};
                    http_response.set(http::field::server, "Boost Beast Server");
                    http_response.set(http::field::content_type, "text/plain");
                    http_response.keep_alive(req.keep_alive());
                    http_response.body() = "Not Found !";
                    http_response.prepare_payload();

                    return http_response;
                }

                std::string file_extension = filename.substr(filename.find_last_of(".") + 1);
                std::string content_type = map_content_type(file_extension);

                std::string file_content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

                boost::beast::http::response<boost::beast::http::string_body> http_response{http::status::ok, req.version()};
                http_response.set(http::field::server, "Boost Beast Server");
                http_response.set(http::field::content_type, content_type);
                http_response.keep_alive(req.keep_alive());
                http_response.body() = file_content;
                http_response.prepare_payload();

                return http_response;
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