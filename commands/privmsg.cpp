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
        send(":" + servername + " 411 " + nickname + " :No recipient given (PRIVMSG)\r\n");
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
    
    if(target[0] == '#' || target[0] == '&' )
    {
        Channel* chan = ser->get_channel(target);
        if(!chan)
        {
            create_reply(":" + servername + " 403 " + nickname + " " + target + " :No such channel\r\n");
            return ;
        }
        if (!in_channel(chan))
        {
            create_reply(":" + servername + " 404 " + nickname + " " + target + " :Cannot send to channel\r\n");
            return;
        }
        chan->sed_msg_client(msg_send , this);
        const std::set<Client *> &all_users = chan->get_users();

        for (std::set<Client *>::const_iterator it = all_users.begin();
            it != all_users.end(); ++it)
        {
            if (*it != this)
            {
                (*it)->create_reply(target);
                ser->trigger_write((*it)->getFd());
            }
        }
    }

    
}