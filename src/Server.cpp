# include "../includes/Server.hpp"



void initSocket()
{
    /*
    socket
    bind
    listen
    accept
    recv
    send
    */
    int status;
    struct addrinfo hints;
    struct addrinfo *servinfo; //point to results
    struct addrinfo *p; //current

    memset(&hints, 0, sizeof hints); //make sure the struct is empty
    hints.ai_family = AF_UNSPEC; //don't care IPv4 or IPv6
    hints.ai_socktype = SOCK_STREAM; //TCP stream sockets
    hints.ai_flags = AI_PASSIVE; //fill in my IP for me
    if((status = getaddrinfo(NULL, info.port, &hints, &servinfo)) != 0)
    {
        std::cerr << "getaddressinfo " <<  gai_strerror(status) << std::endl;
        return false;
    }
    // servinfo now points to a linked list of 1 or more
    // struct addrinfos
    // ... do everything until you don't need servinfo anymore ....
    // loop through all the results and bind to the first we can
    for(p = servinfo; p != NULL; p = p->ai_next)
    {
        if((_server_fd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1)
        {
            perror("socket");
            continue;
        }
        //Set non-blocking
        int flags = fcntl(_server_fd, F_GETFL, 0);
        if(flags == -1)
        {
            perror("fcntl F_GETFL");
            return false;
        }
        // Set the O_NONBLOCK flag
        if (fcntl(_server_fd, F_SETFL, flags | O_NONBLOCK) == -1) {
            perror("fcntl F_SETFL O_NONBLOCK");
            return false;
        }
        if(bind(_server_fd, p->ai_addr, p->ai_addrlen) == -1)
        {
            close(_server_fd);
            perror("bind");
            continue;
        }
        break; // if we get here, we must have connected successfully
    }
    freeaddrinfo(servinfo); // free the linked-list

    if (current == NULL)
    {
        std::cerr << "bind: unable to create server socket" << std::endl;
        return false;
    }

    if(listen(_server_fd, SOMAXCONN) < 0)
    {
        perror("listen");
        close(_server_fd);
        return false;
    }
    std::cout << "Server listening on port " << _config.port << std::endl;
    return true;
}

//getaddrinfo → socket → setsockopt → bind → listen → fcntl → poll

🧭 10. Your next move (clear plan)

Do this EXACTLY:

Phase 1 (today)
Server socket
bind/listen
poll setup
Phase 2
accept clients
store them
Phase 3
recv from clients
print what they send (debug)
Phase 4
implement buffer per client
Phase 5
parse commands