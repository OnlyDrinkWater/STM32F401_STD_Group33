#define __ADC_GLOBAL

#include "adc.h"
#include "delay.h"		
#include "usart.h"




													   
void  Adc_Init(int mode)
{  
	
	

	sample_finish = 0;
	sample_index = 0;
	
	if(mode == SINGLEMODE)
	{
	GPIO_InitTypeDef  GPIO_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	ADC_InitTypeDef       ADC_InitStructure;
	
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//使能GPIOA时钟
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); //使能ADC1时钟

  //先初始化ADC1通道5 IO口
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;//PA5 通道5
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//模拟输入
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;//不带上下拉
  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化  
 
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,ENABLE);	  //ADC1复位
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,DISABLE);	//复位结束	 
 
	
  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;//独立模式
  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;//两个采样阶段之间的延迟5个时钟
  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled; //DMA失能
  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;//预分频4分频。ADCCLK=PCLK2/4=84/4=21Mhz,ADC时钟最好不要超过36Mhz 
  ADC_CommonInit(&ADC_CommonInitStructure);//初始化
	
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;//12位模式
  ADC_InitStructure.ADC_ScanConvMode = DISABLE;//非扫描模式	
  ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;//关闭连续转换
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//禁止触发检测，使用软件触发
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//右对齐	
  ADC_InitStructure.ADC_NbrOfConversion = 1;//1个转换在规则序列中 也就是只转换规则序列1 

  ADC_Init(ADC1, &ADC_InitStructure);//ADC初始化
	
	ADC1->CR2 = ((uint32_t)0x0D0D0001) ;
	
	}
	else if(mode == SCANMODE)
	{
		GPIO_InitTypeDef  GPIO_InitStructure;
		DMA_InitTypeDef dma_init_structure;
    NVIC_InitTypeDef nvic_init_structure;
 

		
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);


		
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_4;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
		
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
		GPIO_Init(GPIOB, &GPIO_InitStructure); 
		
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_0;
		GPIO_Init(GPIOC, &GPIO_InitStructure); 
		
//----------------------------------------------------------------------------------------------------------------------------

				
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); 
		
		RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,ENABLE);	  
		RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,DISABLE);	 
	 /*
		ADC_DeInit();
		
		ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;            //????????
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;         //???????
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T2_TRGO; //???????TIM2
    ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_Rising;//?????
    ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;         //12????
    
    ADC_Init(ADC1, &ADC_InitStructure);
		
		ADC_InitStructure.ADC_NbrOfConversion = 1;
		ADC1->CR2 &= ~(1<<8); //dma fail to turn down by the library function, here to turn it down forcely
		ADC_Init(ADC1, &ADC_InitStructure);
		
	 
		ADC_Cmd(ADC1, ENABLE);

		
		*/
		/*
		    ADC_InitTypeDef adc_init_structure;
 
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);            //??ADC??
 
    ADC_DeInit(ADC1);                                               //??ADC
    ADC_StructInit(&adc_init_structure);                            //???ADC???
 
    adc_init_structure.ADC_ContinuousConvMode = DISABLE;            //????????
    adc_init_structure.ADC_DataAlign = ADC_DataAlign_Right;         //???????
    adc_init_structure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T2_TRGO; //???????TIM2
    adc_init_structure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_Rising;//?????
    adc_init_structure.ADC_Resolution = ADC_Resolution_12b;         //12????
    adc_init_structure.ADC_ScanDirection = ADC_ScanDirection_Upward;//????0-18??
    ADC_Init(ADC1, &adc_init_structure);
 
    ADC_OverrunModeCmd(ADC1, ENABLE);                               //????????
    ADC_ChannelConfig(ADC1, ADC_Channel_0 | ADC_Channel_1 | ADC_Channel_2
                          | ADC_Channel_8 | ADC_Channel_14 | ADC_Channel_15,
                          ADC_SampleTime_13_5Cycles);               //??????,????125nS
    ADC_GetCalibrationFactor(ADC1);                                 //?????ADC
    ADC_Cmd(ADC1, ENABLE);                                          //??ADC1
    while(ADC_GetFlagStatus(ADC1, ADC_FLAG_ADEN) == RESET);         //??ADC1????
 
    ADC_DMACmd(ADC1, ENABLE);                                       //??ADC_DMA
    ADC_DMARequestModeConfig(ADC1, ADC_DMAMode_Circular);           //??DMA?????????
    ADC_StartOfConversion(ADC1);                                    //??????(??)

		
		*/

		
		ADC->CCR = ((uint32_t)0x00010000);
		ADC1->CR1 = ((uint32_t)0x0400A13F);
		ADC1->CR2 = ((uint32_t)0x0D0D0103);//bit 9 
		ADC1->SQR1 = ((uint32_t)0x00000000);//
		ADC1->SQR2 = ((uint32_t)0x00000000);//
		ADC1->SQR3 = ((uint32_t)0x14B41020);//     01010  01011  01000   00100   00001   00000
		ADC1->SMPR2 = ((uint32_t)0x001fffff);
		
		
//----------------------------------------------------------------------------------------------------------------------------
		
		

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2 , ENABLE);              //??DMA??
 
    nvic_init_structure.NVIC_IRQChannel = DMA2_Stream0_IRQn;       //??DMA1????
    nvic_init_structure.NVIC_IRQChannelCmd = ENABLE;                //????
    nvic_init_structure.NVIC_IRQChannelPreemptionPriority = 0; 
		nvic_init_structure.NVIC_IRQChannelSubPriority = 0;		//?????0
    NVIC_Init(&nvic_init_structure);
 
    DMA_DeInit(DMA2_Stream0);
	
	while (DMA_GetCmdStatus(DMA2_Stream0) != DISABLE){}//等待DMA可配置 
	
  /* 配置 DMA Stream */
  dma_init_structure.DMA_Channel = 0;  //通道选择
  dma_init_structure.DMA_PeripheralBaseAddr = (uint32_t) &(ADC1->DR);//DMA外设地址
  dma_init_structure.DMA_Memory0BaseAddr = (uint32_t)&adc_dma_tab[0];;//DMA 存储器0地址
  dma_init_structure.DMA_DIR = DMA_DIR_MemoryToPeripheral;//存储器到外设模式
  dma_init_structure.DMA_BufferSize = 6;//数据传输量 
  dma_init_structure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//外设非增量模式
  dma_init_structure.DMA_MemoryInc = DMA_MemoryInc_Enable;//存储器增量模式
  dma_init_structure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;//外设数据长度:8位
  dma_init_structure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;//存储器数据长度:8位
  dma_init_structure.DMA_Mode = DMA_Mode_Normal;// 使用普通模式 
  dma_init_structure.DMA_Priority = DMA_Priority_Medium;//中等优先级
  dma_init_structure.DMA_FIFOMode = DMA_FIFOMode_Disable;         
  dma_init_structure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
  dma_init_structure.DMA_MemoryBurst = DMA_MemoryBurst_Single;//存储器突发单次传输
  dma_init_structure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;//外设突发单次传输
  DMA_Init(DMA2_Stream0, &dma_init_structure);//初始化DMA Stream
		
	DMA_Cmd(DMA2_Stream0, DISABLE);                      //关闭DMA传输 
	
	while (DMA_GetCmdStatus(DMA2_Stream0) != DISABLE){}	//确保DMA可以被设置  
		
	DMA_SetCurrDataCounter(DMA2_Stream0,6);          //数据传输量  
 
	DMA_Cmd(DMA2_Stream0, ENABLE);                      //开启DMA传输 
//---------------------------------------------------------------------------------------------------------------


	}
}				  
//获得ADC值
//ch: @ref ADC_channels 
//通道值 0~16取值范围为：ADC_Channel_0~ADC_Channel_16
//返回值:转换结果
u16 Get_Adc(u8 ch)   
{
	  	//设置指定ADC的规则组通道，一个序列，采样时间
	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_480Cycles );	//ADC1,ADC通道,480个周期,提高采样时间可以提高精确度			    
  
	ADC_SoftwareStartConv(ADC1);		//使能指定的ADC1的软件转换启动功�

	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//等待转换结束

	return ADC_GetConversionValue(ADC1);	//返回最近一次ADC1规则组的转换结果
}
//获取通道ch的转换值，取times次,然后平均 
//ch:通道编号
//times:获取次数
//返回值:通道ch的times次转换结果平均值
u16 Get_Adc_Average(u8 ch,u8 times)
{
	u32 temp_val=0;
	u8 t;
	for(t=0;t<times;t++)
	{
		temp_val+=Get_Adc(ch);
		delay_ms(5);
	}
	return temp_val/times;
} 

void MySingleAdcRead()
{
	u16 adcx;
	float temp;

	adcx=Get_Adc_Average(0,20);//???? 5 ????,20 ????

	temp=(float)adcx*(3.3/4096); //???????????????,?? 3.1111
	printf("%f \n",temp);
	delay_ms(250); 
		
}


void DMA2_Stream0_IRQHandler()
{
	GPIO_SetBits(GPIOA,GPIO_Pin_5);
    if(DMA_GetITStatus(DMA2_Stream4,DMA_IT_TC))                      //??DMA??????
    {
        if(sample_finish == 0)
        {
            sample_1[sample_index] = adc_dma_tab[0];
            sample_2[sample_index] = adc_dma_tab[1];
            sample_3[sample_index] = adc_dma_tab[2];
            sample_4[sample_index] = adc_dma_tab[3];
            sample_5[sample_index] = adc_dma_tab[5];
            sample_6[sample_index] = adc_dma_tab[4];
            sample_index++;
        }
        if(sample_index >= 128)                         //??????????????
        {
            sample_index = 0;                   //??????,?????
            DMA_Cmd(DMA2_Stream0, DISABLE);            //??????,??DMA
            sample_finish = 1;                          //????????
        }
    }
    DMA_ClearITPendingBit(DMA2_Stream0,DMA_IT_TC);                   //??DMA?????
}


	 









