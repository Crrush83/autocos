
#ifndef HEAD_TEST_H
#define HEAD_TEST_H
#define HEADER_LEN sizeof(struct header)
#define ACC_LEN sizeof(struct access_header)
#pragma  pack(1)
typedef struct header
{
  unsigned int magicnum;
  unsigned short version;
  unsigned short type;
  unsigned int length;
  unsigned int reserved;
} store_com_head_t;
#pragma  pack()
#pragma  pack(1)
typedef struct access_header{
                        unsigned short   magicnum;     
                        unsigned short   reserved;     
                        unsigned                  length;       
                        char version;      
                        char type;         
                } access_head_t;
#pragma  pack()

#endif