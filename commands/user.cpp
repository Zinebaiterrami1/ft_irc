#include "../includes/CommandHandler.hpp"
#include<cctype>
#include "../includes/config.hpp"
#include "../includes/Commandeparse.hpp"
#include "../includes/Client.hpp"
#include "../includes/Server.hpp"

void Client::HandledUSER(const Commandeparse &cmd)
{
    std::string servername = ser->get_hostname();
    if(!c_password)
    {
        ser->sendData(getFd(),":" + servername + " 451 * :You have not registered\r\n");
        return;
    }

    if(Authenticated)
    {
        ser->sendData(getFd(), ":" + servername + " 462 " + nickname + " :You may not reregister\r\n");
        return ;
    }
        if(cmd.args.empty()) // NBDEL MSG
    {
        ser->sendData(getFd(),":" + servername + " 461 * USER :Not enough parameters\r\n");
        return ;
    }
    if(cmd.args[0].find('@') != std::string::npos)
    {
        ser->sendData(getFd(),":" + servername + " 461 " + nickname + " :Invalid username\r\n");
        return ;
    }

    if(cmd.args.size() < 4)
    {
        ser->sendData(getFd(),":" + servername + " 461 * USER :Not enough parameters\r\n");
        return ;
    }

    username = cmd.args[0];
    realname = cmd.args[3];
    
    c_user = true;

    check_register();
}