/**
 * http client
 * 
 * tomken
**/

#include "HttpClient.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <netdb.h>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <errno.h>
using namespace std;

#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>


#define SA struct sockaddr
#define MAXLINE 100
string yml_upload_dstpath();
string yml_upload_storage_type();

HttpClient::HttpClient(int conn)
{
	conn_socket = conn;
}

HttpClient::~HttpClient()
{
}

int HttpClient::get_conn_socket()
{
	return conn_socket;
}
int HttpClient::cos_post_simpleupload(string file_name)
{
	int contentLen = 0;
	ifstream myFile;

	string fileContents = "";
	fileContents.clear();
	char c;
	myFile.open(file_name.c_str(), ios_base::in | ios_base::binary);
	if (myFile.is_open())
	{
		while (myFile.good())
		{
			myFile.read((char *)&c, 1);
			fileContents += c;
			contentLen++;
		}
		myFile.close();
	}

	string dstpath = yml_upload_dstpath();
	string storage_type = yml_upload_storage_type();

	std::stringstream header;
	header.clear();
	header.str("");

	header << "POST " << dstpath;
	header << "?Simpleupload HTTP/1.1\r\n";
	header << "x-cos-storage-class: " << storage_type << "\r\n";
	header << "X-TENCENT-UA: "
		   << "same_origin_costransfer"
		   << "\r\n";
	header << "Content-Length:" << contentLen << "\r\n";
	header << "\r\n";

	string header_str = header.str();

	//	cout << "查看header";
	cout << header.str();
	//	cout << "查看contents";
	cout << fileContents;

	write(conn_socket, header_str.c_str(), header_str.length());
	write(conn_socket, fileContents.c_str(), contentLen);

	readReceiver();
	//	close(mSocket);
	return 0;
}

int HttpClient::cos_post_simpledownload()
{
	string get_msg = "";
	get_msg += "GET /just_test.txt HTTP/1.1";
	get_msg += "\r\n";
	get_msg += "Host:paumk-12332101.qq.com";
	get_msg += "\r\n";
	get_msg += "X-TENCENT-UA: cross_origin_costransfer";
	get_msg += "\r\n";
	get_msg += "\r\n";

	write(conn_socket, get_msg.c_str(), get_msg.length());
	return 0;
}

int HttpClient::get_stat()
{
	return stat;
}

void HttpClient::readReceiver()
{
	ssize_t n;
	char recvline[MAXLINE + 1];
	FILE *fp = fopen("../file/upload_rsp", "w");
	bool get_stat = false;
	while ((n = read(conn_socket, recvline, MAXLINE)) > 0)
	{
		if (!get_stat)
		{
			string state = string(recvline + 9, 3);
			stat = atoi(state.c_str());
			get_stat = true;
		}

		recvline[n] = '\0';
		fwrite(recvline, sizeof(char), n, fp);
	//	cout << recvline;
		//cout << n;
	}
	fclose(fp);
}
char *readline(FILE *f)
{
	char *line = (char *)calloc(1, sizeof(char));
	;
	char c;
	int len = 0;

	while ((c = fgetc(f)) != EOF && c != '\n')
	{
		line = (char *)realloc(line, sizeof(char) * (len + 2));
		line[len++] = c;
		line[len] = '\0';
	}

	return line;
}
void HttpClient::download_helper()
{
	FILE *fp = NULL;
	fp = fdopen(get_conn_socket(), "r"); /*文件描述符转换为文件指针*/
	if (NULL == fp)
	{
		// perror("fd to fp error.");
	}
	else
	{
		// printf("transform successfully...\n");
	}

	char *req_line;
	char http[5] = "HTTP";
	req_line = readline(fp);
//	cout << req_line << endl;
	if (strncmp(req_line, http, 4) != 0)
	{
		printf("not a http rsp\n");
	}
	char state[3];
	std::strncpy(state, req_line + 9, 3);
	stat = atoi(state);

	char *jsrdln;
	for (;;)
	{
		jsrdln = readline(fp);
		printf("%s\n", jsrdln);
		if (strncmp(jsrdln, "Content-Length: ", 16) == 0)
			break;
	}

	int l_len = std::strlen(jsrdln) - 16;
	char l[l_len];
	strncpy(l, jsrdln + 16, l_len);
	unsigned length = strtoul(l, NULL, 10);
	cout << length << endl;

	//再读掉一个空行；
	readline(fp);

	//
	char content[length];

	fread(content, 1, length, fp);
	//	cout << content <<endl;
	FILE *dld_fp = fopen("../file/download_file", "w");
	fwrite(content, sizeof(char), length, dld_fp);
//	cout << content << endl;
	fclose(dld_fp);
}


// end of file
