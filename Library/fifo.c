/*************************************************************************************************************
*                                              INCLUDE FILES                                                 *
*************************************************************************************************************/
#include <fifo.h>

/*************************************************************************************************************
*                                            FUNCTION PROTOTYPES                                             *
*************************************************************************************************************/
void FIFOInit (FIFODataTypeDef *pfifo)
{
    pfifo->info.front = 0;
    pfifo->info.rear = 0;
    pfifo->info.flag = FIFO_STATUS_EMPTY;
}

unsigned char FIFOIn (FIFODataTypeDef *pfifo, unsigned char *byte, unsigned long size)
{   
    unsigned char *p;
    
    if (pfifo->info.flag == FIFO_STATUS_FULL)
    {
        return FIFO_ERROR_FULL;
    }
    
    p = (unsigned char *)((unsigned long)(&pfifo->pdata) + pfifo->info.rear);

    *p = *byte;
    
    pfifo->info.rear = (pfifo->info.rear + 1) % size;
    
    if (pfifo->info.rear == pfifo->info.front)
    {
        pfifo->info.flag = FIFO_STATUS_FULL;
    }
    else
    {
        pfifo->info.flag = FIFO_STATUS_OTHER;
    }
    
    return FIFO_OK;
}

unsigned char FIFOOut (FIFODataTypeDef *pfifo, unsigned char *byte, unsigned long size)
{
    unsigned char *p;
    
    if (pfifo->info.flag == FIFO_STATUS_EMPTY)
    {
        return FIFO_ERROR_EMPTY;
    }

    p = (unsigned char *)((unsigned long)(&pfifo->pdata) + pfifo->info.front);
    
    *byte = *p;
    
    pfifo->info.front = (pfifo->info.front + 1) % size;
    
    if (pfifo->info.rear == pfifo->info.front)
    {
        pfifo->info.flag = FIFO_STATUS_EMPTY;
    }
    else
    {
        pfifo->info.flag = FIFO_STATUS_OTHER;
    }
    
    return FIFO_OK;
}

unsigned char FIFOIsEmpty (FIFODataTypeDef *pfifo)
{
    if (pfifo->info.flag == FIFO_STATUS_EMPTY)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

unsigned char FIFOIsFull (FIFODataTypeDef *pfifo, unsigned long size)
{
    if (pfifo->info.flag == FIFO_STATUS_FULL)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

unsigned long GetFIFOCount (FIFODataTypeDef *pfifo, unsigned long size)
{
    unsigned long count;
    
    if (pfifo->info.rear < pfifo->info.front)
    {
        count = pfifo->info.rear + size - pfifo->info.front;
    }
    else if (pfifo->info.rear == pfifo->info.front)
    {
        if (pfifo->info.flag == FIFO_STATUS_FULL)
        {
            count = size;
        }
        else
        {
            count = 0;
        }
    }
    else
    {
        count = pfifo->info.rear - pfifo->info.front;
    }
     
    return count;
}

/*************************************************************************************************************
*                                                  END FILE                                                  *
*************************************************************************************************************/
