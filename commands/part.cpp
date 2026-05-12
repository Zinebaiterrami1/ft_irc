#include "../includes/CommandHandler.hpp"
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
}