#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <netdb.h>
#include <errno.h>


#include <sys/types.h>
#include <sys/socket.h>
bool send_nbytes(int sockfd, const char *buffer, int len)
{
	int bytes_write = 0;
	int sent = 0;
	//	printf("write out %d bytes to socket %d\n", len, sockfd);
	while (1)
	{
		bytes_write = send(sockfd, buffer + sent, len - sent, 0);
		if (bytes_write == -1)
		{
			if (errno == EAGAIN)
				continue;
			else
				return false;
		}
		else if (bytes_write == 0)
		{
			continue;
		}
		else
		{
			sent += bytes_write;
		}
		if (sent == len)
			return true;
		else
			continue;
	}
}

bool recv_nbytes(int sockfd, char *buffer, int len)
{
	int bytes_read = 0;
	int read = 0;
	memset(buffer, '\0', len);
	for (;;)
	{
		bytes_read = recv(sockfd, buffer + read, len - read, 0);
		if (bytes_read == -1)
		{
			if (errno == EAGAIN)
				continue;
			else
			{

				//cout << "error -1"<<endl;
				printf("%s\n", strerror(errno));
				return false;
			}
		}
		else if (bytes_read == 0)
		{
			continue;
		}
		else
		{
			read += bytes_read;
			//	printf("read in %d bytes from socket %d with content: %s\n", bytes_read, sockfd, buffer);
		}
		if (read == len)
		{
			return true;
		}
	}
}