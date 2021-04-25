
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
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>
#include <string>
#include "CosList.h"
#include "../pb/cosv4_client.pb.h"
#include "header.h"

using namespace std;
bool send_nbytes(int sockfd, const char *buffer, int len);
bool recv_nbytes(int sockfd, char *buffer, int len);

void yml_ListReq(ClientCos::ListReq *ListReq);
CosList::CosList() {}
CosList::~CosList() {}

ClientCos::CosAppMsg CosList::make_list_msg()
{
	ClientCos::CosAppMsg *CosAppMsg = new ClientCos::CosAppMsg;
	CosAppMsg->set_seq(0);
	ClientCos::AppMsg *AppMsg = new ClientCos::AppMsg;
	CosAppMsg->set_allocated_body(AppMsg);
	AppMsg->set_type(ClientCos::AppMsg_Type_CosReqMsg);
	ClientCos::CosReqMsg *CosReqMsg = AppMsg->MutableExtension(ClientCos::CosReqMsg_body);
	CosReqMsg->set_sub_type(ClientCos::CosReqMsg_SubType_ListReq);
	ClientCos::ListReq *ListReq = new ClientCos::ListReq;
	yml_ListReq(ListReq);
	//ListReq->set_prefix("/12332101/paumk/");
	ListReq->set_offset(ListReq->offset());
	//  ListReq->set_delimiter(ListReq->delimiter());
	ListReq->set_delimiter(ListReq->delimiter());
	ListReq->set_count(ListReq->count());
	ListReq->set_flag(ListReq->flag());
	CosReqMsg->set_allocated_listreq_body(ListReq);
	return (*CosAppMsg);
}

bool CosList::send_list_msg(ClientCos::CosAppMsg &CosAppMsg)
{
	cout << CosAppMsg.DebugString() << endl;
	string output;
	//序列化 要改变string 指针
	if (!CosAppMsg.SerializeToString(&output))
	{
		cout << "序列化失败！";
	}
	ClientCos::CosAppMsg *test = new ClientCos::CosAppMsg;
	test->ParseFromString(output);
	unsigned int data_len = output.length();
	struct access_header myhead;

	myhead.magicnum = htons(0x144C);
	myhead.version = 1;
	myhead.type = 1;
	myhead.reserved = htons(0);
	myhead.length = htonl(ACC_LEN + data_len);
	if (!write(get_list_conn(), (char *)&myhead, ACC_LEN))
		return false;
	if (!write(get_list_conn(), output.c_str(), data_len))
		return false;
	return true;
}

ClientCos::CosAppMsg CosList::recv_list_rsp()
{
	struct access_header myheader;
	if (!recv_nbytes(get_list_conn(), (char *)&myheader, ACC_LEN))
	{
		cout << "接收header出错" << endl;
	}
	int message_len = ntohl(myheader.length) - ACC_LEN;
	char *recv = (char *)malloc(message_len + 1);
	recv[message_len] = '\0';
	if (!recv_nbytes(get_list_conn(), recv, message_len))
	{
		cout << "接收data出错" << endl;
	}
	string s;
	int i;
	for (i = 0; i < message_len; i++)
	{
		s += recv[i];
	}

	ClientCos::CosAppMsg msg_pb;
	msg_pb.ParseFromString(s);
	msg_pb.set_seq(0);
	cout << "接收内容：" << msg_pb.DebugString() << endl;
	;
	return msg_pb;
}

bool CosList::check_list_rsp(ClientCos::CosAppMsg &CosAppMsg)
{

	ClientCos::AppMsg AppMsg = CosAppMsg.body();
	assert(AppMsg.HasExtension(ClientCos::CosRspMsg_body));
	ClientCos::CosRspMsg CosRspMsg = AppMsg.GetExtension(ClientCos::CosRspMsg_body);
	set_retmsg(CosRspMsg.retmsg());
	if (CosRspMsg.retcode() == 0)
		return true;
	else
		return false;
}
void CosList::set_retmsg(string s)
{
	retmsg = s;
}
string CosList::get_retmsg()
{
	return retmsg;
}
void CosList::set_list_conn(int socket)
{
	conn_socket = socket;
}
int CosList::get_list_conn()
{
	return conn_socket;
}
