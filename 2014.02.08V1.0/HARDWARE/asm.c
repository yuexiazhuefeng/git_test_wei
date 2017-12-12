/******************************************************************************************
*  File Name                             :asm.c
*  Copyright                             :2014.10 wei
*  Module Name                           :asm
*
*  CPU                                   :STM32F103RE
*  RTOS                                  :NULL
*
*  Create Date                           :2014/10/29
*  Author                                :wei
*  Abstract Dscription                   :
*
*
*----------------------Revision History-----------------------------
*  No   Version   Date        Revised By          Item           Description
*
*
*******************************************************************************/

#include "asm.h"

__asm void SystemReset(void)  
{  
  MOV R0, #1           //;   
  MSR FAULTMASK, R0    //; ??FAULTMASK ????????
  
 LDR R0, =0xE000ED0C  //;  
 LDR R1, =0x05FA0004  //;   
 STR R1, [R0]         //; ??????
     
deadloop      
 B deadloop        //; ???????????????
  
}//请求一次软件复位，





