#include "dht11.h"
#include "delay.h"

/****************************************************
*�������ܣ�DH11 GPIO��ʼ��
*���������
*����ֵ  ��
****************************************************/
void DH11_GPIO_Init(void)
{
		GPIO_InitTypeDef GPIO_InitStructure;
		/*����LED��ص�GPIO����ʱ��*/
		RCC_AHB1PeriphClockCmd (RCC_AHB1Periph_GPIOB, ENABLE); 
		/*ѡ��Ҫ���Ƶ�GPIO����*/															   
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;	

		/*��������ģʽΪ���ģʽ*/
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;   
    
    /*�������ŵ��������Ϊ�������*/
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    
    /*��������Ϊ����ģʽ*/
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;

		/*������������Ϊ2MHz */   
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 

		/*���ÿ⺯����ʹ���������õ�GPIO_InitStructure��ʼ��GPIO*/
		GPIO_Init(GPIOB, &GPIO_InitStructure);	
}
	

/****************************************************
*�������ܣ�DH11��λ
*���������
*����ֵ  ��
****************************************************/
void DHT11_Rst(void)	   
{                 
	DHT11_IO_OUT(); 	//SET OUTPUT
	DHT11_DQ_OUT=0; 	//����DQ
	DWT_Delay_us(20000);    	//��������18ms
	DHT11_DQ_OUT=1; 	//DQ=1 
	DWT_Delay_us(30);     	//��������20~40us
}

/****************************************************
*�������ܣ��ȴ�DHT11�Ļ�Ӧ
*���������
*����ֵ  ��1:δ��⵽DHT11�Ĵ��� 0:����
****************************************************/
u8 DHT11_Check(void) 	   
{   
	u8 retry=0;
	DHT11_IO_IN();//SET INPUT	 
    while (DHT11_DQ_IN&&retry<100)//DHT11������40~80us
	{
		retry++;
		DWT_Delay_us(1);
	};	 
	if(retry>=100)return 1;
	else retry=0;
    while (!DHT11_DQ_IN&&retry<100)//DHT11���ͺ���ٴ�����40~80us
	{
		retry++;
		DWT_Delay_us(1);
	};
	if(retry>=100)return 1;	    
	return 0;
}

/****************************************************
*�������ܣ���DHT11��ȡһ��λ
*���������
*����ֵ  ��1/0
****************************************************/
u8 DHT11_Read_Bit(void) 			 
{
 	u8 retry=0;
	while(DHT11_DQ_IN&&retry<100)//�ȴ���Ϊ�͵�ƽ
	{
		retry++;
		DWT_Delay_us(1);
	}
	retry=0;
	while(!DHT11_DQ_IN&&retry<100)//�ȴ���ߵ�ƽ
	{
		retry++;
		DWT_Delay_us(1);
	}
	DWT_Delay_us(40);//�ȴ�40us
	if(DHT11_DQ_IN)return 1;
	else return 0;		   
}

/****************************************************
*�������ܣ���DHT11��ȡһ���ֽ�
*���������
*����ֵ  ������������
****************************************************/
u8 DHT11_Read_Byte(void)    
{        
    u8 i,dat;
    dat=0;
	for (i=0;i<8;i++) 
	{
   		dat<<=1; 
	    dat|=DHT11_Read_Bit();
    }						    
    return dat;
}

/****************************************************
*�������ܣ���DHT11��ȡһ������
*���������temp:�¶�ֵ(��Χ:0~50��) humi:ʪ��ֵ(��Χ:20%~90%)
*����ֵ  ��0,����;1,��ȡʧ��
****************************************************/
u8 DHT11_Read_Data(u8 *temp,u8 *humi)    
{        
 	u8 buf[5];
	u8 i;
	DHT11_Rst();
	if(DHT11_Check()==0)
	{
		for(i=0;i<5;i++)//��ȡ40λ����
		{
			buf[i]=DHT11_Read_Byte();
		}
		if((buf[0]+buf[1]+buf[2]+buf[3])==buf[4])
		{
			*humi=buf[0];
			*temp=buf[2];
		}
	}else return 1;
	return 0;	    
}

/****************************************************
*�������ܣ���ʼ��DHT11��IO�� DQ ͬʱ���DHT11�Ĵ���
*���������temp:�¶�ֵ(��Χ:0~50��) humi:ʪ��ֵ(��Χ:20%~90%)
*����ֵ  ������1:�����ڣ�����0:����   
****************************************************/
  	 
u8 DHT11_Init(void)
{
	RCC->AHB1ENR|=1<<1;    //ʹ��PORTBʱ��	   	  
	DH11_GPIO_Init();
	DHT11_Rst();
	return DHT11_Check();
}







