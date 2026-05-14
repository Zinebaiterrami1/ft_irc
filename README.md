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

---

### PART
Allows a user to leave a channel.

- Optional reason supported

---

### PRIVMSG
Sends a private message to a user or channel.

---

### TOPIC
Sets or views a channel topic.

- If `+t` mode is enabled, only operators can change it

---

### INVITE
Invites a user to a channel.

- Mainly used with `+i` (invite-only mode)

---

### KICK
Removes a user from a channel.

- Optional reason supported

---

## Channel Modes

### +i / -i (Invite only)
- `+i` : only invited users can join
- `-i` : disables invite-only mode

---

### +t / -t (Topic protection)
- `+t` : only operators can change the topic
- `-t` : everyone can change it

---

### +k / -k (Password)
- `+k <password>` : sets a channel password
- `-k` : removes the password

---

### +o / -o (Operator)
- `+o <user>` : grants operator privileges
- `-o <user>` : removes operator privileges

---

### +l / -l (Limit)
- `+l <number>` : sets a user limit
- `-l` : removes the limit

---

## I/O Multiplexing



---

## Resources

- RFC 1459 / RFC 2812: https://www.rfc-editor.org/rfc/rfc1459.html

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