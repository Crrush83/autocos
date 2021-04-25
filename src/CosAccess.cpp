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

#include "CosAccess.h"
#include "header.h"
using namespace std;
void yml_ObjAttrQueryReq(ClientCos::ObjAttrQueryReq &ObjAttrQueryReq);
bool send_nbytes(int sockfd, const char *buffer, int len);
bool recv_nbytes(int sockfd, char *buffer, int len);
using namespace std;
CosAccess::CosAccess() {}
CosAccess::~CosAccess() {}
int CosAccess::get_acc_conn()
{
    return conn_socket;
}
string CosAccess::get_access_string()
{
    return access_string;
}
void CosAccess::set_acc_conn(int conn)
{
    conn_socket = conn;
}
void CosAccess::set_access_string(string s)
{
    access_string = s;
}
ClientCos::CosAppMsg CosAccess::make_acc_msg()
{
    ClientCos::CosAppMsg *CosAppMsg = new ClientCos::CosAppMsg;
    CosAppMsg->set_seq(0);
    ClientCos::AppMsg *AppMsg = new ClientCos::AppMsg;
    AppMsg->set_type(ClientCos::AppMsg_Type_CosReqMsg);
    ClientCos::CosReqMsg *CosReqMsg = AppMsg->MutableExtension(ClientCos::CosReqMsg_body);
    CosReqMsg->set_sub_type(ClientCos::CosReqMsg_SubType_ObjAttrQueryReq);
    ClientCos::ObjAttrQueryReq *ObjAttrQueryReq = new ClientCos::ObjAttrQueryReq;
    yml_ObjAttrQueryReq(*ObjAttrQueryReq);
    CosReqMsg->set_allocated_objattrqueryreq_body(ObjAttrQueryReq);
    CosAppMsg->set_allocated_body(AppMsg);
    return (*CosAppMsg);
}
bool CosAccess::send_acc_msg(ClientCos::CosAppMsg &CosAppMsg)
{
    //cout << CosAppMsg.DebugString() << endl;
    string output;
    //序列化 要改变string 指针
    if (!CosAppMsg.SerializeToString(&output))
    {
        cout << "序列化失败！";
    }
    ClientCos::CosAppMsg de = CosAppMsg;

    unsigned int data_len = output.length();
    struct access_header myhead;

    myhead.magicnum = htons(0x144C);
    myhead.version = 1;
    myhead.type = 1;
    myhead.reserved = htons(0);
    myhead.length = htonl(ACC_LEN + data_len);
    if (!send_nbytes(get_acc_conn(), (char *)&myhead, ACC_LEN))
        return false;
    if (!send_nbytes(get_acc_conn(), output.c_str(), data_len))
        return false;
    return true;
}

ClientCos::CosAppMsg CosAccess::recv_access_msg()
{
    struct access_header myheader;
    if (!recv_nbytes(get_acc_conn(), (char *)&myheader, ACC_LEN))
    {
        cout << "接收header出错" << endl;
    }
    int message_len = ntohl(myheader.length) - ACC_LEN;
    char *recv = (char *)malloc(message_len + 1);
    recv[message_len] = '\0';
    if (!recv_nbytes(get_acc_conn(), recv, message_len))
    {
        cout << "接收data出错" << endl;
    }
    string s;
    int i;
    for (i = 0; i < message_len; i++)
    {
        s += recv[i];
    }
    access_string = s;
    ClientCos::CosAppMsg msg_pb;
    msg_pb.ParseFromString(s);
    return msg_pb;
}
void CosAccess::set_retmsg(string s)
{
	retmsg = s;
}
string CosAccess::get_retmsg()
{
	return retmsg;
}
    void CosAccess::set_file_size(unsigned size){
        file_size = size;
    }
    unsigned CosAccess::get_file_size(){
        return file_size;
    }
bool CosAccess::check_access_rsp(ClientCos::CosAppMsg &CosAppMsg)
{

	ClientCos::AppMsg AppMsg = CosAppMsg.body();
	assert(AppMsg.HasExtension(ClientCos::CosRspMsg_body));
	ClientCos::CosRspMsg CosRspMsg = AppMsg.GetExtension(ClientCos::CosRspMsg_body);
	set_retmsg(CosRspMsg.retmsg());
    //长度
    assert(CosRspMsg.has_objattrqueryrsp_body());
    ClientCos::ObjAttrQueryRsp *ObjAttrQueryRsp = CosRspMsg.release_objattrqueryrsp_body();
    set_file_size(ObjAttrQueryRsp->file_size());
	if (CosRspMsg.retcode() == 0)
		return true;
	else
		return false;
}


