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

#include <gtest/gtest.h>

// pb部分
// #include "ftncomm.pb.h"
// #include "cosv4_client.pb.h"
// #include "cos_trsf.pb.h"
// #include "cos_comm.pb.h"

#include "PbClient.h"

#include "header.h"
//

using namespace std;
extern void yml_PutObjCopyReq(FTN::PutObjCopyReq &PutObjCopyReq_body);
string yml_CosTransferReqMsg_clientip();
#define clientip_ "9.134.186.82"
#define MAGICNUM 0xabcd2012

bool send_nbytes(int sockfd, const char *buffer, int len);
bool recv_nbytes(int sockfd, char *buffer, int len);

PbClient::PbClient()
{
}
void PbClient::set_conn_socket(int socket)
{
	conn_socket = socket;
}
void PbClient::set_acc_socket(int socket)
{
	acc_conn = socket;
}
int PbClient::get_conn_socket()
{
	return conn_socket;
}
int PbClient::get_acc_socket()
{
	return acc_conn;
}
/*
前置条件：构造函数填充ip port
连接服务器 填充conn_socket
*/
bool PbClient::connectServer()
{
	int res;
	struct sockaddr_in server_addr;

	bzero(&server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(atoi(svr_port.c_str()));

	if (inet_pton(AF_INET, svr_ip.c_str(), &server_addr.sin_addr) == 0)
	{
		perror("inet_pton\n");
		return false;
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

FTN::StorageAppMsg PbClient::make_StorageAppMsg(string &acc,unsigned file_size)
{
	FTN::StorageAppMsg *StorageAppMsg = new FTN::StorageAppMsg;
	//最外层：StorageAppMsg 填充seq和body
	//填充Tseq 简单变量
	StorageAppMsg->set_seq(0);
	//嵌套类型这样定义
	FTN::AppMsg *AppMsg = new FTN::AppMsg;
	StorageAppMsg->set_allocated_body(AppMsg);
	AppMsg->set_type(FTN::AppMsg_Type_CosTransferReqMsg);
	FTN::CosTransferReqMsg *CosTransferReqMsg = AppMsg->MutableExtension(FTN::CosTransferReqMsg_body);
	CosTransferReqMsg->set_sub_type(FTN::CosTransferReqMsg_SubType_PutObjCopyReq);
	CosTransferReqMsg->set_clientip(yml_CosTransferReqMsg_clientip());
	FTN::PutObjCopyReq *PutObjCopyReq = new FTN::PutObjCopyReq;
	CosTransferReqMsg->set_allocated_putobjcopyreq_body(PutObjCopyReq);

	yml_PutObjCopyReq(*PutObjCopyReq);
	PutObjCopyReq->set_src_sha("");
	//没有设置长度
	PutObjCopyReq->set_file_size(file_size);
    PutObjCopyReq->set_slice_size(0); 
    PutObjCopyReq->set_file_type(0);   //u32
	PutObjCopyReq->set_src_obj_attr(acc);

	//cout << StorageAppMsg->DebugString() << endl;
	//装配AppMsg
	return (*StorageAppMsg);
}

/*
前置条件 已经封装好StorageAppMsg
打印传进的消息 > 序列化 > 组装header > 发送header > 发送序列化后字符串
并封装header发送
*/
void PbClient::send_StorageAppMsg(const FTN::StorageAppMsg &StorageAppMsg)
{
	cout << StorageAppMsg.DebugString();
	string output;
	//序列化 要改变string 指针
	if (!StorageAppMsg.SerializeToString(&output))
	{
		cout << "序列化失败！";
	}
	unsigned int data_len = output.length();
	struct header myhead;
	myhead.version = htons(1);
	myhead.type = htons(1);
	myhead.reserved = htonl(0);
	myhead.magicnum = htonl(0xabcd2012);
	myhead.length = htonl(HEADER_LEN + data_len);

	printf("len %u magicnum %u %u %u %u", ntohl(myhead.length), ntohl(myhead.magicnum), ntohl(myhead.reserved), ntohs(myhead.type), ntohs(myhead.version));
	write(get_conn_socket(), (char *)&myhead, HEADER_LEN);
	write(get_conn_socket(), output.c_str(), data_len);
}

//返回是否是合法的header 如果是 修改传入的header指针
bool PbClient::recieveHeader(struct header *header)
{
	struct header test_h;

	if (!recv_nbytes(get_conn_socket(), (char *)&test_h, HEADER_LEN))
	{
		return false;
	}

	cout << test_h.length << endl;
	cout << test_h.magicnum << endl;
	(*header) = test_h;
	return true;
}

bool PbClient::recieveStorageAppMsg(struct header header, FTN::StorageAppMsg **retmsg)
{
	int message_len = header.length - HEADER_LEN;
	if (message_len > 0)
	{
		char msg[message_len];
		if (write(get_conn_socket(), msg, message_len) == message_len)
		{
			//反序列化
			string s;
			int i;
			for (i = 0; i < message_len; i++)
			{
				s += msg[i];
			}

			FTN::StorageAppMsg msg_pb;
			if (cout << msg_pb.ParseFromString(s) << endl)
			{
				*retmsg = &msg_pb;
				return true;
			}
		}
	}
	return false;
}

//检查type
//千万不要返回局部对象的引用！千万不要返回指向局部对象的指针！
//千万不要返回局部对象的引用，也不要返回函数内部用new初始化的指针的引用。
FTN::AppMsg_Type get_AppMsg_Type(FTN::AppMsg &AppMsg)
{
	if (AppMsg.has_type())
		return AppMsg.type();
}
//如果type19的话就是回应报文
FTN::AppMsg *get_AppMsg_body(FTN::StorageAppMsg &msg)
{
	return msg.release_body();
}
//检查代码：检查
//AppMsg的extend的optional部分 message CosTransferRspMsg
//get这部分

FTN::CosTransferReqMsg get_CosTransferReqMsg_body(FTN::AppMsg &AppMsg);
FTN::CosTransferReqMsg_SubType get_CosTransferReqMsg_SubType(FTN::CosTransferReqMsg &CosTransferReqMsg);
FTN::PutObjCopyReq get_PutObjCopyReq_body(FTN::CosTransferReqMsg &CosTransferReqMsg);
void deal_PutObjCopyReq(FTN::PutObjCopyReq &PutObjCopyReq);
////////////////////////////////////
/////////////////////////////////////////////////////////
void yml_ObjAttrQueryReq(ClientCos::ObjAttrQueryReq &ObjAttrQueryReq);
//Access部分
/*
前置条件：已经组装好AppMsg
*/
ClientCos::CosAppMsg PbClient::make_CosAppMsg()
{
	ClientCos::CosAppMsg *CosAppMsg = new ClientCos::CosAppMsg;
	CosAppMsg->set_seq(0);
	ClientCos::AppMsg *AppMsg = new ClientCos::AppMsg;
	make_AppMsg_Type(AppMsg, ClientCos::AppMsg_Type_CosReqMsg);
	make_AppMsg_ext_CosReqMsg_body(*AppMsg);
	CosAppMsg->set_allocated_body(AppMsg);
	return (*CosAppMsg);
}
//调用的时候填入类型
void PbClient::make_AppMsg_Type(ClientCos::AppMsg *AppMsg, ClientCos::AppMsg_Type t)
{
	AppMsg->set_type(t);
}
//在调用过程中显示写出new AppMsg：：入口
//AppMsg.EXTXXXX加入extend字段
void PbClient::make_AppMsg_ext_CosReqMsg_body(ClientCos::AppMsg &AppMsg)
{
	ClientCos::CosReqMsg *CosReqMsg_body = AppMsg.MutableExtension(ClientCos::CosReqMsg_body);
	make_CosReqMsg_with_ObjAttrQueryReq(CosReqMsg_body);
}

//将会调用make_ObjAttrQueryReq
//CosReqMsg作为optional字段 需要上级调用allocated返回指针创建
void PbClient::make_CosReqMsg_with_ObjAttrQueryReq(ClientCos::CosReqMsg *CosReqMsg)
{
	CosReqMsg->set_sub_type(ClientCos::CosReqMsg_SubType_ObjAttrQueryReq);
	ClientCos::ObjAttrQueryReq *ObjAttrQueryReq = new ClientCos::ObjAttrQueryReq;
	make_ObjAttrQueryReq(ObjAttrQueryReq);
	CosReqMsg->set_allocated_objattrqueryreq_body(ObjAttrQueryReq);
}
void PbClient::make_ObjAttrQueryReq(ClientCos::ObjAttrQueryReq *ObjAttrQueryReq)
{
	yml_ObjAttrQueryReq(*ObjAttrQueryReq);
}

/*
  前置条件：已经组装AppMsg
  填充seq和body
  */
bool PbClient::send_Access_CosAppMsg(ClientCos::CosAppMsg &CosAppMsg)
{

	cout << CosAppMsg.DebugString() << endl;
	string output;
	//序列化 要改变string 指针
	if (!CosAppMsg.SerializeToString(&output))
	{
		cout << "序列化失败！";
	}
	cout << "字符串" << output << endl;
	unsigned int data_len = output.length();
	struct access_header myhead;

	myhead.magicnum = htons(0x144C);
	myhead.version = 1;
	myhead.type = 1;
	myhead.reserved = htons(0);
	myhead.length = htonl(ACC_LEN + data_len);
	if (!write(get_acc_socket(), (char *)&myhead, ACC_LEN))
		return false;
	if (!write(get_acc_socket(), output.c_str(), data_len))
		return false;
	return true;
}

string PbClient::recv_AccessString()
{
	struct access_header myheader;
	if (!recv_nbytes(get_acc_socket(), (char *)&myheader, ACC_LEN))
	{
		cout << "接收header出错" << endl;
	}
	int message_len = ntohl(myheader.length) - ACC_LEN;
	char *recv = (char *)malloc(message_len + 1);
	recv[message_len] = '\0';
	if (!recv_nbytes(get_acc_socket(), recv, message_len))
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
	cout << msg_pb.DebugString() << endl;
	msg_pb.SerializeToString(&s);

	return s;
}

////////////////////////////////////////////////////////
void PbClient::set_trans_retmsg(FTN::StorageAppMsg &StorageAppMsg)
{
	FTN::AppMsg AppMsg = StorageAppMsg.body();
	assert(AppMsg.HasExtension(FTN::CosTransferRspMsg_body));
	FTN::CosTransferRspMsg CosTransferRspMsg = AppMsg.GetExtension(FTN::CosTransferRspMsg_body);
	trans_retmsg = CosTransferRspMsg.retmsg();
}
string PbClient::get_trans_retmsg()
{
	return trans_retmsg;
}
string PbClient::access()
{
	//已连接acc服务器 填充acc_conn
	string access_str;
	ClientCos::CosAppMsg CosAppMsg = make_CosAppMsg();
	if (send_Access_CosAppMsg(CosAppMsg))
	{
		// cout << "============接收↓==============" << endl;
		//access_str = access->recv_AccessString();
		access_str = recv_AccessString();
		//access_str = "";
		// ClientCos::CosAppMsg unpack;
		// unpack.ParseFromString(access_str);
		// cout << "发动字符串反序列化结果\n"
		// 	 << unpack.DebugString() << endl;
	}
	else
	{
		cout << "send error" << endl;
	}

	return access_str;
}

string PbClient::carry_copy(string &access_str,unsigned file_size)
{
	// cout << "============连接transfer服务器↓==============" << endl;
	//已经连接trans服务器
	//获得conn_socket
	FTN::StorageAppMsg StorageAppMsg = make_StorageAppMsg(access_str,file_size);

	cout << "============向trans sv发送↓==============" << endl;
	send_StorageAppMsg(StorageAppMsg);

	// cout << "============接收↓==============" << endl;

	struct header myheader;
	if (!recv_nbytes(get_conn_socket(), (char *)&myheader, HEADER_LEN))
	{
		cout << "接收header出错" << endl;
	}
	int message_len = ntohl(myheader.length) - HEADER_LEN;
	char *recv = (char *)malloc(message_len);
	if (!recv_nbytes(get_conn_socket(), recv, message_len))
	{
		cout << "接收data出错" << endl;
	}

	//反序列化
	string s;
	int i;
	for (i = 0; i < message_len; i++)
	{
		s += recv[i];
	}

	FTN::StorageAppMsg msg_pb;
	//cout << "============反序列化 打印↓==============" << endl;
	msg_pb.ParseFromString(s);
	//cout << msg_pb.DebugString() << endl;

	set_trans_retmsg(msg_pb);
	free(recv);
	return get_trans_retmsg();
}
