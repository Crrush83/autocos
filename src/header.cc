typedef struct header
{
  unsigned int magicnum;
  unsigned short version;
  unsigned short type;
  unsigned int length;
  unsigned int reserved;
} store_com_head_t;

typedef struct access_header{
                        unsigned short   magicnum;     
                        unsigned short   reserved;     
                        unsigned                  length;       
                        char version;      
                        char type;         
                } access_head_t;