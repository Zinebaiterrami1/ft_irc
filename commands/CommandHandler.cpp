#include "../includes/CommandHandler.hpp"
#include<cctype>
#include "../includes/config.hpp"
#include "../includes/Server.hpp"
#include "../includes/Commandeparse.hpp"

bool valideArgs(std::vector<std::string> args){
    if(args[0][0] == '#' && args[0][1] && isalnum(args[0][1])){
        for(size_t j = 0, args[0][j], j++){
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

void leavAll(std::string channel, Server &ser){//remove user from serv.channels
    
}

std::vector<std::string> split_Channels(std::string channels){//#ch1,#ch2,
    std::vector<std::string> Splited_channels;
    for(size_t i = 0; i < channels.size(); i++){
        if(channels[i] == '#'){
            i++;
            size_t next = channels.find('#', i);
            std::string name = substr(channels[i], next-1);
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

void join_Multi_Channls(std::vector::<std::string> channels, std::vector::<std::string> keys, std::string username, Server ser){
    for(size_t i = 0; i < channels.size(0); i++){
        
    }
}

void Client::HandledJOIN(const Commandeparse &cmd)
{
    try{

        if(!Authenticated)
            throw;
        if(cmd.args.empty()){
            creat_raply(":" + ser->get_hostname + " 461 " + "JOIN :Not enough parameters" + " :\r\n");
            throw;
        }

        valideArgs(cmd.args, ser)//if no # //existe 

        if(cmd.args.size() == 1 && cmd.args[0] == "0")
        {
            // if(cmd.args[0] != "0")
            //     joinChannel(cmd.args[0], username, ser);//if first client creat channel
            // else 
            leaveAll(cmd.args[0], ser);
        }
        else 
        {//MODE #secret +k hello42   //key == MODE #secret +k hello42
            std::vector::<std::string> channels = split_Channels(cmd.args[0]);
            std::vector::<std::string> keys = split_Keys(cmd.args);
            join_Multi_Channls(channels, keys, username,  ser);//if first client creat channel
        }
    }
    catch(std::string error){
        return;
    }
}

// void CommandHandler::HandledKICK(const Commandeparse &cmd)
// {
    
// }

// void CommandHandler::HandledINVITE(const Commandeparse &cmd)
// {
    
// }

// void CommandHandler::HandledTOPIC(const Commandeparse &cmd)
// {
    
// }

// void CommandHandler::HandledMODE(const Commandeparse &cmd)
// {
    
// }

// void CommandHandler::HandledPASS(const Commandeparse &cmd)
// {
//     // if(client.registerd)
//     // {
//     //     std::cout
//     // }er(*NULL)
//     config con;
//     if(cmd.args.empty())
//     {
//         std::cout << "PASS: no password given\n";
//         return ;
//     }
//     if(cmd.args.size() > 1)
//     {
//         std::cout << "error number args is > 1" << std::endl;
//         return ;
//     }
//     std::string pass = cmd.args[0];
//     if(pass != con.password)
//     {
//         std::cout << "password incorrect " << std::endl;
//     }
//     else
//     {
//         std::cout << "password correct" << std::endl;
//     }
// }
// void CommandHandler::HandledPART(const Commandeparse &cmd)
// {

    
// }

// void CommandHandler::HandledUSER(const Commandeparse &cmd)
// {
//     //check wach deja m regestered b password 
//     // check wach authentificate 


// }

// std::string valide_nick(const std::string &nick)
// {
//     std::string invalid = "#%&()+,*!?=<.>/;:@~";
//     for(size_t i = 0; i < nick.length(); i++)
//     {
//         if(invalid.find(nick[i]) !=std::string::npos)
//         {
//             return nick.substr(0 , i);
//         }
//     }
//     return nick;
// }
// bool is_special(unsigned char &c)
// {
//     if (c == '[' || c == ']' || c == '\\' || c == '`' ||
//         c == '_' || c == '^' || c == '{' || c == '|' || c == '}')
//     {
//         return true;
//     }
//     return false;   
// }
// void CommandHandler::HandledNICK(const Commandeparse &cmd)
// {
//     if(cmd.args.empty())
//     {
//         std::cout << "NICK: no NICK given\n";
//         return ;
//     }
//     if(cmd.args.size() > 1)
//     {
//         std::cout << "error number args is > 1" << std::endl;
//         return ;
//     }
//     std::string nick_name = cmd.args[0];
//     nick_name = valide_nick(nick_name);
//     if(nick_name.length() > 9)
//     {
//         std::cout << ":Erroneous nickname\n";
//         return ;
//     } 
//     unsigned char first = nick_name[0];
//     if(!std::isalpha(first) && is_special(first))
//     {
//         std::cout << ":Erroneous nickname\n";
//         return ;
//     }

//     // hna mzal khasni ndir test ta3 user wach deja kayn wla la mais me7etaja client
//     std::cout <<"nickname" << nick_name << std::endl;
// }

