#include "header.h"
using namespace std;

#include "../pb/ftncomm.pb.h"
#include "../pb/cosv4_client.pb.h"
#include "../pb/cos_trsf.pb.h"
#include "../pb/cos_comm.pb.h"

class CosAccess{
    public:
    CosAccess();
    ~CosAccess();
    private:
    int conn_socket;
    string access_string;
    string retmsg;
    unsigned file_size;

    public:
    int get_acc_conn();
    string get_access_string();
    void set_acc_conn(int conn);
    void set_access_string(string s);
    ClientCos::CosAppMsg make_acc_msg();
    bool send_acc_msg(ClientCos::CosAppMsg & CosAppMsg);
    ClientCos::CosAppMsg recv_access_msg();
    bool check_access_rsp(ClientCos::CosAppMsg &CosAppMsg);
    void set_retmsg(string s);
    string get_retmsg();

    void set_file_size(unsigned size);
    unsigned get_file_size();

    
};