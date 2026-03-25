#include "touch.h"
#include "lcd.h"
#include "stdlib.h"
#include "math.h"
#include "stdio.h"

_m_tp_dev tp_dev = {
    TP_Init,
    GT911_Scan,
    {0},
    {0},
    0,
    0,
};				

 	 			    					  
//画一个大点(2*2的点)		   
//x,y:坐标
//color:颜色
void TP_Draw_Big_Point(uint16_t x,uint16_t y,uint16_t color)
{	    
	LCD_DrawPoint(x,y,color);//中心点 
	LCD_DrawPoint(x+1,y,color);
	LCD_DrawPoint(x,y+1,color);
	LCD_DrawPoint(x+1,y+1,color);	 	  	
}


//触摸屏初始化  		    
uint8_t TP_Init(void)
{
	uint8_t a = 0;
  	while(!GT911_Init()){
		HAL_Delay(500);
	}
	GT911_WriteReg(GT911_READ_ADDR, &a, 1);
	tp_dev.scan=GT911_Scan;	//扫描函数指向GT911触摸屏扫描
	tp_dev.touchtype|=0X80;	//电容屏  
	return 0;
}

// const uint16_t POINT_COLOR_TBL[CT_MAX_TOUCH]={RED,GREEN,BLUE,BROWN,GRED};  
const uint16_t POINT_COLOR_TBL[CT_MAX_TOUCH]={RED};  
//电容触摸屏测试函数

void Load_Drow_Dialog(void)
{
	LCD_ShowString(lcddev.width-24,0,(uint8_t*)"RST",RED,WHITE,16,0);//显示清屏区域
}
//////////////////////////////////////////////////////////////////////////////////
//画一条粗线
//(x1,y1),(x2,y2):线条的起始坐标
//size：线条的粗细程度
//color：线条的颜色
void lcd_draw_bline(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,uint16_t color)
{
	uint16_t t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol; 
	delta_x=x2-x1; //计算坐标增量 
	delta_y=y2-y1; 
	uRow=x1; 
	uCol=y1; 
	if(delta_x>0)incx=1; //设置单步方向 
	else if(delta_x==0)incx=0;//垂直线 
	else {incx=-1;delta_x=-delta_x;} 
	if(delta_y>0)incy=1; 
	else if(delta_y==0)incy=0;//水平线 
	else{incy=-1;delta_y=-delta_y;} 
	if( delta_x>delta_y)distance=delta_x; //选取基本增量坐标轴 
	else distance=delta_y; 
	for(t=0;t<=distance+1;t++ )//画线输出 
	{  
		TP_Draw_Big_Point(uRow,uCol,color);//画点
		xerr+=delta_x ; 
		yerr+=delta_y ; 
		if(xerr>distance) 
		{ 
			xerr-=distance; 
			uRow+=incx; 
		} 
		if(yerr>distance) 
		{ 
			yerr-=distance; 
			uCol+=incy; 
		} 
	}  
}   

void ctp_test(void)
{
	uint8_t t=0;
	uint8_t i=0;	  	    
 	uint16_t lastpos[5][2];		//最后一次的数据 
	tp_dev.init();
	LCD_Fill(0,0,lcddev.width,lcddev.height,WHITE);
	while(1)
	{   
		LCD_ShowString(lcddev.width-24,0,(uint8_t*)"RST",RED,WHITE,16,0);
		tp_dev.scan(0);

		for(t=0;t<CT_MAX_TOUCH;t++)//最多5点触摸
		{
			if((tp_dev.sta)&(1<<t))//判断是否有点触摸？
			{    
				HAL_Delay(1);
				if(tp_dev.x[t]<lcddev.width&&tp_dev.y[t]<lcddev.height)//在LCD范围内
				{
					if(lastpos[t][0]==0XFFFF)
					{
						lastpos[t][0] = tp_dev.x[t];
						lastpos[t][1] = tp_dev.y[t];
					}
					lcd_draw_bline(lastpos[t][0],lastpos[t][1],tp_dev.x[t],tp_dev.y[t],POINT_COLOR_TBL[t]);//画线
					lastpos[t][0]=tp_dev.x[t];
					lastpos[t][1]=tp_dev.y[t];
					if(tp_dev.x[t]>(lcddev.width-24)&&tp_dev.y[t]<16)
					{
						
					}
				}
			}else lastpos[t][0]=0XFFFF;
		}
		HAL_Delay(5);i++;
	}	
}
