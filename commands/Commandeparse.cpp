#include<iostream>
#include<string>
#include <vector>
#include "../includes/Commandeparse.hpp"
#include "../includes/CommandHandler.hpp"
// #include "Channel.hpp"
// #include "CommandHandler.hpp"

void execute(const Commandeparse &cmd)
{
    CommandHandler handler;
    std::string c;

    for (size_t i = 0; i <cmd.name.size() ; i++)
    {
        c += std::toupper(cmd.name[i]);
    }
    // if(cmd.name == "JOIN")
    //     handler.HandledJOIN(cmd);
    // else if(cmd.name == "PASS")
    //     handler.HandledPASS(cmd);
    // else if(cmd.name == "USER")
    //     handler.HandledUSER(cmd);
    // else if(cmd.name == "KICK")
    //     handler.HandledKICK(cmd);
    // else if(cmd.name == "INVITE")
    //      handler.HandledINVITE(cmd);
    // else if(cmd.name == "TOPIC")
    //     handler.HandledTOPIC(cmd);
    // else if(cmd.name == "MODE")
    //     handler.HandledMODE(cmd);
    if(cmd.name == "PRIVMSG")
        handler.HandledPRIVMSG(cmd);
    else
        std::cerr << "Unknown Command " << std::endl;
}
Commandeparse parser_commande(std::string &line)
{
    Commandeparse cmd;
    std::string word;
    bool after_colon = false;
    for(size_t i = 0; i < line.size(); i++)
    {
        char c = line[i];
        if (c == '\r' || c == '\n')
            continue;

        if(c == ' ' && !after_colon)
        {
            if(!word.empty())
            {
                if(cmd.name.empty())
                {
                    cmd.name = word;
                }
                else
                    cmd.args.push_back(word);
                word.clear();
            }
        }
        else if(c == ':')
        {
            if(!word.empty())
            {
                if(cmd.name.empty())
                    cmd.name = word;
                else
                    cmd.args.push_back(word);
               word.clear();

            }
                cmd.args.push_back(line.substr(i + 1));
                break;
        }
        else
        {
            word += c;
        }
    }

    if(!word.empty())
    {
        if(cmd.name.empty())
            cmd.name =word;
        else
            cmd.args.push_back(word);
    }
    return cmd;
}
// int main()
// {
//     std::vector<std::string> tests;

//     // tests.push_back("JOIN #a");
//     // tests.push_back("KICK #a bob :bad user");
//     tests.push_back("PRIVMSG #a :hello world");
//     // tests.push_back("TOPIC #a :new topic");

//     CommandHandler handler;

//     for (size_t i = 0; i < tests.size(); i++)
//     {
//         Commandeparse cmd = parser_commande(tests[i]);

//         std::cout << "\nINPUT: " << tests[i] << std::endl;

//         execute(cmd);
//     }
// }
