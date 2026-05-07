#include "../includes/CommandHandler.hpp"
#include<cctype>


// void CommandHandler::HandledJOIN(const Commandeparse &cmd)
// {

// }

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


void CommandHandler::HandledPRIVMSG(const Commandeparse &cmd)
{
    if(cmd.args.size() < 2)
    {
        std::cerr << "PRIVMSG error : missing argd \n";
        return ;
    }
    std::string target = cmd.args[0];
    std::string message = cmd.args[1];

    // ila kanet chanel 
    if(target[0] == '#')
    {
        
    }
    else
    {
        std::cout << "[PRIVATE MSG] " << target
                  << " : " << message << std::endl;
    }
    
}