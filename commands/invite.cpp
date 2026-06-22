#include<cctype>
#include "../includes/config.hpp"
#include "../includes/Commandeparse.hpp"
#include "../includes/Client.hpp"
#include "../includes/Server.hpp"

void Client::HandledINVITE(const Commandeparse &cmd)
{
    std::string servername = ser->get_hostname();

    if(!Authenticated)
        return ;
    if(cmd.args.size() < 2)
    {
        ser->sendData(getFd(),":" + servername + " 461 " + nickname + " INVITE :Not enough parameters\r\n");
        return ;
    }
    const std::string& nick = cmd.args[0];
    const std::string& name_channel = cmd.args[1];

    Channel *ch = ser->get_channel(name_channel);
    Client *cl_target = ser->find_nicknameclient(nick);
    if(!cl_target)
    {
        ser->sendData(getFd(),":" + servername + " 401 " + nickname + " " + nick + " :No such nick/channel\r\n");
        return ;
    }
    if(!ch)
    {
        ser->sendData(getFd(),":" + servername + " 403 " + nickname + " " + name_channel + " :No such channel\r\n");
        return ;
    }
    if(cl_target->in_channel(ch)) 
    {
        ser->sendData(getFd(),":" + servername + " 443 " + nickname + " " + nick+ " " + name_channel + " :is already on channel\r\n");
        return ;
    }
    if(ch->isInviteOnly() && !ch->isOperator(this))
    {
        ser->sendData(getFd(), ":" + servername + " 482 " + nickname + " " + name_channel + " :You're not channel operator\r\n");
        return ;
    }
    ch->add_invite(nick);
    std::string prefix_invite = ":" + get_prefix() + " INVITE " + nick + " " + name_channel + "\r\n";
    ser->sendData(cl_target->getFd(), prefix_invite);
    ser->sendData(getFd(), ":" + servername + " 341 " + nickname + " " + nick + " " + name_channel + "\r\n");
}