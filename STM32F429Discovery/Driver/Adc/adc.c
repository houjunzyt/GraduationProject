#include "adc.h"



/******************************************************************************************************
*	函 数 名: ADC1_GPIO_Config
*	入口参数: 无
*	返 回 值: 无
*	函数功能: 初始化ADC1的相关引脚
*	说    明: 
*******************************************************************************************************/
static void Rheostat_ADC_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	// 使能 GPIO 时钟
	RCC_AHB1PeriphClockCmd(RHEOSTAT_ADC_GPIO_CLK, ENABLE);
		
	// 配置 IO
	GPIO_InitStructure.GPIO_Pin = RHEOSTAT_ADC_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;	    
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ; //不上拉不下拉
	GPIO_Init(RHEOSTAT_ADC_GPIO_PORT, &GPIO_InitStructure);		
}

/******************************************************************************************************
*	函 数 名: ADC1_Init
*	入口参数: 无
*	返 回 值: 无
*	函数功能: 初始化ADC1
*	说    明: 
*******************************************************************************************************/
void Rheostat_Init(void)
{
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	ADC_InitTypeDef       ADC_InitStructure;
	
	Rheostat_ADC_GPIO_Config();		// 初始化IO口
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);	// 初始化ADC1时钟
	
	ADC_CommonInitStructure.ADC_Mode 				= ADC_Mode_Independent;				// 独立模式
	ADC_CommonInitStructure.ADC_Prescaler 			= ADC_Prescaler_Div8; 				// 时钟分频
	ADC_CommonInitStructure.ADC_DMAAccessMode		= ADC_DMAAccessMode_Disabled; 	// 关闭DMA
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_20Cycles;	// 采样时间间隔，仅适用于多重采样模式
	ADC_CommonInit(&ADC_CommonInitStructure);	

	ADC_InitStructure.ADC_NbrOfConversion 		 = 1;											// 转换通道数量
	ADC_InitStructure.ADC_ScanConvMode 			 = DISABLE;									// 不使用扫描
	ADC_InitStructure.ADC_ContinuousConvMode 	 = DISABLE;									// 不使用连续转换
	ADC_InitStructure.ADC_Resolution 			 = ADC_Resolution_12b;					// ADC分辨率12位
	ADC_InitStructure.ADC_DataAlign 				 = ADC_DataAlign_Right;					// 数据右对齐
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;	// 不使用外部触发
	ADC_Init(ADC1, &ADC_InitStructure);

	ADC_RegularChannelConfig(ADC1, ADC_Channel_13, 1, ADC_SampleTime_480Cycles );	// 设置ADC1_CH6采样时间以及转换顺序，采样时间越久精度越高
	ADC_Cmd(ADC1, ENABLE);	// 使能ADC1
}

/******************************************************************************************************
*	函 数 名: ADC1_GetVaule
*	入口参数: 无
*	返 回 值: ADC1转换值
*	函数功能: 进行AD转换，采集电压
*	说    明: 多次采样求平均值，可以提高测量的精度
*******************************************************************************************************/

u16 ADC1_GetVaule(void)
{
	u8  i = 0;
	u32 AD_Vaule = 0;	// AD转换值
	
	for(i=0;i<30;i++)	// 进行30次转换
	{
		ADC_SoftwareStartConv(ADC1);									// 启动转换
		while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));			//	等待转换完毕
		AD_Vaule = AD_Vaule + ADC_GetConversionValue(ADC1);	// 求和	
	}
	AD_Vaule=AD_Vaule / 30;	// 取平均值
	return (u16)AD_Vaule;	// 返回AD转换值
}
