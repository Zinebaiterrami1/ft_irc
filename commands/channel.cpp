#include "Channel.hpp"

Channel::Channel()
: inviteOnly(false), topicRestricted(false), userLimit(-1)
{
}

Channel::Channel(const std::string &channelName)
: name(channelName), inviteOnly(false), topicRestricted(false), userLimit(-1)
{
}

std::string Channel::getName() const
{
    return name;
}

std::string Channel::getTopic() const
{
    return topic;
}

void Channel::addUser(const std::string &nickname)
{
    users.push_back(nickname);
}

void Channel::removeUser(const std::string &nickname)
{
    for (size_t i = 0; i < users.size(); i++)
    {
        if (users[i] == nickname)
        {
            users.erase(users.begin() + i);
            return;
        }
    }
}

bool Channel::hasUser(const std::string &nickname) const
{
    for (size_t i = 0; i < users.size(); i++)
    {
        if (users[i] == nickname)
            return true;
    }
    return false;
}

void Channel::addOperator(const std::string &nickname)
{
    operators.push_back(nickname);
}

bool Channel::isOperator(const std::string &nickname) const
{
    for (size_t i = 0; i < operators.size(); i++)
    {
        if (operators[i] == nickname)
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
