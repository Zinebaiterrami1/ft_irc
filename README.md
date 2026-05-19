# ft_irc

*This project has been created as part of the 42 curriculum by nel-khad, fakoukou, zait-err.*

---

## Description

The **ft_irc** project is an implementation of an IRC (Internet Relay Chat) server in C++.

IRC is an application-layer protocol that allows communication between multiple clients connected to a server using TCP (transport layer).

The main objective of this project is to understand real-time network communication between multiple clients and a server, as well as handling multiple simultaneous connections.

The server uses an I/O multiplexing mechanism (`poll()` or `select()`) to manage multiple clients within a single process.

---

## Architecture

Client → TCP Socket → Server → Buffer → Parsing → Command Handling → Response → Client

### Workflow:

- The server receives raw data from clients via sockets
- Data is stored in a buffer until a full command is received (`\r\n`)
- The input stream is split into IRC commands
- Each command is parsed
- The command is processed by the appropriate handler
- A response is sent back to the client

---

## IRC Commands

### PASS
Authenticates the client with the server.

A password is required before registration.

---

### NICK
Sets or changes the user's nickname.

- Nicknames must be unique
- Can be changed at any time

---

### USER
Registers a user on the server.

- `0` : user mode (legacy IRC format)
- `*` : server-managed host
- `realname` : user's real name

---

### JOIN
Allows a user to join a channel.

- Creates the channel if it does not exist
- A user can join multiple channels
- ` JOIN <channel>,<channel>` 

---

### PART
Allows a user to leave a channel.

- Optional reason supported
- `part <channel> :<reasn>` 

---

### PRIVMSG
Sends a private message to a user or channel.
- `PRIVMSG <user> or <channel> :<msg>` 

---

### TOPIC
Sets or views a channel topic.

- If `+t` mode is enabled, only operators can change it
- `topic <channel> :<new_topic>` 

---

### INVITE
Invites a user to a channel.

- Mainly used with `+i` (invite-only mode)
- `invite <user> <channel>` 

---

### KICK
Removes a user from a channel.

- Optional reason supported
- `kick <channel> <user> :<reason>` 
---

## Channel Modes

### +i / -i (Invite only)
- `+i` : only invited users can join
- `-i` : disables invite-only mode
- `mode <channel> +i or -i` 

---

### +t / -t (Topic protection)
- `+t` : only operators can change the topic
- `-t` : everyone can change it
- `mode <channel> +t or -t` 

---

### +k / -k (Password)
- `+k <password>` : sets a channel password
- `-k` : removes the password
- `mode <channel> +k or -k <password>` 

---

### +o / -o (Operator)
- `+o <user>` : grants operator privileges
- `-o <user>` : removes operator privileges
- `mode <channel> +o or -o` 

---

### +l / -l (Limit)
- `+l <number>` : sets a user limit
- `-l` : removes the limit
- `mode <channel> +l or -l <limit>` 

---

## I/O Multiplexing

- The server starts by creating a listening socket using socket().
Then, bind() associates the socket with an IP address and a port, while listen() enables the socket to accept incoming client connections and queues pending requests.

- To manage multiple clients simultaneously, the server uses I/O multiplexing with poll(). The server socket file descriptor is stored inside a pollfd structure and monitored inside the main event loop.

- Inside the loop, poll() waits for activity on the monitored file descriptors. When an event occurs:

- If the active file descriptor is the server socket, the server recognizes it as a new incoming connection and calls accept() to create a new client socket.
Otherwise, the event comes from an already connected client, and the server uses recv() to read incoming IRC commands.

- After processing the received command, the server responds to the client using send() to transmit data back through the corresponding client socket.

---

## Resources 📚 

- RFC 1459 / RFC 2812: https://www.rfc-editor.org/rfc/rfc1459.html
- Beej’s Guide to Network Programming: https://beej.us/guide/bgnet/html/split/
---


## AI Usage

Artificial intelligence was used to:

 - Clarify IRC protocol concepts 
 - Help structure the README
 - Understand client/server data flow

---

## Compilation

`make`

## Execution

To run the server, use the following command:

`./ircserv <port> <password>`