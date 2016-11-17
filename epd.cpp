/*
 * Driver for 4.3inch e-Paper
 * Using c++ and wiringPi library
 * Site: http://elinux.vn/driver-for-4-3-inch-e-paper-on-raspberry-pi-2/
 */

/* 
 * File:   epd.cpp
 * Author: Doan Manh
 * Site: http://elinux.vn/
 * Created on 16 November 2016, 10:29
 */

#include <string.h>
#include <wiringPi.h>
#include <wiringSerial.h>
#include "epd.h"

/*******************************************************************************
* Function Name  : epd(char* device, int baud, int wakeup, int reset)
* Description    : 
* Input          : 
* Output         : None
* Return         : 
* Attention      : None
*******************************************************************************/
epd::epd(const char* device, int baud, int wakeup, int reset)
{
    wakeup_pin = wakeup;
    reset_pin = reset;
    //init wakeup and reset pin
    pinMode(wakeup_pin, OUTPUT);
    pinMode(reset_pin, OUTPUT);
    
    //set low for wakeup and reset
    digitalWrite(wakeup_pin, LOW); 
    digitalWrite(reset_pin, LOW);
    
    //init serial device
    dev = device;
    fd = serialOpen(device, baud);
}

/*******************************************************************************
* Function Name  : void epd::~epd(void)
* Description    : 
* Input          : 
* Output         : None
* Return         : 
* Attention		   : None
*******************************************************************************/
epd::~epd(void)
{
    serialClose(fd);
}
/*******************************************************************************
* Function Name  : void epd::reset(void)
* Description    : 
* Input          : 
* Output         : None
* Return         : 
* Attention		   : None
*******************************************************************************/
void epd::reset(void)
{
    digitalWrite(reset_pin, LOW); 
    delay(100);
    digitalWrite(reset_pin, HIGH);
    delay(500);
}
/*******************************************************************************
* Function Name  : void epd::wakeup(void)
* Description    : 
* Input          : 
* Output         : None
* Return         : 
* Attention		   : None
*******************************************************************************/
void epd::wakeup(void)
{
    digitalWrite(wakeup_pin, LOW);
    delay(100);
    digitalWrite(wakeup_pin, HIGH);
    delay(500);
}


/*******************************************************************************
* Function Name  : void epd::handshake(void)
* Description    : 
* Input          : 
* Output         : None
* Return         : 
* Attention		   : None
*******************************************************************************/
void epd::handshake(void)
{
    command(CMD_HANDSHAKE, NULL, 0);
}
/*******************************************************************************
* Function Name  : void epd::set_baud(int baud)
* Description    : set uart baud
* Input          : 
* Output         : None
* Return         : 
* Attention      : None
*******************************************************************************/
void epd::set_baud(int baud)
{
    unsigned char data[4];
    data[0] = (unsigned char) ((baud >> 24) & 0xFF);
    data[1] = (unsigned char) ((baud >> 16) & 0xFF);
    data[2] = (unsigned char) ((baud >> 8) & 0xFF);
    data[3] = (unsigned char) (baud & 0xFF);
    
    command(CMD_SET_BAUD, data, 4);
    delay(200);
    serialClose(fd);
    fd = serialOpen(dev, baud);
}

/*******************************************************************************
* Function Name  : void epd::set_memory(unsigned char mode)
* Description    : set memory mode
* Input          : 
* Output         : None
* Return         : 
* Attention      : None
*******************************************************************************/
void epd::set_memory(unsigned char mode)
{
    command(CMD_MEMORYMODE, &mode, 1);
}

/*******************************************************************************
* Function Name  : void epd::command(unsigned char cmd, unsigned char* data, 
*                                    unsigned short datalen)
* Description    : Display parameter configuration
* Input          : 
* Output         : None
* Return         : 
* Attention      : None
*******************************************************************************/
void epd::command(unsigned char cmd, unsigned char* data, unsigned short datalen)
{
    unsigned short i = 0;
    unsigned short framelen = datalen + 0x0009;
    if (data == NULL) datalen = 0;
    if (framelen > CMD_SIZE) return;
    
    _cmd_buff[0] = FRAME_B;

    _cmd_buff[1] = (framelen >> 8) & 0xFF;
    _cmd_buff[2] = framelen & 0xFF;	

    _cmd_buff[3] = cmd;

    for (i = 0; i < datalen; i++)
    {
        _cmd_buff[i+4] = data[i];
    }
    _cmd_buff[datalen+4] = FRAME_E0;
    _cmd_buff[datalen+5] = FRAME_E1;
    _cmd_buff[datalen+6] = FRAME_E2;
    _cmd_buff[datalen+7] = FRAME_E3;	
    _cmd_buff[datalen+8] = checksum(_cmd_buff, datalen + 8);

    for (i = 0; i < framelen; i++)
    {
        serialPutchar(fd, _cmd_buff[i]);
    }
}
/*******************************************************************************
* Function Name  : void epd::checksum(unsigned char* data, unsigned short datalen)
* Description    : 
* Input          : 
* Output         : None
* Return         : 
* Attention      : None
*******************************************************************************/
unsigned char epd::checksum(unsigned char* data, unsigned short datalen)
{
    int i;
    unsigned char * p = data;
    unsigned char result;

    for(i = 0, result = 0; i < datalen; i++)
    {
            result ^= p[i];
    }

    return result;		
}

/*******************************************************************************
* Function Name  : void epd::clear(void)
* Description    : 
* Input          : 
* Output         : None
* Return         : 
* Attention      : None
*******************************************************************************/
void epd::clear(void)
{
    command(CMD_CLEAR, NULL, 0);
}
/*******************************************************************************
* Function Name  : void epd::refresh(void)
* Description    : 
* Input          : 
* Output         : None
* Return         : 
* Attention      : None
*******************************************************************************/
void epd::refresh(void)
{
    command(CMD_REFRESH, NULL, 0);
}
/*******************************************************************************
* Function Name  : void epd::set_rotate(unsigned char rot)
* Description    : 
* Input          : rot: EPD_NORMAL or EPD_INVERSION
* Output         : None
* Return         : 
* Attention      : None
*******************************************************************************/
void epd::set_rotate(unsigned char rot)
{
    command(CMD_SCREEN_ROTATION, &rot, 1);
}
/*******************************************************************************
* Function Name  : void screen_colour(unsigned char bkcolour, unsigned char colour)
* Description    : 
* Input          : bkcolour - background colour, colour - foreground colour
* Output         : None
* Return         : 
* Attention      : None
*******************************************************************************/
void epd::set_colour(unsigned char bkcolour, unsigned char colour)
{
    unsigned char data[2];
    data[0] = colour;
    data[1] = bkcolour;
    command(CMD_SET_COLOR, data, 2);	
}
/*******************************************************************************
* Function Name  : void epd::load_bitmap(void)
* Description    : 
* Input          : 
* Output         : None
* Return         : 
* Attention      : None
*******************************************************************************/
void epd::load_bitmap(void)
{
    command(CMD_LOAD_PIC, NULL, 0);
}

/*******************************************************************************
* Function Name  : void epd::load_font(void)
* Description    : 
* Input          : 
* Output         : None
* Return         : 
* Attention      : None
*******************************************************************************/
void epd::load_font(void)
{
    command(CMD_LOAD_FONT, NULL, 0);
}
/*******************************************************************************
* Function Name  : void epd::set_en_font(unsigned char font)
* Description    : 
* Input          : 
* Output         : None
* Return         : 
* Attention	 : None
*******************************************************************************/
void epd::set_en_font(unsigned char font)
{
    command(CMD_SET_EN_FONT, &font, 1);
}
/*******************************************************************************
* Function Name  : void epd::set_en_font(unsigned char font)
* Description    : 
* Input          : 
* Output         : None
* Return         : 
* Attention	 : None
*******************************************************************************/
void epd::set_ch_font(unsigned char font)
{
    command(CMD_SET_CH_FONT, &font, 1);
}
/*******************************************************************************
* Function Name  : void epd::draw_pixel(short x0, short y0)
* Description    : 
* Input          : 
* Output         : None
* Return         : 
* Attention	 : None
*******************************************************************************/
void epd::draw_pixel(short x0, short y0)
{
    unsigned char data[4];
    data[0] = (x0 >> 8) & 0xFF;
    data[1] = x0 & 0xFF;
    data[2] = (y0 >> 8) & 0xFF;
    data[3] = y0 & 0xFF;
    command(CMD_DRAW_PIXEL, data, 4);
}
/*******************************************************************************
* Function Name  : void epd::draw_line(short x0, short y0 , short x1, short y1)
* Description    : 
* Input          : 
* Output         : None
* Return         : 
* Attention	 : None
*******************************************************************************/
void epd::draw_line(short x0, short y0 , short x1, short y1)
{
    unsigned char data[8];
    data[0] = (x0 >> 8) & 0xFF;
    data[1] = x0 & 0xFF;
    data[2] = (y0 >> 8) & 0xFF;
    data[3] = y0 & 0xFF;
    data[4] = (x1 >> 8) & 0xFF;
    data[5] = x1 & 0xFF;
    data[6] = (y1 >> 8) & 0xFF;
    data[7] = y1 & 0xFF;	
    command(CMD_DRAW_LINE, data, 8);
	
}
/*******************************************************************************
* Function Name  : void epd::draw_rect(short x0, short y0, short x1, short y1)
* Description    : 
* Input          : 
* Output         : None
* Return         : 
* Attention	 : None
*******************************************************************************/
void epd::draw_rect(short x0, short y0, short x1, short y1)
{
    unsigned char data[8];
    data[0] = (x0 >> 8) & 0xFF;
    data[1] = x0 & 0xFF;
    data[2] = (y0 >> 8) & 0xFF;
    data[3] = y0 & 0xFF;
    data[4] = (x1 >> 8) & 0xFF;
    data[5] = x1 & 0xFF;
    data[6] = (y1 >> 8) & 0xFF;
    data[7] = y1 & 0xFF;	
    command(CMD_DRAW_RECT, data, 8);		
}

/*******************************************************************************
* Function Name  : void epd::fill_rect(short x0, short y0, short x1, short y1)
* Description    : 
* Input          : 
* Output         : None
* Return         : 
* Attention	 : None
*******************************************************************************/
void epd::fill_rect(short x0, short y0, short x1, short y1)
{
    unsigned char data[8];
    data[0] = (x0 >> 8) & 0xFF;
    data[1] = x0 & 0xFF;
    data[2] = (y0 >> 8) & 0xFF;
    data[3] = y0 & 0xFF;
    data[4] = (x1 >> 8) & 0xFF;
    data[5] = x1 & 0xFF;
    data[6] = (y1 >> 8) & 0xFF;
    data[7] = y1 & 0xFF;	
    command(CMD_FILL_RECT, data, 8);
}
/*******************************************************************************
* Function Name  : void epd::draw_circle(short x0, short y0, short r)
* Description    : 
* Input          : 
* Output         : None
* Return         : 
* Attention	 : None
*******************************************************************************/
void epd::draw_circle(short x0, short y0, short r)
{
    unsigned char data[6];
    data[0] = (x0 >> 8) & 0xFF;
    data[1] = x0 & 0xFF;
    data[2] = (y0 >> 8) & 0xFF;
    data[3] = y0 & 0xFF;
    data[4] = (r >> 8) & 0xFF;
    data[5] = r & 0xFF;
    command(CMD_DRAW_CIRCLE, data, 6);
}
/*******************************************************************************
* Function Name  : void epd::fill_circle(short x0, short y0, short r)
* Description    : 
* Input          : 
* Output         : None
* Return         : 
* Attention	 : None
*******************************************************************************/
void epd::fill_circle(short x0, short y0, short r)
{
    unsigned char data[6];
    data[0] = (x0 >> 8) & 0xFF;
    data[1] = x0 & 0xFF;
    data[2] = (y0 >> 8) & 0xFF;
    data[3] = y0 & 0xFF;
    data[4] = (r >> 8) & 0xFF;
    data[5] = r & 0xFF;
    command(CMD_FILL_CIRCLE, data, 6);
}
/*******************************************************************************
* Function Name  : void epd::draw_triangle(short x0, short y0, short x1, short y1, short x2, short y2)
* Description    : 
* Input          : 
* Output         : None
* Return         : 
* Attention	 : None
*******************************************************************************/
void epd::draw_triangle(short x0, short y0, short x1, short y1, short x2, short y2)
{
    unsigned char data[12];	
    data[0] = (x0 >> 8) & 0xFF;
    data[1] = x0 & 0xFF;
    data[2] = (y0 >> 8) & 0xFF;
    data[3] = y0 & 0xFF;
    data[4] = (x1 >> 8) & 0xFF;
    data[5] = x1 & 0xFF;
    data[6] = (y1 >> 8) & 0xFF;
    data[7] = y1 & 0xFF;	
    data[8] = (x2 >> 8) & 0xFF;
    data[9] = x2 & 0xFF;
    data[10] = (y2 >> 8) & 0xFF;
    data[11] = y2 & 0xFF;
    command(CMD_DRAW_TRIANGLE, data, 12);
}
/*******************************************************************************
* Function Name  : void epd::fill_triangle(short x0, short y0, short x1, short y1, short x2, short y2)
* Description    : 
* Input          : 
* Output         : None
* Return         : 
* Attention	 : None
*******************************************************************************/
void epd::fill_triangle(short x0, short y0, short x1, short y1, short x2, short y2)
{
    unsigned char data[12];	
    data[0] = (x0 >> 8) & 0xFF;
    data[1] = x0 & 0xFF;
    data[2] = (y0 >> 8) & 0xFF;
    data[3] = y0 & 0xFF;
    data[4] = (x1 >> 8) & 0xFF;
    data[5] = x1 & 0xFF;
    data[6] = (y1 >> 8) & 0xFF;
    data[7] = y1 & 0xFF;	
    data[8] = (x2 >> 8) & 0xFF;
    data[9] = x2 & 0xFF;
    data[10] = (y2 >> 8) & 0xFF;
    data[11] = y2 & 0xFF;
    command(CMD_FILL_TRIANGLE, data, 12);	
}
/*******************************************************************************
* Function Name  : void epd_clear(void)
* Description    : 
* Input          : 
* Output         : None
* Return         : 
* Attention      : None
*******************************************************************************/
//void epd::clear(void)
//{	
//    command(CMD_CLEAR, NULL, 0);	
//}

/*******************************************************************************
* Function Name  : void epd::disp_char(unsigned char ch, int x0, int y0);
* Description    : 
* Input          : 
* Output         : None
* Return         : 
* Attention	 : None
*******************************************************************************/
void epd::draw_char(const char ch, int x0, int y0)
{
	unsigned char buff[2];
	
	buff[0] = ch;
	buff[1] = 0;
	
	draw_string((const char*)buff, x0, y0);
}
/*******************************************************************************
* Function Name  : void epd::disp_string(unsigned char* str, short x0, short y0)
* Description    : 
* Input          : 
* Output         : None
* Return         : 
* Attention      : None
*******************************************************************************/
void epd::draw_string(const char* str, short x0, short y0)
{
    unsigned char databuff[CMD_SIZE];
    int string_size;
    
    string_size = strlen((const char *)str) + 1;
    databuff[0] = (x0 >> 8) & 0xFF;
    databuff[1] = x0 & 0xFF;
    databuff[2] = (y0 >> 8) & 0xFF;
    databuff[3] = y0 & 0xFF;
    strcpy((char *)(&databuff[4]), str);
    
    command(CMD_DRAW_STRING, databuff, string_size + 4);
}
/*******************************************************************************
* Function Name  : void epd::draw_bitmap(unsigned char* str, short x0, short y0)
* Description    : 
* Input          : 
* Output         : None
* Return         : 
* Attention      : None
*******************************************************************************/
void epd::draw_bitmap(const char* str, short x0, short y0)
{
    unsigned char databuff[CMD_SIZE];
    int string_size;
    
    string_size = strlen((const char *)str) + 1;
    databuff[0] = (x0 >> 8) & 0xFF;
    databuff[1] = x0 & 0xFF;
    databuff[2] = (y0 >> 8) & 0xFF;
    databuff[3] = y0 & 0xFF;
    strcpy((char *)(&databuff[4]), str);
    
    command(CMD_DRAW_BITMAP, databuff, string_size + 4);	
}