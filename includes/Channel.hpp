#pragma once

#include <string>
#include <vector>
#include "../includes/Client.hpp"

class Client;

class Channel
{
private:
    std::string name;
    std::string topic;
    std::string key;

    std::vector<Client *> users;
    std::vector<Client *> operators;

    bool inviteOnly;
    bool topicRestricted;
    int userLimit;

public:
    Channel();
    Channel(const std::string &channelName);

    std::string getName() const;
    std::string getTopic() const;

    void addUser(Client *client);
    void removeUser(Client *client);
    bool hasUser(Client *client) const;

    void addOperator(Client *client);
    bool isOperator(Client *client) const;

    void setTopic(const std::string &newTopic);

    void setInviteOnly(bool mode);
    void setTopicRestricted(bool mode);
    void setKey(const std::string &k);
    void setUserLimit(int limit);

    bool isInviteOnly() const;
    int getUserLimit() const;

    const std::vector<Client *> &getUsers() const;

    void sendMsgClient(const std::string &msg, Client *sender);
};

