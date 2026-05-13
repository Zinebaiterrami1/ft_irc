#include<cctype>
#include "../includes/config.hpp"
#include "../includes/Server.hpp"
#include "../includes/Commandeparse.hpp"

static void valideArgs(std::vector<std::string> args)
{
    std::cout << "-------> " << args[0] << "\n";
    if(args[0][0] == '#' && args[0][1] && isalnum(args[0][1])){
        for(size_t j = 1; args[0][j]; j++){
            if(args[0][j] == '#'){
                if(args[0][j+1] && isalnum(args[0][j+1]) && args[0][j-1] == ','){
                    // if(args[0][j+2] && isalnum(args[0][j+2]))
                    continue;
                    // else throw "invalide channel name (empty or not alphaNum)";
                }         
                else throw "INVALIDE CHANNELS NAME";
            }
        }
    }
    else throw "Channels need '#' at start";
}

void leaveAll(std::vector<Channel*> ch,Client *client)
{//remove user from serv.ch
    for(size_t i = 0; i < ch.size(); i++)
    {
        if(ch[i]->hasUser(client))
        {
            ch[i]->removeUser(client);
            ch[i]->removeOperator(client);
        }
    }
}

std::vector<std::string> split_Channels(std::string chnl){//#ch1,#ch2,
    std::vector<std::string> Splited_chnl;
    std::string str;
    std::stringstream ss(chnl);
    std::getline(ss, str, ',');
    Splited_chnl.push_back(str);
    while (std::getline(ss, str, ','))
    {
        Splited_chnl.push_back(str);
    }
    
    // for(size_t i = 0; i < chnl.size(); i++)
    // {
    //     if(chnl[i] == '#')
    //     {
    //         i++;
    //         size_t next = chnl.find('#', i);
    //         if(next == std::string::npos)
    //             next = chnl.length() + 1;
    //         std::string name = chnl.substr(i, next-1 - i);// -1 to not take ','                 #abc,#ch
    //         Splited_chnl.push_back(name);
    //     }
    // }
    return Splited_chnl;
}


std::vector<std::string> split_Keys(std::vector<std::string> keys){//key1 key2
    std::vector<std::string> Splited_keys;
    for(size_t i = 1; i < keys.size(); i++)
    {
        Splited_keys.push_back(keys[i]);
    }
    return Splited_keys;
}

// bool channelNotFound(std::vector<Channel*> channels, std::string name){
//     for(size_t i = 0; i < channels.size(); i++){
//         if(channels[i]->getName() == name)
//             return false;
//     }
//     return true;
// }

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
        // if(!chl)
        // {
        //     chl = ser->create_channel(channels[i]);
        //     chl->addOperator(client);
        // }
        chl->addUser(client);
        client->c_channels.insert(chl);

        std::string msg = ":" + client->get_prefix() + " JOIN :" + channel_name + "\r\n";
        // ser->sendData(client->getFd(), msg);
        chl->brodcast_Channel(msg, ser);

        std::cout << RED << "CLIENT " << client->get_nickname() << "JOINED  TO CHANNEL " << channel_name <<"\n";

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
    //CHECK IF INVITE ONLY 
    try{

        if(!Authenticated){
            return;
        }
        if(cmd.args.empty()){
            ser->sendData(getFd(), ":" + ser->get_hostname() + " 461 " + "JOIN :Not enough parameters" + " :\r\n");
            return;
        }

        valideArgs(cmd.args);//if no # //existe 

        if(cmd.args.size() == 1 && cmd.args[0] == "0")
        {
            leaveAll(ser->get_all_channels(), this);
        }

        else 
        {//MODE #secret +k hello42   //key == MODE #secret +k hello42
            std::vector<std::string> channels = split_Channels(cmd.args[0]);
            for(size_t i = 0; i < channels.size(); i++){
                std::cout << RED << channels[i] << RESET << "  ";
            }
            std::cout << "\n";
            std::vector<std::string> keys = split_Keys(cmd.args);
            for(size_t i = 0; i < keys.size(); i++){
                std::cout << GRE << keys[i] << RESET << "  ";
            }
            std::cout << "\n";
            join_Multi_Channls(channels, keys, this,  ser);//if first client creat channel
        }
    }
    catch(const char *error){
        std::cerr << error << "\n";
        return;
    }
}
