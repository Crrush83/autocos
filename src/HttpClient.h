/**
 * http client
 * 
 * tomken
**/

#ifndef __HTTP_CLIENT_H__
#define __HTTP_CLIENT_H__

#include <string>
#include <iostream>
#include <sstream>
#include <map>
#include <list>
#include <vector>

#include "../pb/ftncomm.pb.h"
// #include "../pb/cosv4_client.pb.h"
#include "../pb/cos_trsf.pb.h"
#include "../pb/cos_comm.pb.h"

using namespace std;

//class HttpPostChunk;
class HttpClient {
public:
	HttpClient(int conn);
	~HttpClient();

public:
	int cos_post_simpleupload(string file_name);
	int cos_post_simpledownload();
	void download_helper();
	int get_stat();
	int get_conn_socket();
	void readReceiver();
	
	
private:
	int conn_socket;
	int stat;
};

#endif // __HTTP_CLIENT_H__

