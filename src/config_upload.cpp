#include <yaml-cpp/yaml.h>
#include <string>
#include <iostream>
using namespace std;
std::string yml_upload_dstpath(){
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

    return config["SimpleUpload"]["srcpath"].as<string>();
}
std::string yml_upload_storage_type(){
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

    return config["SimpleUpload"]["storageclass"].as<string>();
}
std::string yml_upload_filename(){
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

    return config["SimpleUpload"]["filename"].as<string>();
}