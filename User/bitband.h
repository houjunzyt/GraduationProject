#include "stm32f4xx.h"

// ����ֻ������ GPIO ODR��IDR�������Ĵ�����λ����������ַ�������Ĵ�����û�ж���

//SRAM λ����:    0X2000 0000~0X200F 0000
//SRAM λ��������:0X2200 0000~0X23FF FFFF

//���� λ����:    0X4000 0000~0X400F FFFF
//���� λ��������:0X4200 0000~0X43FF FFFF

// �ѡ�λ����ַ+λ��š�ת���ɱ�����ַ�ĺ�
#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x02000000+((addr & 0x000FFFFF)<<5)+(bitnum<<2)) 

// ��һ����ַת����һ��ָ��
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr))

// ��λ����������ַת����ָ��
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum)) 


// GPIO ODR �� IDR �Ĵ�����ַӳ�� 
#define GPIOA_ODR_Addr    (GPIOA_BASE+20) 
#define GPIOB_ODR_Addr    (GPIOB_BASE+20)   
#define GPIOC_ODR_Addr    (GPIOC_BASE+20)  
#define GPIOD_ODR_Addr    (GPIOD_BASE+20) 
#define GPIOE_ODR_Addr    (GPIOE_BASE+20) 
#define GPIOF_ODR_Addr    (GPIOF_BASE+20)      
#define GPIOG_ODR_Addr    (GPIOG_BASE+20)
#define GPIOH_ODR_Addr    (GPIOH_BASE+20)      
#define GPIOI_ODR_Addr    (GPIOI_BASE+20)
#define GPIOJ_ODR_Addr    (GPIOJ_BASE+20)      
#define GPIOK_ODR_Addr    (GPIOK_BASE+20)

#define GPIOA_IDR_Addr    (GPIOA_BASE+16)  
#define GPIOB_IDR_Addr    (GPIOB_BASE+16)  
#define GPIOC_IDR_Addr    (GPIOC_BASE+16)   
#define GPIOD_IDR_Addr    (GPIOD_BASE+16)  
#define GPIOE_IDR_Addr    (GPIOE_BASE+16)    
#define GPIOF_IDR_Addr    (GPIOF_BASE+16)    
#define GPIOG_IDR_Addr    (GPIOG_BASE+16)  
#define GPIOH_IDR_Addr    (GPIOH_BASE+16)
#define GPIOI_IDR_Addr    (GPIOI_BASE+16)
#define GPIOJ_IDR_Addr    (GPIOJ_BASE+16)
#define GPIOK_IDR_Addr    (GPIOK_BASE+16)


// �������� GPIO��ĳһ��IO�ڣ�n(0,1,2...16),n��ʾ��������һ��IO��
#define PAout(n)   BIT_ADDR(GPIOA_ODR_Addr,n)  //���   
#define PAin(n)    BIT_ADDR(GPIOA_IDR_Addr,n)  //����   
  
#define PBout(n)   BIT_ADDR(GPIOB_ODR_Addr,n)  //���   
#define PBin(n)    BIT_ADDR(GPIOB_IDR_Addr,n)  //����   
  
#define PCout(n)   BIT_ADDR(GPIOC_ODR_Addr,n)  //���   
#define PCin(n)    BIT_ADDR(GPIOC_IDR_Addr,n)  //����   
  
#define PDout(n)   BIT_ADDR(GPIOD_ODR_Addr,n)  //���   
#define PDin(n)    BIT_ADDR(GPIOD_IDR_Addr,n)  //����   
  
#define PEout(n)   BIT_ADDR(GPIOE_ODR_Addr,n)  //���   
#define PEin(n)    BIT_ADDR(GPIOE_IDR_Addr,n)  //����  
  
#define PFout(n)   BIT_ADDR(GPIOF_ODR_Addr,n)  //���   
#define PFin(n)    BIT_ADDR(GPIOF_IDR_Addr,n)  //����  
  
#define PGout(n)   BIT_ADDR(GPIOG_ODR_Addr,n)  //���   
#define PGin(n)    BIT_ADDR(GPIOG_IDR_Addr,n)  //����  

#define PHout(n)   BIT_ADDR(GPIOH_ODR_Addr,n)  //���   
#define PHin(n)    BIT_ADDR(GPIOH_IDR_Addr,n)  //����  

#define PIout(n)   BIT_ADDR(GPIOI_ODR_Addr,n)  //���   
#define PIin(n)    BIT_ADDR(GPIOI_IDR_Addr,n)  //���� 

#define PJout(n)   BIT_ADDR(GPIOJ_ODR_Addr,n)  //���   
#define PJin(n)    BIT_ADDR(GPIOJ_IDR_Addr,n)  //����  

#define PKout(n)   BIT_ADDR(GPIOK_ODR_Addr,n)  //���   
#define PKin(n)    BIT_ADDR(GPIOK_IDR_Addr,n)  //����  
