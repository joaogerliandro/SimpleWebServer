#pragma once

#include <common.h>

namespace Entities
{
    class User
    {
        public:
            uint64_t user_id;
            std::string login;
            std::string password;
            std::string email;
            uint8_t status;
            std::string last_login_attempt;
            uint8_t bad_password_count;
            std::string creation_date;
            std::string update_date;
    };
}