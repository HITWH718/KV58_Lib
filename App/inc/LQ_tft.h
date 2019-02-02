#ifndef _LQ_TFT_H_
#define _LQ_TFT_H_

#include "headfile.h"
#include "SPI_software.h"

#define SIM_SPI     1

/*******************接口定义******************************/
#define TFT18W        132
#define TFT18H        162

#define LCDH    60  //OLED显示的行数
#define LCDW    94  //OLED显示的列数

#define TFT_DC_PIN		PTE6	//液晶命令位硬件定义
#define TFT_REST_PIN	PTE8   //液晶复位引脚定义

#define TFT_DC(x)   	gpio_set(TFT_DC_PIN, x);
#define TFT_REST(x) 	gpio_set(TFT_REST_PIN, x);

#define	u16RED		0xf800
#define	u16GREEN	0x07e0
#define	u16BLUE		0x001f
#define	u16PURPLE	0xf81f
#define	u16YELLOW	0xffe0
#define	u16CYAN		0x07ff 		//蓝绿色
#define	u16ORANGE	0xfc08
#define	u16BLACK	0x0000
#define	u16WHITE	0xffff


/*****************私有函数声明*********************************/

extern void TFT_Init();								 //LCD初始化
extern void TFTSPI_Addr_Rst(void);							         //DDRAM地址重置
extern void TFTSPI_Set_Pos(uint8_t xs,uint8_t ys,uint8_t xe,uint8_t ye);	                         //定位显示像素位置
extern void TFTSPI_CLS(uint16_t color);					                         //全屏显示某种颜色
extern void TFTSPI_Draw_Part(uint8_t xs,uint8_t ys,uint8_t xe,uint8_t ye,uint16_t color_dat);     //填充矩形区域，行起始、终止坐标，列起始、终止坐标，颜色
extern void TFTSPI_Draw_Line(uint8_t xs,uint8_t ys,uint8_t xe,uint8_t ye,uint16_t color_dat);     //画线，行起始、终止坐标，列起始、终止坐标，颜色
extern void TFTSPI_Draw_Rectangle(uint8_t xs,uint8_t ys,uint8_t xe,uint8_t ye,uint16_t color_dat);//画矩形边框，行起始、终止坐标，列起始、终止坐标，颜色
extern void TFTSPI_Draw_Circle(uint8_t x,uint8_t y,uint8_t r,uint16_t color_dat);                 //画圆形边框，圆心横坐标、纵坐标，半径，颜色
extern void TFTSPI_Draw_Dot(uint8_t x,uint8_t y,uint16_t color_dat);	       //画点，横坐标，纵坐标，颜色
extern void TFTSPI_Fill_Area(uint8_t xs,uint8_t ys,uint8_t xe,uint8_t ye,uint16_t color);
extern void TFTSPI_P8X16(uint8_t x, uint8_t y, uint8_t c_dat,uint16_t word_color,uint16_t back_color);
extern void TFTSPI_P8X16Str(uint8_t x, uint8_t y, uint8_t *s_dat,uint16_t word_color,uint16_t back_color);
extern void TFTSPI_P6X8(uint8_t x, uint8_t y, uint8_t c_dat,uint16_t word_color,uint16_t back_color);
extern void TFTSPI_P6X8Str(uint8_t x, uint8_t y, uint8_t *s_dat,uint16_t word_color,uint16_t back_color);
extern void TFTSPI_P6X8NUM(uint8_t x, uint8_t y, uint16_t num, uint8_t num_bit,uint16_t word_color,uint16_t back_color);
extern void TFTSPI_P8X8(uint8_t x, uint8_t y, uint8_t c_dat,uint16_t word_color,uint16_t back_color);
extern void TFTSPI_P8X8Str(uint8_t x, uint8_t y, uint8_t *s_dat,uint16_t word_color,uint16_t back_color);
extern void TFTSPI_P8X8NUM(uint8_t x, uint8_t y, uint16_t num, uint8_t num_bit,uint16_t word_color,uint16_t back_color);
extern void TFTSPI_P16x16Str(uint8_t x,uint8_t y,uint8_t *s_dat,uint16_t word_color,uint16_t back_color);
extern void TFTSPI_P16x12Str(uint8_t x,uint8_t y,uint8_t *s_dat,uint16_t word_color,uint16_t back_color);
extern void TFTSPI_Show_Pic(uint8_t xs,uint8_t ys,uint8_t xe,uint8_t ye,uint8_t *ppic);
extern void TFTSPI_Show_Pic2(uint8_t xs,uint8_t ys,uint8_t w,uint8_t h,uint8_t *ppic);
extern void TFTSPI_Show_Pic3(uint8_t xs,uint8_t ys,uint8_t w,uint8_t h,uint8_t ppic[LCDH][LCDW]);
extern void ColumnarSetting(uint8_t x,uint8_t y, uint8_t w,uint8_t h,uint16_t fclolr,uint16_t bcolor);
extern void DrawingColumnar(uint8_t t,uint8_t duty,uint8_t cl);
extern void DrawSpectrum(void);
extern void Clear_Columnar(void);

#endif
