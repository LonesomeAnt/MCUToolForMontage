/*************************************************************************************************************
*                                              INCLUDE FILES                                                 *
*************************************************************************************************************/
#include <util.h>
#include <crc.h>

#include <socket.h>


/*************************************************************************************************************
*                                              LOCAL TABLES                                                  *
*************************************************************************************************************/
static sPortListTypeDef PortList[SOCKET_PORT_MAX_COUNT];
static unsigned long SocketPipeMask;


/*************************************************************************************************************
*                                            FUNCTION PROTOTYPES                                             *
*************************************************************************************************************/
void SocketInit (void)
{
    SocketPipeMask = SOCKET_PIPE_MASK_DEFAULT;
    
    for (unsigned int i = 0; i < SOCKET_PORT_MAX_COUNT; i++)
    {
        PortList[i].port = SOCKET_PORT_DEFAULT;
        PortList[i].pipe = SOCKET_PIPE_NULL;
        PortList[i].fnct = SocketHandlerDefault;
    }
}

unsigned char SocketPortOpen (unsigned short port, FNCT_SOCKET fnct, sSocketOpenPipeTypeDef pipe)
{
    for (unsigned int i = 0; i < SOCKET_PORT_MAX_COUNT; i++)
    {
        if (PortList[i].port == port)
        {
            return SOCKET_ERROR_PORT_EXIST;
        }
    }
    
    for (unsigned int i = 0; i < SOCKET_PORT_MAX_COUNT; i++)
    {
        if (PortList[i].port == SOCKET_PORT_DEFAULT)
        {
            PortList[i].port = port;
            PortList[i].pipe = pipe;
            PortList[i].fnct = fnct;
            
            return SOCKET_OK;
        }
    }
    
    return SOCKET_ERROR_FULL;
}

unsigned char SocketPortClose (unsigned short port)
{
    for (unsigned int i = 0; i < SOCKET_PORT_MAX_COUNT; i++)
    {
        if (PortList[i].port == port)
        {
            PortList[i].port = SOCKET_PORT_DEFAULT;
            PortList[i].pipe = SOCKET_PIPE_NULL;
            PortList[i].fnct = SocketHandlerDefault;
            
            return SOCKET_OK;
        }
    }
    
    return SOCKET_ERROR_PORT_EXIST;
}

unsigned char SocketPipeRegister (unsigned char *pipe)
{ 
    unsigned char i;
    
    for (i = 0; i < SOCKET_PIPE_NUM_MAX + 1; i++)
    {
        if (!((1 << i) & SocketPipeMask))
        {
            SocketPipeMask |= (1 << i);
            *pipe = i;
            return SOCKET_OK;
        }
    }
    
    *pipe = SOCKET_PIPE_NUM_NULL;
    
    return SOCKET_ERROR_PIPE_FULL;
}

unsigned char SocketPipeUnregister (unsigned char pipe)
{
    if (pipe > SOCKET_PIPE_NUM_MAX)
    {
        return SOCKET_ERROR_PIPE_OVER_RANGE;
    }
    else
    {
        SocketPipeMask &= ~(1 << pipe);
        return SOCKET_OK;
    }
}

unsigned char SocketPortBind (unsigned short port, unsigned char pipe)
{ 
    if (pipe > SOCKET_PIPE_NUM_MAX)
    {
        return SOCKET_ERROR_PIPE_OVER_RANGE;
    }
    
    for (unsigned int i = 0; i < SOCKET_PORT_MAX_COUNT; i++)
    {
        if (PortList[i].port == port)
        {
            PortList[i].pipe |= (1 << pipe);
            
            return SOCKET_OK;
        }
    }
    
    return SOCKET_ERROR_PORT_EXIST;
}

unsigned char SocketPortUnbind (unsigned short port, unsigned char pipe)
{ 
    if (pipe > SOCKET_PIPE_NUM_MAX)
    {
        return SOCKET_ERROR_PIPE_OVER_RANGE;
    }
    
    for (unsigned int i = 0; i < SOCKET_PORT_MAX_COUNT; i++)
    {
        if (PortList[i].port == port)
        {
            PortList[i].pipe &= ~(1 << pipe);
            
            return SOCKET_OK;
        }
    }
    
    return SOCKET_ERROR_PORT_EXIST;
}

unsigned char SocketPipeCheck (unsigned short port, unsigned char pipe)
{ 
    if (pipe > SOCKET_PIPE_NUM_FULL)
    {
        return 0;
    }
    
    if (pipe == SOCKET_PIPE_NUM_FULL)
    {
        return 1;
    }
    
    for (unsigned int i = 0; i < SOCKET_PORT_MAX_COUNT; i++)
    {
        if ((PortList[i].port == port) && (PortList[i].pipe & (1 << pipe)))
        {
            return 1;
        }
    }
    
    return 0;
}

void SocketPackageInit (sSocketPackageTypeDef *package)
{
    package->flag = 0;
    package->count = 0;
}

void SocketPackageReceive (sSocketPackageTypeDef *package, unsigned char *pdata, unsigned int len, 
                           unsigned int size, unsigned char pipe, FNCT_COMM comm)
{
    sSocketHeadTypeDef *head;
    
    head = (sSocketHeadTypeDef *)package->data;
    
    for (unsigned int i = 0; i < len; i++)
    {
        if ((pdata[i] == SOCKET_LEAD_BYTE) && (package->flag == 0))
        {
            package->flag = 1;
            package->count = 0;
        }
            
        if (package->flag == 1)
        {
            package->data[package->count] = pdata[i];
            
            if (package->count == SOCKET_LEAD_COUNT)
            {
                if (HToNL(head->leadcode) != SOCKET_LEAD)
                {
                    package->flag = 0;
                }
            }                 
                                  
            if (package->count == SOCKET_LEN_COUNT)
            {
                if (NToHS(head->crc) != GetCRC16(&package->data[SOCKET_CRC_POS], 
                                                 SOCKET_CRC_COUNT, 
                                                 SOCKET_CRC16_INIT_VALUE))
                {
                    package->flag = 0;
                }
                
                if (SocketPipeCheck(NToHS(head->port), pipe) == 0)
                {
                    package->flag = 0;
                }
            } 
                
            if (package->count == NToHS(head->len) - 1 + SOCKET_HEAD_SIZE)
            {
                SocketReceiveProcess(NToHS(head->port), 
                                     (unsigned char *)(&head->data), 
                                     NToHS(head->len), comm);
                
                package->flag = 0;
            }
            
            if (package->count > size)
            {
                package->flag = 0;
            }
            else
            {
                package->count++;
            }
        }
    }
}

void SocketReceiveProcess (unsigned short port, unsigned char *pdata, unsigned int len, FNCT_COMM comm)
{
    FNCT_SOCKET fnct;
    
    for (unsigned int i = 0; i < SOCKET_PORT_MAX_COUNT; i++)
    {
        if (PortList[i].port == port)
        {
            fnct = PortList[i].fnct;
            
            fnct(pdata, len, comm);
        }
    }
}

void SocketPackageSend (unsigned short port, unsigned char *pdata, unsigned int len)
{
    sSocketHeadTypeDef *head;
    
    head = (sSocketHeadTypeDef *)pdata;
    
    head->leadcode = SOCKET_LEAD;
    head->port = NToHS(port);
    head->len = NToHS(len);
    head->crc = NToHS(GetCRC16(&pdata[SOCKET_CRC_POS], SOCKET_CRC_COUNT, SOCKET_CRC16_INIT_VALUE));
}

void SocketHandlerDefault (unsigned char *pdata, unsigned int len, FNCT_COMM comm)
{
    while(1);
}


/*************************************************************************************************************
*                                               MODULE END                                                   *
*************************************************************************************************************/
