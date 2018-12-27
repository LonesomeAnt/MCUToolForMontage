/*************************************************************************************************************
*                                                  MODULE                                                    *
*************************************************************************************************************/
#ifndef UTIL_H
#define UTIL_H

#ifdef __cplusplus
extern "C" {
#endif
    
/*************************************************************************************************************
*                                            FUNCTION STATEMENT                                              *
*************************************************************************************************************/
unsigned short BCD_2_Hex(unsigned short bcd);
unsigned short Hex_2_BCD(unsigned short hex); 
unsigned short NToHS (unsigned short data);
unsigned short HToNS (unsigned short data);
unsigned long NToHL (unsigned long data);
unsigned long HToNL (unsigned long data);

/*************************************************************************************************************
*                                               MODULE END                                                   *
*************************************************************************************************************/
#ifdef __cplusplus
}
#endif

#endif
