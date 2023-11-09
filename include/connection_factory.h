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
                //connect(hostname, username, password);
                //change_database(database);
            }

            /*void change_database(std::string database)
            {
                try
                {
                    if(con != nullptr)
                        con->setSchema(database);
                    else
                        std::cout << "[WARNING]: Could not change the database. A connection with host server is needed first !" << std::endl;
                }
                catch (sql::SQLException &ex)
                {
                    std::cout << "[ERROR]: "            << ex.what()
                              << " (MySQL error code: " << ex.getErrorCode()
                              << ", SQLState: "         << ex.getSQLState()
                              << " )"                   << std::endl;
                }
            }*/

            void /* sql::Connection* */ get_connection()
            {
                /*if(con != nullptr)
                    return con;
                else
                    std::cout << "[WARNING]: Could not return a connection. A connection with host server is needed first !" << std::endl;*/
            }

            void disconnect()
            {
                //delete con;
            }

        private:
            //sql::Driver *driver;
            //sql::Connection *con;

            /*void connect(std::string hostname, std::string username, std::string password)
            {
                try
                {
                    driver = get_driver_instance();
                    con = driver->connect(hostname, username, password);
                }
                catch (sql::SQLException &ex)
                {
                    std::cout << "[ERROR]: "            << ex.what()
                              << " (MySQL error code: " << ex.getErrorCode()
                              << ", SQLState: "         << ex.getSQLState()
                              << " )"                   << std::endl;
                }
            }*/
    };
}