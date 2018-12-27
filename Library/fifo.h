/*************************************************************************************************************
*                                                  MODULE                                                    *
*************************************************************************************************************/
#ifndef FIFO_H
#define FIFO_H

#ifdef __cplusplus
extern "C" {
#endif
    
/*************************************************************************************************************
*                                                 DEFINES                                                    *
*************************************************************************************************************/ 
#define	FIFO_OK                                 0
#define	FIFO_ERROR_FULL                         1
#define	FIFO_ERROR_EMPTY                        2
#define	FIFO_ERROR_COUNT                        3

#define	FIFO_INFO_SIZE                          12
    
#define	FIFO_STATUS_FULL                        1
#define	FIFO_STATUS_EMPTY                       2
#define	FIFO_STATUS_OTHER                       3
    
/*************************************************************************************************************
*                                                  DATA TYPES                                                *
*************************************************************************************************************/
#pragma pack(1)  
typedef struct
{
    unsigned long front;
    unsigned long rear; 
    unsigned long flag;    
} FIFOInfoTypeDef;  

typedef struct
{
    FIFOInfoTypeDef info;
    unsigned char pdata;     
} FIFODataTypeDef;
#pragma pack()  

/*************************************************************************************************************
*                                            FUNCTION STATEMENT                                              *
*************************************************************************************************************/  
void FIFOInit (FIFODataTypeDef *pfifo);
unsigned char FIFOIn (FIFODataTypeDef *pfifo, unsigned char *byte, unsigned long size);
unsigned char FIFOOut (FIFODataTypeDef *pfifo, unsigned char *byte, unsigned long size);
unsigned char FIFOIsEmpty (FIFODataTypeDef *pfifo);
unsigned char FIFOIsFull (FIFODataTypeDef *pfifo, unsigned long size);
unsigned long GetFIFOCount (FIFODataTypeDef *pfifo, unsigned long size);

/*************************************************************************************************************
*                                               MODULE END                                                   *
*************************************************************************************************************/ 
#ifdef __cplusplus
}
#endif

#endif
