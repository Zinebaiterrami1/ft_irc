#include "../includes/CommandHandler.hpp"
#include<cctype>
#include "../includes/config.hpp"
#include "../includes/Commandeparse.hpp"
#include "../includes/Client.hpp"
#include "../includes/Server.hpp"
void Client::HandledTOPIC(const Commandeparse &cmd)
{
    std::string servername = ser->get_hostname();

    if(!Authenticated)
        return ;
    if(cmd.args.empty())
    {
        ser->sendData(getFd(),":" + servername + " 461 " + nickname + " TOPIC :Not enough parameters\r\n");
        return ;
    }
    const std::string &name_Channel = cmd.args[0];
    Channel *chan = ser->get_channel(name_Channel);
    if(!chan)
    {
       ser->sendData(getFd() , ":" + servername + " 403 " + nickname + " " + name_Channel + " :No such channel\r\n");
        return ;
    }

    if(!in_channel(chan))
    {
        ser->sendData(getFd() , ":" + servername + " 442 " + nickname + " " + name_Channel + " :You're not on that channel\r\n");
        return ;
    }
    if(cmd.args.size() == 1 )
    {
        if(chan->getTopic().empty())
            ser->sendData(getFd() , ":" + servername + " 331 " + nickname + " " + name_Channel + " :No topic is set\r\n");
        else
            ser->sendData(getFd() , ":" + servername + " 332 " + nickname + " " + name_Channel + " :" + chan->getTopic() + "\r\n");
    
        return ;
    }

    const std::string &topic = cmd.args[1];
    //hna mode +t 
    if(chan->istopicRestricted() && !chan->isOperator(this))
    {
        ser->sendData(getFd() , ":" + servername + " 482 " + nickname + " " + name_Channel + " :You're not channel operator\r\n");
        return;
    }

    chan->setTopic(topic);
    std::string prefix_tpoic =  ":" + get_prefix() + " TOPIC " + name_Channel +  " :" + topic + "\r\n";
    // ser->sendData(getFd() , prefix_tpoic);

    chan->brodcast_Channel(prefix_tpoic , ser);
}
