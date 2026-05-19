#include<cctype>
#include "../includes/config.hpp"
#include "../includes/Server.hpp"
#include "../includes/Commandeparse.hpp"

static void valideArgs(std::vector<std::string> args)
{
    if(args[0][0] == '#' && args[0][1] && isalnum(args[0][1])){
        for(size_t j = 1; args[0][j]; j++){
            if(args[0][j] == '#'){
                if(args[0][j+1] && isalnum(args[0][j+1]) && args[0][j-1] == ','){
                    continue;
                }         
                else throw "INVALIDE CHANNELS NAME";
            }
        }
    }
    else throw "Channels need '#' at start";
}

// void leaveAll(std::vector<Channel*> ch,Client *client)
// {
//     for(size_t i = 0; i < ch.size(); i++)
//     {
//         if(ch[i]->hasUser(client))
//         {
//             ch[i]->removeUser(client);
//             ch[i]->removeOperator(client);
//         }
//     }
// }

std::vector<std::string> split_Channels(std::string chnl){
    std::vector<std::string> Splited_chnl;
    std::string str;
    std::stringstream ss(chnl);
    std::getline(ss, str, ',');
    Splited_chnl.push_back(str);
    while (std::getline(ss, str, ','))
    {
        Splited_chnl.push_back(str);
    }
    
    return Splited_chnl;
}

std::vector<std::string> split_Keys(std::vector<std::string> keys){
    std::vector<std::string> Splited_keys;
    for(size_t i = 1; i < keys.size(); i++)
    {
        Splited_keys.push_back(keys[i]);
    }
    return Splited_keys;
}

void join_Multi_Channls(std::vector<std::string> channels, std::vector<std::string> keys, Client *client, Server *ser)
{
    for(size_t i = 0; i < channels.size(); i++)
    {
        Channel *chl = ser->get_channel(channels[i]);
        if(!chl)
        {
            chl = ser->create_channel(channels[i]);
            chl->addOperator(client);
        }
        std::string channel_name = chl->getName();
        if(channel_name.empty() || channel_name[0] != '#')
        {
            ser->sendData(client->getFd(), ":" + ser->get_hostname() + " 476 " 
            + client->get_nickname() + " " + channel_name + " :Bad Channel Mask\r\n");
            continue;
        }
        if(chl->isInviteOnly() && !chl->isInvited(client))
        {
            ser->sendData(client->getFd(), ":" + ser->get_hostname() + " 473 " + client->get_nickname() 
            + " " + channel_name + " :Cannot join channel (+i)\r\n");
            continue;
        }
        if(chl->hasLimit() && chl->getLimit() <= chl->getUsers().size())
        {
            ser->sendData(client->getFd(), ":" + ser->get_hostname() + " 471 " + client->get_nickname() 
            + " " + channel_name + "  :Cannot join channel (+l)\r\n");
            continue;
        }
        if((chl->hasKey() && i >= keys.size() )|| (chl->hasKey() && keys[i] != chl->getKey()))
        {
            ser->sendData(client->getFd(), ":" + ser->get_hostname() + " 475 " + client->get_nickname() 
            + " " + channel_name + " :Cannot join channel (+k)\r\n");
            continue;
        }
        if(client->in_channel(chl))
        {
            continue;
        }
        if(chl->isInvited(client))
        {
            chl->removeInvite(client);
        }

        chl->addUser(client);
        client->c_channels.insert(chl);

        std::string msg = ":" + client->get_prefix() + " JOIN :" + channel_name + "\r\n";
        chl->brodcast_Channel(msg, ser);


        if(!chl->getTopic().empty())
            ser->sendData(client->getFd(), ":" + ser->get_hostname() + " 332 " + client->get_nickname() + " " + channel_name + " :" + chl->getTopic() + "\r\n");
        else
            ser->sendData(client->getFd(), ":" + ser->get_hostname() + " 331 " + client->get_nickname() + " " + channel_name + " :No topic is set\r\n");

        std::string list_users = ":" + ser->get_hostname() + " 353 " + client->get_nickname() + " = " + channel_name + " :";
        std::vector<Client *> all_users = chl->getUsers();

        for(size_t i = 0; i < all_users.size(); i++)
        {
            Client *user = all_users[i];
            if(chl->isOperator(user))
                list_users += "@";

            list_users += user->get_nickname();

            if (i != all_users.size() - 1)
                list_users += " ";
        }

        list_users += "\r\n";
        ser->sendData(client->getFd(), list_users);
        ser->sendData(client->getFd(), ":" + ser->get_hostname() + " 366 " + client->get_nickname() 
                        + " " + channel_name + " :End of /NAMES list.\r\n");
    }
}

void Client::HandledJOIN(const Commandeparse &cmd)
{
    try{

        if(!Authenticated){
            return;
        }
        if(cmd.args.empty()){
            ser->sendData(getFd(), ":" + ser->get_hostname() + " 461 " + "JOIN :Not enough parameters" + " :\r\n");
            return;
        }
        valideArgs(cmd.args);
        std::vector<std::string> channels = split_Channels(cmd.args[0]);
        std::vector<std::string> keys = split_Keys(cmd.args);
        join_Multi_Channls(channels, keys, this,  ser);
    }
    catch(const char *error){
        std::cerr << error << "\n";
        return;
    }
}
