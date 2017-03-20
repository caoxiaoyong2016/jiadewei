#ifndef __LCD1602_H__
#define __LCD1602_H__

#include "common.h"

sbit lcd1602_rs_bit = P0^6;/*������ʵ����Ҫ�޸ġ�*/
sbit lcd1602_rw_bit = P0^5;/*������ʵ����Ҫ�޸ġ�*/
sbit lcd1602_en_bit = P0^7;/*������ʵ����Ҫ�޸ġ�*/

#define LCD1602_DATA_PORT	P0/*������ʵ����Ҫ�޸ġ�*/

/*default type:  
px^0---DB0(LSB)  
px^1---DB1  
px^2---DB2  	   
px^3---DB3  
px^4---DB4  
px^5---DB5  
px^6---DB6  
px^7---DB7(MSB)  
*/

/*  
RS          Rw      
0           0   ---->д����    
0           1   ---->��״̬    
1           0   ---->д����   
1           1   ---->������        
*/ 

/*LCD1602 RW ����ѡ��*/
#define LCD1602_READ_OPERATION		1/*������*/
#define LCD1602_WRITE_OPERATION		0/*д����*/

/*LCD1602 RS ����ѡ��*/
#define LCD1602_DATA_OPERATION		1/*����ģʽ*/
#define LCD1602_COMMAND_OPERATION	0/*����ģʽ*/

/*LCD1602 EN ����ѡ��*/
#define LCD1602_ENABLE				1/*�������LCD1602*/
#define LCD1602_DISABLE				0/*��ֹ����LCD1602*/

/*��ʾģʽ��16X2���ַ��� 5X7����8λ���ݿ�*/
#define LCD1602_DEFAULT_DISPALY_MODE	0x38

/*
====================================================
LCD1602���ã���ʾ������ֹ�������ʾ���ֹ�������˸���ֹ  
====================================================
/*  
0   0   0   0   1   D   C   B            
										|   |   |------�����˸  : 1-->���� ; 0-->��ֹ                      			
										|   |----------�����ʾ	 : 1-->���� ; 0-->��ֹ                     			
										|-------------Һ������ʾ : 1-->���� ; 0-->��ֹ   
*/

#define LCD1602_DISPLAY_ENABLE			0X04				 								/*����Һ����ʾ*/
#define LCD1602_CURSOR_DISPLAY_ENABLE	0X02				 							/*��������ʾ*/
#define LCD1602_CURSOR_BLINK_ENABLE 	0X01				 							/*��������˸*/
#define LCD1602_DISPLAY_DISABLE			(0X04 & (~(0x01<<2)))				/*��ֹҺ����ʾ*/
#define LCD1602_CURSOR_DISPLAY_DISABLE	(0X02 & (~(0x01<<1)))		/*��ֹ�����ʾ*/
#define LCD1602_CURSOR_BLINK_DISABLE 	(0X01 & (~(0x01<<0)))			/*��ֹ�����˸*/

/*Ĭ������:lcd��ʾ�������ʾ�������˸*/					 
/*������ʵ����Ҫ�޸ġ�*/
#define LCD1602_DEFAULT_DISPLAY_AND_CURSOR_MODE 0x08 			|\
														LCD1602_DISPLAY_ENABLE				|\
														LCD1602_CURSOR_DISPLAY_ENABLE	|\
														LCD1602_CURSOR_BLINK_ENABLE

/*
====================================================
LCD1602���ã�ָ���һ���һ�����λ�ü�һ���һ
==================================================== 
0   0   0   0   0   1   N   S  
												|   |-----  ��Ļ�ƶ�:1-->���� ; 0-->��ֹ                         
												|---------  
												N=1:������дһ���ַ����ַָ���1���ҹ���1                                    
												N=0:�Ȼ���дһ���ַ����ַָ���1���ҹ���1                        
												��S=1ʱ ��                            
												��N=1������Ļ����                                
												N=0������Ļ����                              
												�Դﵽ��겻�ƶ�����������Ļ�ƶ���Ч��  
*/   	 									

#define LCD1602_POINT_AND_CURSOR_PLUS		0X02				 						/*ָ���һ������һ*/
#define LCD1602_SCREEN_MOVE_ENABLE			0x01				 						/*������Ļ�ƶ�*/
#define LCD1602_POINT_AND_CURSOR_MINUS		(0X02 & (~(0X01<<1)))	/*ָ���һ������һ*/
#define LCD1602_SCREEN_MOVE_DISABLE			(0x01 & (~(0X01<<0)))		/*��ֹ��Ļ�ƶ�*/

/*Ĭ������:����ָ���һ����Ļ���ƶ�*/						     
/*������ʵ����Ҫ�޸ġ�*/
#define LCD1602_DEFAULT_POINT_AND_POINT_ADDRESS_MODE 0X04				|\
																LCD1602_POINT_AND_CURSOR_PLUS		|\
																LCD1602_SCREEN_MOVE_DISABLE

/*��λ�ò�����ע���е�ַ��0��ʼ*/											
#define LCD1602_ROW0	0										/*��0��*/		
#define LCD1602_ROW1	1										/*��1��*/		
#define LCD1602_MIN_ROW	LCD1602_ROW0			/*��������Сֵ��ΪLCD1602_ROW0����0*/
#define LCD1602_MAX_ROW	LCD1602_ROW1			/*���������ֵ��ΪLCD1602_ROW1����1*/

/*��λ�ò�����ע���е�ַ��0��ʼ*/
#define LCD1602_ROW_LENGHT	0x28 					/*ÿһ�еĳ���Ϊ0x28,��40*/
#define LCD1602_MIN_COLUMN	0							/*��������Сֵ��Ϊ0*/
#define LCD1602_MAX_COLUMN	(LCD1602_ROW_LENGHT-1)			/*���������ֵ��Ϊ39*/
/*ÿһ�е���ʼ��ַ*/
#define LCD1602_ROW0_ADDRESS_START	0X80								/*��0����ʼ��ַ*/
#define LCD1602_ROW1_ADDRESS_START	(0X80+0X40)					/*��1����ʼ��ַ*/

/*ÿһ�еĽ�����ַ*/
#define LCD1602_ROW0_ADDRESS_END (LCD1602_ROW0_ADDRESS_START+LCD1602_ROW_LENGHT)/*��0�н�����ַ*/
#define LCD1602_ROW1_ADDRESS_END (LCD1602_ROW1_ADDRESS_START+LCD1602_ROW_LENGHT)/*��1�н�����ַ*/

/*���ָ��*/
#define LCD1602_CLEAN_ALL_DISPALY					0x01				/*����ָ��*/
#define LCD1602_CURSOR_RETURN_TO_ORIGIN		0x02				/*����λָ��*/

/*���󷵻�ֵ*/
#define LCD1602_ROW_OR_COLUMN_OVERFLOW 		-1

/*****************�ⲿ�ӿں���******************/
//д����
extern void Lcd1602WriteCommand(UB8 commandCode);
//д����
extern void Lcd1602WriteData(UB8 dataCode);
//����
extern void Lcd1602CleanAll(void) ;
//����λ
extern void Lcd1602CursorHoming(void);
//LCD1602��ʼ��
extern void Lcd1602Init(void) ;
//LCD1602��ַд�ֽ�
extern SB8 Lcd1602AddressWriteByte(UB8 row,UB8 column,UB8 dataCode);
//LCD1602��ַд�ַ���
extern SB8 Lcd1602AddressWriteString(UB8 row,UB8 column,UB8 *stringCode);
/**********************************************/
#endif	/*__LCD1602_H__*/