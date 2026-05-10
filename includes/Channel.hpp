#pragma once

#include <string>
#include <vector>
#include "../includes/Client.hpp"
#include<set>
class Client;
class Server;
class Channel
{
private:
    std::string name;
    std::string topic;
    std::string key;

    std::vector<Client *> users;
    std::vector<Client *> operators;
    std::set<std::string> invited;
    bool inviteOnly;
    bool topicRestricted;
    size_t userLimit;

public:
    Channel();
    Channel(const std::string &channelName);

    std::string getName() const;
    std::string getTopic() const;

    void addUser(Client *client);
    void removeUser(Client *client);
    void removeOperator(Client *client);
    bool hasUser(Client *client) const;
    bool hasKey();
    bool hasLimit();

    void addOperator(Client *client);
    bool isOperator(Client *client) const;

    void setTopic(const std::string &newTopic);

    void setInviteOnly(bool mode);
    void setTopicRestricted(bool mode);
    void setKey(const std::string &k);
    void setUserLimit(size_t limit);

    std::string getKey();
    size_t getLimit();


    bool isInviteOnly() const;
    bool istopicRestricted() const {return topicRestricted;}
    size_t getUserLimit() const;
    void add_invite(const std::string& nickname);
    const std::vector<Client *> &getUsers() const;
    std::string get_mode() const;

    void brodcast_Channel(const std::string &msg, Client *sender, Server *ser);
};

