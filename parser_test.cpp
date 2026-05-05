#include<iostream>
#include<string>
#include <vector>

struct commande
{
    /* data */
    std::string name;
    std::vector<std::string> args;
};
commande parser_commande(std::string &line)
{
    commande cmd;
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

    tests.push_back("JOIN #a");
    tests.push_back("KICK #a bob :bad user");
    tests.push_back("PRIVMSG #a :hello word");
    tests.push_back("TOPIC #a :new topic");

    for(size_t i = 0; i < tests.size(); i++)
    {
        commande cmd = parser_commande(tests[i]);
        std::cout << "commande" << cmd.name <<std::endl;
        std::cout << "args   ";

        for(size_t j = 0;j < cmd.args.size(); j++)
        {
            std::cout << cmd.args[j];
            if( j + 1 < cmd.args.size())
                std::cout << "   |  ";
        }

        std::cout <<"   ---------------->  "<< std::endl;
    }
}  