#include "adc.h"



/******************************************************************************************************
*	�� �� ��: ADC1_GPIO_Config
*	��ڲ���: ��
*	�� �� ֵ: ��
*	��������: ��ʼ��ADC1���������
*	˵    ��: 
*******************************************************************************************************/
static void Rheostat_ADC_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	// ʹ�� GPIO ʱ��
	RCC_AHB1PeriphClockCmd(RHEOSTAT_ADC_GPIO_CLK, ENABLE);
		
	// ���� IO
	GPIO_InitStructure.GPIO_Pin = RHEOSTAT_ADC_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;	    
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ; //������������
	GPIO_Init(RHEOSTAT_ADC_GPIO_PORT, &GPIO_InitStructure);		
}

/******************************************************************************************************
*	�� �� ��: ADC1_Init
*	��ڲ���: ��
*	�� �� ֵ: ��
*	��������: ��ʼ��ADC1
*	˵    ��: 
*******************************************************************************************************/
void Rheostat_Init(void)
{
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	ADC_InitTypeDef       ADC_InitStructure;
	
	Rheostat_ADC_GPIO_Config();		// ��ʼ��IO��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);	// ��ʼ��ADC1ʱ��
	
	ADC_CommonInitStructure.ADC_Mode 				= ADC_Mode_Independent;				// ����ģʽ
	ADC_CommonInitStructure.ADC_Prescaler 			= ADC_Prescaler_Div8; 				// ʱ�ӷ�Ƶ
	ADC_CommonInitStructure.ADC_DMAAccessMode		= ADC_DMAAccessMode_Disabled; 	// �ر�DMA
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_20Cycles;	// ����ʱ�������������ڶ��ز���ģʽ
	ADC_CommonInit(&ADC_CommonInitStructure);	

	ADC_InitStructure.ADC_NbrOfConversion 		 = 1;											// ת��ͨ������
	ADC_InitStructure.ADC_ScanConvMode 			 = DISABLE;									// ��ʹ��ɨ��
	ADC_InitStructure.ADC_ContinuousConvMode 	 = DISABLE;									// ��ʹ������ת��
	ADC_InitStructure.ADC_Resolution 			 = ADC_Resolution_12b;					// ADC�ֱ���12λ
	ADC_InitStructure.ADC_DataAlign 				 = ADC_DataAlign_Right;					// �����Ҷ���
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;	// ��ʹ���ⲿ����
	ADC_Init(ADC1, &ADC_InitStructure);

	ADC_RegularChannelConfig(ADC1, ADC_Channel_13, 1, ADC_SampleTime_480Cycles );	// ����ADC1_CH6����ʱ���Լ�ת��˳�򣬲���ʱ��Խ�þ���Խ��
	ADC_Cmd(ADC1, ENABLE);	// ʹ��ADC1
}

/******************************************************************************************************
*	�� �� ��: ADC1_GetVaule
*	��ڲ���: ��
*	�� �� ֵ: ADC1ת��ֵ
*	��������: ����ADת�����ɼ���ѹ
*	˵    ��: ��β�����ƽ��ֵ��������߲����ľ���
*******************************************************************************************************/

u16 ADC1_GetVaule(void)
{
	u8  i = 0;
	u32 AD_Vaule = 0;	// ADת��ֵ
	
	for(i=0;i<30;i++)	// ����30��ת��
	{
		ADC_SoftwareStartConv(ADC1);									// ����ת��
		while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));			//	�ȴ�ת�����
		AD_Vaule = AD_Vaule + ADC_GetConversionValue(ADC1);	// ���	
	}
	AD_Vaule=AD_Vaule / 30;	// ȡƽ��ֵ
	return (u16)AD_Vaule;	// ����ADת��ֵ
}
