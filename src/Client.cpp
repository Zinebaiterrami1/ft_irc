#include "../includes/Server.hpp"

Client::Client(int fd, char* hostname) : fd(fd), hostname(hostname), isRegistred(false){}
Client::~Client(){}

int Client::getFd(){
    return fd;
}

void Client::create_reply(const std::string &buffer)
{
    write_buffer += buffer;
}
void Client::set_hostname(const std::string& host)
{
    client_hostname = host;
}


std::string Client::get_prefix() const
{
    return nickname + "!" + username + "@" + client_hostname;
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
        broadcastmessage(channel, joinmssg);
    }
}

void Client::HandleBOT(Channel* channel, std::string mssg)
{
    std::string replay;

    if(mssg == "!hello")
    {
        replay = ;
    }
    else if(mssg == "!dice")
    {
        replay = ;
    }
    else if(mssg == "!help")
    {
        replay = ;
    }
    else if(mssg == "!time")
    {
        replay = ;
    }
    if(!replay.empty())
        broadcastmssg(channel, replay);
}