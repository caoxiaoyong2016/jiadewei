#include	"FM1602LF.h"


//����˿�
sbit SDA_I2C_FM1602LF = P0^6; //RSΪ�Ĵ���ѡ�񣬸ߵ�ƽʱѡ�����ݼĴ������͵�ƽʱѡ��ָ��Ĵ�����
sbit SCL_I2C_FM1602LF = P0^5; //R/WΪ��д�ź��ߣ��ߵ�ƽʱ���ж��������͵�ƽʱ����д������
sbit EN = P0^7; //E��Ϊʹ�ܶˣ���E���ɸߵ�ƽ����ɵ͵�ƽʱ��Һ��ģ��ִ�����

#define		FM1602LF_EEPROM_SLA			0xe6
#define 	EEPROM_WR               0
#define 	EEPROM_RD               1

#define RS_CLR RS=0 
#define RS_SET RS=1

#define RW_CLR RW=0 
#define RW_SET RW=1 

#define EN_CLR EN=0
#define EN_SET EN=1


//EX_extern	void Init_FM1602LF_I2C(void);
//EX_extern	void writeChar_FM1602LF(UINT8 data_I2C);
//EX_extern	void LCD_Init(void);
//EX_extern	void LCD_Write_Com(unsigned char com) ;
//EX_extern	void DelayMs(unsigned char t);

/*------------------------------------------------------
 mS��ʱ����
 ����������� unsigned char t���޷���ֵ
 unsigned char �Ƕ����޷����ַ���������ֵ�ķ�Χ��
 0~255 ����ʹ�þ���16M����ȷ��ʱ��ʹ�û��
------------------------------------------------------*/
void DelayMs(unsigned char t)
{
		uint8_t	i=1334;
    while(t--)
    {
        //������ʱ1mS
        while(i--);
    }
}

/*------------------------------------------------
              I2C��ʼ������
------------------------------------------------*/
void Init_FM1602LF_I2C(void)
{
		P05_OpenDrain_Mode;
		P06_OpenDrain_Mode;
    Init_I2C();                                 //set SDA and SCL pins high
}

/*------------------------------------------------
             Send one byte
------------------------------------------------*/
void writeChar_FM1602LF(UINT8 data_I2C)
{
		start_I2C();
		writebyte_I2C(FM1602LF_EEPROM_SLA|EEPROM_WR);
		respons_I2C();
		writebyte_I2C(data_I2C);
		respons_I2C();
		stop_I2C();
}

/*------------------------------------------------
              LCD��ʼ������
------------------------------------------------*/
void LCD_Init(void) 
{
		LCD_Write_Com(0x38);    /*��ʾģʽ����*/ 
    DelayMs(5); 
    LCD_Write_Com(0x38); 
    DelayMs(5); 
    LCD_Write_Com(0x38); 
    DelayMs(5); 
    LCD_Write_Com(0x38);  
    LCD_Write_Com(0x08);    /*��ʾ�ر�*/ 
    LCD_Write_Com(0x01);    /*��ʾ����*/ 
    LCD_Write_Com(0x06);    /*��ʾ����ƶ�����*/ 
    DelayMs(5); 
    LCD_Write_Com(0x0C);    /*��ʾ�����������*/
}

/*------------------------------------------------------
                    д�������
------------------------------------------------------*/
void LCD_Write_Com(unsigned char com) 
{  
//    while(LCD_Check_Busy()); //æ��ȴ�

}

/*------------------------------------------------------
                    д�����ݺ���
------------------------------------------------------*/
void LCD_Write_Data(unsigned char Data) 
{ 
//    while(LCD_Check_Busy()); //æ��ȴ�

}


/*------------------------------------------------------
                    ��������
------------------------------------------------------*/
void LCD_Clear(void) 
{ 
    LCD_Write_Com(0x01); 
    DelayMs(5);
}


/*------------------------------------------------------
                    д���ַ�������
------------------------------------------------------*/
void LCD_Write_String(unsigned char x,unsigned char y,unsigned char *s) 
{     
    if (y == 0) 
    {     
        LCD_Write_Com(0x80 + x);     //��ʾ��һ��
    }
    else 
    {      
        LCD_Write_Com(0xC0 + x);      //��ʾ�ڶ���
    } 

    while (*s) 
    {     
        LCD_Write_Data( *s);     
        s ++;     
    }
}

/*------------------------------------------------------
                    д���ַ�����
------------------------------------------------------*/
void LCD_Write_Char(unsigned char x,unsigned char y,unsigned char Data) 
{     
    if (y == 0) 
    {     
        LCD_Write_Com(0x80 + x);     
    }    
    else 
    {     
        LCD_Write_Com(0xC0 + x);     
    }
    
    LCD_Write_Data( Data);  
}