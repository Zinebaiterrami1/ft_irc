#include<iostream>
#include<string>
#include <vector>
#include "Commandeparse.hpp"
// #include "Channel.hpp"
#include "CommandHandler.hpp"
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
int main()
{
    std::vector<std::string> tests;

    // tests.push_back("JOIN #a");
    // tests.push_back("KICK #a bob :bad user");
    tests.push_back("PRIVMSG #a :hello world");
    // tests.push_back("TOPIC #a :new topic");

    CommandHandler handler;

    for (size_t i = 0; i < tests.size(); i++)
    {
        Commandeparse cmd = parser_commande(tests[i]);

        std::cout << "\nINPUT: " << tests[i] << std::endl;

        handler.execute(cmd);
    }
}
