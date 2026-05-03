#include "CommandHandler.hpp"
void CommandHandler::execute(const Commandeparse &cmd)
{
    if(cmd.name == "JOIN")
        HandledJOIN(cmd);
    else if(cmd.name == "KICK")
        HandledKICK(cmd);
    else if(cmd.name == "INVITE")
         HandledINVITE(cmd);
    else if(cmd.name == "TOPIC")
        HandledTOPIC(cmd);
    else if(cmd.name == "MODE")
        HandledMODE(cmd);
    else if(cmd.name == "PRIVMSG")
        HandledPRIVMSG(cmd);

    else
        std::cerr << "Unknown Command " << std::endl;
}



void CommandHandler::HandledJOIN(const Commandeparse &cmd)
{

}

void CommandHandler::HandledKICK(const Commandeparse &cmd)
{
    
}

void CommandHandler::HandledINVITE(const Commandeparse &cmd)
{
    
}

void CommandHandler::HandledTOPIC(const Commandeparse &cmd)
{
    
}

void CommandHandler::HandledMODE(const Commandeparse &cmd)
{
    
}


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
        std::cout << "CHANNEL MSG " << target <<  " :" << message
        << std::endl;
    }
    else
    {
        std::cout << "[PRIVATE MSG] " << target
                  << " : " << message << std::endl;
    }
    
}