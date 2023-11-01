#include <common.h>

using boost::asio::ip::tcp;

std::string read_html_file(const std::string& file_path) 
{
    std::ifstream file(file_path);
    if (file.is_open()) 
    {
        std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        return content;
    } 
    else 
    {
        return "HTTP/1.1 404 Not Found\r\nContent-Type: text/html\r\n\r\nFile Not Found";
    }
}

int main()
{
    try
    {
        boost::asio::io_service io_service;
        tcp::acceptor acceptor(io_service, tcp::endpoint(tcp::v4(), 80));

        std::cout << "[SERVER]: Awaiting connections ..." << std::endl;

        while (true)
        {
            tcp::socket *socket = new tcp::socket(io_service);

            acceptor.accept(*socket);

            std::cout << "[SERVER]: Connection established with [" << socket->remote_endpoint() << "]" << std::endl;

            std::string file_content = read_html_file("index.html");
            std::string response;

            if (file_content.find("404 Not Found") == std::string::npos) 
            {
                response = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n" + file_content;
            } 
            else 
            {
                response = file_content;
            }

            boost::system::error_code ignored_error;
            boost::asio::write(*socket, boost::asio::buffer(response), ignored_error);

            std::cout << "[SERVER]: Connection closed with ..." << socket->remote_endpoint() << "]" << std::endl;
        }
    }
    catch (std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }

    return EXIT_SUCCESS;
}