
#ifndef	__ADC_H
#define	__ADC_H

#include	"config.h"
#include 	"Define.h"
#include 	"Function_define.h"
#include 	"GPIO.h"

/*************	���س�������	**************/

//#define	ADC_P11		0x02	//IO���� P11
//#define	ADC_P03		0x08	//IO���� P03
//#define	ADC_P04		0x10	//IO���� P04
//#define	ADC_P05		0x20	//IO���� P05
//#define	ADC_P06		0x40	//IO���� P06
//#define	ADC_P07		0x80	//IO���� P07
//#define	ADC_P30		0x01	//IO���� P30
//#define	ADC_P17		0x80	//IO���� P17
//#define GPIO_Px		1		//0.1.3

#define ADC_90T		(3<<5)
#define ADC_180T	(2<<5)
#define ADC_360T	(1<<5)
#define ADC_540T	0
#define ADC_FLAG	(1<<4)	//�����0
#define ADC_START	(1<<3)	//�Զ���0
#define ADC_CH0		0
#define ADC_CH1		1
#define ADC_CH2		2
#define ADC_CH3		3
#define ADC_CH4		4
#define ADC_CH5		5
#define ADC_CH6		6
#define ADC_CH7		7

#define ADC_RES_H2L8	1
#define ADC_RES_H8L2	0


/*************	���ر�������	**************/


/*************	���غ�������	**************/
typedef struct
{
	u8	ADC_Px;			//����Ҫ��ADC��IO,	ADC_P10 ~ ADC_P17,ADC_P1_All
	u8	ADC_Speed;		//ADC�ٶ�			ADC_90T,ADC_180T,ADC_360T,ADC_540T
	u8	ADC_Power;		//ADC��������/�ر�	ENABLE,DISABLE
	u8	ADC_AdjResult;	//ADC�������,	ADC_RES_H2L8,ADC_RES_H8L2
	u8	ADC_Polity;		//���ȼ�����	PolityHigh,PolityLow
	u8	ADC_Interrupt;	//�ж�����		ENABLE,DISABLE
} ADC_InitTypeDef;

void	ADC_Inilize(u8 GPIO, ADC_InitTypeDef *ADCx);
void	ADC_PowerControl(u8 pwr);
u16		Get_ADC10bitResult(u8 channel);	//channel = 0~7

/*************  �ⲿ�����ͱ������� *****************/

#endif
