#include "bsp_ds18b20.h"
#include "bsp_time.h"
#include <string.h>
#define DS18B20_PIN_MODE_LOW 0
#define DS18B20_PIN_MODE_HIGH 1
//设置输出模式

Temperature_Typedef TemperatureObj;



void Ds18b20_OUT(uint8_t value){
	if(value==0){
		HAL_GPIO_WritePin(DS18B20_GPIO_PORT,DS18B20_GPIO_PIN,GPIO_PIN_RESET);
	}else{
		HAL_GPIO_WritePin(DS18B20_GPIO_PORT,DS18B20_GPIO_PIN,GPIO_PIN_SET);
	}
}


//设置输入模式
uint8_t Ds18b20_IN(void){
	if(GPIO_PIN_RESET==HAL_GPIO_ReadPin(DS18B20_GPIO_PORT,DS18B20_GPIO_PIN)){
		return 0;
	}else{
		return 1;
	}
}





//复位DS18B20
void DS18B20_Rst(void)	   
{                 
	DS18B20_IO_OUT();   //设置为输出
  Ds18b20_OUT(0);  //拉低DQ
  DelayUs(750);      //拉低750us
  Ds18b20_OUT(1);  //DQ=1 
	DelayUs(15);       //15US
}

//等待DS18B20的回应
//返回1:未检测到DS18B20的存在
//返回0:存在
uint8_t DS18B20_Check(void) 	   
{   
	uint8_t retry=0;
	DS18B20_IO_IN();    //设置为输入
    while (Ds18b20_IN()&&retry<200)
	{
		retry++;
		DelayUs(1);
	};	 
	if(retry>=200)return 1;
	else retry=0;
    while (!Ds18b20_IN()&&retry<240)
	{
		retry++;
		DelayUs(1);
	};
	if(retry>=240)return 1;	    
	return 0;
}

//从DS18B20读取一个位
//返回值：1/0
uint8_t DS18B20_Read_Bit(void) 
{
  uint8_t data;
	DS18B20_IO_OUT();   //设置为输出
  Ds18b20_OUT(0); 
	DelayUs(2);
  Ds18b20_OUT(1);
	DS18B20_IO_IN();    //设置为输入
	DelayUs(12);
	data=Ds18b20_IN();
  DelayUs(50);           
  return data;
}

//从DS18B20读取一个字节
//返回值：读到的数据
uint8_t DS18B20_Read_Byte(void)   
{        
  uint8_t i,j,dat;
  dat=0;
	for (i=1;i<=8;i++) 
	{
        j=DS18B20_Read_Bit();
        dat=(j<<7)|(dat>>1);
    }						    
  return dat;
}

//写一个字节到DS18B20
//dat：要写入的字节
void DS18B20_Write_Byte(uint8_t dat)     
 {             
    uint8_t j;
    uint8_t testb;
    DS18B20_IO_OUT();     //设置为输出
    for (j=1;j<=8;j++) 
	{
        testb=dat&0x01;
        dat=dat>>1;
        if(testb)       // 写1
        {
            Ds18b20_OUT(0);
            DelayUs(2);                            
            Ds18b20_OUT(1);
            DelayUs(60);             
        }
        else            //写0
        {
            Ds18b20_OUT(0);
            DelayUs(60);             
            Ds18b20_OUT(1);
            DelayUs(2);                          
        }
    }
}
 
//开始温度转换
void DS18B20_Start(void)
{   						               
    DS18B20_Rst();	   
    DS18B20_Check();	 
    DS18B20_Write_Byte(0xcc);// skip rom
    DS18B20_Write_Byte(0x44);// convert
}

//初始化DS18B20的IO口 DQ 同时检测DS的存在
//返回1:不存在
//返回0:存在    	 
uint8_t DS18B20_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_GPIOC_CLK_ENABLE();			//开启GPIOB时钟
	
    GPIO_Initure.Pin=DS18B20_GPIO_PIN;           //PC7
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //推挽输出
    GPIO_Initure.Pull=GPIO_PULLUP;          //上拉
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //高速
    HAL_GPIO_Init(DS18B20_GPIO_PORT,&GPIO_Initure);     //初始化
 
 	  DS18B20_Rst();
	  return DS18B20_Check();
}

//从ds18b20得到温度值
//精度：0.1C
//返回值：温度值 （-550~1250） 
short DS18B20_Get_Temp(void)
{
    uint8_t temp;
    uint8_t TL,TH;
    short tem;
    DS18B20_Start ();           //开始转换
    DS18B20_Rst();
    DS18B20_Check();	 
    DS18B20_Write_Byte(0xcc);   // skip rom
    DS18B20_Write_Byte(0xbe);   // convert	    
    TL=DS18B20_Read_Byte();     // LSB   
    TH=DS18B20_Read_Byte();     // MSB   
    if(TH>7)
    {
        TH=~TH;
        TL=~TL; 
        temp=0;//温度为负  
    }else temp=1;//温度为正	  	  
    tem=TH; //获得高八位
    tem<<=8;    
    tem+=TL;//获得底八位
    tem=(double)tem*0.625;//转换     
	if(temp)return tem; //返回温度值
	else return -tem;    
}



void TemperatureServerInit(Temperature_Typedef *obj){
	while(DS18B20_Init());
	memset(obj->buf,0,obj->buflen*sizeof(uint8_t));
	obj->buflen=TMPERATURE_BUF_LEN;
	obj->to_avrage_cunt=TMPERATURE_BUF_LEN;
	obj->buf_index=0;
}

//温度检测服务函数
void TemperatureServer(Temperature_Typedef *obj){
	uint64_t sum=0;
	obj->bussy=TMPERATURE_BUSSY;
	//采集温度
	//obj->buf[obj->buf_index]=DS18B20_Get_Temp();	
	sum+=DS18B20_Get_Temp();
	obj->buf_index++;
	if(obj->buf_index==obj->to_avrage_cunt){//求平均
		obj->bussy=TMPERATURE_BUSSY;
		obj->temperature[0]=sum/obj->buf_index;
		obj->bussy=TMPERATURE_NOBUSSY;
		obj->temperature[1]=obj->temperature[0];
	}
}


//除以10  是温度整数值   对10求余是小数值

short TemperatureGetData(Temperature_Typedef *obj){
	if(obj->bussy==TMPERATURE_BUSSY){
		return obj->temperature[1];
	}else{
		return obj->temperature[0];
	}
}
