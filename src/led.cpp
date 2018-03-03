//============================================================================
// Name        : led.cpp
// Author      : LebrancBW
// Version     :
// Copyright   : 
// Description : Hello World in C++, Ansi-style
//============================================================================

/*
	命令行传参格式：argv【0】为执行目录
*/
#include <iostream>
#include <wiringPi.h>
#include <string>
#include "oled.h"

using std::string;
using std::cout;
using std::endl;
int main(int argc, char* argv[]) {
	if(wiringPiSetup() == -1)
	{
		cout<< "初始化失败";
		return -1;
	}
	for(int i=1;i<argc;i++)
	{
		if(argv[i][0]=='-')
		switch(argv[i][1])
		{
			case 'u':
			case 'U':
				Draw_BMP(20, 24,up_arrow);
				LCD_Print((uint8_t)48, (uint8_t)32, argv[2]);
				cout<< "OLED屏 << " << argv[++i] <<std::endl;
				break;
			case 'D':
			case 'd':
				Draw_BMP(20, 24, down_arrow);
				LCD_Print((uint8_t)48, (uint8_t)32, argv[2]);
				cout<< "OLED屏 << " << argv[++i] <<std::endl;
				break;
			case 'm':
			case 'M':
				LCD_Print((uint8_t)0, (uint8_t)0, argv[2]);
				cout<< "OLED屏 << " << argv[++i] <<std::endl;
				break;
			case 'h':
			case 'H':
//				LCD_Print((uint8_t)0, (uint8_t)0, (char*)" OLED [-U/D/M/H] mess");
//				LCD_Print((uint8_t)0, (uint8_t)8, (char*)"-U display up arrow");
//				LCD_Print((uint8_t)0, (uint8_t)16, (char*)"-D display down arrow");
//				LCD_Print((uint8_t)0, (uint8_t)24, (char*)"-M display message");
//				LCD_Print((uint8_t)0, (uint8_t)32, (char*)"-H display help");
//				LCD_Print((uint8_t)0, (uint8_t)40, (char*)"-L display logo");
				cout <<"OLED接线：SCL:29 SDA:28 RST:27 DC:25，另外如果VCC没地方接则接入24"<<std::endl;
				cout <<"命令：OLED [-U/-D/-M/T] Message (上箭头/下箭头/直接输出/时间输出)"<<std::endl;
				cout <<"OLED [-L/-C/I] 显示logo/清屏/初始化"<<std::endl;
				break;
			case 'l':
			case 'L':
				LCD_Draw(0, 8, logo, 56, 47);
				cout<< "OLED屏幕显示Logo"<<std::endl;
				break;
			case 't':case'T':
				LCD_Print(75, 0, argv[++i]);
				break;
			case 'c':case 'C':
				LCD_CLS();
				cout <<"清屏" <<std::endl;
				break;
			case 'i':case 'I':
				LCD_Init();
				cout <<"初始化" <<std::endl;
				break;
			case 'w':case 'W':
				LCD_Print(50, 16, argv[++i]);
				LCD_Print(60, 32, (uint8_t*)argv[++i], TYPE16X16, TYPE8X16);
				LCD_Draw(84, 32, dgree, 16, 16);
//				LCD_Print(60, 48, argv[++i]);

				break;
			default:
				cout << "不合法的输入！"<< std::endl;
				LCD_Print((uint8_t)0, (uint8_t)0, (char*)"Invaild command!");

		};
	}
	return 0;
}
