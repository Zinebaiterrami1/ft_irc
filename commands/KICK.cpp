#include "../includes/CommandHandler.hpp"
#include<cctype>
#include "../includes/config.hpp"
#include "../includes/Commandeparse.hpp"
#include "../includes/Client.hpp"
#include "../includes/Server.hpp"

void Client::HandledKICK(const Commandeparse &cmd)
{
    std::string servername = ser->get_hostname();

    if(!Authenticated)
        return ;
    if(cmd.args.size() < 2)
    {
        ser->sendData(getFd(),":" + servername + " 461 " + nickname + " KICK :Not enough parameters\r\n");
        return ;
    }

    const std::string& target = cmd.args[0]; // hna semiya ta3 channel
    const std::string& nick = cmd.args[1];

    std::string reason;
    if(cmd.args.size() > 2)
        reason = cmd.args[2];
    else 
        reason =  "Kicked by operator" ;

    if (target.empty() || (target[0] != '#' && target[0] != '&'))
    {
        ser->sendData(getFd(),":" + servername + " 476 " + nickname + " " + target + " :Bad Channel Mask\r\n");
        return ;
    }
    Channel *chan = ser->get_channel(target);

    if(!chan)
    {
       ser->sendData(getFd() , ":" + servername + " 403 " + nickname + " " + target + " :No such channel\r\n");
        return ;
    }

    if(!in_channel(chan))
    {
        ser->sendData(getFd() , ":" + servername + " 442 " + nickname + " " + target + " :You're not on that channel\r\n");
        return ;
    }
    if(!chan->isOperator(this))
    {
        ser->sendData(getFd() , ":" + servername + " 482 " + nickname + " " + target + " :You're not channel operator\r\n");
        return;
    }

    Client *cl_nick = ser->find_nicknameclient(nick);
    if(!cl_nick)
    {
        ser->sendData(getFd(),":" + servername + " 401 " + nickname + " " + target + " :No such nick/channel\r\n");
        return;
    }

    if(!cl_nick->in_channel(chan))
    {
        ser->sendData(getFd() , ":" + servername + " 441 " + nickname + " " + target + " " + target + " :They are not on that channel\r\n");
        return;
    }

    std::string msg_predix = ":" + get_prefix() + " KICK " + target + " " + nick + " :" + reason + "\r\n";
    
    chan->sendMsgClient(msg_predix, this, ser);

    chan->removeUser(cl_nick);
    cl_nick->renoveChannel(chan);

    if(chan->getUsers().empty())
        ser->delete_channel(target);
}    

