#ifndef ILI9341_STM32_DRIVER_H
#define ILI9341_STM32_DRIVER_H

#include "stm32f1xx_hal.h"


#define HORIZONTAL_IMAGE	  0
#define VERTICAL_IMAGE	      1

#define ILI9341_SCREEN_HEIGHT 240 
#define ILI9341_SCREEN_WIDTH  320

//SPI INSTANCE
#define HSPI_INSTANCE         &hspi2

//DATA COMMAND PIN AND PORT, STANDARD GPIO
#define LCD_DC_PORT			  LCD_DC_GPIO_Port
#define LCD_DC_PIN			  LCD_DC_Pin

//RESET PIN AND PORT, STANDARD GPIO
#define	LCD_RST_PORT		  LCD_RES_GPIO_Port
#define	LCD_RST_PIN			  LCD_RES_Pin


#define BURST_MAX_SIZE 	      500

#define BLACK       0x0000      
#define NAVY        0x000F      
#define DARKGREEN   0x03E0      
#define DARKCYAN    0x03EF      
#define MAROON      0x7800      
#define PURPLE      0x780F      
#define OLIVE       0x7BE0      
#define LIGHTGREY   0xC618      
#define DARKGREY    0x7BEF      
#define BLUE        0x001F      
#define GREEN       0x07E0      
#define CYAN        0x07FF      
#define RED         0xF800     
#define MAGENTA     0xF81F      
#define YELLOW      0xFFE0      
#define WHITE       0xFFFF      
#define ORANGE      0xFD20      
#define GREENYELLOW 0xAFE5     
#define PINK        0xF81F

#define SCREEN_VERTICAL_1		0
#define SCREEN_HORIZONTAL_1		1
#define SCREEN_VERTICAL_2	    2
#define SCREEN_HORIZONTAL_2		3




typedef struct{
void (*reset)(void);
void (*set_rotation)(uint8_t rotation);
void (*init)(void);
void (*fill_screen)(uint16_t colour);
void (*draw_pixel)(uint16_t x,uint16_t y,uint16_t colour);
void (*draw_colour_burst)(uint16_t colour, uint32_t size);
void (*draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t colour);
void (*draw_horizontal_line)(uint16_t x, uint16_t y, uint16_t width, uint16_t colour);
void (*draw_vertical_line)(uint16_t x, uint16_t y, uint16_t height, uint16_t colour);
	
void (*draw_hollow_circle)(uint16_t x, uint16_t y, uint16_t radius, uint16_t colour);
void (*draw_filled_circle)(uint16_t x, uint16_t y, uint16_t radius, uint16_t colour);
void (*draw_hollow_cectangle_coord)(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t colour);
void (*draw_filled_rectangle_coord)(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t colour);
void (*draw_char)(char character, uint8_t x, uint16_t y, uint16_t colour, uint16_t size, uint16_t background_colour);
void (*draw_text)(const char* text, uint8_t x, uint16_t y, uint16_t colour, uint16_t size, uint16_t background_colour);
void (*draw_image)(const char* image_array, uint8_t orientation);
}lcd_type;
extern lcd_type lcd;


#endif

