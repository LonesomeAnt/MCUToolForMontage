/*************************************************************************************************************
*                                                  MODULE                                                    *
*************************************************************************************************************/
#ifndef SOCKET_H
#define SOCKET_H

#ifdef __cplusplus
extern "C" {
#endif

    
/*************************************************************************************************************
*                                                 DEFINES                                                    *
*************************************************************************************************************/ 
#define SOCKET_DATA_MAX_SIZE                    128u
#define SOCKET_HEAD_SIZE                        10u
#define SOCKET_PORT_MAX_COUNT                   10u

#define SOCKET_LEAD                             0xFFFFFFFF
#define SOCKET_LEAD_BYTE                        0xFF
    
#define SOCKET_LEAD_COUNT                       3u
#define SOCKET_LEN_COUNT                        9u

#define SOCKET_CRC_POS                          6u    
#define SOCKET_CRC_COUNT                        4u

#define SOCKET_PORT_DEFAULT                     0xFFFF
    
#define SOCKET_PIPE_NULL                        0x00000000UL
#define SOCKET_PIPE_FULL                        0xFFFFFFFFUL
    
#define SOCKET_PIPE_MASK_DEFAULT                0x00000000UL   
#define SOCKET_PIPE_NUM_MAX                     31u
#define SOCKET_PIPE_NUM_FULL                    32u
#define SOCKET_PIPE_NUM_NULL                    33u
    
#define SOCKET_OK                               0
#define SOCKET_ERROR_FULL                       1
#define SOCKET_ERROR_EMPTY                      2
#define SOCKET_ERROR_PORT_NONE                  3
#define SOCKET_ERROR_PORT_EXIST                 4
#define SOCKET_ERROR_PIPE_FULL                  5
#define SOCKET_ERROR_PIPE_OVER_RANGE            6
    
#define SOCKET_CRC16_INIT_VALUE                 CRC16_INIT_0000H


/*************************************************************************************************************
*                                                  DATA TYPES                                                *
*************************************************************************************************************/
typedef void (*FNCT_COMM)(unsigned char *pdata, unsigned int len); 
typedef void (*FNCT_SOCKET)(unsigned char *pdata, unsigned int len, FNCT_COMM comm);

typedef enum
{
    SOCKET_OPEN_PIPE_NULL = SOCKET_PIPE_NULL, 
    SOCKET_OPEN_PIPE_FULL = SOCKET_PIPE_FULL,
} sSocketOpenPipeTypeDef;

#pragma pack(1)
typedef struct
{
    unsigned long  leadcode;
    unsigned short crc;
    unsigned short port;
    unsigned short len;
    unsigned char  data;
} sSocketHeadTypeDef;

typedef struct
{
    unsigned short flag;
    unsigned short count;
    unsigned char  data[SOCKET_DATA_MAX_SIZE];
} sSocketPackageTypeDef;
#pragma pack() 

typedef struct
{
    unsigned short port;
    unsigned long  pipe;
    FNCT_SOCKET    fnct;
} sPortListTypeDef;


/*************************************************************************************************************
*                                            FUNCTION STATEMENT                                              *
*************************************************************************************************************/
void SocketInit (void);
unsigned char SocketPortOpen (unsigned short port, FNCT_SOCKET fnct, sSocketOpenPipeTypeDef pipe);
unsigned char SocketPortClose (unsigned short port);
unsigned char SocketPipeRegister (unsigned char *pipe);
unsigned char SocketPipeUnregister (unsigned char pipe);
unsigned char SocketPortBind (unsigned short port, unsigned char pipe);
unsigned char SocketPortUnbind (unsigned short port, unsigned char pipe);
unsigned char SocketPipeCheck (unsigned short port, unsigned char pipe);
void SocketPackageInit (sSocketPackageTypeDef *package);
void SocketPackageReceive (sSocketPackageTypeDef *package, unsigned char *pdata, unsigned int len, 
                           unsigned int size, unsigned char pipe, FNCT_COMM comm);
void SocketReceiveProcess (unsigned short port, unsigned char *pdata, unsigned int len, FNCT_COMM comm);
void SocketPackageSend (unsigned short port, unsigned char *pdata, unsigned int len);
void SocketHandlerDefault (unsigned char *pdata, unsigned int len, FNCT_COMM comm);


/*************************************************************************************************************
*                                               MODULE END                                                   *
*************************************************************************************************************/
#ifdef __cplusplus
}
#endif

#endif
