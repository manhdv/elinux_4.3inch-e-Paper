/* 
 * File:   epd.h
 * Author: Doan Manh
 *
 * Created on 16 November 2016, 10:32
 */

#ifndef EPD_H
#define EPD_H

#define    CMD_SIZE                           64	

/*
frame format
*/
#define    FRAME_B                            0xA5
#define    FRAME_E0                           0xCC
#define    FRAME_E1                           0x33
#define    FRAME_E2                           0xC3
#define    FRAME_E3                           0x3C


/*
color define
*/
#define    WHITE                              0x03
#define    GRAY                               0x02
#define    DARK_GRAY                          0x01
#define    BLACK                              0x00

/*
command define
*/
#define    CMD_HANDSHAKE                      0x00                                                     //handshake
#define    CMD_SET_BAUD                       0x01                                                     //set baud
#define    CMD_READ_BAUD                      0x02                                                     //read baud
#define    CMD_MEMORYMODE                     0x07                                                     //set memory mode
#define    CMD_STOPMODE                       0x08                                                     //enter stop mode 
#define    CMD_REFRESH                        0x0A                                                     //refresh
#define    CMD_SCREEN_ROTATION                0x0D                                                     //set screen rotation
#define    CMD_LOAD_FONT                      0x0E                                                     //load font
#define    CMD_LOAD_PIC                       0x0F                                                     //load picture

#define    CMD_SET_COLOR                      0x10                                                     //set color
#define    CMD_READ_EN_FONT                   0x1C                                                     //read englist font
#define    CMD_READ_CH_FONT                   0x1D                                                     //read chinese font
#define    CMD_SET_EN_FONT                    0x1E                                                     //set english font
#define    CMD_SET_CH_FONT                    0x1F                                                     //set chinese font

#define    CMD_DRAW_PIXEL                     0x20                                                     //set pixel
#define    CMD_DRAW_LINE                      0x22                                                     //draw line
#define    CMD_FILL_RECT                      0x24                                                     //fill rectangle
#define    CMD_DRAW_RECT                      0x25                                                     //draw rectangle
#define    CMD_DRAW_CIRCLE                    0x26                                                     //draw circle
#define    CMD_FILL_CIRCLE                    0x27                                                     //fill circle
#define    CMD_DRAW_TRIANGLE                  0x28                                                     //draw triangle
#define    CMD_FILL_TRIANGLE                  0x29                                                     //fill triangle
#define    CMD_CLEAR                          0x2E                                                     //clear screen use back color

#define    CMD_DRAW_STRING                    0x30                                                     //draw string

#define    CMD_DRAW_BITMAP                    0x70                                                     //draw bitmap


/*
FONT
*/
#define    GBK32                              0x01
#define    GBK48                              0x02
#define    GBK64                              0x03
	
#define    ASCII32                            0x01
#define    ASCII48                            0x02
#define    ASCII64                            0x03


/*
Memory Mode
*/
#define    MEM_NAND                           0
#define    MEM_TF                             1

/*
set screen rotation
*/
#define    EPD_NORMAL                         0                                                        //screen normal
#define    EPD_INVERSION                      1                                                        //screen inversion  

class epd {
    int wakeup_pin;
    int reset_pin;
    const char * dev;
    int fd;
    unsigned char checksum(unsigned char* data, unsigned short datalen);
    unsigned char _cmd_buff[CMD_SIZE];
public:
    epd(const char* device, int baud, int wakeup, int reset);
    ~epd(void);
    
    //screen control
    void init(void);
    void reset(void);
    void wakeup(void);
    void clear(void);
    void refresh(void);
    void handshake(void);
    void set_baud(int baud);
    void set_memory(unsigned char mode);
    void set_rotate(unsigned char rot);
    void set_colour(unsigned char bkcolour, unsigned char colour);
    void set_en_font(unsigned char font);
    void set_ch_font(unsigned char font);
    void load_bitmap(void);
    void load_font(void);
    
    //draw some basic sharps
    void draw_pixel(short x0, short y0);
    void draw_char(const char ch, int x0, int y0);
    void draw_string(const char* str, short x0, short y0);
    void draw_bitmap(const char* str, short x0, short y0);
    void draw_line(short x0, short y0 , short x1, short y1);
    void draw_circle(short x0, short y0, short r);
    void fill_circle(short x0, short y0, short r);
    void draw_rect(short x0, short y0, short x1, short y1);
    void fill_rect(short x0, short y0, short x1, short y1);
    void draw_triangle(short x0, short y0, short x1, short y1, short x3, short y3);
    void fill_triangle(short x0, short y0, short x1, short y1, short x3, short y3);
    
    //main command function
    void command(unsigned char cmd, unsigned char* data, unsigned short datalen);
};
#endif /* EPD_H */

