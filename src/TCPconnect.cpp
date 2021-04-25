#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
// #include <netdb.h>
// #include <fstream>
// #include <iostream>

// #include <sstream>
 #include <errno.h>

 #include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "TCPconnect.h"


	TCPconnect::TCPconnect(string s_ip,string s_port){
        ip = s_ip;
	    port = s_port;
	    connectServer();
    }
   TCPconnect::~TCPconnect(){
       close(conn_socket);
   }
    bool TCPconnect::connectServer(){
	int res;
	struct sockaddr_in server_addr;

	bzero(&server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(atoi(port.c_str()));

	if (inet_pton(AF_INET, ip.c_str(), &server_addr.sin_addr) == 0)
	{
		perror("inet_pton\n");
		exit(1);
	}
	conn_socket = socket(AF_INET, SOCK_STREAM, 0);

conn:
	res = connect(conn_socket, (struct sockaddr *)&server_addr, sizeof(server_addr));
	if (res == 0)
	{
		// TODO 连接成功，按题设要求打印服务器端地址server[ip:port]3
		char ip[100];
		inet_ntop(AF_INET, &server_addr.sin_addr.s_addr, ip, sizeof(ip));
		int port = ntohs(server_addr.sin_port);
		printf("已连接！\n", ip, port);
	}
	else if (res == -1 && errno == EINTR)
	{
		goto conn; // 若connect因系统信号中断而失败，则再次执行connect；
	}
	return true;
    }

    int TCPconnect::get_conn_socket()
{
	return conn_socket;
}