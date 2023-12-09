#include <webserver.h>

namespace beast = boost::beast;    
namespace http = beast::http;
namespace net = boost::asio;
using tcp = net::ip::tcp;

int main()
{
    try
    {
        net::io_context io_context;
        tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), 80));

        WebServer::RequestHandler *request_handler = new WebServer::RequestHandler();

        while (true)
        {
            tcp::socket socket(io_context);
            acceptor.accept(socket);

            beast::flat_buffer buffer;
            http::request<http::string_body> request;
            http::read(socket, buffer, request);

            request_handler->process_request(request, socket);
        }

        delete request_handler;
    }
    catch (std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }

    return EXIT_SUCCESS;
}