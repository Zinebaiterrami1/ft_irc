#include<iostream>
#include <cstdlib>
#include"config.hpp"

bool is_valid_port(const std::string &str, int &port )
{
    if(str.empty())
        return false;
    long val = 0;
    for(size_t i = 0; i < str.size(); i++)
    {
        if(!std::isdigit(str[i]))
            return false;
        val = val * 10 + (str[i] - '0');
        if(val > 65535)
            return false;
    }
    if(val < 1)
        return false;

    port = static_cast<int>(val);
    return true;
}


int main(int ac, char **av)
{
    if (ac != 3)
    {
        std::cerr << "./ircserv <port> <password>" << std::endl;
        return 1;
    }

    config con;

    if (!is_valid_port(av[1], con.port))
    {
        std::cerr << "Invalid port" << std::endl;
        return 1;
    }

    con.password = av[2];

    if (con.password.empty())
    {
        std::cerr << "password empty" << std::endl;
        return 1;
    }
    
    std::cout << "Port: " << con.port << std::endl;
    std::cout << "Password: " << con.password << std::endl;
}
