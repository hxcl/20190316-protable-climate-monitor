#include "SI7021.h"

#include <intrins.h>


void delay_x_us ( void )//(1+2)*60=180ָ������ 180us
{
	char x=60;
	for ( ; x>0; x-- )
		_nop_();
}       
                                                          
//�������ƣ� start_i2c
//�������ܣ� i2c ������ʼ����
//����������
// �� �� ֵ��
void start_i2c ( void )
{
	SI7021_SDA_HIGH();  
	delay_x_us();
	SI7021_SCLK_HIGH();   
	delay_x_us();     
	SI7021_SDA_LOW();   
	delay_x_us(); 		
	SI7021_SCLK_LOW(); 
	delay_x_us();
}
//�������ƣ� stop_i2c
//�������ܣ� i2c ���ͽ�������
//����������
// �� �� ֵ��
void stop_i2c ( void )
{
	SI7021_SDA_LOW(); 
	delay_x_us();  
	SI7021_SCLK_HIGH(); 
	delay_x_us();
	SI7021_SDA_HIGH(); 
	delay_x_us();
}
//�������ƣ� send_1byte
//�������ܣ� дһ���ֽڵ�si7021оƬ
//����������
// �� �� ֵ��
bit send_1byte ( unsigned char send_data )
{
	unsigned char bit_cnt;
	bit	b_ack=0;
	unsigned char i=200;
	
	for( bit_cnt=0; bit_cnt<8; bit_cnt++ ) 
	{ 
	     SI7021_SCLK_LOW(); 
	     if ( (send_data<<bit_cnt)&0x80 ) 
		 	SI7021_SDA_HIGH();  
	     else SI7021_SDA_LOW();  
		 delay_x_us();              
	     SI7021_SCLK_HIGH();       
	     delay_x_us();           
	}

	delay_x_us();
	SI7021_SCLK_LOW();
	SI7021_SDA_HIGH();        //
	delay_x_us();
	

	delay_x_us(); 
	SI7021_SCLK_HIGH();
	delay_x_us();
	 
	i = 200;
	while ( i-- )
	{
		delay_x_us();
		if(SI7021_SDA==0)
		{
			b_ack = 1;
			break;
		}  
	}

	if ( i == 0 ) b_ack = 0; 
	        
	SI7021_SCLK_LOW();
	delay_x_us ();

	return b_ack;
}

//�������ƣ� read_1byte
//�������ܣ� ��si7021��ȡһ���ֽ�
//����������
// �� �� ֵ��
unsigned char read_1byte ( void )
{
	unsigned char read_value=0;
	unsigned char bit_cnt;

	for ( bit_cnt=0; bit_cnt<8; bit_cnt++ )
	{          
	     SI7021_SCLK_HIGH();       
	     delay_x_us();
		 read_value <<= 1;

	     if ( SI7021_SDA==1 ) 
		 	read_value +=1;

	     SI7021_SCLK_LOW();
	}
	return (read_value);
}
//�������ƣ�master_i2c_ack
//�������ܣ�MCUӦ�������
//����������
// �� �� ֵ��
void master_i2c_ack ( void )
{
	SI7021_SDA_LOW();   
	delay_x_us();
	SI7021_SCLK_LOW();
	delay_x_us();      
	SI7021_SCLK_HIGH();
	delay_x_us();
	SI7021_SCLK_LOW();    
	delay_x_us(); 
	SI7021_SDA_HIGH();
	delay_x_us();
}
//�������ƣ�master_i2c_noack
//�������ܣ�MCU������Ӧ���źŵ�������
//����������
// �� �� ֵ��
void master_i2c_noack ( void )
{
	SI7021_SDA_HIGH(); 
	delay_x_us();      
	SI7021_SCLK_HIGH();
	delay_x_us();
	SI7021_SCLK_LOW(); 
}
//�������ƣ�measure_si7021
//�������ܣ�HOLD MASTERģʽ�¶�ȡ��ʪ�� 
//����������
// �� �� ֵ��
void measure_si7021 ( unsigned char model, union16 *value )
{
	int i;
	//����ʼ�ź�
	start_i2c();               
	if ( 0== send_1byte ( SALVE_ADDR ) )//дslave addr
	{
		value->uint = 0xAABB;
		return ;
	}

	if ( 0 == send_1byte( model ) )//measure cmd
	{
		value->uint = 0x1234;
		return ;
	}
	//Ĭ��ʪ��ת��ʱ��Ϊ10~12ms �¶� 6~10ms  
	i = 600;	//600*180=108ms	��׼ȷ��ʱ
	while ( i-- )
		delay_x_us ();	//��ʪ������ת����ʱ�ȴ�����ʱ�����ֲᣬ�˴���Ϊ���ԣ�

	start_i2c ();			 	//���·���ʼ�ź�
	if ( 0==send_1byte(SALVE_ADDR+1) ) //������
	{
		value->uint = 0x3456;
		return ;
	}

	value->uchar[HSB] = read_1byte ();//��ȡ��ʪ�ȵĸ�λ�ֽ�
	//mcuӦ��
	master_i2c_ack ();
	value->uchar[LSB] = read_1byte ();//��ȡ��ʪ�ȵĵ�λ�ֽ�����
	//mcu��Ӧ��
	master_i2c_noack ();
	
	//mcuӦ��->read_1byte()��ȡУ��ֵ->mcu��Ӧ��

	//����ֹͣλ
//	value->uint = 0x7890;
	stop_i2c ();
}




















