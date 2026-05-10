#include "../includes/CommandHandler.hpp"
#include<cctype>
#include "../includes/config.hpp"
#include "../includes/Server.hpp"
#include "../includes/Commandeparse.hpp"

// static void valideArgs(std::vector<std::string> args)
// {
//     std::cout << "-------> " << args[1] << "\n";
//     if(args.size() > 1 && args[1][0] == '#' && args[1][1] && isalnum(args[1][1])){
//         for(size_t j = 0; args[1][j]; j++){
//             if(args[1][j] == ','){
//                 if(args[1][j+1] && args[1][j+1] == '#'){
//                     if(args[1][j+2] && isalnum(args[1][j+2]))
//                         continue;
//                     else throw "invalide channel name (empty or not alphaNum)";
//                 }         
//                 else throw "coma (,) folowed by invalide character";
//             }
//         }
//     }
//     else throw "Channels need '#' at start";
// }


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
            ch[i]->removeUser(client);
    }
}

std::vector<std::string> split_Channels(std::string chnl){//#ch1,#ch2,
    std::vector<std::string> Splited_chnl;
    // char *str;
    // std::stringstream ss(chnl);
    // std::getline(ss, str, ',');
    // Splited_chnl.push_back(str + 1);
    // while (std::getline(ss, str, ','))
    // {
    //     Splited_chnl.push_back(str+1);
    // }
    
    for(size_t i = 0; i < chnl.size(); i++)
    {
        if(chnl[i] == '#')
        {
            i++;
            size_t next = chnl.find('#', i);
            if(next == std::string::npos)
                next = chnl.length() + 1;
            std::string name = chnl.substr(i, next-1 - i);// -1 to not take ','                 #abc,#ch
            Splited_chnl.push_back(name);
        }
    }
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

void join_Multi_Channls(std::vector<std::string> channels, std::vector<std::string> keys, Client *client, Server *ser){
    for(size_t i = 0; i < channels.size(); i++){
        if(!ser->get_channel(channels[i])){
            Channel *newChnl;
            newChnl = ser->create_channel(channels[i]);
            newChnl->addOperator(client);
            newChnl->addUser(client);
            std::cout << MAG << client->get_nickname() <<" " << channels[i] << " CHANNEL CREATED " << RESET << "\n";
        }
        else{
            Channel *chl = ser->get_channel(channels[i]);
            if(chl->hasLimit() && chl->getLimit() <= chl->getUsers().size())
            {
                ser->sendData(client->getFd(), ":" + ser->get_hostname() + " 475 " + client->get_nickname() 
                + " can not JOIN " + chl->getName() + " Limits reached\r\n");
                continue;
            }
            if((chl->hasKey() && i >= keys.size() )|| (chl->hasKey() && keys[i] != chl->getKey()))
            {
                ser->sendData(client->getFd(), ":" + ser->get_hostname() + " 471 " + client->get_nickname() 
                + " can not JOIN " + chl->getName() + " Incorrect KEY\r\n");
                continue;
            }
            if(client->in_channel(chl))
            {
                ser->sendData(client->getFd(), ":" + ser->get_hostname() + client->get_nickname() 
                + " is already in channel : " + chl->getName() + "\r\n");
                continue;
            }
            else
            {
                chl->addUser(client);
                chl->brodcast_Channel(client->get_nickname() + " Joined the channel\n", client, ser);
                std::cout << RED << "CLIENT " << client->get_nickname() << " ADDED\n";
            }
        }
            // ser->get_channel(channels[i])->addUser(client);
    }
}

void Client::HandledJOIN(const Commandeparse &cmd)
{
    //CHECK IF INVITE ONLY 
    try{

        if(!Authenticated){
            throw;
        }
        if(cmd.args.empty()){
            ser->sendData(getFd(), ":" + ser->get_hostname() + " 461 " + "JOIN :Not enough parameters" + " :\r\n");
            throw "emty arguments";
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