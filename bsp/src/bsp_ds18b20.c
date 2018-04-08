#include "bsp_ds18b20.h"
#include "bsp_time.h"
#include <string.h>
#define DS18B20_PIN_MODE_LOW 0
#define DS18B20_PIN_MODE_HIGH 1
//�������ģʽ

Temperature_Typedef TemperatureObj;



void Ds18b20_OUT(uint8_t value){
	if(value==0){
		HAL_GPIO_WritePin(DS18B20_GPIO_PORT,DS18B20_GPIO_PIN,GPIO_PIN_RESET);
	}else{
		HAL_GPIO_WritePin(DS18B20_GPIO_PORT,DS18B20_GPIO_PIN,GPIO_PIN_SET);
	}
}


//��������ģʽ
uint8_t Ds18b20_IN(void){
	if(GPIO_PIN_RESET==HAL_GPIO_ReadPin(DS18B20_GPIO_PORT,DS18B20_GPIO_PIN)){
		return 0;
	}else{
		return 1;
	}
}





//��λDS18B20
void DS18B20_Rst(void)	   
{                 
	DS18B20_IO_OUT();   //����Ϊ���
  Ds18b20_OUT(0);  //����DQ
  DelayUs(750);      //����750us
  Ds18b20_OUT(1);  //DQ=1 
	DelayUs(15);       //15US
}

//�ȴ�DS18B20�Ļ�Ӧ
//����1:δ��⵽DS18B20�Ĵ���
//����0:����
uint8_t DS18B20_Check(void) 	   
{   
	uint8_t retry=0;
	DS18B20_IO_IN();    //����Ϊ����
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

//��DS18B20��ȡһ��λ
//����ֵ��1/0
uint8_t DS18B20_Read_Bit(void) 
{
  uint8_t data;
	DS18B20_IO_OUT();   //����Ϊ���
  Ds18b20_OUT(0); 
	DelayUs(2);
  Ds18b20_OUT(1);
	DS18B20_IO_IN();    //����Ϊ����
	DelayUs(12);
	data=Ds18b20_IN();
  DelayUs(50);           
  return data;
}

//��DS18B20��ȡһ���ֽ�
//����ֵ������������
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

//дһ���ֽڵ�DS18B20
//dat��Ҫд����ֽ�
void DS18B20_Write_Byte(uint8_t dat)     
 {             
    uint8_t j;
    uint8_t testb;
    DS18B20_IO_OUT();     //����Ϊ���
    for (j=1;j<=8;j++) 
	{
        testb=dat&0x01;
        dat=dat>>1;
        if(testb)       // д1
        {
            Ds18b20_OUT(0);
            DelayUs(2);                            
            Ds18b20_OUT(1);
            DelayUs(60);             
        }
        else            //д0
        {
            Ds18b20_OUT(0);
            DelayUs(60);             
            Ds18b20_OUT(1);
            DelayUs(2);                          
        }
    }
}
 
//��ʼ�¶�ת��
void DS18B20_Start(void)
{   						               
    DS18B20_Rst();	   
    DS18B20_Check();	 
    DS18B20_Write_Byte(0xcc);// skip rom
    DS18B20_Write_Byte(0x44);// convert
}

//��ʼ��DS18B20��IO�� DQ ͬʱ���DS�Ĵ���
//����1:������
//����0:����    	 
uint8_t DS18B20_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_GPIOC_CLK_ENABLE();			//����GPIOBʱ��
	
    GPIO_Initure.Pin=DS18B20_GPIO_PIN;           //PC7
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //�������
    GPIO_Initure.Pull=GPIO_PULLUP;          //����
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //����
    HAL_GPIO_Init(DS18B20_GPIO_PORT,&GPIO_Initure);     //��ʼ��
 
 	  DS18B20_Rst();
	  return DS18B20_Check();
}

//��ds18b20�õ��¶�ֵ
//���ȣ�0.1C
//����ֵ���¶�ֵ ��-550~1250�� 
short DS18B20_Get_Temp(void)
{
    uint8_t temp;
    uint8_t TL,TH;
    short tem;
    DS18B20_Start ();           //��ʼת��
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
        temp=0;//�¶�Ϊ��  
    }else temp=1;//�¶�Ϊ��	  	  
    tem=TH; //��ø߰�λ
    tem<<=8;    
    tem+=TL;//��õװ�λ
    tem=(double)tem*0.625;//ת��     
	if(temp)return tem; //�����¶�ֵ
	else return -tem;    
}



void TemperatureServerInit(Temperature_Typedef *obj){
	while(DS18B20_Init());
	memset(obj->buf,0,obj->buflen*sizeof(uint8_t));
	obj->buflen=TMPERATURE_BUF_LEN;
	obj->to_avrage_cunt=TMPERATURE_BUF_LEN;
	obj->buf_index=0;
}

//�¶ȼ�������
void TemperatureServer(Temperature_Typedef *obj){
	uint64_t sum=0;
	obj->bussy=TMPERATURE_BUSSY;
	//�ɼ��¶�
	//obj->buf[obj->buf_index]=DS18B20_Get_Temp();	
	sum+=DS18B20_Get_Temp();
	obj->buf_index++;
	if(obj->buf_index==obj->to_avrage_cunt){//��ƽ��
		obj->bussy=TMPERATURE_BUSSY;
		obj->temperature[0]=sum/obj->buf_index;
		obj->bussy=TMPERATURE_NOBUSSY;
		obj->temperature[1]=obj->temperature[0];
	}
}


//����10  ���¶�����ֵ   ��10������С��ֵ

short TemperatureGetData(Temperature_Typedef *obj){
	if(obj->bussy==TMPERATURE_BUSSY){
		return obj->temperature[1];
	}else{
		return obj->temperature[0];
	}
}
