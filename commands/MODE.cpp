#include "../includes/CommandHandler.hpp"
#include<cctype>
#include "../includes/config.hpp"
#include "../includes/Commandeparse.hpp"
#include "../includes/Client.hpp"
#include "../includes/Server.hpp"


std::string getChannelName(std::vector<std::string> args){
    return args[1].substr(1, args[1].length());
}

void Client::HandledMODE(const Commandeparse &cmd){
    try{

        // valideArgs(cmd);//size == 3,  ars 2 start #,  size arg3 == 2 , start by - + 
        std::string ch_name = getChannelName(cmd.args);
        Channel *chl = ser->get_channel(ch_name);
        
        if(!chl)
        {
            // sendData();
            throw "channel not found";
        }
        if(!chl->isOperator(this))
        {
            // sendData();
            throw "User is not an operator";
        }

        std::vector<std::string> modes = cmd.args;
        for (size_t i = 2; i < modes.size(); i++){
            if(modes[i] == "+i")
                chl->setInviteOnly(1);
            else if(modes[i] == "-i")
                chl->setInviteOnly(0);
            else if(modes[i] == "+t")
                chl->setTopicRestricted(1);
            else if(modes[i] == "-t")
                chl->setTopicRestricted(0);
            else if(modes[i] == "+k")
                chl->setKey(modes[3]);
            else if(modes[i] == "-k")
                chl->setKey("");
            else if(modes[i] == "+o")
                chl->addOperator(this);
            else if(modes[i] == "-o")
                chl->removeOperator(this);
        }

    }
    catch(const char *ERROR){
        return;
    }
}