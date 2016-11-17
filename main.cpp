/* 
 * File:   main.cpp
 * Author: Doan Manh
 * Site: http://elinux.vn/
 * Created on 16 November 2016, 10:29
 */

#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wiringPi.h>
#include <wiringSerial.h>
#include "epd.h"

#define WAKEUP_PIN  4
#define RESET_PIN   5

using namespace std;

/*
 *Example for using 4.3inch e-paper driver
 */
int main(int argc, char** argv) {

    //create new epaper obj
    epd e_paper("/dev/ttyAMA0", 115200, WAKEUP_PIN, RESET_PIN);
    
    //configure parameters
    e_paper.wakeup();
    e_paper.set_memory(MEM_TF);
    e_paper.set_colour(WHITE, BLACK);
    e_paper.clear();
    e_paper.set_ch_font(GBK32);
    e_paper.set_en_font(ASCII32);
    
    //draw string
    e_paper.draw_string("ASCII32: Hello, World!", 0, 300);
    e_paper.refresh();
    delay(3000);
    
    //draw bitmap from TF card
    e_paper.clear();
    e_paper.draw_bitmap("PIC0.BMP", 0, 0);
    e_paper.refresh();	
    delay(3000);
    
    //draw some circles
    e_paper.clear();
    e_paper.fill_circle(100, 100, 50);
    e_paper.refresh();	
    delay(3000);
    e_paper.clear();
    e_paper.draw_circle(500, 500, 50);
    e_paper.refresh();	
    delay(3000);
    
    //draw line
    e_paper.clear();
    e_paper.draw_line(0, 0, 100, 100);
    e_paper.refresh();
    delay(2000);
    
    //draw bitmaps
    e_paper.clear();
    e_paper.draw_bitmap("PIC1.BMP", 0, 100);
    e_paper.draw_bitmap("PIC3.BMP", 400, 100);
    e_paper.refresh();	
    delay(5000);
    
    //change baud rate
    e_paper.set_baud(9600);
    //test new baud
    e_paper.clear();
    e_paper.draw_string("ASCII32: 9600 baud", 0, 300);
    e_paper.refresh();
    delay(3000);
    return 0;
}

