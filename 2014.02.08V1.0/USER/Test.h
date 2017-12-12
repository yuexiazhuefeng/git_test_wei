#include "stm32f10x.h"
#include "flash.h"
#include "spi.h"
#include "usart.h"
#include "GSM.h"
#include "upload.h"
#include "sys_struct.h"
#include "OBD.h"


extern u8 test_arry_write[20] ;
extern u8 test_arry_read[20] ;
extern u8 SPI_TEST ;
extern u16 timer2_100ms ;      //¶¨Ê±Æ÷2  100ms¶¨Ê±Æ÷
extern u8 test_OBD_PID_T;
extern void init_arry(void);
extern void TESET_GSM_Send(void);
extern void TEST_Terminal_authentication(void);  //²âÊÔÖÕ¶Ë×¢²á

extern void test_SPI(void);
extern void test_show_led(void);
extern void OBD_PID_Test(void);






