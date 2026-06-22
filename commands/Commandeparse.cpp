#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include "../includes/Commandeparse.hpp"
#include "../includes/Server.hpp"

Commandeparse parser_commande(std::string &line)
{
    Commandeparse cmd;
    std::string word;

    line.erase(std::remove(line.begin(), line.end(), '\r'), line.end());
    line.erase(std::remove(line.begin(), line.end(), '\n'), line.end());

    for (size_t i = 0; i < line.size(); i++)
    {
        char c = line[i];

        if (c == ' ')
        {
            if (!word.empty())
            {
                if (cmd.name.empty())
                    cmd.name = word;
                else
                    cmd.args.push_back(word);

                word.clear();
            }
        }
        else if (c == ':' && (i == 0 || line[i - 1] == ' '))
        {
            if (!word.empty())
            {
                if (cmd.name.empty())
                    cmd.name = word;
                else
                    cmd.args.push_back(word);

                word.clear();
            }

            size_t start = i + 1;
            while (start < line.size() && line[start] == ' ')
                start++;

            cmd.args.push_back(line.substr(start));
            break;
        }
        else
        {
            word += c;
        }
    }

    if (!word.empty())
    {
        if (cmd.name.empty())
            cmd.name = word;
        else
            cmd.args.push_back(word);
    }

    return cmd;
}