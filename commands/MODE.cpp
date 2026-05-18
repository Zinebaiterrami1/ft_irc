#include <cstdlib>
#include "../includes/config.hpp"
#include "../includes/Commandeparse.hpp"
#include "../includes/Client.hpp"
#include "../includes/Server.hpp"
#include "../includes/Channel.hpp"

void Client::HandledMODE(const Commandeparse &cmd)
{

        std::string servername = ser->get_hostname();

        if (!Authenticated)
            return;

        if (cmd.args.empty())
        {
            ser->sendData(getFd(),
                ":" + servername + " 461 " + nickname + " MODE :Not enough parameters\r\n");
            return;
        }

        std::string ch_name = cmd.args[0];
        Channel *chl = ser->get_channel(ch_name);

        if (!chl)
        {
            ser->sendData(getFd(),
                ":" + servername + " 403 " + nickname + " " + ch_name + " :No such channel\r\n");
            return;
        }

        if (cmd.args.size() == 1)
        {
            ser->sendData(getFd(),
                ":" + servername + " 324 " + nickname + " " + ch_name + " " + chl->get_mode() + "\r\n");
            return;
        }

        if (!in_channel(chl))
        {
            ser->sendData(getFd(),
                ":" + servername + " 442 " + nickname + " " + ch_name + " :You're not on that channel\r\n");
            return;
        }

        if (!chl->isOperator(this))
        {
            ser->sendData(getFd(),
                ":" + servername + " 482 " + nickname + " " + ch_name + " :You're not channel operator\r\n");
            return;
        }

        const std::string &modes = cmd.args[1];
        std::vector<std::string> param(cmd.args.begin() + 2, cmd.args.end());

        char sign = 0;
        size_t idx = 0;

        std::string applied_modes;
        std::string applied_params;

        for (size_t i = 0; i < modes.size(); i++)
        {
            char m = modes[i];

            if (m == '+' || m == '-')
            {
                sign = m;
                continue;
            }

            if (!sign)
                continue;

            if (m == 'i')
                chl->setInviteOnly(sign == '+');
            else if (m == 't')
                chl->setTopicRestricted(sign == '+');
            else if (m == 'k')
            {
                if (sign == '+' && idx < param.size())
                    chl->setKey(param[idx++]);
                else if (sign == '-')
                    chl->setKey("");
            }
            else if (m == 'l')
            {
                if (sign == '+' && idx < param.size())
                {

                    chl->setUserLimit(std::atoi(param[idx].c_str()));
                    applied_params += " " + param[idx]; 
                    idx++;
                }
                else if (sign == '-')
                {
                    chl->removeUserLimit();
                }
            }
            else if (m == 'o')
            {
                if (idx < param.size())
                {
                    Client *cl = ser->find_nicknameclient(param[idx]);
                    if (cl && cl->in_channel(chl))
                    {
                        if (sign == '+')
                            chl->addOperator(cl);
                        else
                            chl->removeOperator(cl);
                        applied_params += " " + param[idx];
                    }
                    idx++;
                }
            }
            else
            {
                ser->sendData(getFd(),
                    ":" + servername + " 472 " + nickname + " " + m + " :Unknown mode\r\n");
                continue;
            }

            applied_modes += sign;
            applied_modes += m;
        }

        if (!applied_modes.empty())
        {
            std::string msg = ":" + get_prefix() + " MODE " + ch_name + " " +
                              applied_modes + applied_params + "\r\n";
            chl->brodcast_Channel1(msg,this, ser);
        }
}
