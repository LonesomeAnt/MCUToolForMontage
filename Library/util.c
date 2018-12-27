/*************************************************************************************************************
*                                              INCLUDE FILES                                                 *
*************************************************************************************************************/

/*************************************************************************************************************
*                                            FUNCTION PROTOTYPES                                             *
*************************************************************************************************************/
unsigned short BCD_2_Hex(unsigned short bcd)
{
	unsigned short tmp;
	tmp = (bcd/0x10)*10+bcd%0x10;
	return tmp;
}

unsigned short Hex_2_BCD(unsigned short hex)
{
	unsigned short tmp;
	tmp = (hex/10)*0x10 + hex%10;
	return tmp;
}

unsigned short NToHS (unsigned short data)
{
   return (((data >> 8) & 0x00FF) | ((data << 8) & 0xFF00));
}

unsigned short HToNS (unsigned short data)
{
   return (((data >> 8) & 0x00FF) | ((data << 8) & 0xFF00));
}

unsigned long NToHL (unsigned long data)
{
   return (((data >> 24) & 0x000000FF) | ((data << 24) & 0xFF000000) 
        | ((data >> 8) & 0x0000FF00) | ((data << 8) & 0x00FF0000));
}

unsigned long HToNL (unsigned long data)
{
   return (((data >> 24) & 0x000000FF) | ((data << 24) & 0xFF000000) 
        | ((data >> 8) & 0x0000FF00) | ((data << 8) & 0x00FF0000));
}

/*************************************************************************************************************
*                                               MODULE END                                                   *
*************************************************************************************************************/
