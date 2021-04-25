using namespace std;
 #include <string>
class TCPconnect{
public:
	TCPconnect(string ip,string port);
    ~TCPconnect();
    bool connectServer();
    int get_conn_socket();

private:
    int conn_socket;
    string ip;
    string port;
    int stat;
    bool has_set_stat;
};