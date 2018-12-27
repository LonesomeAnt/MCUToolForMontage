/*************************************************************************************************************
*                                              INCLUDE FILES                                                 *
*************************************************************************************************************/
#include <string.h>

#include <esc.h>

/*************************************************************************************************************
*                                             LOCAL VARIABLES                                                *
*************************************************************************************************************/
static unsigned char ESCBuff[ESC_DATA_BUFF_SIZE];

/*************************************************************************************************************
*                                            FUNCTION PROTOTYPES                                             *
*************************************************************************************************************/
unsigned char ESC_Receive (unsigned char *pSrc, unsigned short iSrcLen, unsigned short *pDstLen)
{
	unsigned short i,j;
    unsigned char flag;

    i = 0;
    flag = 0;
    memcpy(ESCBuff, pSrc, iSrcLen);
	
	for (j = 0; j < iSrcLen; j++)
	{
        if ((ESCBuff[j] == JTT808_MSGFLAG) && (ESCBuff[j + 1] == JTT808_MSGFLAG) && ((j + 1) < iSrcLen))
		{
            flag = 1;
			break;
		}
		if (ESCBuff[j] == JTT808_MSGCONVERT)
		{
			j++;
			if (ESCBuff[j] == 2)
            {
				pSrc[i++] = JTT808_MSGFLAG;
            }
			else if (pSrc[j] == 1)
            {
				pSrc[i++] = JTT808_MSGCONVERT;
            }
		}
		else
        {
			pSrc[i++] = ESCBuff[j];
        }
	}
    
	*pDstLen = i;
    
    if (flag == 1)
    {
        return 0;
    }
    
	return 1;
}

unsigned short ESC_Send (unsigned char *pSrc, unsigned short iSrcLen)
{
	unsigned short i, iLenOfESC, iESC, Index;
    unsigned char d;
    
	Index = iSrcLen;
	iLenOfESC = Index;
	for (i = 1; i < Index; i++)
	{
		if ((pSrc[i] == JTT808_MSGFLAG) || (pSrc[i] == JTT808_MSGCONVERT))
        {
			iLenOfESC++;
        }
	}
	if( iLenOfESC != Index)
	{
		iESC = iLenOfESC - 1;
		for (i = Index - 1; i >= 1; i--)
		{
			d = pSrc[i];
			if (d == JTT808_MSGFLAG)
			{
				pSrc[iESC--] = 2;
				pSrc[iESC--] = JTT808_MSGCONVERT;
			}
			else if (d == JTT808_MSGCONVERT)
			{
				pSrc[iESC--] = 1;
				pSrc[iESC--] = JTT808_MSGCONVERT;
			}
			else
            {
				pSrc[iESC--] = d;
            }
		}
	}
	return iLenOfESC;
}

/*************************************************************************************************************
*                                               MODULE END                                                   *
*************************************************************************************************************/
