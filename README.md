# auto_cos

### 功能

通过配置文件自动选择文件上传、复制、list、下载、删除（暂时未实现）

目的：上线前cos园区的测试

### 文件位置

上传文件：默认在file文件夹下，可在./etc/upload.yml中配置路径

下载文件：默认为file下的download_file，暂时未实现配置

### 编译

首先 编译.proto文件到pb目录下（已编译）。然后，在src目录下执行：

```
g++ -std=c++11 PbClient.cpp HttpClient.cpp config_upload.cpp config.cpp main.cpp CosAccess.cpp iohelper.cpp TCPconnect.cpp CosList.cpp ../pb/cos_comm.pb.cc ../pb/cos_trsf.pb.cc ../pb/cosv4_client.pb.cc ../pb/ftncomm.pb.cc -o ../bin/test_all ../lib/libgtest.a ../lib/libgtest_main.a ../lib/libprotobuf.a ../lib/libyaml-cpp.a -lpthread
```

得到bin目录下的可执行文件test_all

### 运行

bin目录下有已经编译好的可执行文件test_all

| 测试用例                      | 描述       |
| ----------------------------- | ---------- |
| TEST(LIST,LIST)               | 基本的LIST |
| TEST(Upload,SimpleUpload)     | 简单上传   |
| TEST(Trans,Copy)              | 复制       |
| TEST(Download, Download_Save) | 下载并保存 |

**运行全部**

```
./test_all
```

 依次执行上传、复制、下载的所有用例。

**运行部分**

 可用选项

```
--gtest_filter=[用例集].[用例名]
```

运行指定的模块

例如，您可以使用：

```
./test_all --gtest_filter=Upload.SimpleUpload
```

来执行简单上传的测试用例

*匹配一个用例集下的所有用例，例如您可以使用以下命令运行Upload用例集中的所有用例：

```
./test_all --gtest_filter=Upload.*
```

### 代码结构

```
auto_cos_gtest/
|-- bin 【可执行文件】
|   `-- test_all
|-- etc 
|   |-- config.yml 【配置文件】
|-- file
|   |-- download_file 【下载的文件】
|   |-- tesa 【示例：上传的文件】
|   `-- upload_rsp 【上传返回报文】
|-- lib
|   |-- libgtest.a
|   |-- libgtest_main.a
|   |-- libprotobuf.so.25
|   `-- libyaml-cpp.a
|-- pb【结构体定义】
|   |-- cos_comm.pb.cc
|   |-- cos_comm.pb.h
|   |-- cos_comm.proto
|   |-- cos_trsf.pb.cc
|   |-- cos_trsf.pb.h
|   |-- cos_trsf.proto
|   |-- cosv4_client.pb.cc
|   |-- cosv4_client.pb.h
|   |-- cosv4_client.proto
|   |-- ftncomm.pb.cc
|   |-- ftncomm.pb.h
|   `-- ftncomm.proto
`-- src【功能代码】
    |-- CosAccess.cpp
    |-- CosAccess.h
    |-- CosList.cpp
    |-- CosList.h
    |-- HttpClient.cpp
    |-- HttpClient.h
    |-- PbClient.cpp
    |-- PbClient.h
    |-- TCPconnect.cpp
    |-- TCPconnect.h
    |-- config.cpp
    |-- config_upload.cpp
    |-- header.cc
    |-- header.h
    |-- iohelper.cpp
    `-- main.cpp
```
