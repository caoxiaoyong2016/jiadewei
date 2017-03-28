//******************************************************************** 
// �ļ����ƣ�  I2C.c 
// �������ڣ�  2016-10-11 
// ���¸��ģ�  2016-10-11 
// ������      I2C��ʼ��  
//********************************************************************                                     
//#include "xc.h" 
#include "FM1602_I2C.h" 

/********************************************************************* 
** Function name:      I2CInit  
** Descriptions:       ע�⣺���뽫SDA��SCL��������Ϊ�������ţ�<<DS P293>> 
** input parameters:   �� 
** output parameters:  ��
** Returned value:     ��  
**********************************************************************/ 
#if 0
	void I2CInit (void)
	{                                            
			SSP1STATbits.SMP = 1;           // ��ֹ��׼�ٶ�ģʽ�µ�ѹ���ʿ���     
			SSP1STATbits.CKE = 0;           // ��ֹSMBus�ض�����      
			SSP1CON1bits.SSPEN = 1;         // ʹ��I2C,����SDA ��SCL������     
			SSP1CON1bits.SSPM = 0x8;        // I2C��ģʽ      
			SSP1ADD = I2C_BPS;              // Fclock = Fosc / ((SSP1ADD + 1)*4)                                    
	}  
#endif
	void I2CInit (void)
	{                                            
			P13_Quasi_Mode;                         //set SCL (P13) is Quasi mode
			P14_Quasi_Mode;                         //set SDA (P14) is Quasi mode
			I2CPX = 0;  														//ѡ��I2C��
			set_I2CEN; 															// ʹ��I2C
			I2CLK = I2C_CLOCK;											// Fclock = Fosc / ((I2CLK + 1)*4) 
	} 

/********************************************************************* 
** Function name:       I2C_Error ( ) 
** Descriptions:        I2C���� 
** input parameters:    �� 
** output parameters:   �� 
** Returned value:      ��  
**********************************************************************/ 	
void I2CError(void)
{
//    P3 = I2STAT;
//    P3 = ERROR_CODE;
    while (1)P12 = 0;    
}

/********************************************************************* 
** Function name:       I2CStart ( ) 
** Descriptions:        I2C��ʼ 
** input parameters:    �� 
** output parameters:   �� 
** Returned value:      ��  
**********************************************************************/   
	void I2CStart ( )
	{    
//			while (!Idle);      
//			SSP1CON2bits.SEN = 1;           // ��SDA��SCL�����Ϸ�������������Ӳ���Զ�����     
//			while(SSP1CON2bits.SEN);        // ���������������     
			while(!Idle);  
			set_STA;                          /* Send Start bit to I2C EEPROM */
//			clr_SI;
//			while (!SI);                      //Check SI set or not
//			if (I2STAT != 0x08)               //Check status value after every step
//					I2CError();
	}   
/********************************************************************* 
** Function name:       I2CStop ( )
** Descriptions:        I2Cֹͣ 
** input parameters:    �� 
** output parameters:   �� 
** Returned value:      ��  
**********************************************************************/ 
	void I2CStop()
	{     
			//while (!Idle);      
//			SSP1CON2bits.PEN = 1;           // ��SDA��SCL�����Ϸ���ֹͣ������Ӳ���Զ�����     
//			while(SSP1CON2bits.PEN);        // ֹͣ�����������     
			set_STO;
			clr_SI;
			while (STO);                      /* Check STOP signal */
			while(!Idle);  
	}   
/********************************************************************* 
** Function name:       ReStart() 
** Descriptions:        I2C ��ReStart 
** input parameters:    �� 
** output parameters:   �� 
** Returned value:      ��  
**********************************************************************/ 
	void ReStart()
	{      
			while (!Idle);                
//			SSP1CON2bits.RSEN = 1;          // ��SDA��SCL�����Ϸ����ظ�����������Ӳ���Զ�����      
//			while(SSP1CON2bits.RSEN);       // �ظ����������������    
			set_STA;                          /* Send Start bit to I2C EEPROM */
			while(!Idle);  
	}   
/********************************************************************* 
** Function name:       I2CSendByte() 
** Descriptions:        SSP1BUF��д���ֽ� 
** input parameters:    I2CSnBy��Ҫ���͵����� 
** output parameters:   �� 
** Returned value:      ��  
**********************************************************************/ 
	void I2CSendByte(unsigned char I2CSnBy) 
	{    
//			while (!Idle);      
//			SSP1BUF = I2CSnBy;              // SSP1BUF��д���ֽ� 
//			while(!Idle); 
				clr_STA; 
				I2DAT =	I2CSnBy;
//				clr_SI;
//				while (!SI);                                //Check SI set or not
//				if (I2STAT != 0x18)              
//						I2CError();
	}   
/********************************************************************* 
** Function name:        
** Descriptions:         
** input parameters: 
** output parameters:   �� 
** Returned value:      �� 
**********************************************************************/   
	void WriteCommand(unsigned char Command)
	{    
//			unsigned char Temp_C;      
//			I2CStart();                     // I2C��ʼ     
//			I2CSendByte(Slave_Add);         // ��������ַ      
//			Temp_C = Command & 0xF0;     
//			Temp_C |= 0x0C;                 // P3=1  EN=1  RW=0  RS=0      
//			I2CSendByte(Temp_C);                   
//			Temp_C &= 0xF8;                 // P3=1  EN=0  RW=0  RS=0      
//			I2CSendByte(Temp_C);                
//			Temp_C = (Command & 0x0F)<< 4;      
//			Temp_C |= 0x0C;                 // P3=1  EN=1  RW=0  RS=0     
//			I2CSendByte(Temp_C);     
//			Temp_C &= 0xF8;                 // P3=1  EN=0  RW=0  RS=0     
//			I2CSendByte(Temp_C);            
//			I2CStop();                      // I2Cֹͣ 
			I2CStart();
			clr_SI;
			while (!SI);                      //Check SI set or not
			if (I2STAT != 0x08)               //Check status value after every step
					I2CError();
			I2CSendByte(Slave_Add);
			clr_SI;
			while (!SI);                      //Check SI set or not
			if (I2STAT != 0x18)               //Check status value after every step
					I2CError();
			I2CSendByte(Command);
			clr_SI;
			while (!SI);                      //Check SI set or not
			if (I2STAT != 0x28)               //Check status value after every step
					I2CError();
			I2CStop(); 
	}    
/********************************************************************* 
** Function name:       
** Descriptions:         
** input parameters:     
** output parameters:   �� 
** Returned value:      ��  
**********************************************************************/   
	void WriteData (unsigned char Data)
	{      
			unsigned char Temp_D;      
			I2CStart();                     // I2C��ʼ     
			I2CSendByte(Slave_Add);         // ��������ַ       
			Temp_D = Data & 0xF0;                  
			Temp_D |= 0x0D;                 // P3=1  EN=1  RW=0  RS=1    
			I2CSendByte(Temp_D);      
			Temp_D &= 0xF9;                 // P3=1  EN=0  RW=0  RS=1    
			I2CSendByte(Temp_D);
			Temp_D = (Data & 0x0F)<< 4;      
			Temp_D |= 0x0D;                 // P3=1  EN=1  RW=0  RS=1     
			I2CSendByte(Temp_D);      
			Temp_D &= 0xF9;                 // P3=1  EN=0  RW=0  RS=1     
			I2CSendByte(Temp_D);            
			I2CStop();                      // I2Cֹͣ 
	}  
 /********************************************************************* 
** Function name:       LCD1602Init(void),LCD1602��ʼ�� 
** Descriptions:        дһ�Σ�ż������������ʾ���ظ�2�� 
** input parameters:     
** output parameters:   �� 
** Returned value:      ��  
**********************************************************************/ 
	void LCD1602Init(void)
		{      
				__delay_ms(10);                                         
				WriteCommand(0x33); 
				__delay_ms(5);   
				WriteCommand(0x32);
				__delay_ms(5);   
				WriteCommand(0x28);
				__delay_ms(5);  
				WriteCommand(0x0C); 
				__delay_ms(5);    
				WriteCommand(0x06);
				__delay_ms(5);      
				WriteCommand(0x01); 
				__delay_ms(5); // ����          
				__delay_ms(10);                                          
				WriteCommand(0x33); 
				__delay_ms(5);   
				WriteCommand(0x32); 
				__delay_ms(5);   
				WriteCommand(0x28); 
				__delay_ms(5);   
				WriteCommand(0x0C); 
				__delay_ms(5);   
				WriteCommand(0x06); 
				__delay_ms(5);     
				WriteCommand(0x01); 
				__delay_ms(5); // ����          
		}   
/******************************************************************** 
** Function name:       L1602_char(uchar col,uchar row,char sign)  
** Descriptions:        �ı�Һ����ĳλ��ֵ�����Ҫ�õ�һ�У�������ַ���ʾ"b" ��                         
		���øú�����,Dip_Single_char(1,5,'A'); 
** input parameters:    �У��У���Ҫ����1602������ 
** output parameters:   �� 
** Returned value:      ��
*********************************************************************/   
	void Dip_Single_char(unsigned char col,unsigned char row,unsigned char sign)
	{     
			unsigned char a;     
			if(col == 1) a = 0x80;     
			if(col == 2) a = 0xc0;     
			a = a + row - 1;     
			WriteCommand(a);     
			WriteData(sign); 
	}   
/******************************************************************** 
** Function name:       Dip_Single_char 
** Descriptions:        ��ʾint������,5λ 
** input parameters:    �У��У����� 
** output parameters:   �� 
** Returned value:      ��  
*********************************************************************/   
	void DisDec(unsigned char col_D,unsigned char row_D,unsigned int Temp_k )
	{           
			unsigned int Temp_Ts;      
			unsigned char Table[5];         // ������1602��ʾ��ת��            
			unsigned char q;               
			if(Temp_k>=65535) Temp_k=65535;       
			Table[0] = Temp_k/10000+48;     // ��λ       
			Temp_Ts  = Temp_k%10000;        // ȡ�࣬0-9999      
			Table[1] = Temp_Ts/1000+48;     // ǧλ     
			Temp_Ts  = Temp_Ts%1000;        // ȡ�࣬0-999     
			Table[2] = Temp_Ts/100+48;      // ��λ     
			Temp_Ts  = Temp_Ts%100;         // ȡ�࣬0-99     
			Table[3] = Temp_Ts/10+48;       // ʮλ    
			Table[4] = Temp_Ts%10+48;       // ��λ           
			for(q=0;q<5;q++)
			{               // ��ʾ         
					Dip_Single_char(col_D,q+row_D,Table[q]);    
			} 
	}   
/********************************************************************* 
**                            The End  
*********************************************************************/