/************************************************************************
;copyright		:shenzhen coolwaveasia
;filename		:lcd.c
;lcd			:TC802B-01
;control mcu	:w78e52b
;writeby		:
;describe		:LCD control
;notice			:
;***********************************************************************/

#include "..\lcd.h"
#include <intrins.h>
#include <string.h>
#include <reg52.h>


// lcd bit define
sbit     LCD_RW = P2^2;
sbit     LCD_RS = P2^1;
sbit     LCD_EN = P2^0;
#define  LCD_DATA P0
// sfr      LCD_DATA = 0x80; 




/***********************************************************
name:		ini_lcd		
input:		none
output:		none
describe:	��ʼ��LCD	
notice:
creat date: 2008-7-25
creator:	dengyihong
************************************************************/
void ini_lcd(void)
{
    lcd_write_reg(0x38);
    lcd_write_reg(0x38);
    lcd_write_reg(0x38);
    lcd_write_reg(0x08);
    lcd_wait();
    lcd_write_reg(0x01);
    lcd_wait();
    delay_2us(50000);
    
    lcd_write_reg(0x14);
    lcd_wait();
    lcd_write_reg(0x06);
    lcd_wait();
    lcd_write_reg(0x80);
    lcd_wait();
    lcd_write_reg(0x0c);
    lcd_wait();	
}


/***********************************************************
name:		lcd_printf_string	
input:		*disp_str  ---  �����ʾ�ַ���ͷָ��
			row_len_col  ---  ��ʾ�����������ʾ�ַ���
			row_len_col:
				bit[7:6]: ��ʾ������
				bit[5:3]: ��ʾ�ַ�����
				bit[2:0]: ��ʾ�ַ�����ʼ������
output:		none
describe:	����Ļ����ʾ����ַ�		
notice:
creat date: 2008-7-25
creator:	dengyihong
************************************************************/
void lcd_printf_string(U8 *disp_str, U8 row_len_col)
{
    U8 i = 0;
    U8 len = 0;
    
    len = (row_len_col & LEN_MASK) >> 0x03;
    
    lcd_write_reg(row_len_col & ROW_COL);
    lcd_wait();
    
    for(i=0; i<=len; i++)
    {
        lcd_write_data(*disp_str++);
        lcd_wait();	
    }
}


/***********************************************************
name:		lcd_printf_char		
input:		disp_char  ---  Ҫ��ʾ���ַ�
			row_col    ---  ��ʾ����
output:		none
describe:	����Ļ����ʾһ���ַ�		
notice:
creat date: 2008-7-25
creator:	dengyihong
************************************************************/
/*
void lcd_printf_char(U8 disp_char, U8 row_col)
{
    lcd_write_reg(row_col & ROW_COL);
    lcd_wait();
    lcd_write_data(disp_char);
    lcd_wait();	
}
*/

/***********************************************************
name:		lcd_write_reg		
input:		command  ---  Ҫд�������
output:		none
describe:	д�������LCD	
notice:
creat date: 2008-7-25
creator:	dengyihong
************************************************************/
void lcd_write_reg(U8 command)
{
    LCD_DATA = command;		// д��������
    LCD_RS = 0;
    LCD_RW = 0;
    LCD_EN = 1;
    delay_2us(100);
    LCD_EN = 0;
}


/***********************************************************
name:		lcd_write_data		
input:		value  ---  Ҫд�������
output:		none
describe:	д���ݵ�LCD	
notice:
creat date: 2008-7-25
creator:	dengyihong
************************************************************/
void lcd_write_data(U8 value)
{
   LCD_DATA =  value;		//д����
   LCD_RS = 1;
   LCD_RW = 0;
   LCD_EN = 1;
   delay_2us(100);
   LCD_EN = 0; 	
}

/***********************************************************
name:		lcd_wait	
input:		none
output:		none
describe:	�ȴ�LCD�ڲ��������	
notice:
creat date: 2008-7-25
creator:	dengyihong
************************************************************/
void lcd_wait(void)
{
    U8 value = 0;
       
    do
    {
        LCD_RS = 0;
        LCD_RW = 1;
        LCD_EN = 1;
        value = LCD_DATA;	
        LCD_EN = 0;
    }while(value & 0x80);		// �ȴ��ڲ��������	
}

/**********************************************************
name:		delay_2us
input:		delay_cnt
output:		none
describe:	delay x*2us
notice:
creat date:	2008-7-24
creator:	dengyihong
**********************************************************/
void delay_2us(U16 delay_cnt)
{
    while(delay_cnt--);
}

void hex_to_assic(U8 hex_data, U8 *str)
{
	if(hex_data < 10)
	{
		*str++ = hex_data + '0';
		*str++ = ' ';
		*str++ = ' ';
	}
	else if(hex_data < 100)
	{
		*str++ = hex_data/10 + '0';
		*str++ = hex_data%10 + '0';
		*str++ = ' ';
	}
	else if(hex_data <= 255)
	{
		*str++ = hex_data/100 + '0';
		*str++ = (hex_data/10)%10 + '0';
		*str++ = hex_data%10 + '0';
	}
}


