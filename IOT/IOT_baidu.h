#ifndef _IOT_BAIDU_H
#define _IOT_BAIDU_H

#include "stm32f1xx_hal.h"
#include "MQTTPacket.h"
#include "transport.h"
#include "string.h"
#include "stdlib.h"
#include "usart.h"


#define  PRODUCTKEY           "a1w0XJbXwh0"                                        //产品ID
#define  PRODUCTKEY_LEN       strlen(PRODUCTKEY)                                   //产品ID长度
#define  DEVICENAME			 "SmartLED_01"                                              //设备名  
#define  DEVICENAME_LEN       strlen(DEVICENAME)                                   //设备名长度
#define  DEVICESECRE          "uwMJYOGSoAPNdZBOi8hyDX2qBnHQYgGo"                  				 //设备秘钥   
#define  DEVICESECRE_LEN      strlen(DEVICESECRE)                                  //设备秘钥长度
 
 
extern uint16_t buflen;
extern unsigned char buf[200];

uint8_t IOT_baidu_connect(void);
uint8_t IOT_baidu_ping(void);


#endif

