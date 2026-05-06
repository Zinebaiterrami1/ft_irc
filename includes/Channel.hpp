#pragma once

#include <string>
#include <vector>

class Channel
{
private:
    std::string name;
    std::string topic;
    std::vector<std::string> users;
    std::vector<std::string> operators;

    bool inviteOnly;
    bool topicRestricted;
    std::string key;
    int userLimit;

public:
    Channel();
    Channel(const std::string &channelName);

    std::string getName() const;
    std::string getTopic() const;

    void addUser(const std::string &nickname);
    void removeUser(const std::string &nickname);
    bool hasUser(const std::string &nickname) const;

    void addOperator(const std::string &nickname);
    bool isOperator(const std::string &nickname) const;

    void setTopic(const std::string &newTopic);

    void setInviteOnly(bool mode);
    void setTopicRestricted(bool mode);
    void setKey(const std::string &k);
    void setUserLimit(int limit);

    bool isInviteOnly() const;
    int getUserLimit() const;
};
