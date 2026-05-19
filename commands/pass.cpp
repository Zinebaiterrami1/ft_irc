#include<cctype>
#include "../includes/config.hpp"
#include "../includes/Commandeparse.hpp"
#include "../includes/Client.hpp"
#include "../includes/Server.hpp"

void Client::HandledPASS(const Commandeparse &cmd)
{
    if (Authenticated)
    {
        ser->sendData(getFd(),
        ":" + ser->get_hostname() + " 462 * :You may not reregister\r\n");
        return;
    }
    if(c_password)
    {
        ser->sendData(getFd(),":" + ser->get_hostname() + " 462 " + nickname +" :You may not reregister\r\n");
        return ;
    }
    if(cmd.args.empty())
    {
        ser->sendData(getFd(), ":" + ser->get_hostname() + " 461 " + nickname + " PASS :Not enough parameters\r\n");
        return;
    }
    const std::string& password = cmd.args[0];
    const std::string& server_password = ser->get_config().password;
    if(password == server_password)
    {
        c_password = true;
        check_register();
    }
    else{
        ser->sendData(getFd(),":" + ser->get_hostname() + " 464 " + nickname + " :Password incorrect\r\n");
    }
}