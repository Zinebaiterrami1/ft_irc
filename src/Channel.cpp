#include "../includes/Channel.hpp"
#include <iostream>


Channel::Channel()
    : name(""), topic(""),
      inviteOnly(false),
      topicRestricted(false),
      userLimit(-1)
{
}

Channel::Channel(const std::string &channelName)
    : name(channelName),
      topic(""),
      inviteOnly(false),
      topicRestricted(false),
      userLimit(-1)
{
}


void Channel::brodcast_Channel(const std::string &msg,  Server *ser)
{
    for (size_t i = 0; i < users.size(); i++)
    {
        Client *recipient = users[i];

        if (recipient)
            ser->sendData(recipient->getFd(), msg);
    }
}

std::string Channel::getKey(){
    return key;
}

bool Channel::hasKey(){
    return !key.empty();
}

bool Channel::hasLimit(){
    return userLimit != -1;
}

size_t Channel::getLimit(){
    return userLimit;
}

std::string Channel::getName() const
{
    return name;
}

std::string Channel::getTopic() const
{
    return topic;
}

std::string Channel::get_mode() const
{
    std::string modes = "+";
    std::string param;

    if (inviteOnly)
        modes += 'i';

    if (topicRestricted)
        modes += 't';

    if (!key.empty())
    {
        modes += 'k';
        param += " " + key;
    }

    if (userLimit > 0)
    {
        std::stringstream value;
        value << userLimit;

        modes += 'l';
        param += " " + value.str();
    }

    if (modes == "+")
        return "";

    return modes + param;
}
void Channel::addUser(Client *client)
{
    users.push_back(client);
}

void Channel::removeUser(Client *client)
{
    for (size_t i = 0; i < users.size(); i++)
    {
        if (users[i] == client)
        {
            users.erase(users.begin() + i);
            return;
        }
    }
}

void Channel::removeOperator(Client *client){
    for (size_t i = 0; i < operators.size(); i++)
    {
        if (operators[i] == client)
        {
            operators.erase(operators.begin() + i);
            return;
        }
    }
}

void Channel::removeInvite(Client *client)
{
    invited.erase(client->get_nickname());
}

bool Channel::hasUser(Client *client) const
{
    for (size_t i = 0; i < users.size(); i++)
    {
        if (users[i] == client)
            return true;
    }
    return false;
}


void Channel::addOperator(Client *client) 
{
    operators.push_back(client);
}

bool Channel::isOperator(Client *client) const
{
    for (size_t i = 0; i < operators.size(); i++)
    {
        if (operators[i] == client)
            return true;
    }
    return false;
}

bool Channel::isInvited(Client *client) const
{
    return invited.find(client->get_nickname()) != invited.end();
}

void Channel::setTopic(const std::string &newTopic)
{
    topic = newTopic;
}

void Channel::setInviteOnly(bool mode)
{
    inviteOnly = mode;
}

void Channel::setTopicRestricted(bool mode)
{
    topicRestricted = mode;
}

void Channel::setKey(const std::string &k)
{
    key = k;
}

void Channel::setUserLimit(size_t limit)
{
    userLimit = limit;
}


bool Channel::isInviteOnly() const
{
    return inviteOnly;
}

size_t Channel::getUserLimit() const
{
    return userLimit;
}

const std::vector<Client *> &Channel::getUsers() const
{
    return users;
}

void Channel::add_invite(const std::string& nickname)
{
    invited.insert(nickname);
}