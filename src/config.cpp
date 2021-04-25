#include <yaml-cpp/yaml.h>
#include <fstream>
#include <iostream>
#include <string>
#include "../pb/cos_trsf.pb.h"
#include "../pb/cosv4_client.pb.h"
#include <sstream>



using namespace std;
string yml_CosTransferReqMsg_clientip(){
       //打开配置文件
    YAML::Node config;
    try
    {
        config = YAML::LoadFile("../etc/config.yml");
    }
    catch (YAML::BadFile &e)
    {
        std::cout << "read error!" << std::endl;
        exit(0);
    }
    return config["CosTransferReqMsg"]["clientip"].as<string>(); 
}
void yml_PutObjCopyReq(FTN::PutObjCopyReq &PutObjCopyReq_body)
{
    //打开配置文件
    YAML::Node config;
    try
    {
        config = YAML::LoadFile("../etc/config.yml");
    }
    catch (YAML::BadFile &e)
    {
        std::cout << "read error!" << std::endl;
        exit(0);
    }

    PutObjCopyReq_body.set_srcpath(config["CosTransferReqMsg"]["PutObjCopyReq_body"]["srcpath"].as<string>());
    PutObjCopyReq_body.set_srcidc(config["CosTransferReqMsg"]["PutObjCopyReq_body"]["srcidc"].as<string>());
    PutObjCopyReq_body.set_storagetype(config["CosTransferReqMsg"]["PutObjCopyReq_body"]["storagetype"].as<string>());
    PutObjCopyReq_body.set_dstpath(config["CosTransferReqMsg"]["PutObjCopyReq_body"]["dstpath"].as<string>());
    PutObjCopyReq_body.set_dstidc(config["CosTransferReqMsg"]["PutObjCopyReq_body"]["dstidc"].as<string>());
}

void yml_ObjAttrQueryReq(ClientCos::ObjAttrQueryReq &ObjAttrQueryReq)
{
    //打开配置文件
    YAML::Node config;
    try
    {
        config = YAML::LoadFile("../etc/config.yml");
    }
    catch (YAML::BadFile &e)
    {
        std::cout << "read error!" << std::endl;
        exit(0);
    }

    ObjAttrQueryReq.set_path(config["ObjAttrQueryReq_body"]["path"].as<string>());
    ObjAttrQueryReq.set_version(config["ObjAttrQueryReq_body"]["version"].as<string>());
}



void yml_ListReq(ClientCos::ListReq *ListReq){
           //打开配置文件
    YAML::Node config;
    try
    {
        config = YAML::LoadFile("../etc/config.yml");
    }
    catch (YAML::BadFile &e)
    {
        std::cout << "read error!" << std::endl;
        exit(0);
    }
    ListReq->set_prefix(config["ListReq"]["prefix"].as<string>());
}

void upload_addr(string &ip,string &port){
     YAML::Node config;
    try
    {
        config = YAML::LoadFile("../etc/config.yml");
    }
    catch (YAML::BadFile &e)
    {
        std::cout << "read error!" << std::endl;
        exit(0);
    }
    ip = config["server_address"]["upload"]["ip"].as<string>();
    port = config["server_address"]["upload"]["port"].as<string>();
}
void download_addr(string &ip,string &port){
         YAML::Node config;
    try
    {
        config = YAML::LoadFile("../etc/config.yml");
    }
    catch (YAML::BadFile &e)
    {
        std::cout << "read error!" << std::endl;
        exit(0);
    }
    ip = config["server_address"]["download"]["ip"].as<string>();
    port = config["server_address"]["download"]["port"].as<string>();
}
void trans_addr(string &ip,string &port){
    YAML::Node config;
    try
    {
        config = YAML::LoadFile("../etc/config.yml");
    }
    catch (YAML::BadFile &e)
    {
        std::cout << "read error!" << std::endl;
        exit(0);
    }
    ip = config["server_address"]["trans"]["ip"].as<string>();
    port = config["server_address"]["trans"]["port"].as<string>();
}
void access_addr(string &ip,string &port){
        YAML::Node config;
    try
    {
        config = YAML::LoadFile("../etc/config.yml");
    }
    catch (YAML::BadFile &e)
    {
        std::cout << "read error!" << std::endl;
        exit(0);
    }
    ip = config["server_address"]["access"]["ip"].as<string>();
    port = config["server_address"]["access"]["port"].as<string>();
}
void list_addr(string &ip,string &port){
        YAML::Node config;
    try
    {
        config = YAML::LoadFile("../etc/config.yml");
    }
    catch (YAML::BadFile &e)
    {
        std::cout << "read error!" << std::endl;
        exit(0);
    }
    ip = config["server_address"]["list"]["ip"].as<string>();
    port = config["server_address"]["list"]["port"].as<string>();
}