#pragma once

#include <connection_factory.h>
#include <entities/user.h>

namespace Database::DAO
{
    class UserDAO
    {
        public:
            UserDAO(Database::ConnectionFactory *con_fact) : dao_con_fact(con_fact) {  };

            Entities::User get_user_by_username(std::string username)
            {
                Entities::User user;

                // Prepare Statement

                // Execute Statement

                // Trait result

                return user;
            }
    
        private:
            Database::ConnectionFactory *dao_con_fact;
    };
}