#include "../includes/Server.hpp"

Client::Client(int fd, char* hostnamee)
: fd(fd),
  nickname(""),
  username(""),
  hostname(hostnamee),
  realname(""),
  client_hostname("127.0.0.1"),
  c_channels(),
  read_buffer(""),
  Authenticated(false),
  c_password(false),
  c_user(false),
  ser(NULL),
  c_nick(false)
{}
Client::~Client(){}


int Client::getFd(){
    return fd;
}

// void Client::create_reply(const std::string &buffer)
// {
//     write_buffer += buffer;
// }
void Client::set_hostname(const std::string& host)
{
    client_hostname = host;
}

void Client::check_register()
{
    if(c_password && c_nick && c_user && !Authenticated)
    {
        Authenticated = true;

        std::string servername = ser->get_hostname();

        ser->sendData(getFd(),
        ":" + servername + " 001 " + nickname +
        " :Welcome to ft_irc server " +
        nickname + "@" + username + "\r\n");

        ser->sendData(getFd(),
        ":" + servername + " 002 " + nickname +
        " :Your host is " + servername +
        ", running version 1.0\r\n");

        ser->sendData(getFd(),
        ":" + servername + " 003 " + nickname +
        " :This server was created " +
        std::string(__DATE__) + "\r\n");
    }
}

std::string Client::get_prefix() const
{
    return nickname + "!" + username + "@" + client_hostname;
}

bool Client::in_channel(Channel *chan) const
{
    return c_channels.find(chan) != c_channels.end();
}
void Client::renoveChannel(Channel *chan)
{
    c_channels.erase(chan);
}

void Client::execute(const Commandeparse &cmd)
{
    std::string name = cmd.name;

    for (size_t i = 0; i < name.size(); i++)
        name[i] = std::toupper(name[i]);

    if (name == "CAP" || name == "PING" || name == "PONG" || name == "WHO" || name == "QUIT")
        return;

    std::cout << name << std::endl;

    if (name == "JOIN")
        HandledJOIN(cmd);
    else if (name == "PASS")
        HandledPASS(cmd);
    else if (name == "USER")
        HandledUSER(cmd);
    else if (name == "NICK")
        HandledNICK(cmd);
    else if (name == "KICK")
        HandledKICK(cmd);
    else if (name == "INVITE")
        HandledINVITE(cmd);
    else if (name == "TOPIC")
        HandledTOPIC(cmd);
    else if (name == "MODE")
        HandledMODE(cmd);
    else if (name == "PRIVMSG")
        HandledPRIVMSG(cmd);
    else if (name == "PART")
        HandledPART(cmd);
    else
        std::cerr << "Unknown Command" << std::endl;
}