#include "IOT_baidu.h"
#include "utils_hmac.h"

uint16_t buflen=200;
unsigned char buf[200];

char ClientID[128];
uint8_t ClientID_len;

char Username[128];
uint8_t Username_len;

char Password[128];
uint8_t Password_len;

uint8_t IOT_baidu_connect()
{
		uint32_t len;
		
		char temp[128];
		
	
		u1_printf("进入连接云服务器函数\r\n");
		MQTTPacket_connectData data = MQTTPacket_connectData_initializer;//配置部分可变头部的值
    
		buflen = sizeof(buf);
		memset(buf,0,buflen);
	
		memset(ClientID,128,0);                                               //客户端ID的缓冲区全部清零
		sprintf(ClientID,"%s|securemode=3,signmethod=hmacsha1|",DEVICENAME);  //构建客户端ID，并存入缓冲区
	
		memset(Username,128,0);                                               //用户名的缓冲区全部清零
		sprintf(Username,"%s&%s",DEVICENAME,PRODUCTKEY);                      //构建用户名，并存入缓冲区
	
		Username_len = strlen(Username);
	
		memset(temp,128,0);                                                                      //临时缓冲区全部清零
		sprintf(temp,"clientId%sdeviceName%sproductKey%s",DEVICENAME,DEVICENAME,PRODUCTKEY);     //构建加密时的明文   
		utils_hmac_sha1(temp,strlen(temp),Password,DEVICESECRE,DEVICESECRE_LEN);                 //以DeviceSecret为秘钥对temp中的明文，进行hmacsha1加密，结果就是密码，并保存到缓冲区中
		Password_len = strlen(Password);                                                         //计算用户名的长度
	
		u1_printf("ClientId:%s\r\n",ClientID);
		u1_printf("Username:%s\r\n",Username);
		u1_printf("Password:%s\r\n",Password);
		
		data.MQTTVersion = 3;
		data.clientID.cstring = ClientID;						//客户端标识，用于区分每个客户端xxx为自定义，后面为固定格式
		data.keepAliveInterval = 120;							//保活计时器，定义了服务器收到客户端消息的最大时间间隔,单位是秒
		data.cleansession = 1;									//该标志置1服务器必须丢弃之前保持的客户端的信息，将该连接视为“不存在”
		data.username.cstring = Username;						//用户名 DeviceName&ProductKey
		data.password.cstring = Password;						//密码，工具生成
		
		len = MQTTSerialize_connect(buf, buflen, &data); 		/*1 构造连接报文*/
		
		transport_sendPacketBuffer(3,buf, len);				//发送连接请求
		
		unsigned char sessionPresent, connack_rc;
        do
        {
            while(MQTTPacket_read(buf, buflen, transport_getdata) != CONNACK)//对接收到的报文进行解析
            {
                ;
            }
        }
        while(MQTTDeserialize_connack(&sessionPresent, &connack_rc, buf, buflen) != 1 || connack_rc != 0);
					if(connack_rc != 0)
					{
						u1_printf("connack_rc:%uc\r\n",connack_rc);
					}
					
					u1_printf("Connect Success!\r\n");
					
					while(1)
					{
						HAL_Delay(3000);
						len = MQTTSerialize_pingreq(buf, buflen);
						transport_sendPacketBuffer(3, buf, len);
						HAL_Delay(100);
						u1_printf("Ping...\r\n");
						if (MQTTPacket_read(buf, buflen, transport_getdata) == PINGRESP){
							u1_printf("Pong\r\n");
						
						}
						else {
							u1_printf("OOPS\r\n");
						
						}
					}
		return 0;
}

uint8_t IOT_baidu_ping(void)
{
		uint32_t len;
		len = MQTTSerialize_pingreq(buf, buflen);
		transport_sendPacketBuffer(3, buf, len);
		u1_printf("Ping...\r\n");
		if (MQTTPacket_read(buf, buflen, transport_getdata) == PINGRESP){
			u1_printf("Pong\r\n");
			return 0;
		}
		else {
			u1_printf("OOPS\r\n");
			return 1;
		}

}

