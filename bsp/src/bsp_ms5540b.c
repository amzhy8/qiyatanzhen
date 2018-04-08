#include "bsp_ms5540b.h"
#include "bsp_time.h"

#include <math.h>



MS5540_Typedef MS5540;
//管脚操作


#include <stdio.h> 



/*******************************************************************************
* Function Name  : Delay
* Description    : Inserts a delay time.
* Input          : nCount: specifies the delay time length.
* Output         : None
* Return         : None
*******************************************************************************/
/*
void Ms5540_Delay(uint32_t nCount)
{
  for(; nCount != 0; nCount--);
}
*/
/*******************************************************************************
* Function Name  : void          setSCLK      (unsigned char state);
* Description    : /set SCLK to the specified state (true/false)
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void setSCLK  (unsigned char state)
{
    if (state==1)
			HAL_GPIO_WritePin(MS5540_SCK_PORT,MS5540_SCK_PIN,GPIO_PIN_SET);
    else
        HAL_GPIO_WritePin(MS5540_SCK_PORT,MS5540_SCK_PIN,GPIO_PIN_RESET);
}
/*******************************************************************************
* Function Name  :void setDIN   (unsigned char state)
* Description    : set DIN  to the specified state (true/false)
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void setDIN   (unsigned char state)
{
    if (state==1)
        HAL_GPIO_WritePin(MS5540_DIN_PORT,MS5540_DIN_PIN,GPIO_PIN_SET);
    else
        HAL_GPIO_WritePin(MS5540_DIN_PORT,MS5540_DIN_PIN,GPIO_PIN_RESET);
}
/*******************************************************************************
* Function Name  : unsigned char getDOUT  (void)
* Description    : returns the current state of DOUT
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
unsigned char getDOUT  (void)
{
    unsigned  short data;
		if(GPIO_PIN_SET==HAL_GPIO_ReadPin(MS5540_DOUT_PORT,MS5540_DOUT_PIN))data=1;
		else data=0;
    return(data);
}

/*******************************************************************************
* Function Name  : unsigned short SerialGet16(void)
* Description    : shift in (from IC to PC) a 16 bit value
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
unsigned short SerialGet16(void)//将IC中的数据按位传到ARM并且得到16位的数
{
    unsigned char i;
    unsigned short v;
    unsigned short data;
    v = 0;
    data=0;
   // GPIO_Configuration();
    setSCLK(0);
    //Ms5540_Delay(100);

    for (i=0; i<16; i++)
    {
        setSCLK(1);
        //Ms5540_Delay(100);
        setSCLK(0);
        v=getDOUT();
        v = v <<(15-i);
        data+=v;
        //Ms5540_Delay(100);
    }
    return(data);
}
/*******************************************************************************
* Function Name  : void SerialSendLsbFirst(unsigned char pattern, unsigned char nbr_clock)
* Description    : shift out (from PC to IC) a  sequence of bits
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SerialSendLsbFirst(unsigned char pattern, unsigned char nbr_clock)//将一个16位的数从ARM按位传输到IC，低位优先
{
    unsigned char i;
    unsigned char c;
    //GPIO_Configuration();
    setSCLK(0);
    //Ms5540_Delay(100);
    for (i=0; i<nbr_clock; i++)
    {
        
        c = pattern & 1;
        if (c==1)
            setDIN(1);
        else
            setDIN(0);
       // Ms5540_Delay(100);
        setSCLK(1);
       // Ms5540_Delay(100);
        setSCLK(0);
        pattern = pattern >> 1;
    }
}
/*******************************************************************************
* Function Name  : void reset(void)
* Description    : send a reset sequence to the IC
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void reset(void)
{
    SerialSendLsbFirst(0x55, 8);
    SerialSendLsbFirst(0x55, 8);
    SerialSendLsbFirst(0x00, 5);
}
/*******************************************************************************
* Function Name  : unsigned short getW  (unsigned char index)  
* Description    : Read the corresponding calibration word of the IC (index [1:4])
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
unsigned short getW  (unsigned char index)  // 得到W1 to 4
{
    unsigned short data;

    data = 0;
    switch(index)
    {
        case 1:
            SerialSendLsbFirst((char) 0x57, (char) 8);
            SerialSendLsbFirst((char) 0x01, (char) 5);
            data = SerialGet16();
            break;

        case 2:
            SerialSendLsbFirst((char) 0xD7, (char) 8);
            SerialSendLsbFirst((char) 0x00, (char) 5);
            data = SerialGet16();
            break;

        case 3:
            SerialSendLsbFirst((char) 0x37, (char) 8);
            SerialSendLsbFirst((char) 0x01, (char) 5);
            data = SerialGet16();
            break;

        case 4:
            SerialSendLsbFirst((char) 0xB7, (char) 8);
            SerialSendLsbFirst((char) 0x00, (char) 5);
            data = SerialGet16();
            break;
    }
    SerialSendLsbFirst(0x00, 1);  // to be compliant with the data sheet
    return(data);
}
/*******************************************************************************
* Function Name  : unsigned short getD1            (unsigned char *error_pt)
* Description    : Start a D1 acquisition, wait for end of conversion and return the value
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
unsigned short getD1            (void)//得到D1，要等待dout为低的时候代表传输结束
{
    unsigned short d1;
    //unsigned short data;
  
    char m;
   // GPIO_Configuration();
    SerialSendLsbFirst(0x2F, 8);
    SerialSendLsbFirst(0x00, 2);
    setSCLK(1);
	//	Ms5540_Delay(50);
    setSCLK(0);
		//Ms5540_Delay(50);
    setSCLK(1);
		//Ms5540_Delay(50);
    setSCLK(0);
		//Ms5540_Delay(50);
    do
    {
      setSCLK(0);
      m=getDOUT();
    }while(m!=0);//等待dout为低，传输结束标志
    
    
    d1=SerialGet16();
   
    SerialSendLsbFirst(0x00, 1);  // to be compliant with the data sheet
    return(d1);
}
/*******************************************************************************
* Function Name  : unsigned short getD2(unsigned char *error_pt)
* Description    : Start a D2 acquisition, wait for end of conversion and return the value
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
unsigned short getD2( void)
{
    unsigned short d2;
    char m;
   // GPIO_Configuration();
    SerialSendLsbFirst(0x4F, 8);
    SerialSendLsbFirst(0x00, 3); // Note the difference with getD1 
    setSCLK(1);
		//Ms5540_Delay(50);
    setSCLK(0);
		//Ms5540_Delay(50);
    setSCLK(1);
		//Ms5540_Delay(50);
    
    do
   {
     setSCLK(0);
      m=getDOUT();
    }while(m!=0);//等待dout为低，传输结束
    
    d2=SerialGet16();
    SerialSendLsbFirst(0x00, 1);
    return(d2);
}
/*******************************************************************************
* Function Name  : long multiply(short a, short b)
* Description    : multiply
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
long multiply(short a, short b)
{
    return(((long) a) * ((long) b));
}
/*******************************************************************************
* Function Name  : void  Tsensor5540C_ConvertWtoC (short W1, short W2, short W3, short W4)
* Description    :  Convert the Wx coefficients into the Cx *
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
/*
void  Tsensor5540C_ConvertWtoC ( long W1, long W2, long W3, long W4)
{

  c1 = W1>>1;
  c2 = W3 & 0x3f;
  c2 <<= 6;
  c2 |= (W4 & 0x3f);
  c3 = W4>>6;
  c4 = W3>>6;
  c5 = W1<<10;
  c5 &= 0x400;
  c5 += W2>>6;
  c6 = W2&0x3f;    
}
*/


void Ms5540_GetW2C(MS5540_Typedef *obj){
	
	reset();
  obj->W1= getW(1);
  obj->W2 = getW(2);
  obj->W3 = getW(3);
  obj->W4 = getW(4);
	
	obj->C1 = obj->W1>>1;
  obj->C2 = obj->W3 & 0x3f;
  obj->C2 <<= 6;
  obj->C2 |= (obj->W4 & 0x3f);
  obj->C3 = obj->W4>>6;
  obj->C4 = obj->W3>>6;
  obj->C5 = obj->W1<<10;
  obj->C5 &= 0x400;
  obj->C5 += obj->W2>>6;
  obj->C6 = obj->W2&0x3f;
}
void Ms5540_Init(MS5540_Typedef *obj)//初始化气压传感器
{
	Ms5540_GetW2C(obj);
	
}

/*******************************************************************************
* Function Name  : long press()
* Description    : 压力读取函数
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/


	
	
//测量温度及压力
void Ms5540_ReadTempPressure(MS5540_Typedef *obj){
	
     obj->D1 = getD1();            /* read pressure value    */
     obj->D2 = getD2();            /* read temperature value */
     obj->UT1  = 8*obj->C5+20224;
     obj->dT  = obj->D2-obj->UT1;             
		 obj->temperatureT= (int16_t)(200+((obj->dT*(obj->C6+50))>>10));/* temperature in 0.1 °C  */
     obj->OFF  = obj->C2*4+(((obj->C4-512)*obj->dT)>>14);
     obj->SENS = obj->C1+((obj->C3*obj->dT)>>10)+24576;
     obj->X    = (obj->SENS*(obj->D1-7168))/16384-obj->OFF;
     obj->PreesureT =(int16_t)( obj->X *10/32+250*10);   /* pressure in 0.01 mbar  */

	   Ms5540_SencondCompenssation(obj,obj->PreesureT,obj->temperatureT);//2次补偿

}	
	

//2次温度补偿

void Ms5540_SencondCompenssation(MS5540_Typedef *obj,int16_t pressure,int16_t temperature){
	int32_t TEMP,P,T2,P2;
	TEMP=temperature;
	P=pressure;
	if(TEMP<200){
		T2=(11*((int32_t)obj->C6+24)*(200-TEMP)*(200-TEMP))>>20;
		P2=(3*T2*(P-3500))>>14;
	}else if((TEMP>=200)&&(TEMP<=450)){
		T2=0;
		P2=0;
	}else{
		T2=(3*((int32_t)obj->C6+24)*(450-TEMP)*(450-TEMP))>>20;
		P2=(T2*(P-10000))>>13;		
	}
	
	obj->temperature=TEMP-T2;
	obj->Preesure=P-P2;
	
	obj->temperaturebuf[0]=(uint8_t)((obj->temperature)&0x00ff);
	obj->temperaturebuf[1]=(uint8_t)(((obj->temperature)&0xff00)>>8);
	obj->pressurebuf[0]=(uint8_t)((obj->Preesure)&0x00ff);
	obj->pressurebuf[1]=(uint8_t)(((obj->Preesure)&0xff00)>>8);
	
}
