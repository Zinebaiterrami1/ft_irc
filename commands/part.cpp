#include<cctype>
#include "../includes/config.hpp"
#include "../includes/Commandeparse.hpp"
#include "../includes/Client.hpp"
#include "../includes/Server.hpp"



void Client::HandledPART(const Commandeparse &cmd)
{
    std::string servername = ser->get_hostname();
    if(cmd.args.empty())
    {
        ser->sendData(getFd(), ":" + servername + " 461 " + nickname + " PART :Not enough parameters\r\n");
        return; 
    }

    std::vector<std::string> channels = split_Channels(cmd.args[0]);
    std::string reason;
    if(cmd.args.size() > 1)
        reason = cmd.args[1];
    else 
        reason = nickname;
    
    for(size_t i = 0; i < channels.size(); i++)
    {
        Channel *ch = ser->get_channel(channels[i]);
        if(!ch)
        {
            ser->sendData(fd, ":" + ser->get_hostname() + " 403 " + nickname 
                    + " " + channels[i] + " :No such channel\r\n");
            continue;
        }
        if(!in_channel(ch))
        {
            ser->sendData(fd, ":" + ser->get_hostname() + " 442 " + nickname 
                + " " + ch->getName() + " :You're not on that channel\r\n");
            continue;
        }
        std::string part_msg = ":" + get_prefix() + " PART " 
                            + ch->getName() + " :" + reason + "\r\n";
        ser->sendData(fd, part_msg);
        ch->brodcast_Channel1(part_msg,this, ser);
        renoveChannel(ch);
        ch->removeUser(this);
        ch->removeOperator(this);
        
        if(ch->getUsers().size() == 0)
        {
            ser->delete_channel(ch->getName());
        }
    }
}