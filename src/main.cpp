#include <string>
#include <gtest/gtest.h>
#include "PbClient.h"
#include "HttpClient.h"
#include "TCPconnect.h"
#include "CosList.h"
#include "CosAccess.h"


#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
bool send_nbytes(int sockfd, const char *buffer, int len);
bool recv_nbytes(int sockfd, char *buffer, int len);
string copy(const char* acc_ip,const char* acc_port,const char* trans_ip,const char*trans_port);
string yml_upload_filename();
void trans_addr(string &ip,string&port);
void access_addr(string &ip,string&port);
void upload_addr(string &ip,string&port);
void download_addr(string &ip,string&port);
void list_addr(string &ip,string&port);
int main(int argc, char *argv[])
{
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}

TEST(Upload, SimpleUpload)
{
	string ip;
	string port;
	upload_addr(ip,port);
	//待测试：连接服务器
	TCPconnect *tcp = new TCPconnect(ip,port);
	HttpClient *http = new HttpClient(tcp->get_conn_socket());
	//待测试：上传文件
	string uploadfile = yml_upload_filename();
	http->cos_post_simpleupload(uploadfile);
	EXPECT_EQ(200, http->get_stat());
}

TEST(Trans,Copy){
	//"9.86.161.186", "46000" access
	//"9.27.151.156", "49000" copy

	string ip;
	string port;
	access_addr(ip,port);
	TCPconnect *acc_tcp = new TCPconnect(ip,port);
	////分离Acc版本
	CosAccess *acc = new CosAccess;
	acc->set_acc_conn((acc_tcp->get_conn_socket()));
	ClientCos::CosAppMsg acc_msg = acc->make_acc_msg();
	acc->send_acc_msg(acc_msg);
	ClientCos::CosAppMsg ret = acc->recv_access_msg();
	//cout << "recv acc ret" << endl;
	if(!acc->check_access_rsp(ret)) cout << acc->get_retmsg() << endl;

	string as = acc->get_access_string();

	string ip_;
	string port_;
	trans_addr(ip_,port_);
	TCPconnect *copy_tcp = new TCPconnect(ip_,port_);

	PbClient *copy = new PbClient;
	copy->set_conn_socket(copy_tcp->get_conn_socket());

	EXPECT_EQ("success",copy->carry_copy(as,acc->get_file_size()));
}
TEST(Download, Download_Save)
{
	string ip;
	string port;
	download_addr(ip,port);
	//待测试：连接服务器
	TCPconnect *dldtcp = new TCPconnect(ip,port);
	HttpClient *downloader = new HttpClient(dldtcp->get_conn_socket());

	//待测试：上传文件
	downloader->cos_post_simpledownload();
	downloader->download_helper();
	EXPECT_EQ(200, downloader->get_stat());
}

TEST(List,List){
	string ip;
	string port;
	list_addr(ip,port);
	TCPconnect *list_tcp = new TCPconnect(ip,port);

	CosList *list = new CosList;
	list->set_list_conn(list_tcp->get_conn_socket());

	ClientCos::CosAppMsg CosAppMsg = list->make_list_msg();
	if(!list->send_list_msg(CosAppMsg)) cout << "send error" <<endl;
	ClientCos::CosAppMsg ret = list->recv_list_rsp();
	list->check_list_rsp(ret);
	EXPECT_EQ("success",list->get_retmsg());
}