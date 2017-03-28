#include	"LCD1602.h"
	
	
	void LCD_Delay(uint n)
	{
		while(--n);				// 8usһ��
	}

	
	void LCD_BusyCheck(void)
	{
		uchar isBusy;
		LCD_RS = 0;
		LCD_RW = 1;
		LCD_E  = 1;
		LCD_Delay(10);
		isBusy = DataPin & 0x80;     
		LCD_E  = 0;

		if(isBusy == 0x80)
		{
			LCD_BusyCheck();
		}
	}

	void LCD_WriteByte(uchar var,bit RS )	   //���� RSΪ LCD����/ ����ѡ�� ,�ߵ�ƽʱѡ�����ݼĴ���
	{
		LCD_BusyCheck();
		LCD_RS = RS;
		LCD_RW = 0;
		DataPin = var;
		LCD_E = 0;
		LCD_Delay(70);					  //������ʱ500us����		
		LCD_E  = 1;
		LCD_Delay(5);
		LCD_E = 0;
	}

	void LCD_WriteString(uchar *str,uchar line)
	{
		uchar i = 0,addr;
		if(line == 1) { addr = 0x80;}
		if(line == 2) { addr = 0xC0;}
		while(1)		
		{			
			LCD_WriteByte(addr+i,0);	   //д���ַ
			LCD_WriteByte(str[i],1);
			i++;
			if(i==16 || str[i] == '\0'){ break;}
		}			
	}

	void LCD_CGRAM_WriteByte(uchar *p,uchar num)		   //���Զ����ַ�д��CGRAM��,���64�ֽ�,uchar numΪ�Զ����ַ����λ�� 0-7
	{
		uchar i,place;
		place = 0x40 + 8*(num&0x07);					// num & 0x07 ��֤numȡֵ��ΧΪ 0-7
		for(i=0;i<8;i++)
		{
			LCD_WriteByte(place+i,0);		
			LCD_WriteByte(p[i],1);
		}
	}


	void LCD_Init(void)
	{
		LCD_Delay(1875);			//��ʱ15ms
		LCD_WriteByte(0x38,0);
		LCD_Delay(625);				//��ʱ5ms
		LCD_WriteByte(0x38,0);  //����LCDΪ4λ��������,˫����ʾ,5*7�����ַ�
		LCD_WriteByte(0x08,0);  //����LCD�ر���ʾ,�޹��,����˸
		LCD_WriteByte(0x01,0);  //LCD����
		LCD_WriteByte(0x06,0);  //LCD�������
		LCD_WriteByte(0x0C,0);  //��LCD��ʾ
	}