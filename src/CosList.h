#ifndef __COSLIST__
#define __COSLIST__
#include <string>
using namespace std;
#include "../pb/cosv4_client.pb.h"
class CosList{
public:
    CosList();
    ~CosList();
private:
    int conn_socket;
    string retmsg;
public:
    ClientCos::CosAppMsg make_list_msg();
    bool send_list_msg(ClientCos::CosAppMsg &CosAppMsg);
    void set_list_conn(int socket);
    int get_list_conn();
    ClientCos::CosAppMsg recv_list_rsp();
    bool check_list_rsp(ClientCos::CosAppMsg &CosAppMsg);
    void set_retmsg(string s);
    string get_retmsg();
};
#endif