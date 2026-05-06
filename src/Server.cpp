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

void signalHandler(int signum)
{
    (void)signum;
    std::cout << "\nSignal received, shutting down..." << std::endl;
    Server::sig = true;
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
    if((status = getaddrinfo(NULL, std::to_string(_config.port).c_str(), &hints, &servinfo)) != 0)
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
    std::cout << "Server listening on port " << _config.port << std::endl;
    
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
    std::cout << "New client connected: " << inet_ntoa(clientAddr.sin_addr) << std::endl;
}

// void Server::receiveData(&Client client)
// {
//     char buffer[513];
//     buffer[0] = '\0';
//     recv(client.getFd(), buffer, sizeof(buffer), 0);
//     // client.setCommande(buffer);
//     if(strlen(buffer) > 0){
//         client.commande.append(buffer);
//     }
//     else
//         if(!client.commande.empty() && client.count("\r\n"))
//             //parse
//             //execute //send inside methode
//         else 
//             //close connection and error
// }

void Server::receiveData(int clientFd)
{
    std::string buffer;
    char tmp[513];
    tmp[0] = '\0';
    // buffer = '\0';
    int bytes = recv(clientFd, tmp, sizeof(buffer), 0);
    if(bytes > 0){//append buffer 
        buffer += tmp;
        size_t found = buffer.find('\n');
        if(found != std::string::npos){
            //donne a parse commande et execute
            int x = buffer[found-1] != '\r'?0:1;
            std::string cmd = buffer.substr(0, found-x);
            buffer = buffer.substr(found-x, buffer.length());
            execute(parser_commande(cmd));
        }
    }
    else if(bytes == 0){//connection close //remove client
        
    }
    else{//an error occured perror()
        perror("recv : ");
    }
}