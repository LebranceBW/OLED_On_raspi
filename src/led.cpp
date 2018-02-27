//============================================================================
// Name        : led.cpp
// Author      : LebrancBW
// Version     :
// Copyright   : 
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <wiringPi.h>

#include "oled.h"
using std::cout;
using std::endl;
int main(void) {
	if(wiringPiSetup() == -1)
	{
		cout<< "初始化失败";
		return -1;
	}
	LCD_Init();
	LCD_Print(0, 0, (uint8_t*)"hello world!", TYPE16X16, TYPE8X16);
	return 0;
}
