
#include "header.h"
#include <string>
using namespace std;
#include "../pb/ftncomm.pb.h"
#include "../pb/cosv4_client.pb.h"
#include "../pb/cos_trsf.pb.h"
#include "../pb/cos_comm.pb.h"

//#include "header.h"
class PbClient
{
public:
  // PbClient();
  PbClient();
  ~PbClient();

public:
  //一些获取私有变量的方法
  void set_conn_socket(int socket);
  void set_acc_socket(int socket);
  int get_conn_socket();
  int get_acc_socket();
  //  void yml_PutObjCopyReq(FTN::PutObjCopyReq &PutObjCopyReq_body);
  void send_StorageAppMsg(const FTN::StorageAppMsg &StorageAppMsg);
  //  FTN::StorageAppMsg make_StorageAppMsg(FTN::AppMsg &AppMsg);
  //
  FTN::StorageAppMsg make_StorageAppMsg(string &access_str,unsigned file_size);

  //返回是否是合法的header 如果是 修改传入的header指针
  bool recieveHeader(store_com_head_t *header);
  bool recieveStorageAppMsg(store_com_head_t header, FTN::StorageAppMsg **retmsg);

  //检查type
  //千万不要返回局部对象的引用！千万不要返回指向局部对象的指针！
  //千万不要返回局部对象的引用，也不要返回函数内部用new初始化的指针的引用。
  //不是吧 这些函数有必要再封装一遍吗？！没有必要的！！
  FTN::AppMsg *get_AppMsg_body(FTN::StorageAppMsg &msg);
  FTN::AppMsg_Type get_AppMsg_Type(FTN::AppMsg &AppMsg);
  FTN::CosTransferReqMsg get_CosTransferReqMsg_body(FTN::AppMsg &AppMsg);
  FTN::CosTransferReqMsg_SubType get_CosTransferReqMsg_SubType(FTN::CosTransferReqMsg &CosTransferReqMsg);
  FTN::PutObjCopyReq get_PutObjCopyReq_body(FTN::CosTransferReqMsg &CosTransferReqMsg);
  void deal_PutObjCopyReq(FTN::PutObjCopyReq &PutObjCopyReq);
  string carry_copy(string &acc_str,unsigned file_size);
  bool connectServer();

  //Access部分 CosAppMsg
public:
  ClientCos::CosAppMsg make_CosAppMsg();
  void make_AppMsg_Type(ClientCos::AppMsg *AppMsg, ClientCos::AppMsg_Type t);
  //AppMsg.EXTXXXX加入extend字段
  void make_AppMsg_ext_CosReqMsg_body(ClientCos::AppMsg &AppMsg);

  void make_CosReqMsg_SubType();
  //将会调用make_ObjAttrQueryReq
  //CosReqMsg作为optional字段 需要上级调用allocated返回指针创建
  void make_CosReqMsg_with_ObjAttrQueryReq(ClientCos::CosReqMsg *CosReqMsg);
  void make_ObjAttrQueryReq(ClientCos::ObjAttrQueryReq *ObjAttrQueryReq);

  /*
  前置条件：已经组装AppMsg
  填充seq和body
  */
  bool send_Access_CosAppMsg(ClientCos::CosAppMsg &CosAppMsg);
  string recv_AccessString();
  string get_trans_retmsg();
  void set_trans_retmsg(FTN::StorageAppMsg &StorageAppMsg);
  string access();

private:
  bool closeSocket;

private:
  bool isConnect;
  string svr_ip;
  string svr_port;
  int conn_socket;
  int acc_conn;
  string trans_retmsg;
};
