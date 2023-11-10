#include <webserver.h>

using boost::asio::ip::tcp;

int main()
{
    try
    {
        boost::asio::io_service io_service;
        tcp::acceptor acceptor(io_service, tcp::endpoint(tcp::v4(), 80));

        WebServer::RequestHandler *request_handler = new WebServer::RequestHandler();

        while (true)
        {
            tcp::socket *socket = new tcp::socket(io_service);

            acceptor.accept(*socket);

            boost::asio::streambuf request;

            try
            {
                boost::asio::read_until(*socket, request, "\r\n\r\n");
            }
            catch(const std::exception& e)
            {
                socket->close();
                continue;
            }
            
            std::istream request_stream(&request);
            std::string request_line;
            std::getline(request_stream, request_line);

            request_handler->process_request(request_line, socket);

            socket->close();
        }

        delete request_handler;
    }
    catch (std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }

    return EXIT_SUCCESS;
}