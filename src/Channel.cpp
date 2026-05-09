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

void Channel::sendMsgClient(const std::string &msg, Client *sender)
{
    for (size_t i = 0; i < users.size(); i++)
    {
        if (users[i] != sender)
        {
            users[i]->create_reply(msg);
        }
    }
}


std::string Channel::getName() const
{
    return name;
}

std::string Channel::getTopic() const
{
    return topic;
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
    for (size_t i = 0; i < operators.size(); i++)
    {
        if (operators[i] == client)
        {
            operators.erase(operators.begin() + i);
            return;
        }
    }
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

void Channel::setUserLimit(int limit)
{
    userLimit = limit;
}


bool Channel::isInviteOnly() const
{
    return inviteOnly;
}

int Channel::getUserLimit() const
{
    return userLimit;
}

const std::vector<Client *> &Channel::getUsers() const
{
    return users;
}