/***************************************************************************
ģ    ��:LCD1602.c  
˵    ��:LCD1602��������
��    ��:Version2.0.0		
2016/01/06 12:08(OK)
���뻷��:Keil 8051 C complier V9.01
����оƬ:STC12C5A60S2  		
@11.0592MHZ
��    ��:����
��ϵ��ʽ:��QQ��279729201		 
��Email��279729201@qq.com            		          
yangrui90s@163.com		 
�޸ļ�¼:
=================	
2017/01/6 12:08
	��¼:		
1.��������ע�͡�		
2.�޸ĺ�������ģʽ����д��ĸ��Ϊ��д������lcd1602ReadStatus�޸�ΪLcd1602ReadStatus��		
3.�޸���ʱ��������Delay1msForLcd1602�����޸�ΪDelay80usForLcd1602������ע�⣬��������������		  
��STC-ISP���ɣ�Delay80usForLcd1602������STC-ISP V6.85Q�ġ������ʱ�����������STC-Y3ָ�		  
@11.0592MHZ�Զ����ɡ��������ԣ���ʱ59us���ܻ����ֻ��д�������ݵ��������ʱ60us���ÿ��ԡ�		  
Ϊ�˰�ȫ��Ҳ���Խ�����ʱ�޸ĵĸ�����
=================

=================	
2014/04/31 20:09	
��¼:	
1.����󹤳��У���δ��װLCD1602�����µĹ��̿����ں���	
lcd1602CheckBusy()�����⣬��		
do
{
	;		
}
while( (lcd1602ReadStatus()) & 0x80));		
�޸�Ϊ		
do
{			
	i++;		
}while( ((lcd1602ReadStatus()) & 0x80)  && (i<10));		
��Ϊ��δ��װLCD1602ʱ��ͨ��"��״̬"����lcd1602ReadStatus()����������Զ��0xff,		
������õ�һ���㷨���ᵼ�³���"����"��
=================

=================	
2014/02/24 23:44
��¼:	
1.���Ӻ���lcd1602AddressWriteString(...)
=================

=================
2014/02/24 15:00��¼:
1.���Ӻ���delay1msForLcd1602()	  
STC12C5A60S2��Ƭ�������ٶȿ�ȴ�ͳ8051�죬ĳЩ������ʱ�����㹻��
=================
***************************************************************************/

#include <intrins.h>
#include <string.h>
#include "1602.h"
/*�ⲿ�ӿں�����lcd1602.h������*/
/*****************�ڲ�����******************/
static void Delay80usForLcd1602(void) ;
static UB8 Lcd1602ReadStatus(void) ;
static void Lcd1602CheckBusy(void) ;
/**********************************************/

/******************************************************************
- �������ƣ�Delay80usForLcd1602
- ������������ʱ����
- ����ģ�飺LCD1602ģ��
- �������ԣ��ڲ�����
- ����˵������
- ����˵������
- ע���˺�����STC-ISP V6.85Q�ġ������ʱ�����������STC-Y3ָ� @11.0592MHZ�Զ����ɣ���    
ָ�����ͬ���������޸ġ�
******************************************************************/
static void Delay80usForLcd1602(void)
{
	unsigned char i, j;
	_nop_();
	_nop_();
	_nop_();
	i = 1;
	j = 216;
	do
	{
		while (--j);
	} while (--i);
}

/******************************************************************
- �������ƣ�Lcd1602ReadStatus
- ������������ȡLCD1602״ֵ̬
- ����ģ�飺LCD1602ģ��
- �������ԣ��ڲ�����
- ����˵������
- ����˵����LCD1602��״ֵ̬
******************************************************************/
static UB8 Lcd1602ReadStatus(void)
{
	UB8 statusCode ;
	lcd1602_en_bit = LCD1602_DISABLE ;			/*��ֹ����LCD1602*/
	lcd1602_rs_bit = LCD1602_COMMAND_OPERATION ;/*����ģʽ*/
	lcd1602_rw_bit = LCD1602_READ_OPERATION ;	/*������*/
	LCD1602_DATA_PORT = 0xff ;
	Delay80usForLcd1602() ;
	lcd1602_en_bit = LCD1602_ENABLE ;			/*�������LCD1602*/
	Delay80usForLcd1602() ;
	statusCode = LCD1602_DATA_PORT ;			/*��ȡ״ֵ̬*/
	lcd1602_en_bit = LCD1602_DISABLE ;			/*��ֹ����LCD1602*/
	return statusCode ;
}

/******************************************************************
- �������ƣ�Lcd1602CheckBusy
- �����������ж�LCD1602�Ƿ�æµ����æµ��ȴ�����������ִ������
- ����ģ�飺LCD1602ģ��
- �������ԣ��ڲ����� 
- ����˵������ 
- ����˵������
- ע����LCD1602_DATA_PORT[7]����1����ʾLCD1602æµ������Ҫ�ȴ�һ���   
��LCD1602_DATA_PORT[7]����0����ʾLCD1602���У�����Ҫ����ִ�к��������				  
LCD1602_DATA_PORT[7]==0,lcd1602 is free.
- ���䣺����ġ�10���Ǿ������Եģ������У����ô��ڴ�ӡ
��i��ֵΪ1������д10�Ѿ��㹻���ˡ�
******************************************************************/
static void Lcd1602CheckBusy(void)
{
	UW16 i=0;
	do
	{
		i++;
	}while( ((Lcd1602ReadStatus()) & 0x80) && (i<10));
}

/******************************************************************
- �������ƣ�Lcd1602WriteCommand
- ����������LCD1602д����
- ����ģ�飺LCD1602ģ��
- �������ԣ��ⲿ���������û�����
- ����˵������Ҫ���͸�LCD1602��ָ��ֵ
- ����˵������
******************************************************************/

void Lcd1602WriteCommand(UB8 commandCode)
{
	Lcd1602CheckBusy();
	lcd1602_en_bit = LCD1602_DISABLE ;			/*��ֹ����LCD1602*/
	lcd1602_rs_bit = LCD1602_COMMAND_OPERATION ;/*����ģʽ*/
	lcd1602_rw_bit = LCD1602_WRITE_OPERATION ;	/*д����*/
	LCD1602_DATA_PORT = commandCode ;			/*����ָ��ֵ*/
	Delay80usForLcd1602();
	lcd1602_en_bit = LCD1602_ENABLE ;			/*�������LCD1602*/
	Delay80usForLcd1602();
	lcd1602_en_bit = LCD1602_DISABLE ;			/*��ֹ����LCD1602*/
}

/******************************************************************
- �������ƣ�Lcd1602WriteData
- ����������LCD1602д����
- ����ģ�飺LCD1602ģ��
- �������ԣ��ⲿ���������û�����
- ����˵������Ҫ���͸�LCD1602������
- ����˵������
******************************************************************/

void Lcd1602WriteData(UB8 dataCode)
{	
	Lcd1602CheckBusy() ;
	lcd1602_en_bit = LCD1602_DISABLE ;			/*��ֹ����LCD1602*/
	lcd1602_rs_bit = LCD1602_DATA_OPERATION ;	/*����ģʽ*/
	lcd1602_rw_bit = LCD1602_WRITE_OPERATION ;	/*д����*/
	LCD1602_DATA_PORT = dataCode;				/*��������*/
	Delay80usForLcd1602();
	lcd1602_en_bit = LCD1602_ENABLE ;			/*�������LCD1602*/
	Delay80usForLcd1602();
	lcd1602_en_bit = LCD1602_DISABLE ;	 		/*��ֹ����LCD1602*/ 
}


/******************************************************************
- �������ƣ�Lcd1602CleanAll
- ���������������Ļ��ʾ
- ����ģ�飺LCD1602ģ��
- �������ԣ��ⲿ���������û�����
- ����˵������
- ����˵������
- ע�������Ļ��ʾ������λ(���Ͻ�)����ַ������AC��Ϊ0
******************************************************************/

void Lcd1602CleanAll(void)
{
	Lcd1602WriteCommand(LCD1602_CLEAN_ALL_DISPALY);
}


/******************************************************************
- �������ƣ�Lcd1602CursorHoming
- ��������������λ
- ����ģ�飺LCD1602ģ��
- �������ԣ��ⲿ���������û�����
- ����˵������
- ����˵������
- ע������Ϊ������Ļ�ƶ���ʾʱ��lcd1602��ʾ�������ݺ�
	   ���ô˺�������Ļ��ʾ�����ж��������λ������ڵ�һ
	   ��λ��(0x80)��
******************************************************************/

void Lcd1602CursorHoming(void)
{
	Lcd1602WriteCommand(LCD1602_CURSOR_RETURN_TO_ORIGIN);
}


/******************************************************************
 
- �������ƣ�Lcd1602Init
- ����������LCD1602��ʼ�� 
- ����ģ�飺LCD1602ģ��
- �������ԣ��ⲿ���������û�����
- ����˵������
- ����˵������
- ע������
��1����ʾģʽ
��2��Һ����ʾ������ֹ�������ʾ������ֹ�������˸������ֹ
��3����ַָ���һ���һ�����ָ���һ���һ����Ļ���ƻ�����
��ͨ���޸�LCD1602.H�е�LCD1602_DEFAULT_DISPALY_MODE��LCD1602_DEFAULT_DISPLAY_AND_CURSOR_MODE��
 LCD1602_DEFAULT_POINT_AND_POINT_ADDRESS_MODE�ﵽ��ͬ����ʾЧ����
******************************************************************/

void Lcd1602Init(void)
{
		lcd1602_en_bit = LCD1602_DISABLE ;
		Lcd1602CleanAll();
		Lcd1602WriteCommand(LCD1602_DEFAULT_DISPALY_MODE);
		Lcd1602WriteCommand(LCD1602_DEFAULT_DISPLAY_AND_CURSOR_MODE);
		Lcd1602WriteCommand(LCD1602_DEFAULT_POINT_AND_POINT_ADDRESS_MODE);
		/*�ɺ��ԣ���Lcd1602CleanAll()�������˸ù���*/
		//Lcd1602CursorHoming(); 
		lcd1602_en_bit = LCD1602_DISABLE ;
}


/******************************************************************
- �������ƣ�Lcd1602AddressWriteByte
- ������������LCD1602��row��column��д������dataCode
- ����ģ�飺LCD1602ģ��
- �������ԣ��ⲿ���������û�����
- ����˵����row-->�е�ַ����ЧֵΪLCD1602_ROW0��LCD1602_ROW1
column-->�е�ַ����ЧֵΪ0~39֮�������
dataCode-->��Ҫд������ݡ�
- ����˵����0���ɹ�������-1��ʧ�ܣ�
- ע���е�ַ���е�ַ���Ǵ�0��ʼ�ġ�
����LCD1602�ĵ�0�е�2����ʾ���ﲮ����5�����÷�ʽΪ
 Lcd1602AddressWriteByte(LCD1602_ROW0,2,��5������
��DEBUGģʽ�£������ν�����Ч���жϣ�Э�����������Ժ��ڿ�ͨ��
����common.h�е�"#define DEBUG  1"��СĿ���ļ��Ĵ�С��   
******************************************************************/

SB8 Lcd1602AddressWriteByte(UB8 row,UB8 column,UB8 dataCode)
{
	#ifdef   DEBUG
	if(column< LCD1602_MIN_COLUMN|| column > LCD1602_MAX_COLUMN|| \
	( (row != LCD1602_ROW0) &&(row != LCD1602_ROW1)))
	{
			return LCD1602_ROW_OR_COLUMN_OVERFLOW ;
	}
	#endif /*DEBUG*/
	if(row == LCD1602_ROW0)
	{
		Lcd1602WriteCommand(LCD1602_ROW0_ADDRESS_START+column) ;
	}
	else if(row == LCD1602_ROW1)
	{
		Lcd1602WriteCommand(LCD1602_ROW1_ADDRESS_START+column) ;
	}
	Lcd1602WriteData(dataCode);
	return 0;
}

/******************************************************************
- �������ƣ�Lcd1602AddressWriteString
- ������������LCD1602��row��column�п�ʼд���ַ���stringCode
- ����ģ�飺LCD1602ģ��
- �������ԣ��ⲿ���������û�����
- ����˵����row-->�е�ַ����ЧֵΪLCD1602_ROW0��LCD1602_ROW1
						column-->�е�ַ����ЧֵΪ0~39֮�������
						stringCode-->�ַ���
- ����˵����0���ɹ�������-1��ʧ�ܣ�
- ע���е�ַ���е�ַ���Ǵ�0��ʼ�ġ�
			����LCD1602�ĵ�0�е�2����ʾ"test"�����÷�ʽΪ
			Lcd1602AddressWriteString(LCD1602_ROW0,2,"test"����
			����ʹ�õ���strlen��������sizeof��
			��DEBUGģʽ�£������ν�����Ч���жϣ�Э�����������Ժ��ڿ�ͨ��
			����common.h�е�"#define DEBUG  1"��СĿ���ļ��Ĵ�С��
******************************************************************/

SB8 Lcd1602AddressWriteString(UB8 row,UB8 column,UB8 *stringCode)
{
		UB8 length = strlen(stringCode) ; 
		#ifdef DEBUG
		if(column< LCD1602_MIN_COLUMN|| (column+strlen(stringCode)-1) > LCD1602_MAX_COLUMN|| \
		( (row != LCD1602_ROW0) &&(row != LCD1602_ROW1)))
		{
					return LCD1602_ROW_OR_COLUMN_OVERFLOW ;
		}
		#endif	/*DEBUG*/
		if(row == LCD1602_ROW0)
		{
			Lcd1602WriteCommand(LCD1602_ROW0_ADDRESS_START+column) ;
		}
		else if(row == LCD1602_ROW1)
		{
			Lcd1602WriteCommand(LCD1602_ROW1_ADDRESS_START+column) ;
		}
		while(length--)
		{
			Lcd1602WriteData(*stringCode);
			stringCode++;
		}
		return 0;
}