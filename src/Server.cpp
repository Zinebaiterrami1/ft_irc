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