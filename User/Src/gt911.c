#include "gt911.h"
#include "gpio.h"
#include "lcd.h"

#include <string.h>
#include "stdio.h"


/**
 * @brief 向GT911寄存器写入数据
 * 通过I2C通信，向GT911的指定寄存器写入指定长度的数据。
 * @param _usRegAddr 寄存器地址
 * @param _pRegBuf 数据缓冲区指针
 * @param _ucLen 数据长度
 */
uint8_t GT911_WriteReg(uint16_t RegAddr, uint8_t *pRegBuf, uint8_t ucLen)
{
	uint8_t data[2 + ucLen];
    data[0] = RegAddr >> 8;
    data[1] = RegAddr & 0xFF;
    
    memcpy(&data[2], pRegBuf, ucLen);
    
    if(HAL_I2C_Master_Transmit(&GT911_I2C, GT911_IIC_ADDR << 1, data, 2 + ucLen, 100) != HAL_OK)
        return 0;
    
    return 1;
}

/**
 * @brief 读取GT911寄存器
 * 通过I2C接口读取GT911指定寄存器的值，并将读取到的数据保存到缓冲区中。
 * @param _usRegAddr 寄存器地址
 * @param _pRegBuf 存储读取数据的缓冲区指针
 * @param _ucLen 读取数据的长度
 */
uint8_t GT911_ReadReg(uint16_t RegAddr, uint8_t *pRegBuf, uint8_t ucLen)
{
	uint8_t regAddr[2] = {RegAddr >> 8, RegAddr & 0xFF};
    
    // 先发送寄存器地址
    if (HAL_I2C_Master_Transmit(&GT911_I2C, GT911_IIC_ADDR << 1, regAddr, 2, HAL_MAX_DELAY) != HAL_OK)
    {
        return HAL_ERROR;
    } 
    // 然后读取数据
    if (HAL_I2C_Master_Receive(&GT911_I2C, GT911_IIC_ADDR << 1, pRegBuf, ucLen, HAL_MAX_DELAY) != HAL_OK)
    {
        return HAL_ERROR;
    }
    return HAL_OK;
}


//==================================================================
//Name:						GT911_reset
//Author:					
//Date:						2017-07-06
//Function:				Rest GT911, just reset without sync
//Input:					void
//Output:					void
//option:
//==================================================================
void GT911_reset(void)
{
	HAL_GPIO_WritePin(T_RST_GPIO_Port,T_RST_Pin,GPIO_PIN_RESET);
	HAL_Delay(10);
	HAL_GPIO_WritePin(T_RST_GPIO_Port,T_RST_Pin,GPIO_PIN_SET);
	HAL_Delay(100);
}

//==================================================================
//Name:						Touch_Init
//Author:					
//Date:						2017-07-06
//Function:				GT911 init, including GPIO init, sync, and version check
//Input:					unsigned int addr				//address of register
//								unsigned char *value		//pointer of data output
//Output:					void
//option:
//==================================================================
uint8_t GT911_Init(void)
{
	uint8_t touchIC_ID[4];	
	GT911_reset();
	GT911_ReadReg(GT911_ID_ADDR,touchIC_ID,4);
//	touchIC_ID[0] = Touch_Get_Count();
	// if( touchIC_ID[0] == '9' )
	// {
	// 	printf("Touch ID: %s \r\n",touchIC_ID);
	// 	return 1;
	// }
	// else
	// {
	// 	printf("Touch Error\r\n");
	// 	return 0;
	// }
	return 1;
}

uint8_t Touch_Get_Count(void)
{
	uint8_t count[1] = {0};
	GT911_ReadReg (GT911_READ_ADDR,count,1);	//read touch data
	return (count[0]&0x0f);
}

const uint16_t TPX[] = {0x8150,0x8158,0x8160,0x8168,0x8170}; //电容屏触摸点数据地址（1~5）

//==================================================================
//Name:						Touch_Get_Data
//Author:					
//Date:						2017-07-06
//Function:				Get GT911 data, such as point and coordinate
//Input:					void
//Output:					void
//option:
//==================================================================
uint8_t GT911_Scan(uint8_t mode)
{
	uint8_t buf[4];
	uint8_t i=0;
	uint8_t res=0;
	uint8_t temp;
	uint8_t tempsta;
 	static uint8_t t=0;//控制查询间隔,从而降低CPU占用率   
	t++;
	if((t%10)==0||t<10)//空闲时,每进入10次CTP_Scan函数才检测1次,从而节省CPU使用率
	{ 
 		GT911_ReadReg(GT911_READ_ADDR, &mode,1);
		if(mode&0X80&&((mode&0XF)<6))
		{
			temp=0;	
			GT911_WriteReg (GT911_READ_ADDR,&temp,1);
		}		
		if((mode&0XF)&&((mode&0XF)<6))
		{
			temp=0XFF<<(mode&0XF);		//将点的个数转换为1的位数,匹配tp_dev.sta定义 
			tempsta=tp_dev.sta;			//保存当前的tp_dev.sta值
			tp_dev.sta=(~temp)|TP_PRES_DOWN|TP_CATH_PRES; 
			tp_dev.x[4]=tp_dev.x[0];	//保存触点0的数据
			tp_dev.y[4]=tp_dev.y[0];
			for(i=0;i<5;i++)
			{
				if(tp_dev.sta&(1<<i))	//触摸有效?
				{
					GT911_ReadReg(TPX[i],buf,4);	//读取XY坐标值
					if(DFT_SCAN_DIR==U2D_L2R)//横屏
					{
						tp_dev.y[i]=320-((uint16_t)buf[1]<<8)-buf[0];
						tp_dev.x[i]=(((uint16_t)buf[3]<<8)+buf[2]);
					}
					else if(DFT_SCAN_DIR==R2L_U2D)
					{
						tp_dev.x[i]=((uint16_t)buf[1]<<8)+buf[0];
						tp_dev.y[i]=((uint16_t)buf[3]<<8)+buf[2];
					} 
					else if(DFT_SCAN_DIR==L2R_D2U)
					{
						tp_dev.x[i]=320-(((uint16_t)buf[1]<<8)+buf[0]);
						tp_dev.y[i]=480-(((uint16_t)buf[3]<<8)+buf[2]);
					} 
					else 
					{
						tp_dev.y[i]=((uint16_t)buf[1]<<8)+buf[0];
						tp_dev.x[i]=480-(((uint16_t)buf[3]<<8)+buf[2]);
					}				
				}			
			} 
			res=1;
			if(tp_dev.x[0]>lcddev.width||tp_dev.y[0]>lcddev.height)//非法数据(坐标超出了)
			{ 
				if((mode&0XF)>1)		//有其他点有数据,则复第二个触点的数据到第一个触点.
				{
					tp_dev.x[0]=tp_dev.x[1];
					tp_dev.y[0]=tp_dev.y[1];
					t=0;				//触发一次,则会最少连续监测10次,从而提高命中率
				}else					//非法数据,则忽略此次数据(还原原来的)  
				{
					tp_dev.x[0]=tp_dev.x[4];
					tp_dev.y[0]=tp_dev.y[4];
					mode=0X80;		
					tp_dev.sta=tempsta;	//恢复tp_dev.sta
				}
			}else t=0;					//触发一次,则会最少连续监测10次,从而提高命中率
		}
	}
	if((mode&0X8F)==0X80)//无触摸点按下
	{ 
		if(tp_dev.sta&TP_PRES_DOWN)	//之前是被按下的
		{
			tp_dev.sta&=~(1<<7);	//标记按键松开
		}else						//之前就没有被按下
		{ 
			tp_dev.x[0]=0xffff;
			tp_dev.y[0]=0xffff;
			tp_dev.sta&=0XE0;	//清除点有效标记	
		}	 
	} 	
	if(t>240)t=10;//重新从10开始计数
	return res;
}

