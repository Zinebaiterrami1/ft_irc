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
// Channel* Server::get_channel(const std::string &name)
// {
//     std::map<std::string, Channel*>::iteratpr it 
// }

Client *Server::getClient(int fd){
        std::vector<Client *>::iterator it = clients.begin();
        for(;it != clients.end(); it++){
                if((*(*it)).fd == fd)
                        return *it;
        }
        return NULL;
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
    int status;
    struct addrinfo hints;
    struct addrinfo *servinfo; //point to results
    struct addrinfo *p; //current

    memset(&hints, 0, sizeof hints); //make sure the struct is empty
    hints.ai_family = AF_UNSPEC; //don't care IPv4 or IPv6
    hints.ai_socktype = SOCK_STREAM; //TCP stream sockets
    hints.ai_flags = AI_PASSIVE; //fill in my IP for me
    if((status = getaddrinfo(NULL, toString(_config.port).c_str(), &hints, &servinfo)) != 0)
    {
        std::cerr << "getaddressinfo " <<  gai_strerror(status) << std::endl;
        return false;
    }
    for(p = servinfo; p != NULL; p = p->ai_next)
    {
        if((_srvSoc_fd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1)
        {
            perror("socket");
            continue;
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
        if(bind(_srvSoc_fd, p->ai_addr, p->ai_addrlen) == -1)
        {
            close(_srvSoc_fd);
            perror("bind");
            continue;
        }
        break; // if we get here, we must have connected successfully
    }
    freeaddrinfo(servinfo); // free the linked-list

    if (p == NULL)
    {
        std::cerr << "bind: unable to create server socket" << std::endl;
        return false;
    }

    if(listen(_srvSoc_fd, SOMAXCONN) < 0)
    {
        perror("listen");
        close(_srvSoc_fd);
        return false;
    }
    char hostname[256];
    if(gethostname(hostname, sizeof(hostname)))
        server_hostname = hostname;
    else
        server_hostname = "localhost";
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
    Client *client = new Client(clientFd, inet_ntoa(clientAddr.sin_addr));
    clients.push_back(client);
    ClientFds.push_back(client->getFd());
    fds.push_back(clientPollFd);
    std::cout << "New client connected: " << inet_ntoa(clientAddr.sin_addr) << std::endl;
}

void Server::receiveData(int clientFd)
{
    char tmp[1024];
    tmp[0] = '\0';
    Client &client = *getClient(clientFd);
    std::string &buffer = client.read_buffer;
    
    int bytes = recv(clientFd, tmp, sizeof(tmp)-1, 0);
    tmp[bytes] = '\0';
    if(bytes > 0){//append buffer 
        buffer += tmp;
        size_t found = buffer.find('\n');
        if(found != std::string::npos){
            //donne a parse commande et execute
            int x = buffer[found-1] != '\r'?0:1;
            std::string cmd = buffer.substr(0, found-x);
            if(cmd.length() > 510)
                throw 45;
            buffer = buffer.substr(found+1, buffer.length());
            client.execute(parser_commande(cmd));
        }
    }
    else if(bytes == 0){//connection close //remove client
        //leave all channels
        //remove frome clients in server 
        removeClient(clientFd, 0);
    }
    else{//an error occured perror()
        perror("recv : ");
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
    else
        std::cout << "Sent to " << fd << ": " << mssg << std::endl;
}

void Server::removeClient(int fd, int flag)
{
    //remove from poll vector, vector clientfds, client
    if(flag)
    {
        std::vector<Client*>::iterator it = this->clients.begin();
        while(it != clients.end())
        {
            delete *it;
            it++;
        }
        this->clients.clear();
        return ;
    }
    // Remove from pollfd vector
    for(std::vector<struct pollfd>::iterator it = fds.begin(); it != fds.end(); it++)
    {
        if(it->fd == fd)
        {
            fds.erase(it);
            break;
        }
    }
    //remove from client fds vector
    for(std::vector<int>::iterator it = ClientFds.begin(); it != ClientFds.end(); it++)
    {
        if(*it == fd)
        {
            ClientFds.erase(it);
            break;
        }
    }
    std::cout << "Client " << fd << " removed" << std::endl;
    close(fd);
}

void Server::CloseConnection()
{
    std::cout << "Closing all connections..." << std::endl;
    //close sockets of every client here
    for(std::vector<Client*>::iterator it = clients.begin(); it != clients.end(); it++)
    {
        close((*it)->getFd());
        delete *it;
    }
    
    //close socket of server
    if(this->_srvSoc_fd != -1)
    {
        close(_srvSoc_fd);
    }
    std::cout << "All connections closed. Server shutdown complete." << std::endl;
}

// void Server::ClearChannels()
// {
//     std::vector<Channel*>::iterator it;

//     for(it = channels.begin(); it != channels.end(); it++)
//     {
//         if(*it)
//             delete *it;
//     }
//     channels.clear();
// }

void Server::runSocket()
{
    signal(SIGINT, signalHandler);
    signal(SIGQUIT, signalHandler);
    initSocket();
    std::cout << "Server Launched and listening on port " << _config.port << std::endl;
}

void Server::StartServer()
{
    //call run socket. create event loop, if client new create it, if not receive his data, if signal received, shut down server
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
                    //add new client
                    addNewClient();
                }
                else
                {
                    //receive data from existing client
                    receiveData(fds[i].fd);
                }
            }
        }
    }
    removeClient(_srvSoc_fd, 1);
    CloseConnection();
    // ClearChannels();
}