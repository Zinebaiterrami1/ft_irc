#include <cctype>
#include "../includes/config.hpp"
#include "../includes/Commandeparse.hpp"
#include "../includes/Client.hpp"
#include "../includes/Server.hpp"

std::string valide_nick(const std::string &nick)
{
    std::string invalid = "#%&()+,*!?=<.>/;:@~ ";

    for (size_t i = 0; i < nick.length(); i++)
    {
        if (invalid.find(nick[i]) != std::string::npos)
            return "";
    }

    return nick;
}

bool is_special(unsigned char c)
{
    if (c == '[' || c == ']' || c == '\\' || c == '`' ||
        c == '_' || c == '^' || c == '{' || c == '|' || c == '}')
    {
        return true;
    }

    return false;
}

void Client::HandledNICK(const Commandeparse &cmd)
{

    std::string servername = ser->get_hostname();

    if (!c_password)
    {
        ser->sendData(getFd(),
        ":" + servername + " 451 * :You have not registered\r\n");
        return;
    }

    if (cmd.args.empty())
    {
        ser->sendData(getFd(),
        ":" + servername + " 431 * :No nickname given\r\n");
        return;
    }

    std::string nick_name = valide_nick(cmd.args[0]);

    if (nick_name.empty())
    {
        ser->sendData(getFd(),
        ":" + servername + " 432 * " + cmd.args[0] + " :Erroneous nickname\r\n");
        return;
    }

    if (nick_name.length() > 9)
    {
        ser->sendData(getFd(),
        ":" + servername + " 432 * " + nick_name + " :Erroneous nickname\r\n");
        return;
    }

    unsigned char first_char = static_cast<unsigned char>(nick_name[0]);

    if (!std::isalpha(first_char) && !is_special(first_char))
    {
        ser->sendData(getFd(),
        ":" + servername + " 432 * " + nick_name + " :Erroneous nickname\r\n");
        return;
    }

    if (ser->nickname_use(nick_name, this))
    {
        std::string current;

        if (nickname.empty())
            current = "*";
        else
            current = nickname;

        ser->sendData(getFd(),
        ":" + servername + " 433 " + current + " " + nick_name +
        " :Nickname is already in use\r\n");

        return;
    }

    std::string old_nick = nickname;

    if (nick_name == nickname)
        return;

    nickname = nick_name;
    c_nick = true;

    std::string nick_msg =
    ":" + old_nick + "!" + username + "@" +
    client_hostname + " NICK :" + nickname + "\r\n";

    if (Authenticated)
    {
        ser->sendData(getFd(), nick_msg);
        ser->message_to_all_channel_commun(this, nick_msg);
    }

    check_register();
}