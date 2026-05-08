#include "../includes/CommandHandler.hpp"
#include<cctype>
#include "../includes/config.hpp"
#include "../includes/Commandeparse.hpp"
#include "../includes/Client.hpp"
#include "../includes/Server.hpp"

void Client::HandledPRIVMSG(const Commandeparse &cmd)
{
    std::string servername = ser->get_hostname();
    if(!Authenticated)
        return ;

    if(cmd.args.empty())
    {
        create_reply(":" + servername + " 411 " + nickname + " :No recipient given (PRIVMSG)\r\n");
        return ;

    }
    if(cmd.args.size() < 2)
    {
        create_reply(":" + servername + " 412 " + nickname + " :No text to send\r\n");
        return ;
    }
    std::string target = cmd.args[0];
    std::string message = cmd.args[1];
    std::string msg_send = ":" + get_prefix() + " PRIVMSG " + target + " :" + message + "\r\n";
    // if(target[0] == '#' || target[0] == '&' )
    // {
    //     Channel* chan = ser->
    // }
    if(target[0] == '#')
    {
        return;
    }
    else
    {
        std::cout << "[PRIVATE MSG] " << target
                  << " : " << message << std::endl;
    }
    
}