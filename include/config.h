#pragma once

#include <connection_factory.h>

namespace Util
{
    class Config
    {
        public:
            Config(std::string_view file_stream = { "config.cfg" })
            {
                load_default_setting();

                std::fstream config_file(file_stream.data());

                if(!config_file.is_open())
                {
                    std::cout << "[WARNING]: Could not read configuration file ! Creating a new ..." << std::endl;

                    config_file.open(file_stream.data(), std::fstream::in | std::fstream::out | std::fstream::trunc);

                    if(!config_file.is_open())
                        throw "[ERROR]: Unable to create settings file ! Leaving the program ...";

                    write_cfg_template(config_file);

                    std::cout << "[INFO]: Configuration file created !" << std::endl;
                }
                else
                {
                    read_config(config_file);
                }

                config_file.close();
            }

            void connect_to_database()
            {
                con_factory.connect(db_host, db_name, db_username, db_password);
            }

            sql::Connection* get_connection()
            {
                return con_factory.get_connection();
            }

            void disconnect_to_database()
            {
                con_factory.disconnect();
            }

        private:
            std::string db_host;
            std::string db_name;
            std::string db_username;
            std::string db_password;

            Database::ConnectionFactory con_factory;

            void load_default_setting()
            {
                db_host     = "host";
                db_name     = "database_name";
                db_username = "username";
                db_password = "password";
            }

            void write_cfg_template(std::fstream& cfg_file)
            {
                cfg_file << "Hostname = " << db_host     << "\n";
                cfg_file << "Database = " << db_name     << "\n";
                cfg_file << "Username = " << db_username << "\n";
                cfg_file << "Password = " << db_password << "\n";
            }

            void read_config(std::fstream& cfg_file)
            {
                std::string file_line;
                std::string line_content;

                std::stringstream str_stream;

                while(std::getline(cfg_file, file_line))
                {
                    str_stream.clear();
                    str_stream.str(file_line);

                    str_stream >> line_content;

                    if(line_content == "Hostname")
                    {
                        try
                        {
                            db_host = read_value(str_stream, line_content);
                        }
                        catch(const std::exception& e)
                        {
                            std::cerr << e.what() << '\n';
                        }
                    }
                    else if(line_content == "Database")
                    {
                        try
                        {
                            db_name = read_value(str_stream, line_content);
                        }
                        catch(const std::exception& e)
                        {
                            std::cerr << e.what() << '\n';
                        }
                    }
                    else if(line_content == "Username")
                    {
                        try
                        {
                            db_username = read_value(str_stream, line_content);
                        }
                        catch(const std::exception& e)
                        {
                            std::cerr << e.what() << '\n';
                        }
                    }
                    else if(line_content == "Password")
                    {
                        try
                        {
                            db_password = read_value(str_stream, line_content);
                        }
                        catch(const std::exception& e)
                        {
                            std::cerr << e.what() << '\n';
                        }
                    }
                }
            }

            std::string read_value(std::stringstream& str_stream, std::string option)
            {
                std::string line_content;
                str_stream >> line_content;

                if(line_content == "=")
                {
                    str_stream >> line_content;
                    return line_content.c_str();
                }
                else
                {
                    std::string ex = "[WARNING]: Could not load " + option + " configuration ! The default configuration will be kept.\n";
                    throw std::invalid_argument(ex);
                }
            }
    };
}