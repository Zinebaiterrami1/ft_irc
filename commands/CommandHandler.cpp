#include "../includes/CommandHandler.hpp"
#include<cctype>
#include "../includes/config.hpp"
#include "../includes/Server.hpp"
#include "../includes/Commandeparse.hpp"


void Client::HandledJOIN(const Commandeparse &cmd)
{
    try{

        if(!Authenticated)
            throw;
        if(cmd.args.empty()){
            creat_raply(":" + ser->get_hostname + " 461 " + "JOIN :Not enough parameters" + " :\r\n");
            throw;
        }
        if(!valideArgs(cmd.args))//if no # //existe 
        {
            throw;
        }
        if(cmd.args.size() == 1 )
        {
            if(cmd.args[0] != "0")
                joinChannel(cmd.args[0], username, ser);//if first client creat channel
            else 
                leaveAll(cmd.args[0], username, ser);
        }
        else 
        {//MODE #secret +k hello42   //key == MODE #secret +k hello42
            std::string *channels = split_Channels(cmd.args);
            std::string *keys = split_Keys(cmd.args);
            joinMultiChannl(channels, keys, username, ser);
        }
    }
    catch(...){
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

