#include "../includes/CommandHandler.hpp"
#include<cctype>
#include "../includes/config.hpp"
#include "../includes/Server.hpp"
#include "../includes/Commandeparse.hpp"

void valideArgs(std::vector<std::string> args)
{
    if(args[0][0] == '#' && args[0][1] && isalnum(args[0][1])){
        for(size_t j = 0; args[0][j]; j++){
            if(args[0][j] == ','){
                if(args[0][j+1] && args[0][j+1] == '#'){
                    if(args[0][j+2] && isalnum(args[0][j+2]))
                        continue;
                    else throw "invalide channel name (empty or not alphaNum)";
                }         
                else throw "coma (,) folowed by invalide character";
            }
        }
    }
    else throw "Channels need '#' at start";
}

void leaveAll(std::vector<Channel*> channels,Client *client){//remove user from serv.channels
    for(size_t i = 0; i < channels.size(); i++){
        if(channels[i]->hasUser(client))
            channels[i]->removeUser(client);
    }
}

std::vector<std::string> split_Channels(std::string channels){//#ch1,#ch2,
    std::vector<std::string> Splited_channels;
    for(size_t i = 0; i < channels.size(); i++){
        if(channels[i] == '#'){
            i++;
            size_t next = channels.find('#', i);
            std::string name = channels.substr(i, next-1);
            Splited_channels.push_back(name);
        }
    }
    return Splited_channels;
}

std::vector<std::string> split_Keys(std::vector<std::string> keys){//key1 key2
    std::vector<std::string> Splited_keys;
    for(size_t i = 1; i < keys.size(); i++){
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
    // void(keys);
    for(size_t i = 0; i < channels.size(); i++){
        if(!ser->get_channel(channels[i])){
            Channel *newChannel;
            newChannel = ser->create_channel(channels[i]);
            newChannel->addOperator(client);
            newChannel->addUser(client);
        }
        else
            ser->get_channel(channels[i])->addUser(client);
    }
}

void Client::HandledJOIN(const Commandeparse &cmd)
{
    try{

        if(!Authenticated)
            throw;
        if(cmd.args.empty()){
            // creat_reply(":" + ser->get_hostname() + " 461 " + "JOIN :Not enough parameters" + " :\r\n");
            throw "emty arguments";
        }

        valideArgs(cmd.args);//if no # //existe 

        if(cmd.args.size() == 1 && cmd.args[0] == "0")
        {
            // if(cmd.args[0] != "0")
            //     joinChannel(cmd.args[0], username, ser);//if first client creat channel
            // else 
            leaveAll(ser->get_all_channels(), this);
        }
        else 
        {//MODE #secret +k hello42   //key == MODE #secret +k hello42
            std::vector<std::string> channels = split_Channels(cmd.args[0]);
            std::vector<std::string> keys = split_Keys(cmd.args);
            join_Multi_Channls(channels, keys, this,  ser);//if first client creat channel
        }
    }
    catch(std::string error){
        return;
    }
}

// void CommandHandler::HandledKICK(const Commandeparse &cmd)
// void Client::HandledJOIN(const Commandeparse &cmd)
// {
//     try{

//         if(!Authenticated)
//             throw;
//         if(cmd.args.empty()){
//             creat_raply(":" + ser->get_hostname + " 461 " + "JOIN :Not enough parameters" + " :\r\n");
//             throw;
//         }
//         if(!valideArgs(cmd.args))//if no # //existe 
//         {
//             throw;
//         }
//         if(cmd.args.size() == 1 )
//         {
//             if(cmd.args[0] != "0")
//                 joinChannel(cmd.args[0], username, ser);//if first client creat channel
//             else 
//                 leaveAll(cmd.args[0], username, ser);
//         }
//         else 
//         {//MODE #secret +k hello42   //key == MODE #secret +k hello42
//             std::string *channels = split_Channels(cmd.args);
//             std::string *keys = split_Keys(cmd.args);
//             joinMultiChannl(channels, keys, username, ser);
//         }
//     }
//     catch(...){
//         return;
//     }
// }

// // void CommandHandler::HandledKICK(const Commandeparse &cmd)
// // {
    
// // }

// // void CommandHandler::HandledINVITE(const Commandeparse &cmd)
// // {
    
// // }

// // void CommandHandler::HandledTOPIC(const Commandeparse &cmd)
// // {
    
// // }

// // void CommandHandler::HandledMODE(const Commandeparse &cmd)
// // {
    
// // }

// // void CommandHandler::HandledPASS(const Commandeparse &cmd)
// // {
// //     // if(client.registerd)
// //     // {
// //     //     std::cout
// //     // }er(*NULL)
// //     config con;
// //     if(cmd.args.empty())
// //     {
// //         std::cout << "PASS: no password given\n";
// //         return ;
// //     }
// //     if(cmd.args.size() > 1)
// //     {
// //         std::cout << "error number args is > 1" << std::endl;
// //         return ;
// //     }
// //     std::string pass = cmd.args[0];
// //     if(pass != con.password)
// //     {
// //         std::cout << "password incorrect " << std::endl;
// //     }
// //     else
// //     {
// //         std::cout << "password correct" << std::endl;
// //     }
// // }
// // void CommandHandler::HandledPART(const Commandeparse &cmd)
// // {

    
// // }

// // void CommandHandler::HandledUSER(const Commandeparse &cmd)
// // {
// //     //check wach deja m regestered b password 
// //     // check wach authentificate 


// // }

