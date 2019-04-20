#include "ds18b20.h"
#include "delay.h"	

const uint8_t IDROM1[8]={0x28,0xf5,0x7b,0x79,0x17,0x13,0x01,0x76};//�����ӵ�
const uint8_t IDROM2[8]={0x28,0xff,0x66,0xc3,0x67,0x18,0x01,0x6f};//��ɫ

/****************************************************
*�������ܣ�DS18B20 GPIO��ʼ��
*���������
*����ֵ  ��
****************************************************/
void DS18B20_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	/*����LED��ص�GPIO����ʱ��*/
	RCC_AHB1PeriphClockCmd (RCC_AHB1Periph_GPIOF, ENABLE); 
	/*ѡ��Ҫ���Ƶ�GPIO����*/															   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;	

	/*��������ģʽΪ���ģʽ*/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;   
	
	/*�������ŵ��������Ϊ�������*/
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	
	/*��������Ϊ����ģʽ*/
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;

	/*������������Ϊ2MHz */   
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 

	/*���ÿ⺯����ʹ���������õ�GPIO_InitStructure��ʼ��GPIO*/
	GPIO_Init(GPIOF, &GPIO_InitStructure);	
}

//��λDS18B20
void DS18B20_Rst(void)	   
{            
	DS18B20_IO_OUT(); //SET PG11 OUTPUT
	DS18B20_DQ_OUT=0; //����DQ
	DWT_Delay_us(750);    //����750us
	DS18B20_DQ_OUT=1; //DQ=1 
	DWT_Delay_us(15);     //15US
}
//�ȴ�DS18B20�Ļ�Ӧ
//����1:δ��⵽DS18B20�Ĵ���
//����0:����
uint8_t DS18B20_Check(void) 	   
{   
	uint8_t retry=0;
	DS18B20_IO_IN();//SET PG11 INPUT	 
  while(DS18B20_DQ_IN&&retry<200)
	{
		retry++;
		DWT_Delay_us(1);
	};	 
	if(retry>=200)
		return 1;
	else 
		retry=0;
	while (!DS18B20_DQ_IN&&retry<240)
	{
		retry++;
		DWT_Delay_us(1);
	}
	if(retry>=240)
		return 1;	    
	return 0;
}

//��DS18B20��ȡһ��λ
//����ֵ��1/0
uint8_t DS18B20_Read_Bit(void) 			  
{
  uint8_t data;
	DS18B20_IO_OUT();//SET PG11 OUTPUT
  DS18B20_DQ_OUT=0; 
	DWT_Delay_us(2);
  DS18B20_DQ_OUT=1; 
	DS18B20_IO_IN();//SET PG11 INPUT
	DWT_Delay_us(12);
	if(DS18B20_DQ_IN)
		data=1;
  else 
		data=0;	 
  DWT_Delay_us(50);           
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
	DS18B20_IO_OUT();//SET PG11 OUTPUT;
	for (j=1;j<=8;j++) 
	{
		testb=dat&0x01;
		dat=dat>>1;
		if (testb) 
		{
			DS18B20_DQ_OUT=0;// Write 1
			DWT_Delay_us(2);                            
			DS18B20_DQ_OUT=1;
			DWT_Delay_us(60);             
		}
		else 
		{
			DS18B20_DQ_OUT=0;// Write 0
			DWT_Delay_us(60);             
			DS18B20_DQ_OUT=1;
			DWT_Delay_us(2);                          
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
	uint8_t ret=1;
  DS18B20_GPIO_Init();
	rt_base_t level;
	level=rt_hw_interrupt_disable();
 	DS18B20_Rst();
	ret=DS18B20_Check();
	rt_hw_interrupt_enable(level);
	return ret;
}  
//��ds18b20�õ��¶�ֵ
//���ȣ�0.1C
//����ֵ���¶�ֵ ��-550~1250�� 
short DS18B20_Get_Temp(uint8_t index)
{
	uint8_t temp,i;
	uint8_t TL,TH;
	short tem;
	DS18B20_Start();// ds1820 start convert
	DS18B20_Rst();
	DS18B20_Check();			
	DS18B20_Write_Byte(0x55);//ƥ��rom ID
	for(i=0;i<8;i++)         //����?��64??����??
	{
		switch(index)
		{
			case 1 :DS18B20_Write_Byte(IDROM1[i]);break;
			case 2 :DS18B20_Write_Byte(IDROM2[i]);break;		
		}
	}	
	DS18B20_Write_Byte(0xbe);// convert	    
	TL=DS18B20_Read_Byte(); // LSB   
	TH=DS18B20_Read_Byte(); // MSB   
	if(TH>7)
	{
		TH=~TH;
		TL=~TL; 
		temp=0;		//�¶�Ϊ��  
	}
	else 
		temp=1;	//�¶�Ϊ��	  	  
	tem=TH; 		//��ø߰�λ
	tem<<=8;    
	tem+=TL;		//��õװ�λ
	tem=(double)tem*0.625;	//ת��     
	if(temp)
		return tem;	 	//�����¶�ֵ
	else 
		return -tem;    
}

void GetRomId(uint8_t *IdRoom)
{
	uint8_t i;
	DS18B20_Write_Byte(0x33);// read ID cmd
	for(i=0;i<8;i++)
	{
		IdRoom[i]=DS18B20_Read_Byte();//read
	}	
}