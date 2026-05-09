#include "../includes/Server.hpp"

Client::Client(int fd, char* hostnamee)
: fd(fd),
  nickname(""),
  username(""),
  hostname(hostnamee),
  client_hostname("127.0.0.1"),
  c_channels(),
  Authenticated(false),
  c_password(false),
  c_user(false),
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
    if(!c_password && c_nick && c_user && !Authenticated)
    {
        Authenticated = true;
        std::string servername = ser->get_hostname();
        ser->sendData(getFd() ,":" + servername + " 001 " + nickname + " :Welcome to ft_irc server " + nickname + "@" + username + "\r\n");
        ser->sendData(getFd(),":" + servername + " 002 " + nickname + " :Your host is " + servername + ", running version 1.0\r\n");
        ser->sendData(getFd(), ":" + servername + " 003 " + nickname + " :This server was created " + std::string(__DATE__) + "\r\n");
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

void Client::execute(const Commandeparse &cmd)
{

    std::string c;

    for (size_t i = 0; i <cmd.name.size() ; i++)
    {
        c += std::toupper(cmd.name[i]);
    }
    if(cmd.name == "JOIN")
        HandledJOIN(cmd);
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
        HandledPRIVMSG(cmd);
    else
        std::cerr << "Unknown Command " << std::endl;
}

std::string Client::getClientUserName() const
{
    return username;
}

std::string Client::getClientNickName() const
{
    return nickname;
}

void Client::setClientUserName(std::string uname) const
{
    username = uname;
}

void Client::setClientNickName(std::string nname) const
{
    username = nname;
}

void Client::botJoinChannel(Channel *channel)
{
    if(!hasBot())
    {
        //bot join logic, 
        channel->addClient(bot);
        //🔥 Then optionally broadcast JOIN message
        std::string joinmssg = ":" + bot->get_prefix() + "@server JOIN " + channel->getName()
                                + "\r\n";
        sendMsgClient(joinmssg, bot);
    }
}

void Client::HandleBOT(std::string mssg)
{
    std::string replay;

    if(mssg == "!hello")
    {
        replay = ":" + bot->getClientNickName() + "!" + bot->getClientUserName() + "@server PRIVMSG " + channel->getName()
                + "Hi There 👋​";
    }
    else if(mssg == "!dice")
    {
        int random_num = rand() % 101; //give random number 0 - 100
        //convert int to string
        std::stringstream ss;
        ss << random_num;
        std::string num_str = ss.str();

        replay = ":" + bot->getClientNickName() + "!" + bot->getClientUserName() + "@server PRIVMSG " + channel->getName()
                + "random number 🎲(0 - 100)​: " + num_str + "\r\n";
    }
    else if(mssg == "!help")
    {
        replay = ":" + bot->getClientNickName() + "!" + bot->getClientUserName() + "@server PRIVMSG " + channel->getName()
                + "👀​ Commands: !hello, !help, !dice, !time" + "\r\n";
    }
    else if(mssg == "!time")
    {
        time_t current_time = time(0);
        char buffer[80];
        strftime(buffer, 80, "%Y-%m-%d %H:%M:%S", localtime(&current_time));
        replay = ":" + bot->getClientNickName() + "!" + bot->getClientUserName() + "@server PRIVMSG " + channel->getName()
                + "⏰ Current time is : " + buffer + "\r\n";
    }
    if(!replay.empty())
        sendMsgClient(replay, bot);
}