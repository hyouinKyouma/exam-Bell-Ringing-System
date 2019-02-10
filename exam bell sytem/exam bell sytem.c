/*
 * exam_bell_sytem.c
 *
 * Created: 4/5/2018 10:31:28 AM
 *  Author: ADITYA
 */ 


#include <avr/io.h>
#define F_CPU 1000000UL
#include <util/delay.h>
#include <atmega/lcdheader2.h>
#include <atmega/USART.h>
#include <I2C.h>
#define sadd 0Xd0
/*

#define I2C_ON TWCR=(1<<TWEN)|(1<<TWINT)
#define I2C_START TWCR=(1<<TWEN)|(1<<TWINT)|(1<<TWSTA) TWCR=(1<<TWEN)|(1<<TWINT)|(1<<TWSTA)
#define I2C_STOP TWCR=(1<<TWSTO)|(1<<TWEN)|(1<<TWINT)

#define check_TWINT !(TWCR&(1<<TWINT))
//#define check_TWSR (TWSR&(0XF8))


char RESULT_I2C;
void ASCII_conv_print(int c)
{
	unsigned char d[4];
	d[0]=c%10;
	d[1]=(c/10);
	//d[2]=(c/100)%10;
	//d[3]=c/1000;
	for (int i=1;i>=0;i--)
	{
		ldta(d[i]+48);
	}
	
	
}


char transmit_I2C(char madd,char data)
{
	//start sending
	TWCR=(1<<TWEN)|(1<<TWINT)|(1<<TWSTA);
	while(check_TWINT); // ASCII_conv_print(TWDR); lcmd(0x80); _delay_ms(1000);
	if ((TWSR&(0XF8))!=0x08)
		return 0x01;
	
	//slave address + write
	TWDR=sadd;
	TWCR=(1<<TWEN)|(1<<TWINT);
	while(check_TWINT); // ASCII_conv_print(TWDR); lcmd(0x80); _delay_ms(1000);
	if ((TWSR&(0XF8))!=0x18)
		return 0x02;
	
	//memory address
	TWDR=madd;
	TWCR=(1<<TWEN)|(1<<TWINT);
	while(check_TWINT); // ASCII_conv_print(TWDR); lcmd(0x80); _delay_ms(1000);
	if((TWSR&(0XF8))!=0x28)
		return  0x03;
	
	//data byte
	TWDR=data;
	TWCR=(1<<TWEN)|(1<<TWINT);
	while(check_TWINT); // ASCII_conv_print(TWDR); lcmd(0x80); _delay_ms(1000);
	if((TWSR&(0XF8))!=0x28)
		return 0x04;
	
	//stop
	TWCR=(1<<TWSTO)|(1<<TWEN)|(1<<TWINT);
	_delay_ms(1);
	return 0X55;
}

char receive_I2C(char madd)
{
	
	//start
	TWCR=(1<<TWEN)|(1<<TWINT)|(1<<TWSTA);
	while(check_TWINT); // ASCII_conv_print(TWDR); lcmd(0x80); _delay_ms(1000);
	if ((TWSR&(0XF8)) != 0x08)
		return 0x01;
	
	//slave address + write
	TWDR=sadd;
	TWCR=(1<<TWEN)|(1<<TWINT);
	while(check_TWINT); // ASCII_conv_print(TWDR); lcmd(0x80); _delay_ms(1000);
	if ((TWSR&(0XF8))!=0x18)
		return 0x02;
	
	//memory address
	TWDR=madd;
	TWCR=(1<<TWEN)|(1<<TWINT);
	while(check_TWINT); // ASCII_conv_print(TWDR); lcmd(0x80); _delay_ms(1000);
	if((TWSR&(0XF8))!=0x28)
		return  0x03;
	
	//repeated start
	TWCR=(1<<TWEN)|(1<<TWINT)|(1<<TWSTA);
	while(check_TWINT); // ASCII_conv_print(TWDR); lcmd(0x80); _delay_ms(1000);
	if((TWSR&(0XF8))!=0x10)
		return 0x05;
		
	//slave address + read
	TWDR=sadd|0x01;
	TWCR=(1<<TWEN)|(1<<TWINT);
	while(check_TWINT); // ASCII_conv_print(TWDR); lcmd(0x80); _delay_ms(1000);
	if((TWSR&(0XF8))!=0x40)
		return 0x06;
		
	//receive data
	TWCR=(1<<TWEN)|(1<<TWINT);
	while(check_TWINT); // ASCII_conv_print(TWDR); lcmd(0x80); _delay_ms(1000);
	RESULT_I2C=TWDR;
	

	//stop
	TWCR=(1<<TWSTO)|(1<<TWEN)|(1<<TWINT);
	_delay_ms(1);
	return 0X55;
}

void vbcd(char add,char num)
{
	char d0,d1;
	d0=num&0x0f;
	d1=(num>>4);
	lcmd(add);
	ldta(d1|0X30);
	ldta(d0|0X30);
}

*/
void clock_start()
{
	PC0ON;
	PC1ON;
}
/*
void initialize_Keyboard()
{
	baudX2;
	TXON;
	RXON;
	initialize_UCSRC;
	baud_rate(4800);	
}			//in interupt ldta(rx());
*/
void lcd_start()
{
	 DDRB=0XFF;
	 PC3OUT;
	 PC4OUT;
	 PC5OUT;
	 initialize_LCD(8,curoff);
}
char error;
void display_clock(int length,char a[],char b[])
{
	
	char hour,min,alert[20];
	 error=receive_I2C(2);
	 vbcd(0x80,RESULT_I2C);
	 ldta(':');
	 
	 for(int i=0;i<=length;i++)
	 {
		 hour=(RESULT_I2C&0x0f)+((RESULT_I2C&0x0f)>>4)*10;
		 if (hour==a[i])
		 {
			 alert[i]=1;
		 }
		 else
			alert[i]=0;
		 
	 }
	 error=receive_I2C(1);
	 vbcd(0x83,RESULT_I2C);
	 
	 for(int i=0;i<=length;i++)
	 {
		 hour=(RESULT_I2C&0x0f)+((RESULT_I2C&0x0f)>>4)*10;
		 if (min==b[i] && alert[i]==1)
			PD2ON;
		 else
			PD2OFF;
		 
	 }
	 error=receive_I2C(0);
	 //vbcd(0x86,RESULT_I2C);
	 error=receive_I2C(4);
	 vbcd(0xc0,RESULT_I2C);
	 ldta('/');
	 error=receive_I2C(5);
	 vbcd(0xc3,RESULT_I2C);
	 ldta('/');
	 error=receive_I2C(6);
	 vbcd(0xc6,RESULT_I2C);
}
int main(void)
{
	clock_start();
	lcd_start();
	PD2OUT;
	//initialize_Keyboard();
	signed char hour=21,min=16,duration=1,i=0,length;
	signed char alert_hour[20]={-1};
	signed char alert_min[20]={-1};
		
		
	if (hour>=24 ||hour<0 ||min>=60||min<0||duration<=0)
	{
		lcmd(0x01);
		str("error");
		return;
	}
	
	
	alert_hour[0]=hour;
	alert_min[0]=min-15;
	alert_hour[1]=hour;
	alert_min[1]=min;
	for(i=1;i<duration;i++)
	{
		alert_hour[i+1]=hour+i;
		alert_min[i+1]=min;
		
	}
	length=i+1;
	alert_hour[length]=alert_hour[length-1];
	alert_min[length]=min-30;
	alert_hour[length+1]=alert_hour[length-1];
	alert_min[length+1]=min-15;
	alert_hour[length+2]=alert_hour[length-1];
	alert_min[length+2]=min-5;
	alert_hour[length+3]=hour+duration;
	alert_min[length+3]=min;
	length=length+3;
	
		
	for(i=0;i<=length;i++)
	{
		if(alert_min[i]<0)
		{
			alert_hour[i]-=1;
			alert_min[i]+=60;
		}
	}
	while(1)
	{
		display_clock(length,alert_hour,alert_min);   
		
	}
}