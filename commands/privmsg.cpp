#include "../includes/CommandHandler.hpp"
#include<cctype>
#include "../includes/config.hpp"
#include "../includes/Commandeparse.hpp"
#include "../includes/Client.hpp"
#include "../includes/Server.hpp"

void Client::HandledPRIVMSG(const Commandeparse &cmd)
{
    std::string servername = ser->get_hostname();

    if (!Authenticated)
        return;

    if (cmd.args.empty())
    {
        ser->sendData(getFd(),
        ":" + servername + " 411 " + nickname + " :No recipient given (PRIVMSG)\r\n");
        return;
    }

    if (cmd.args.size() < 2)
    {
        ser->sendData(getFd(),
        ":" + servername + " 412 " + nickname + " :No text to send\r\n");
        return;
    }

    std::string target = cmd.args[0];
    std::string message = cmd.args[1];

    std::string msg_send =
        ":" + get_prefix() + " PRIVMSG " + target + " :" + message + "\r\n";

    if (target[0] == '#' || target[0] == '&')
    {
        Channel* chan = ser->get_channel(target);

        if (!chan)
        {
            ser->sendData(getFd(),
            ":" + servername + " 403 " + nickname + " " + target + " :No such channel\r\n");
            return;
        }

        if (!in_channel(chan))
        {
            ser->sendData(getFd(),
            ":" + servername + " 404 " + nickname + " " + target + " :Cannot send to channel\r\n");
            return;
        }
        
        chan->brodcast_Channel(msg_send, ser);
    }
    else
    {
        Client* target_client = ser->find_nicknameclient(target);

        if (!target_client)
        {
            ser->sendData(getFd(),
            ":" + servername + " 401 " + nickname + " " + target + " :No such nick/channel\r\n");
            return;
        }

        ser->sendData(target_client->getFd(), msg_send);
    }
}