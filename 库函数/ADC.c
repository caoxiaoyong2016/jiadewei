

#include	"adc.h"


//========================================================================
// ����: void	ADC_Inilize(ADC_InitTypeDef *ADCx)
// ����: ADC��ʼ������.
// ����: ADCx: �ṹ����,��ο�adc.h��Ķ���.
// ����: none.
// �汾: V1.0, 2012-10-22
//========================================================================
void	ADC_Inilize(u8 GPIO, ADC_InitTypeDef *ADCx)
{
/******************ADCͨ��ѡ��********************/	
	#if 1
/******************	IO����	********************/
	GPIO_InitTypeDef	GPIO_InitStructure;		//�ṹ����
	GPIO_InitStructure.Pin  = ADCx->ADC_Px;
	GPIO_InitStructure.Mode = GPIO_HighZ;		//��������
	GPIO_Inilize(GPIO,&GPIO_InitStructure);
	#endif
	
	ADCCON0&=0xF0;
	//if(GPIO==1 && GPIO_InitStructure.Pin==1)
	AINDIDS=0x00;
	//Enable_ADC_AIN0;
	#if 0
	P1ASF = ADCx->ADC_Px;
	ADC_CONTR = (ADC_CONTR & ~ADC_90T) | ADCx->ADC_Speed;
	if(ADCx->ADC_Power == ENABLE)	ADC_CONTR |= 0x80;
	else							ADC_CONTR &= 0x7F;
	if(ADCx->ADC_AdjResult == ADC_RES_H2L8)	PCON2 |=  (1<<5);	//10λAD����ĸ�2λ��ADC_RES�ĵ�2λ����8λ��ADC_RESL��
	else									PCON2 &= ~(1<<5);	//10λAD����ĸ�8λ��ADC_RES����2λ��ADC_RESL�ĵ�2λ��
	if(ADCx->ADC_Interrupt == ENABLE)	EADC = 1;			//�ж�����		ENABLE,DISABLE
	else								EADC = 0;
	if(ADCx->ADC_Polity == PolityHigh)	PADC = 1;		//���ȼ�����	PolityHigh,PolityLow
	else								PADC = 0;
	#endif
}


//========================================================================
// ����: void	ADC_PowerControl(u8 pwr)
// ����: ADC��Դ���Ƴ���.
// ����: pwr: ��Դ����,ENABLE��DISABLE.
// ����: none.
// �汾: V1.0, 2012-10-22
//========================================================================
void	ADC_PowerControl(u8 pwr)
{
	#if 0
	if(pwr == ENABLE)	ADC_CONTR |= 0x80;
	else				ADC_CONTR &= 0x7f;
	#endif
}

//========================================================================
// ����: u16	Get_ADC10bitResult(u8 channel)
// ����: ��ѯ����һ��ADC���.
// ����: channel: ѡ��Ҫת����ADC.
// ����: 10λADC���.
// �汾: V1.0, 2012-10-22
//========================================================================
u16	Get_ADC10bitResult(u8 channel)	//channel = 0~7
{
	if(channel > ADC_CH7)	return	1024;	//����,����1024,���õĳ����ж�	
	
	ADCCON0|=channel;
	switch(channel)
	{
		case	ADC_CH0:
			AINDIDS|=SET_BIT0;
			break;
		case	ADC_CH1:
			AINDIDS|=SET_BIT1;
			break;
		case	ADC_CH2:
			AINDIDS|=SET_BIT2;
			break;
		case	ADC_CH3:
			AINDIDS|=SET_BIT3;
			break;
		case	ADC_CH4:
			AINDIDS|=SET_BIT4;
			break;
		case	ADC_CH5:
			AINDIDS|=SET_BIT5;
			break;
		case	ADC_CH6:
			AINDIDS|=SET_BIT6;
			break;
		case	ADC_CH7:
			AINDIDS|=SET_BIT7;
			break;
		default:
			break;
	}
	ADCCON1|=SET_BIT0;
	
	#if 0
	u16	adc;
	u8	i;

	if(channel > ADC_CH7)	return	1024;	//����,����1024,���õĳ����ж�	
	ADC_RES = 0;
	ADC_RESL = 0;

	ADC_CONTR = (ADC_CONTR & 0xe0) | ADC_START | channel; 
	NOP(4);			//��ADC_CONTR������Ҫ4T֮����ܷ���

	for(i=0; i<250; i++)		//��ʱ
	{
		if(ADC_CONTR & ADC_FLAG)
		{
			ADC_CONTR &= ~ADC_FLAG;
			if(PCON2 &  (1<<5))		//10λAD����ĸ�2λ��ADC_RES�ĵ�2λ����8λ��ADC_RESL��
			{
				adc = (u16)(ADC_RES & 3);
				adc = (adc << 8) | ADC_RESL;
			}
			else		//10λAD����ĸ�8λ��ADC_RES����2λ��ADC_RESL�ĵ�2λ��
			{
				adc = (u16)ADC_RES;
				adc = (adc << 2) | (ADC_RESL & 3);
			}
			return	adc;
		}
	}
	return	1024;	//����,����1024,���õĳ����ж�
	#endif
	return 1024;
}


//========================================================================
// ����: void ADC_int(void) interrupt ADC_VECTOR
// ����: ADC�жϺ���.
// ����: none.
// ����: none.
// �汾: V1.0, 2012-10-22
//========================================================================
//void ADC_int (void) interrupt ADC_VECTOR
//{
//	#if 0
//	ADC_CONTR &= ~ADC_FLAG;
//	#endif
//}


