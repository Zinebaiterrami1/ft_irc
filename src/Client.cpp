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

    std::string c;

    for (size_t i = 0; i <cmd.name.size() ; i++)
    {
        c += std::toupper(cmd.name[i]);
    }
    if(cmd.name == "JOIN")
        HandledJOIN(cmd);
    else if(cmd.name == "PASS")
        HandledPASS(cmd);
    else if(cmd.name == "USER")
        HandledUSER(cmd);
    else if(cmd.name == "NICK")
        HandledNICK(cmd);
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
    else if(cmd.name ="PART")
        HandledPART(cmd);
    else
        std::cerr << "Unknown Command " << std::endl;
}