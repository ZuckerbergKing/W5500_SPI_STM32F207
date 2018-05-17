/**
******************************************************************************
* @file    			main.c                                              
* @version 			V1.0.0_beta_20180515										  		
* @date    			2018-5-17								 
* @brief 				��3.5.0�汾�⽨�Ĺ���ģ��						  	
* @attention		�������ԣ��뱣֤W5500��IP�����PC����ͬһ�����ڣ��Ҳ���ͻ         
*								����������߸�PC��ֱ����������PC���������ӵ�ַIPΪ��̬IP
*				
* @company  		�Ϻ��汦���ܿƼ����޹�˾	����������Ƽ���ֲ�޸��޸ģ�
* @information  
* @website  		www.qbs-sh.com																							 	 				
* @email				zhanbo.wang@qbs-sh.com	       																							 																											 
******************************************************************************
*/

/*W5500��STM32�� SPI1Ӳ�����߶���*/
//	W5500_SCS    --->		STM32_GPIOA1								 /*W5500��Ƭѡ�ܽ�*/
//	W5500_SCLK	 --->	  STM32_GPIOA5								 /*W5500��ʱ�ӹܽ�*/
//  W5500_MISO	 --->		STM32_GPIOA6								 /*W5500��MISO�ܽ�*/
//	W5500_MOSI	 --->		STM32_GPIOA7								 /*W5500��MOSI�ܽ�*/
//	W5500_RESET	 --->   STM32_GPIOA2								 /*W5500��RESET�ܽ�*/
//	W5500 INT 	 --->	  NONE												 /*��ʹ���жϴ���*/
#include <stdio.h>
#include <string.h>
#include "stm32f2xx.h"
#include "bsp_usart1.h"
#include "bsp_i2c_ee.h"
#include "bsp_i2c_gpio.h"
#include "bsp_TiMbase.h"
#include "w5500.h"
#include "W5500_conf.h"
#include "socket.h"
#include "utility.h"
#include "tcp_demo.h"
uint8 ace[9] = {0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39};  //used for test
void delay(int t);
int main(void)
{ 	 
  uint16 ms_cnt = 0;
  SystemInit();									//SystemCoreClock  init
  systick_init(72);				        //systick_init 
  TIM6_Configuration();
  USART1_Config(); 				           /*��ʼ������ͨ��:115200@8-n-1*/
  delay(50);
	
  gpio_for_w5500_config();					/*��ʼ��MCU�������*/
  reset_w5500();							/*Ӳ��λW5500*/
  PHY_check();								/*��������Ƿ����*/   
  set_w5500_mac();							/*����MAC��ַ*/
  set_w5500_ip();							/*����IP��ַ*/
  socket_buf_init(txsize, rxsize);			/*��ʼ��8��Socket�ķ��ͽ��ջ����С*/

	printf(" ������ΪTCP������,��W5500��Ϊ TCP�ͻ���ȥ���� \r\n");
	printf(" ������IP:%d.%d.%d.%d\r\n",remote_ip[0],remote_ip[1],remote_ip[2],remote_ip[3]);
	printf(" �����˿�:%d \r\n",remote_port);
	printf(" ���ӳɹ��󣬷������������ݸ�W5500��W5500�����ض�Ӧ���� \r\n");
	printf(" Ӧ�ó���ִ����\r\n"); 
	
  while(1)                							/*ѭ��ִ�еĺ���*/ 
  {
		do_tcp_client();                    /*TCP_Client ���ݻػ����Գ���*/
		if(bl_main_loop_F == TRUE)
		{
			bl_main_loop_F = FALSE;
			if(++ms_cnt >= 100)
			{
				ms_cnt = 0;
				send(SOCK_TCPC,&ace[0],9);
			}

		}
  }
} 
void delay(int t)
{
	int tt = 5000;
	while(t--)
	{
		for(;tt > 0;tt--);
	}
}
