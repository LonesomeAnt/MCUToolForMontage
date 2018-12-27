/*************************************************************************************************************
*                                              INCLUDE FILES                                                 *
*************************************************************************************************************/

/*************************************************************************************************************
*                                                CONSTANTS                                                   *
*************************************************************************************************************/
const unsigned short CRC16Table[16] =
{
    0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50A5, 0x60C6, 0x70E7,
    0x8108, 0x9129, 0xA14A, 0xB16B, 0xC18C, 0xD1AD, 0xE1CE, 0xF1EF
};

const unsigned char CRC8Table[16] =
{
    0,    94, 188, 226,  97,  63, 221, 131, 
    194, 156, 126,  32, 163, 253,  31,  65
};


/*************************************************************************************************************
*                                            FUNCTION PROTOTYPES                                             *
*************************************************************************************************************/
unsigned short GetCRC16 (const unsigned char *pdata, unsigned short len, unsigned short init)
{
    unsigned short val;
    unsigned char tmp;
	unsigned char data;
    unsigned short i;
        
    val = init;
        
    for (i = 0; i < len; i++)
    {
        tmp = val >> 12; 
        val <<= 4;
		data = pdata[i];
        val ^= CRC16Table[tmp ^ (data >> 4)]; 
                
        tmp = val >> 12; 
        val <<= 4; 
        val ^= CRC16Table[tmp ^ (data & 0x0F)]; 
    }
     
    return val;
}

unsigned char GetCRC8 (const unsigned char *pdata, unsigned char len) 
{
    unsigned char crc;
    unsigned char da;
    
    crc = 0;
    while(len-- != 0)
    {
        da = crc >> 4; 
        crc <<= 4; 
        crc ^= CRC8Table[da ^ (*pdata / 16)]; 
                          
        da = crc >> 4; 
        crc <<= 4; 
        crc ^= CRC8Table[da ^ (*pdata & 0x0f)]; 
        pdata++;
    }
    
    return  crc;
}

/*************************************************************************************************************
*                                               MODULE END                                                   *
*************************************************************************************************************/
