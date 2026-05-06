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