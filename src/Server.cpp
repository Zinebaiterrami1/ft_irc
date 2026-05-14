# include "../includes/Server.hpp"


bool Server::sig = false;

Server::Server(const config &cfg) : _srvSoc_fd(-1) , _config(cfg)
{

}

Server::~Server()
{
    if(_srvSoc_fd != -1)
        close(_srvSoc_fd);
}
Channel *Server::get_channel(const std::string &name)
{
    for (size_t i = 0; i < Channels.size(); i++)
    {
        if (Channels[i]->getName() == name)
            return Channels[i];
    }
    return NULL;
}

std::vector<Channel*> Server::get_all_channels(){
    return Channels;
}

Channel *Server::create_channel(const std::string &name)
{
    for (size_t i = 0; i < Channels.size(); i++)
    {
        if (Channels[i]->getName() == name)
            return Channels[i];
    }

    Channel *ch = new Channel(name);
    Channels.push_back(ch);
    return ch;
}
void Server::delete_channel(const std::string& name)
{
    for (std::vector<Channel*>::iterator it = Channels.begin();
         it != Channels.end();
         ++it)
    {
        if ((*it)->getName() == name)
        {
            delete *it;          
            Channels.erase(it); 
            return;
        }
    }
}

Client *Server::getClient(int fd){
        std::vector<Client *>::iterator it = clients.begin();
        for(;it != clients.end(); it++){
                if((*(*it)).fd == fd)
                        return *it;
        }
        return NULL;
}
bool Server::nickname_use(const std::string &nick, const Client *cl)
{
    for(std::vector<Client *>::iterator it = clients.begin(); it != clients.end() ; it++)
    {
        const Client *c = (*it);
        if(c == cl)
            continue;
        if(c->get_nickname() == nick)
            return true;
    }
    return false;
}

Client* Server::find_nicknameclient(const std::string &nick)
{
    for(std::vector<Client*>::iterator it = clients.begin(); it != clients.end(); it++)
    {
        if((*it)->get_nickname() == nick)
        {
            return *it;
        }
    }
    return NULL;
}
void Server::message_to_all_channel_commun(Client *cl, const std::string &msg)
{
    if (!cl)
        return;

    std::set<Client *> notified;

    const std::set<Channel *> &channels = cl->c_channels;

    for (std::set<Channel *>::const_iterator it = channels.begin();
         it != channels.end();
         ++it)
    {
        Channel *ch = *it;
        if (!ch)
            continue;

        const std::vector<Client *> &members = ch->getUsers();

        for (size_t j = 0; j < members.size(); j++)
        {
            Client *recipient = members[j];

            if (!recipient || recipient == cl)
                continue;

            if (notified.insert(recipient).second)
            {
                sendData(recipient->getFd(), msg);
            }
        }
    }
}
void signalHandler(int signum)
{
    (void)signum;
    std::cout << "\nSignal received, shutting down..." << std::endl;
    Server::sig = true;
}

std::string toString(int value)
{
    std::ostringstream oss;
    oss << value;
    return oss.str();
}

bool Server::initSocket()
{
    if((_srvSoc_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("socket");
        std::cerr << "Socket creation failed " << std::endl;
        return false;
    }
    //Set non-blocking
    int flags = fcntl(_srvSoc_fd, F_GETFL, 0);
    if(flags == -1)
    {
        perror("fcntl F_GETFL");
        return false;
    }
    // Set the O_NONBLOCK flag
    if (fcntl(_srvSoc_fd, F_SETFL, flags | O_NONBLOCK) == -1) {
        perror("fcntl");
        return false;
    }
    int opt = 1;
    if(setsockopt(_srvSoc_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)\
    {
        perror("setsockopt");
        close(_srvSoc_fd);
        return false;
    }
    memset(&_address, 0, sizeof(_address));
    _address.sin_family = AF_INET;
    int listen_port = _config.port;
    _address.sin_port = htons(listen_port);
    _address.sin_addr.s_addr = INADDR_ANY;

    if(bind(_srvSoc_fd, (struct sockaddr*)&_address, sizeof(_address)) < 0)
    {
        close(_srvSoc_fd);
        perror("bind");
        return false;
    }

    if(listen(_srvSoc_fd, SOMAXCONN) < 0)
    {
        perror("listen");
        close(_srvSoc_fd);
        return false;
    }
    
    server_hostname = "ft_irc_server";
    struct pollfd ev;

    ev.fd = _srvSoc_fd;
    ev.events = POLLIN;
    ev.revents = 0;

    fds.push_back(ev);
    return true;
}


void Server::addNewClient()
{
    //accept
    sockaddr_in clientAddr;
    socklen_t addrLen = sizeof(clientAddr);
    if(sig)
        return ;
    int clientFd = accept(_srvSoc_fd, (struct sockaddr *)&clientAddr, &addrLen);
    if(clientFd == -1)
    {
        perror("accept");
        return ;
    }
    //create new client
    pollfd clientPollFd;

    clientPollFd.fd = clientFd;
    clientPollFd.events = POLLIN;
    clientPollFd.revents = 0;
    Client *client = new Client(clientFd);
    client->ser = this;
    clients.push_back(client);
    ClientFds.push_back(client->getFd());
    fds.push_back(clientPollFd);
    std::cout << "New client connected\n";
}

void Server::receiveData(int clientFd)
{
    char tmp[1024];

    Client &client = *getClient(clientFd);
    std::string &buffer = client.read_buffer;

    ssize_t bytes = recv(clientFd, tmp, sizeof(tmp), 0);
    
    if(bytes <= 0)
    {
        if(bytes < 0)
            perror("recv failed : ");
        else
        {
            removeClient(clientFd, 0);
        }
        return;
    }
    
    buffer.append(tmp, bytes);
    size_t pos;
    while((pos = buffer.find('\n')) != std::string::npos)
    {
        std::string line = buffer.substr(0, pos);
        buffer.erase(0, pos + 1);
        if(!line.empty() && line[line.length()-1] == '\r')
            line.erase(line.length()-1);
        if(line.length() > 510)
        {
            std::cerr << RED << "ERROR : Commande length more than 510\n" << RESET;
            continue;
        }
        if(!line.empty())
            client.execute(parser_commande(line));
    }
}

void Server::sendData(int fd, std::string mssg)
{
    int bytes = send(fd, mssg.c_str(), mssg.length(), 0);
    if(bytes == -1)
    {
        perror("send");
        removeClient(fd, 0);
    }
}

void Server::removeClient(int fd, int flag)
{
    if(flag)
    {
        std::vector<Client*>::iterator it = this->clients.begin();
        while(it != clients.end())
        {
            delete *it;
            it++;
        }
        this->clients.clear();
        fds.clear();
        ClientFds.clear();
        return ;
    }
    else
    {
        std::vector<Client*>::iterator it = this->clients.begin();
        while(it != clients.end())
        {
            if((*it)->fd == fd)
            {
                it = clients.erase(it);
                delete *it;
            }
            else 
                it++;
        }
        //remove client from server
        for(std::vector<struct pollfd>::iterator it = fds.begin(); it != fds.end(); it++)
        {
            if(it->fd == fd)
            {
                fds.erase(it);
                break;
            }
        }
        for(std::vector<int>::iterator it = ClientFds.begin(); it != ClientFds.end(); it++)
        {
            if(*it == fd)
            {
                ClientFds.erase(it);
                break;
            }
        }
    }
    std::cout << "Client " << fd << " removed" << std::endl;
    close(fd);
}

void Server::CloseConnection()
{
    std::cout << "Closing all connections..." << std::endl;
    for(std::vector<Client*>::iterator it = clients.begin(); it != clients.end(); it++)
    {
        close((*it)->getFd());
        delete *it;
    }
    
    if(this->_srvSoc_fd != -1)
    {
        close(_srvSoc_fd);
    }
    std::cout << "All connections closed. Server shutdown complete." << std::endl;
}

void Server::ClearChannels()
{
    for (size_t i = 0; i < Channels.size(); i++)
    {
        delete Channels[i];
    }
    Channels.clear();
}

void Server::runSocket()
{
    signal(SIGINT, signalHandler);
    signal(SIGQUIT, signalHandler);
    initSocket();
    std::cout << "Server Launched and listening on port " << _config.port << std::endl;
}

void Server::StartServer()
{
    runSocket();
    while(!sig)
    {
        if(poll(fds.data(), fds.size(), -1) == -1 && !sig)
        {
            perror("poll");
            break;
        }
        for(size_t i = 0; i < fds.size(); i++)
        {
            if(fds[i].revents & POLLIN)
            {
                if(fds[i].fd == _srvSoc_fd)
                {
                    addNewClient();
                }
                else
                {
                    receiveData(fds[i].fd);
                }
            }
        }
    }
    removeClient(_srvSoc_fd, 1);
    CloseConnection();
    ClearChannels();
}

