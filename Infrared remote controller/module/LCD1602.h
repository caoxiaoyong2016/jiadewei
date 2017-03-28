#ifndef __LCD1602_H__
#define __LCD1602_H__
	
	
	#define uchar unsigned  char     // �궨��uchar Ϊ�޷����ַ�
	#define uint  unsigned  int  

	#define DataPin	P0 

	sbit  LCD_RS = P0^4;	//	LCD	����/ ����ѡ�� ,�ߵ�ƽʱѡ�����ݼĴ���
	sbit  LCD_RW = P0^3;	//  LCD �� / дѡ��	   ,�ߵ�ƽʱ������
	sbit  LCD_E  = P0^2;	//  LCD ʹ��
	sbit  IR_DQ  = P0^1;	//	�������ͷ������

	void  LCD_BusyCheck(void);
	void  LCD_WriteByte(uchar var,bit RS);
	void  LCD_Init(void);
	void  LCD_WriteString(uchar *str,uchar line);
	void  LCD_CGRAM_WriteByte(uchar *p,uchar num);
	void  LCD_Delay(uint n);
	
#endif