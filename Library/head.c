/*************************************************************************************************************
*                                              INCLUDE FILES                                                 *
*************************************************************************************************************/
#include <util.h>
#include <crc.h>

#include <head.h>


/*************************************************************************************************************
*                                              LOCAL TABLES                                                  *
*************************************************************************************************************/



/*************************************************************************************************************
*                                            FUNCTION PROTOTYPES                                             *
*************************************************************************************************************/
void HeadReplayData (unsigned short port, unsigned short type, unsigned char *pdata, 
                     unsigned short len, FNCT_COMM comm)
{
    sHeadTypeDef *head;
    
    head = (sHeadTypeDef *)&pdata[SOCKET_HEAD_SIZE];
    head->len = NToHS(len);
    
    head->type = NToHS(type);
    
    head->crc = NToHS(GetCRC16(&pdata[SOCKET_HEAD_SIZE + HEAD_CRC_POS], 
                               HEAD_HEAD_SIZE - HEAD_CRC_LEN + len, 
                               HEAD_CRC16_INIT_VALUE)); 
    SocketPackageSend(port, pdata, HEAD_HEAD_SIZE + len); 
    comm(pdata, SOCKET_HEAD_SIZE + HEAD_HEAD_SIZE + len);
}


/*************************************************************************************************************
*                                               MODULE END                                                   *
*************************************************************************************************************/
