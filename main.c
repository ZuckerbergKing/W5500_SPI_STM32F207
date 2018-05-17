/**
******************************************************************************
* @file    			main.c                                              
* @version 			V1.0.0_beta_20180515										  		
* @date    			2018-5-17								 
* @brief 				用3.5.0版本库建的工程模板						  	
* @attention		内网测试，请保证W5500的IP与测试PC机在同一网段内，且不冲突         
*								如果是用网线跟PC机直连，请设置PC机本地连接地址IP为静态IP
*				
* @company  		上海奇宝智能科技有限公司	（基于炜世科技移植修改修改）
* @information  
* @website  		www.qbs-sh.com																							 	 				
* @email				zhanbo.wang@qbs-sh.com	       																							 																											 
******************************************************************************
*/

/*W5500接STM32的 SPI1硬件接线定义*/
//	W5500_SCS    --->		STM32_GPIOA1								 /*W5500的片选管脚*/
//	W5500_SCLK	 --->	  STM32_GPIOA5								 /*W5500的时钟管脚*/
//  W5500_MISO	 --->		STM32_GPIOA6								 /*W5500的MISO管脚*/
//	W5500_MOSI	 --->		STM32_GPIOA7								 /*W5500的MOSI管脚*/
//	W5500_RESET	 --->   STM32_GPIOA2								 /*W5500的RESET管脚*/
//	W5500 INT 	 --->	  NONE												 /*不使用中断触发*/
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
  USART1_Config(); 				           /*初始化串口通信:115200@8-n-1*/
  delay(50);
	
  gpio_for_w5500_config();					/*初始化MCU相关引脚*/
  reset_w5500();							/*硬复位W5500*/
  PHY_check();								/*检查网线是否接入*/   
  set_w5500_mac();							/*配置MAC地址*/
  set_w5500_ip();							/*配置IP地址*/
  socket_buf_init(txsize, rxsize);			/*初始化8个Socket的发送接收缓存大小*/

	printf(" 电脑作为TCP服务器,让W5500作为 TCP客户端去连接 \r\n");
	printf(" 服务器IP:%d.%d.%d.%d\r\n",remote_ip[0],remote_ip[1],remote_ip[2],remote_ip[3]);
	printf(" 监听端口:%d \r\n",remote_port);
	printf(" 连接成功后，服务器发送数据给W5500，W5500将返回对应数据 \r\n");
	printf(" 应用程序执行中\r\n"); 
	
  while(1)                							/*循环执行的函数*/ 
  {
		do_tcp_client();                    /*TCP_Client 数据回环测试程序*/
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
