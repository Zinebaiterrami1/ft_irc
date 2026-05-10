#include "../includes/CommandHandler.hpp"
#include<cctype>
#include "../includes/config.hpp"
#include "../includes/Commandeparse.hpp"
#include "../includes/Client.hpp"
#include "../includes/Server.hpp"


// std::string getChannelName(std::vector<std::string> args){
//     return args[1].substr(1, args[1].length());
// }

void Client::HandledMODE(const Commandeparse &cmd)
{
    try
    {
        std::string servername = ser->get_hostname();

        if (!Authenticated)
            return;

        if (cmd.args.empty())
        {
            ser->sendData(getFd(),":" + servername + " 461 " + nickname + " MODE :Not enough parameters\r\n");
            return;
        }

        std::string ch_name = cmd.args[0];

        Channel *chl = ser->get_channel(ch_name);

        if(!chl)
        {
            ser->sendData(getFd(), ":" + servername + " 403 " + nickname + " " + ch_name + " :No such channel\r\n");
            return;
        }

        if(cmd.args.size() == 1)
        {
            ser->sendData(getFd(),":" + servername + " 324 " + nickname + " " + ch_name + " " + chl->get_mode() + "\r\n");
            return;
        }
        if(!in_channel(chl))
        {
            ser->sendData( getFd() ,":" + servername + " 442 " + nickname + " " + ch_name + " :You're not on that channel\r\n");
            return;          
        }
        if(!chl->isOperator(this))
        {
            ser->sendData(getFd(), ":" + servername + " 482 " + nickname + " " + ch_name + " :You're not channel operator\r\n");
            return;
        }



        const std::string& modes = cmd.args[1];
        std::vector<std::string> param;

        for(size_t i = 2; i < cmd.args.size(); i++)
        {
            param.push_back(cmd.args[i]);
        }

        char sign = 0; // stocker la signe + ou - ou 0  kay3eni makaynch signe
        // db exmple hna 3edna mode #ch +k scret 10 db exmpple scret -> idx =0 et 10 -<> idex 1
        size_t idx = 0;
        std::string applied_modes; // achmn mode appliquer
        std::string applied_params; // argumenent des modes
    
        for(size_t i = 0 ; i< modes.size() ; i++)
        {
            char m = mode[i];
            if(m == '+' || m =='-')
            {
                sign = m;
                continue;
            }
            if(!sign)
                continue;

            if(m == 'i')
                chl->setInviteOnly(sign == '+');
            else if( m == 't')
                chl->setTopicRestricted(sign == '+')
            else if(m == 'k' && idx < cmd.args.size())
            {
                if(sign == '+')
                    chl->setKey(cmd.args[idx]);
                else
                    chl->setKey("");
                applied_params += " " + cmd.args[idex++];
            }

            else if( m == 'o' && idx < cmd.args.size())
            {
                Client *cl = ser->find_nicknameclient(args[idx]);
                if(cl && cl->in_channel(chl))
                {
                    if(sign == '+')
                        chl->addOperator(cl);
                    else
                        chl->removeOperator(cl);

                    applied_params += " " + cmd.args[idx];
                }
                idx++;
            }
            else
            {
                ser->sendData(getFd() ,":" + servername + " 472 " + nickname + " " + m + " :Unknown mode\r\n");
                continue;
            }
            applied_modes += sign;
            applied_modes += m;
        }
        if(!applied_modes.empty())
            chl->brodcast_Channel
    }
    catch(const char *ERROR)
    {
        std::cerr << ERROR << std::endl;
    }
}