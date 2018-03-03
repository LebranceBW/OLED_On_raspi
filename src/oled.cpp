/*OLED底层驱动
*@采用列行式，分辨率为128*64
*存放格式如下.
*[0]0 1 2 3 ... 127
*[1]0 1 2 3 ... 127
*[2]0 1 2 3 ... 127
*[3]0 1 2 3 ... 127
*[4]0 1 2 3 ... 127
*[5]0 1 2 3 ... 127
*[6]0 1 2 3 ... 127
*[7]0 1 2 3 ... 127
*/
#include "oled.h"

//汉字大小，英文数字大小
const int SCL = 29;
const int SDA = 28;
const int RST = 27;
const int DC = 25;
const int VCC = 24;

void LCD_WrDat(uint8_t data)
{
	uint8_t i=8;
  digitalWrite(DC, 1);
  digitalWrite(SCL,  0);
  while(i--)
  {
    if(data&0x80){digitalWrite(SDA, 1);}
    else{digitalWrite(SDA,  0);}
    digitalWrite(SCL, 1);
    digitalWrite(SCL,  0);
    data<<=1;
  }
}
void LCD_WrCmd(uint8_t cmd)
{
	uint8_t i=8;
	digitalWrite(DC, 0);
  digitalWrite(SCL,  0);
  while(i--)
  {
    if(cmd&0x80){digitalWrite(SDA, 1);}
    else{digitalWrite(SDA,  0);}
    digitalWrite(SCL, 1);
    digitalWrite(SCL, 0);
    cmd<<=1;;
  }
}

void LCD_Set_Pos(uint8_t x, uint8_t y)
{
  LCD_WrCmd(0xb0+(y>>3));
  LCD_WrCmd(((x&0xf0)>>4)|0x10);
  LCD_WrCmd((x&0x0f)|0x01);
}
void LCD_Fill(uint8_t bmp_data)
{
	uint8_t y,x;

	for(y=0;y<8;y++)
	{
		LCD_WrCmd(0xb0+y);
		LCD_WrCmd(0x01);
		LCD_WrCmd(0x10);
		for(x=0;x<X_WIDTH;x++)
			LCD_WrDat(bmp_data);
	}
}
void LCD_CLS(void)
{
	uint8_t y,x;
	for(y=0;y<8;y++)
	{
		LCD_WrCmd(0xb0+y);
		LCD_WrCmd(0x01);
		LCD_WrCmd(0x10);
		for(x=0;x<X_WIDTH;x++)
			LCD_WrDat(0);
	}
}

void LCD_CLS_y(char y)
{
	uint8_t x;

	LCD_WrCmd(0xb0+(y>>3));
	LCD_WrCmd(0x01);
	LCD_WrCmd(0x10);
	for(x=0;x<X_WIDTH;x++)
	{
		LCD_WrDat(0);
	}

}


void LCD_CLS_line_area(uint8_t start_x,uint8_t start_y,uint8_t width)
{
	uint8_t x;

	LCD_WrCmd(0xb0+(start_y>>3));
	LCD_WrCmd(0x01);
	LCD_WrCmd(0x10);
	for(x=start_x;x<width;x++)
	{
		LCD_WrDat(0);
	}

}


void LCD_DLY_ms(uint16_t ms)
{
	delay(ms/2);
}

void LCD_Init(void)
{
	    pinMode (SCL, OUTPUT);
		pinMode (SDA, OUTPUT);
		pinMode (RST, OUTPUT);
		pinMode (DC, OUTPUT);
		pinMode (VCC, OUTPUT);
		digitalWrite(VCC, 1);
		digitalWrite(SCL, 1);
		digitalWrite(RST,  0);
		LCD_DLY_ms(20);
		digitalWrite(RST, 1);

  LCD_WrCmd(0xae);//--turn off oled panel
  LCD_WrCmd(0x00);//---set low column address
  LCD_WrCmd(0x10);//---set high column address
  LCD_WrCmd(0x40);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
  LCD_WrCmd(0x81);//--set contrast control register
  LCD_WrCmd(0xcf); // Set SEG Output Current Brightness
  LCD_WrCmd(0xa1);//--Set SEG/Column Mapping     0xa0���ҷ��� 0xa1����
  LCD_WrCmd(0xc8);//Set COM/Row Scan Direction   0xc0���·��� 0xc8����
  LCD_WrCmd(0xa6);//--set normal display
  LCD_WrCmd(0xa8);//--set multiplex ratio(1 to 64)
  LCD_WrCmd(0x3f);//--1/64 duty
  LCD_WrCmd(0xd3);//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
  LCD_WrCmd(0x00);//-not offset
  LCD_WrCmd(0xd5);//--set display clock divide ratio/oscillator frequency
  LCD_WrCmd(0x80);//--set divide ratio, Set Clock as 100 Frames/Sec
  LCD_WrCmd(0xd9);//--set pre-charge period
  LCD_WrCmd(0xf1);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
  LCD_WrCmd(0xda);//--set com pins hardware configuration
  LCD_WrCmd(0x12);
  LCD_WrCmd(0xdb);//--set vcomh
  LCD_WrCmd(0x40);//Set VCOM Deselect Level
  LCD_WrCmd(0x20);//-Set Page Addressing Mode (0x00/0x01/0x02)
  LCD_WrCmd(0x02);//
  LCD_WrCmd(0x8d);//--set Charge Pump enable/disable
  LCD_WrCmd(0x14);//--set(0x10) disable
  LCD_WrCmd(0xa4);// Disable Entire Display On (0xa4/0xa5)
  LCD_WrCmd(0xa6);// Disable Inverse Display On (0xa6/a7)
  LCD_WrCmd(0xaf);//--turn on oled panel
  LCD_Set_Pos(0,0);
  LCD_CLS();
}
void LCD_PutPixel(uint8_t x,uint8_t y)
{
	uint8_t data1;  //data1��ǰ�������

    //LCD_Set_Pos(x,y);
	data1 = 0x01<<(y%8);
	LCD_WrCmd(0xb0+(y>>3));
	LCD_WrCmd(((x&0xf0)>>4)|0x10);
	LCD_WrCmd((x&0x0f)|0x00);
	LCD_WrDat(data1);
}
void LCD_Put_Column(uint8_t x,uint8_t y,uint8_t data)
{
	LCD_WrCmd(0xb0+(y>>3));
	LCD_WrCmd(((x&0xf0)>>4)|0x10);
	LCD_WrCmd((x&0x0f)|0x00);
	LCD_WrDat(data);
}


void LCD_Rectangle(uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2,uint8_t gif)
{
	uint8_t n;

	LCD_Set_Pos(x1,y1>>3);
	for(n=x1;n<=x2;n++)
	{
		LCD_WrDat(0x01<<(y1%8));
		if(gif == 1) 	LCD_DLY_ms(20);
	}
	LCD_Set_Pos(x1,y2>>3);
  for(n=x1;n<=x2;n++)
	{
		LCD_WrDat(0x01<<(y2%8));
		if(gif == 1) 	LCD_DLY_ms(5);
	}

}

void LCD_P6x8Str(uint8_t x,uint8_t y,uint8_t *ch,const uint8_t *F6x8)
{
	uint8_t c=0,i=0,j=0;

	while (*(ch+j)!='\0')
	{
		c =*(ch+j)-32;
		if(x>126)
		{
			x=0;
			y++;
		}
		LCD_Set_Pos(x,y);
		for(i=0;i<6;i++)
		{
			LCD_WrDat(*(F6x8+c*6+i));
		}
		x+=6;
		j++;
	}
}
void LCD_P8x16Str(uint8_t x,uint8_t y,uint8_t *ch,const uint8_t *F8x16)
{
  uint8_t c=0,i=0,j=0;

  while (*(ch+j)!='\0')
  {
    c =*(ch+j)-32;
    if(x>120)
	{
		x=0;
		y++;
	}
    LCD_Set_Pos(x,y);
  	for(i=0;i<8;i++)
  	{
  	  	LCD_WrDat(*(F8x16+c*16+i));
  	}
  	LCD_Set_Pos(x,y+8);
  	for(i=0;i<8;i++)
  	{
  	  	LCD_WrDat(*(F8x16+c*16+i+8));
  	}
  	x+=8;
  	j++;
  }
}
void LCD_P14x16Str(uint8_t x,uint8_t y,uint8_t ch[],const uint8_t *F14x16_Idx,const uint8_t *F14x16)
{
	uint8_t wm=0,ii = 0;
	uint16_t adder=1;

	while(ch[ii] != '\0')
	{
	  	wm = 0;
	  	adder = 1;
	  	while(*(F14x16_Idx+wm) > 127)
	  	{
	  		if(*(F14x16_Idx+wm) == ch[ii])
	  		{
	  			if(*(F14x16_Idx+wm+1) == ch[ii + 1])
	  			{
	  				adder = wm * 14;
	  				break;
	  			}
	  		}
	  		wm += 2;
	  	}
	  	if(x>118)
		{
			x=0;
			y++;
		}
	  	LCD_Set_Pos(x , y);
	  	if(adder != 1)// ��ʾ����
	  	{
	  		LCD_Set_Pos(x , y);
	  		for(wm = 0;wm < 14;wm++)
	  		{
	  			LCD_WrDat(*(F14x16+adder));
	  			adder += 1;
	  		}
	  		LCD_Set_Pos(x,y + 1);
	  		for(wm = 0;wm < 14;wm++)
	  		{
	  			LCD_WrDat(*(F14x16+adder));
	  			adder += 1;
	  		}
	  	}
	  	else			  //��ʾ�հ��ַ�
	  	{
	  		ii += 1;
	      	LCD_Set_Pos(x,y);
	  		for(wm = 0;wm < 16;wm++)
	  		{
	  			LCD_WrDat(0);
	  		}
	  		LCD_Set_Pos(x,y + 1);
	  		for(wm = 0;wm < 16;wm++)
	  		{
	  			LCD_WrDat(0);
	  		}
	  	}
	  	x += 14;
	  	ii += 2;
	}
}
void LCD_P16x16Str(uint8_t x,uint8_t y,uint8_t *ch,const uint8_t *F16x16_Idx,const uint8_t *F16x16)
{
	uint8_t wm=0,ii = 0;
	uint16_t adder=1;

	while(*(ch+ii) != '\0')
	{
  	wm = 0;
  	adder = 1;
  	while(*(F16x16_Idx+wm) > 127)
  	{
  		if(*(F16x16_Idx+wm) == *(ch+ii))
  		{
  			if(*(F16x16_Idx+wm + 1) == *(ch+ii + 1))
  			{
  				adder = wm * 16;
  				break;
  			}
  		}
  		wm += 2;
  	}
  	if(x>118){x=0;y++;}
  	LCD_Set_Pos(x , y);
  	if(adder != 1)// ��ʾ����
  	{
  		LCD_Set_Pos(x , y);
  		for(wm = 0;wm < 16;wm++)
  		{
  			LCD_WrDat(*(F16x16+adder));
  			adder += 1;
  		}
  		LCD_Set_Pos(x,y + 8);
  		for(wm = 0;wm < 16;wm++)
  		{
  			LCD_WrDat(*(F16x16+adder));
  			adder += 1;
  		}
  	}
  	else			  //��ʾ�հ��ַ�
  	{
  		ii += 1;
      	LCD_Set_Pos(x,y);
  		for(wm = 0;wm < 16;wm++)
  		{
  			LCD_WrDat(0);
  		}
  		LCD_Set_Pos(x,y + 1);
  		for(wm = 0;wm < 16;wm++)
  		{
  			LCD_WrDat(0);
  		}
  	}
  	x += 16;
  	ii += 2;
	}
}


void LCD_Print(uint8_t x, uint8_t y, uint8_t *ch,uint8_t char_size, uint8_t ascii_size)
{
	uint8_t ch2[3];
	uint8_t ii=0;
	while(*(ch+ii) != '\0')
	{
		if(*(ch+ii) > 127)
		{
			ch2[0] = *(ch+ii);
	 		ch2[1] = *(ch+ii+1);
			ch2[2] = '\0';
			LCD_P16x16Str(x , y, ch2,F16x16_Idx,F16x16);
			x += 16;
			ii += 2;
		}
		else
		{
			ch2[0] = *(ch+ii);
			ch2[1] = '\0';
			if(TYPE8X16==ascii_size)
			{
				LCD_P8x16Str(x , y ,ch2,F8X16);
				x += 8;
			}
			else if(TYPE6X8==ascii_size)
			{
				LCD_P6x8Str(x , y ,ch2,F6x8);	//��ʾ��ĸ
				x += 6;
			}

			ii+= 1;
		}
	}
}
void LCD_Print(uint8_t x, uint8_t y, char* ch)
{
	LCD_Print(x, y, (uint8_t*)ch, TYPE6X8, TYPE6X8);
}
void Draw_BMP(uint8_t x,uint8_t y,const uint8_t *bmp)
{
	uint8_t wm=0;
	uint8_t adder=0;

  	LCD_Set_Pos(x , y);
	for(wm = 0;wm < 16;wm++)
	{
		LCD_WrDat(*(bmp+adder));
		adder += 1;
	}
	LCD_Set_Pos(x,y + 8);
	for(wm = 0;wm < 16;wm++)
	{
		LCD_WrDat(*(bmp+adder));
		adder += 1;
	}
}
void LCD_Draw(uint8_t x, uint8_t y, const uint8_t* bmp, uint8_t rows, uint8_t coloms)
{
	//rows 行 coloms列
	LCD_Set_Pos(x, y);
	for(int i=0;i<rows/8;i++)
	{
		for(int j=0;j<coloms;j++)
			LCD_WrDat(*(bmp++));
		y+=8;
		LCD_Set_Pos(x, y);
	}
}

