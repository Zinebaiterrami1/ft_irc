# include "../includes/Server.hpp"

void Server::StartServer()
{
	runSocket();
	if (fds.empty())
		return;

	while (!sig)
	{
		int ready = poll(fds.data(), fds.size(), -1);
		if (ready == -1)
		{
			if (sig)
				break;
			perror("poll");
			break;
		}

		for (size_t i = 0; i < fds.size();)
		{
			short revents = fds[i].revents;
			int fd = fds[i].fd;

			fds[i].revents = 0;

			if (revents == 0)
			{
				++i;
				continue;
			}

			if (fd == _srvSoc_fd && (revents & POLLIN))
			{
				addNewClient();
				++i;
				continue;
			}

			if (revents & POLLIN)
				receiveData(fd);

			if (revents & (POLLHUP | POLLERR | POLLNVAL))
			{
				removeClient(fd, 0);
				continue;
			}

			++i;
		}
	}

	CloseConnection();
	ClearChannels();
}

poll - Explanation
poll is a system call for monitoring multiple file descriptors to see if they're ready for I/O operations. It's commonly used in network programming and server applications.

What it does
poll allows a program to check multiple file descriptors (sockets, pipes, files, etc.) simultaneously to determine which ones are ready for reading, writing, or have errors—without blocking the entire program.

How it works
1.You provide poll with an array of file descriptors and specify what events you want to monitor for each (read, write, error)
2.poll blocks until at least one of the monitored events occurs (or a timeout)
3.It returns information about which file descriptors are ready for which operations
4.Your program can then handle the ready file descriptors

Function signature
#include <poll.h>

int poll(struct pollfd *fds, nfds_t nfds, int timeout);

Parameters
Parameter	Type	     Description
fds	   struct pollfd *	  Array of structures, one per file descriptor to monitor
nfds	  nfds_t	      Number of elements in the fds array
timeout	  int	          Maximum time to wait in milliseconds. -1 = wait indefinitely, 0 = return immediately

The pollfd structure
struct pollfd {
    int fd;        // File descriptor to monitor
    short events;  // Events of interest (bitmask)
    short revents; // Events that occurred (returned by poll)
};

Common event flags
POLLIN - Data available to read
POLLOUT - Ready to write
POLLERR - Error condition
POLLHUP - Hung up (connection closed)
POLLNVAL - Invalid file descriptor

Return value
Positive integer: Number of file descriptors with events
0: Timeout occurred, no events
-1: Error occurred


Example usage
struct pollfd fds[2];
fds[0].fd = socket1;
fds[0].events = POLLIN;  // Want to read from socket1
fds[1].fd = socket2;
fds[1].events = POLLIN;  // Want to read from socket2

int ret = poll(fds, 2, 5000);  // Wait up to 5 seconds

if (ret > 0) {
    if (fds[0].revents & POLLIN) {
        // socket1 is ready to read
    }
    if (fds[1].revents & POLLIN) {
        // socket2 is ready to read
    }
}

This is particularly useful for IRC servers like the one in your project, 
as you can monitor multiple client connections and the server socket simultaneously.