#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include "../includes/Commandeparse.hpp"
#include "../includes/Server.hpp"

Commandeparse parser_commande(std::string &line)
{
    std::cout<< RED << line << std::endl;
    Commandeparse cmd;
    std::string word;

    // 🔥 remove \r and \n safely (important for HexChat)
    line.erase(std::remove(line.begin(), line.end(), '\r'), line.end());
    line.erase(std::remove(line.begin(), line.end(), '\n'), line.end());

    for (size_t i = 0; i < line.size(); i++)
    {
        char c = line[i];

        // SPACE = separator (except after ':')
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
        // ':' = trailing parameter (IRC rule)
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

            // skip spaces after ':'
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

    // last word flush
    if (!word.empty())
    {
        if (cmd.name.empty())
            cmd.name = word;
        else
            cmd.args.push_back(word);
    }

    return cmd;
}