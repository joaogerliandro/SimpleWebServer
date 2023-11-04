#include <common.h>

using boost::asio::ip::tcp;

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

std::string generate_response(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
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

int main()
{
    try
    {
        boost::asio::io_service io_service;
        tcp::acceptor acceptor(io_service, tcp::endpoint(tcp::v4(), 80));

        while (true)
        {
            tcp::socket *socket = new tcp::socket(io_service);

            acceptor.accept(*socket);

            boost::system::error_code ignored_error;

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

            if (request_line.find("GET") != std::string::npos) 
            {
                size_t first_space = request_line.find(' ');
                size_t second_space = request_line.find(' ', first_space + 1);
                std::string requested_file = request_line.substr(first_space + 1, second_space - first_space - 1);
                
                if (requested_file == "/") 
                {
                    requested_file = "index.html";
                }
                else
                    requested_file = requested_file.substr(1);

                std::string response = generate_response(requested_file);
                boost::asio::write(*socket, boost::asio::buffer(response), ignored_error);
            }

            socket->close();
        }
    }
    catch (std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }

    return EXIT_SUCCESS;
}