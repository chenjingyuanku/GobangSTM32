/* Includes ------------------------------------------------------------------*/
#include "ili9341_stm32_driver.h"
#include "5x5_font.h"
#include "spi.h"
#include "gpio.h"

/* Global Variables ------------------------------------------------------------------*/
volatile uint16_t LCD_HEIGHT = ILI9341_SCREEN_HEIGHT;
volatile uint16_t LCD_WIDTH = ILI9341_SCREEN_WIDTH;


void ili9341_spi_send(uint8_t spi_data);
void ili9341_write_command(uint8_t command);
void ili9341_write_data(uint8_t data);
void ili9341_set_address(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
void ili9341_reset(void);
void ili9341_set_rotation(uint8_t rotation);
void ili9341_enable(void);
void ili9341_init(void);
void ili9341_fill_screen(uint16_t colour);
void ili9341_draw_pixel(uint16_t x,uint16_t y,uint16_t colour);
void ili9341_draw_colour_burst(uint16_t colour, uint32_t size);


void ili9341_draw_rectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t colour);
void ili9341_draw_horizontal_line(uint16_t x, uint16_t y, uint16_t width, uint16_t colour);
void ili9341_draw_vertical_line(uint16_t x, uint16_t y, uint16_t height, uint16_t colour);
	
void ili9341_draw_hollow_circle(uint16_t x, uint16_t y, uint16_t radius, uint16_t colour);
void ili9341_draw_filled_circle(uint16_t x, uint16_t y, uint16_t radius, uint16_t colour);
void ili9341_draw_hollow_cectangle_coord(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t colour);
void ili9341_draw_filled_rectangle_coord(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t colour);
void ili9341_draw_char(char character, uint8_t x, uint16_t y, uint16_t colour, uint16_t size, uint16_t background_colour);
void ili9341_draw_text(const char* text, uint8_t x, uint16_t y, uint16_t colour, uint16_t size, uint16_t background_colour);
void ili9341_draw_image(const char* image_array, uint8_t orientation);




/*Send data (char) to LCD*/
void ili9341_spi_send(uint8_t spi_data)
{
	HAL_SPI_Transmit(HSPI_INSTANCE, &spi_data, 1, 1);
}

/* Send command (char) to LCD */
void ili9341_write_command(uint8_t command)
{
	HAL_GPIO_WritePin(LCD_DC_PORT, LCD_DC_PIN, GPIO_PIN_RESET);
	ili9341_spi_send(command);
}

/* Send data (char) to LCD */
void ili9341_write_data(uint8_t data)
{
	HAL_GPIO_WritePin(LCD_DC_PORT, LCD_DC_PIN, GPIO_PIN_SET);
	ili9341_spi_send(data);
}

/* Set Address - Location block - to draw into */
void ili9341_set_address(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
	ili9341_write_command(0x2A);
	ili9341_write_data(x1 >> 8);
	ili9341_write_data(x1);
	ili9341_write_data(x2 >> 8);
	ili9341_write_data(x2);

	ili9341_write_command(0x2B);
	ili9341_write_data(y1 >> 8);
	ili9341_write_data(y1);
	ili9341_write_data(y2 >> 8);
	ili9341_write_data(y2);

	ili9341_write_command(0x2C);
}

/*HARDWARE RESET*/
void ili9341_reset(void)
{
	HAL_GPIO_WritePin(LCD_RST_PORT, LCD_RST_PIN, GPIO_PIN_RESET);
	HAL_Delay(200);
	HAL_GPIO_WritePin(LCD_RST_PORT, LCD_RST_PIN, GPIO_PIN_SET);
}

/*Ser rotation of the screen - changes x0 and y0*/
void ili9341_set_rotation(uint8_t rotation)
{

	uint8_t screen_rotation = rotation;

	ili9341_write_command(0x36);
	HAL_Delay(1);

	switch (screen_rotation)
	{
	case SCREEN_VERTICAL_1:
		ili9341_write_data(0x40 | 0x08);
		LCD_WIDTH = 240;
		LCD_HEIGHT = 320;
		break;
	case SCREEN_HORIZONTAL_1:
		ili9341_write_data(0x20 | 0x08);
		LCD_WIDTH = 320;
		LCD_HEIGHT = 240;
		break;
	case SCREEN_VERTICAL_2:
		ili9341_write_data(0x80 | 0x08);
		LCD_WIDTH = 240;
		LCD_HEIGHT = 320;
		break;
	case SCREEN_HORIZONTAL_2:
		ili9341_write_data(0x40 | 0x80 | 0x20 | 0x08);
		LCD_WIDTH = 320;
		LCD_HEIGHT = 240;
		break;
	default:
		//EXIT IF SCREEN ROTATION NOT VALID!
		break;
	}
}

/*Enable LCD display*/
void ili9341_enable(void)
{
	HAL_GPIO_WritePin(LCD_RST_PORT, LCD_RST_PIN, GPIO_PIN_SET);
	HAL_GPIO_WritePin(LCD_BLK_GPIO_Port, LCD_BLK_Pin, GPIO_PIN_SET);
}

/*Initialize LCD display*/
void ili9341_init(void)
{

	ili9341_enable();
	ili9341_reset();

	//SOFTWARE RESET
	ili9341_write_command(0x01);
	HAL_Delay(1000);

	//POWER CONTROL A
	ili9341_write_command(0xCB);
	ili9341_write_data(0x39);
	ili9341_write_data(0x2C);
	ili9341_write_data(0x00);
	ili9341_write_data(0x34);
	ili9341_write_data(0x02);

	//POWER CONTROL B
	ili9341_write_command(0xCF);
	ili9341_write_data(0x00);
	ili9341_write_data(0xC1);
	ili9341_write_data(0x30);

	//DRIVER TIMING CONTROL A
	ili9341_write_command(0xE8);
	ili9341_write_data(0x85);
	ili9341_write_data(0x00);
	ili9341_write_data(0x78);

	//DRIVER TIMING CONTROL B
	ili9341_write_command(0xEA);
	ili9341_write_data(0x00);
	ili9341_write_data(0x00);

	//POWER ON SEQUENCE CONTROL
	ili9341_write_command(0xED);
	ili9341_write_data(0x64);
	ili9341_write_data(0x03);
	ili9341_write_data(0x12);
	ili9341_write_data(0x81);

	//PUMP RATIO CONTROL
	ili9341_write_command(0xF7);
	ili9341_write_data(0x20);

	//POWER CONTROL,VRH[5:0]
	ili9341_write_command(0xC0);
	ili9341_write_data(0x23);

	//POWER CONTROL,SAP[2:0];BT[3:0]
	ili9341_write_command(0xC1);
	ili9341_write_data(0x10);

	//VCM CONTROL
	ili9341_write_command(0xC5);
	ili9341_write_data(0x3E);
	ili9341_write_data(0x28);

	//VCM CONTROL 2
	ili9341_write_command(0xC7);
	ili9341_write_data(0x86);

	//MEMORY ACCESS CONTROL
	ili9341_write_command(0x36);
	ili9341_write_data(0x48);

	//PIXEL FORMAT
	ili9341_write_command(0x3A);
	ili9341_write_data(0x55);

	//FRAME RATIO CONTROL, STANDARD RGB COLOR
	ili9341_write_command(0xB1);
	ili9341_write_data(0x00);
	ili9341_write_data(0x18);

	//DISPLAY FUNCTION CONTROL
	ili9341_write_command(0xB6);
	ili9341_write_data(0x08);
	ili9341_write_data(0x82);
	ili9341_write_data(0x27);

	//3GAMMA FUNCTION DISABLE
	ili9341_write_command(0xF2);
	ili9341_write_data(0x00);

	//GAMMA CURVE SELECTED
	ili9341_write_command(0x26);
	ili9341_write_data(0x01);

	//POSITIVE GAMMA CORRECTION
	ili9341_write_command(0xE0);
	ili9341_write_data(0x0F);
	ili9341_write_data(0x31);
	ili9341_write_data(0x2B);
	ili9341_write_data(0x0C);
	ili9341_write_data(0x0E);
	ili9341_write_data(0x08);
	ili9341_write_data(0x4E);
	ili9341_write_data(0xF1);
	ili9341_write_data(0x37);
	ili9341_write_data(0x07);
	ili9341_write_data(0x10);
	ili9341_write_data(0x03);
	ili9341_write_data(0x0E);
	ili9341_write_data(0x09);
	ili9341_write_data(0x00);

	//NEGATIVE GAMMA CORRECTION
	ili9341_write_command(0xE1);
	ili9341_write_data(0x00);
	ili9341_write_data(0x0E);
	ili9341_write_data(0x14);
	ili9341_write_data(0x03);
	ili9341_write_data(0x11);
	ili9341_write_data(0x07);
	ili9341_write_data(0x31);
	ili9341_write_data(0xC1);
	ili9341_write_data(0x48);
	ili9341_write_data(0x08);
	ili9341_write_data(0x0F);
	ili9341_write_data(0x0C);
	ili9341_write_data(0x31);
	ili9341_write_data(0x36);
	ili9341_write_data(0x0F);

	//EXIT SLEEP
	ili9341_write_command(0x11);
	HAL_Delay(120);

	//TURN ON DISPLAY
	ili9341_write_command(0x29);

	//STARTING ROTATION
	ili9341_set_rotation(SCREEN_VERTICAL_1);

	//FILL WHITE BACKGROUND COLOR
	ili9341_fill_screen(WHITE);
}


//INTERNAL FUNCTION OF LIBRARY
/*Sends block colour information to LCD*/
void ili9341_draw_colour_burst(uint16_t colour, uint32_t size)
{
	//SENDS COLOUR
	uint32_t buffer_size = 0;
	if ((size * 2) < BURST_MAX_SIZE)
	{
		buffer_size = size;
	}
	else
	{
		buffer_size = BURST_MAX_SIZE;
	}

	HAL_GPIO_WritePin(LCD_DC_PORT, LCD_DC_PIN, GPIO_PIN_SET);

	uint8_t chifted = colour >> 8;
	uint8_t burst_buffer[buffer_size];
	for (uint32_t j = 0; j < buffer_size; j += 2)
	{
		burst_buffer[j] = chifted;
		burst_buffer[j + 1] = colour;
	}

	uint32_t sending_size = size * 2;
	uint32_t sending_in_block = sending_size / buffer_size;
	uint32_t Remainder_from_block = sending_size % buffer_size;

	if (sending_in_block != 0)
	{
		for (uint32_t j = 0; j < (sending_in_block); j++)
		{
			HAL_SPI_Transmit(HSPI_INSTANCE, (uint8_t *)burst_buffer, buffer_size, 10);
		}
	}

	//REMAINDER!
	HAL_SPI_Transmit(HSPI_INSTANCE, (uint8_t *)burst_buffer, Remainder_from_block, 10);
}

//FILL THE ENTIRE SCREEN WITH SELECTED COLOUR (either #define-d ones or custom 16bit)
/*Sets address (entire screen) and Sends height*width ammount of colour information to LCD*/
void ili9341_fill_screen(uint16_t colour)
{
	ili9341_set_address(0, 0, LCD_WIDTH, LCD_HEIGHT);
	ili9341_draw_colour_burst(colour, LCD_WIDTH * LCD_HEIGHT);
}

//DRAW PIXEL AT XY POSITION WITH SELECTED COLOUR
//
//Location is dependant on screen orientation. x0 and y0 locations change with orientations.
//Using pixels to draw big simple structures is not recommended as it is really slow
//Try using either rectangles or lines if possible
//
void ili9341_draw_pixel(uint16_t x, uint16_t y, uint16_t colour)
{
	if ((x >= LCD_WIDTH) || (y >= LCD_HEIGHT))
		return; //OUT OF BOUNDS!

	//ADDRESS
	HAL_GPIO_WritePin(LCD_DC_PORT, LCD_DC_PIN, GPIO_PIN_RESET);
	ili9341_spi_send(0x2A);
	HAL_GPIO_WritePin(LCD_DC_PORT, LCD_DC_PIN, GPIO_PIN_SET);

	//XDATA
	uint8_t Temp_Buffer[4] = {x >> 8, x, (x + 1) >> 8, (x + 1)};
	HAL_SPI_Transmit(HSPI_INSTANCE, Temp_Buffer, 4, 1);

	//ADDRESS
	HAL_GPIO_WritePin(LCD_DC_PORT, LCD_DC_PIN, GPIO_PIN_RESET);
	ili9341_spi_send(0x2B);
	HAL_GPIO_WritePin(LCD_DC_PORT, LCD_DC_PIN, GPIO_PIN_SET);

	//YDATA
	uint8_t Temp_Buffer1[4] = {y >> 8, y, (y + 1) >> 8, (y + 1)};
	HAL_SPI_Transmit(HSPI_INSTANCE, Temp_Buffer1, 4, 1);

	//ADDRESS
	HAL_GPIO_WritePin(LCD_DC_PORT, LCD_DC_PIN, GPIO_PIN_RESET);
	ili9341_spi_send(0x2C);
	HAL_GPIO_WritePin(LCD_DC_PORT, LCD_DC_PIN, GPIO_PIN_SET);

	//COLOUR
	uint8_t Temp_Buffer2[2] = {colour >> 8, colour};
	HAL_SPI_Transmit(HSPI_INSTANCE, Temp_Buffer2, 2, 1);
}

//DRAW RECTANGLE OF SET SIZE AND HEIGTH AT X and Y POSITION WITH CUSTOM COLOUR
//
//Rectangle is hollow. X and Y positions mark the upper left corner of rectangle
//As with all other draw calls x0 and y0 locations dependant on screen orientation
//

void ili9341_draw_rectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t colour)
{
	if ((x >= LCD_WIDTH) || (y >= LCD_HEIGHT))
		return;
	if ((x + width - 1) >= LCD_WIDTH)
	{
		width = LCD_WIDTH - x;
	}
	if ((y + height - 1) >= LCD_HEIGHT)
	{
		height = LCD_HEIGHT - y;
	}
	ili9341_set_address(x, y, x + width - 1, y + height - 1);
	ili9341_draw_colour_burst(colour, height * width);
}

//DRAW LINE FROM X,Y LOCATION to X+width,Y LOCATION
void ili9341_draw_horizontal_line(uint16_t x, uint16_t y, uint16_t width, uint16_t colour)
{
	if ((x >= LCD_WIDTH) || (y >= LCD_HEIGHT))
		return;
	if ((x + width - 1) >= LCD_WIDTH)
	{
		width = LCD_WIDTH - x;
	}
	ili9341_set_address(x, y, x + width - 1, y);
	ili9341_draw_colour_burst(colour, width);
}

//DRAW LINE FROM X,Y LOCATION to X,Y+height LOCATION
void ili9341_draw_vertical_line(uint16_t x, uint16_t y, uint16_t height, uint16_t colour)
{
	if ((x >= LCD_WIDTH) || (y >= LCD_HEIGHT))
		return;
	if ((y + height - 1) >= LCD_HEIGHT)
	{
		height = LCD_HEIGHT - y;
	}
	ili9341_set_address(x, y, x, y + height - 1);
	ili9341_draw_colour_burst(colour, height);
}

/*Draw hollow circle at X,Y location with specified radius and colour. X and Y represent circles center */
void ili9341_draw_hollow_circle(uint16_t centre_x, uint16_t centre_y, uint16_t radius, uint16_t colour)
{
	int x = radius - 1;
	int y = 0;
	int dx = 1;
	int dy = 1;
	int err = dx - (radius << 1);

	while (x >= y)
	{
		ili9341_draw_pixel(centre_x + x, centre_y + y, colour);
		ili9341_draw_pixel(centre_x + y, centre_y + x, colour);
		ili9341_draw_pixel(centre_x - y, centre_y + x, colour);
		ili9341_draw_pixel(centre_x - x, centre_y + y, colour);
		ili9341_draw_pixel(centre_x - x, centre_y - y, colour);
		ili9341_draw_pixel(centre_x - y, centre_y - x, colour);
		ili9341_draw_pixel(centre_x + y, centre_y - x, colour);
		ili9341_draw_pixel(centre_x + x, centre_y - y, colour);

		if (err <= 0)
		{
			y++;
			err += dy;
			dy += 2;
		}
		if (err > 0)
		{
			x--;
			dx += 2;
			err += (-radius << 1) + dx;
		}
	}
}

/*Draw filled circle at X,Y location with specified radius and colour. X and Y represent circles center */
void ili9341_draw_filled_circle(uint16_t centre_x, uint16_t centre_y, uint16_t radius, uint16_t colour)
{

	int x = radius;
	int y = 0;
	int xChange = 1 - (radius << 1);
	int yChange = 0;
	int radiusError = 0;

	while (x >= y)
	{
		for (int i = centre_x - x; i <= centre_x + x; i++)
		{
			ili9341_draw_pixel(i, centre_y + y, colour);
			ili9341_draw_pixel(i, centre_y - y, colour);
		}
		for (int i = centre_x - y; i <= centre_x + y; i++)
		{
			ili9341_draw_pixel(i, centre_y + x, colour);
			ili9341_draw_pixel(i, centre_y - x, colour);
		}

		y++;
		radiusError += yChange;
		yChange += 2;
		if (((radiusError << 1) + xChange) > 0)
		{
			x--;
			radiusError += xChange;
			xChange += 2;
		}
	}
	//Really slow implementation, will require future overhaul
	//TODO:	https://stackoverflow.com/questions/1201200/fast-algorithm-for-drawing-filled-circles
}

/*Draw a hollow rectangle between positions x0,y0 and x1,y1 with specified colour*/
void ili9341_draw_hollow_cectangle_coord(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t colour)
{
	uint16_t x_length = 0;
	uint16_t y_length = 0;
	uint8_t Negative_X = 0;
	uint8_t Negative_Y = 0;
	float Calc_Negative = 0;

	Calc_Negative = x1 - x0;
	if (Calc_Negative < 0)
		Negative_X = 1;
	Calc_Negative = 0;

	Calc_Negative = y1 - y0;
	if (Calc_Negative < 0)
		Negative_Y = 1;

	//DRAW HORIZONTAL!
	if (!Negative_X)
	{
		x_length = x1 - x0;
	}
	else
	{
		x_length = x0 - x1;
	}
	ili9341_draw_horizontal_line(x0, y0, x_length, colour);
	ili9341_draw_horizontal_line(x0, y1, x_length, colour);

	//DRAW VERTICAL!
	if (!Negative_Y)
	{
		y_length = y1 - y0;
	}
	else
	{
		y_length = y0 - y1;
	}
	ili9341_draw_vertical_line(x0, y0, y_length, colour);
	ili9341_draw_vertical_line(x1, y0, y_length, colour);

	if ((x_length > 0) || (y_length > 0))
	{
		ili9341_draw_pixel(x1, y1, colour);
	}
}

/*Draw a filled rectangle between positions x0,y0 and x1,y1 with specified colour*/
void ili9341_draw_filled_rectangle_coord(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t colour)
{
	uint16_t x_length = 0;
	uint16_t y_length = 0;
	uint8_t Negative_X = 0;
	uint8_t Negative_Y = 0;
	int32_t Calc_Negative = 0;

	uint16_t x0_true = 0;
	uint16_t y0_true = 0;

	Calc_Negative = x1 - x0;
	if (Calc_Negative < 0)
		Negative_X = 1;
	Calc_Negative = 0;

	Calc_Negative = y1 - y0;
	if (Calc_Negative < 0)
		Negative_Y = 1;

	//DRAW HORIZONTAL!
	if (!Negative_X)
	{
		x_length = x1 - x0;
		x0_true = x0;
	}
	else
	{
		x_length = x0 - x1;
		x0_true = x1;
	}

	//DRAW VERTICAL!
	if (!Negative_Y)
	{
		y_length = y1 - y0;
		y0_true = y0;
	}
	else
	{
		y_length = y0 - y1;
		y0_true = y1;
	}

	ili9341_draw_rectangle(x0_true, y0_true, x_length, y_length, colour);
}

/*Draws a character (fonts imported from fonts.h) at X,Y location with specified font colour, size and Background colour*/
/*See fonts.h implementation of font on what is required for changing to a different font when switching fonts libraries*/
void ili9341_draw_char(char character, uint8_t x, uint16_t y, uint16_t colour, uint16_t size, uint16_t background_colour)
{
	uint8_t function_char;
	uint8_t i, j;

	function_char = character;

	if (function_char < ' ')
	{
		character = 0;
	}
	else
	{
		function_char -= 32;
	}
	char temp[CHAR_WIDTH];
	for (uint8_t k = 0; k < CHAR_WIDTH; k++)
	{
		temp[k] = font[function_char][k];
	}

	// Draw pixels
	ili9341_draw_rectangle(x, y, CHAR_WIDTH * size, CHAR_HEIGHT * size, background_colour);
	for (j = 0; j < CHAR_WIDTH; j++)
	{
		for (i = 0; i < CHAR_HEIGHT; i++)
		{
			if (temp[j] & (1 << i))
			{
				if (size == 1)
				{
					ili9341_draw_pixel(x + j, y + i, colour);
				}
				else
				{
					ili9341_draw_rectangle(x + (j * size), y + (i * size), size, size, colour);
				}
			}
		}
	}
}

/*Draws an array of characters (fonts imported from fonts.h) at X,Y location with specified font colour, size and Background colour*/
/*See fonts.h implementation of font on what is required for changing to a different font when switching fonts libraries*/
void ili9341_draw_text(const char *text, uint8_t x, uint16_t y, uint16_t colour, uint16_t size, uint16_t background_colour)
{
	while (*text)
	{
		ili9341_draw_char(*text++, x, y, colour, size, background_colour);
		x += CHAR_WIDTH * size;
	}
}

/*Draws a full screen picture from flash. Image converted from RGB .jpeg/other to C array using online converter*/
//65K colour (2Bytes / pixel)
void ili9341_draw_image(const char *image_array, uint8_t orientation)
{
	if (orientation == SCREEN_HORIZONTAL_1)
	{
		ili9341_set_rotation(SCREEN_HORIZONTAL_1);
		ili9341_set_address(0, 0, ILI9341_SCREEN_WIDTH, ILI9341_SCREEN_HEIGHT);

		HAL_GPIO_WritePin(LCD_DC_GPIO_Port, LCD_DC_PIN, GPIO_PIN_SET);
		HAL_GPIO_WritePin(LCD_BLK_GPIO_Port, LCD_BLK_Pin, GPIO_PIN_RESET);

		uint8_t Temp_small_buffer[BURST_MAX_SIZE];
		uint32_t counter = 0;
		for (uint32_t i = 0; i < ILI9341_SCREEN_WIDTH * ILI9341_SCREEN_HEIGHT * 2 / BURST_MAX_SIZE; i++)
		{
			for (uint32_t k = 0; k < BURST_MAX_SIZE; k++)
			{
				Temp_small_buffer[k] = image_array[counter + k];
			}
			HAL_SPI_Transmit(HSPI_INSTANCE, (uint8_t *)Temp_small_buffer, BURST_MAX_SIZE, 10);
			counter += BURST_MAX_SIZE;
		}
		HAL_GPIO_WritePin(LCD_BLK_GPIO_Port, LCD_BLK_Pin, GPIO_PIN_SET);
	}
	else if (orientation == SCREEN_HORIZONTAL_2)
	{
		ili9341_set_rotation(SCREEN_HORIZONTAL_2);
		ili9341_set_address(0, 0, ILI9341_SCREEN_WIDTH, ILI9341_SCREEN_HEIGHT);

		HAL_GPIO_WritePin(LCD_DC_GPIO_Port, LCD_DC_PIN, GPIO_PIN_SET);
		HAL_GPIO_WritePin(LCD_BLK_GPIO_Port, LCD_BLK_Pin, GPIO_PIN_RESET);

		uint8_t Temp_small_buffer[BURST_MAX_SIZE];
		uint32_t counter = 0;
		for (uint32_t i = 0; i < ILI9341_SCREEN_WIDTH * ILI9341_SCREEN_HEIGHT * 2 / BURST_MAX_SIZE; i++)
		{
			for (uint32_t k = 0; k < BURST_MAX_SIZE; k++)
			{
				Temp_small_buffer[k] = image_array[counter + k];
			}
			HAL_SPI_Transmit(HSPI_INSTANCE, (uint8_t *)Temp_small_buffer, BURST_MAX_SIZE, 10);
			counter += BURST_MAX_SIZE;
		}
		HAL_GPIO_WritePin(LCD_BLK_GPIO_Port, LCD_BLK_Pin, GPIO_PIN_SET);
	}
	else if (orientation == SCREEN_VERTICAL_2)
	{
		ili9341_set_rotation(SCREEN_VERTICAL_2);
		ili9341_set_address(0, 0, ILI9341_SCREEN_HEIGHT, ILI9341_SCREEN_WIDTH);

		HAL_GPIO_WritePin(LCD_DC_GPIO_Port, LCD_DC_PIN, GPIO_PIN_SET);
		HAL_GPIO_WritePin(LCD_BLK_GPIO_Port, LCD_BLK_Pin, GPIO_PIN_RESET);

		uint8_t Temp_small_buffer[BURST_MAX_SIZE];
		uint32_t counter = 0;
		for (uint32_t i = 0; i < ILI9341_SCREEN_WIDTH * ILI9341_SCREEN_HEIGHT * 2 / BURST_MAX_SIZE; i++)
		{
			for (uint32_t k = 0; k < BURST_MAX_SIZE; k++)
			{
				Temp_small_buffer[k] = image_array[counter + k];
			}
			HAL_SPI_Transmit(HSPI_INSTANCE, (uint8_t *)Temp_small_buffer, BURST_MAX_SIZE, 10);
			counter += BURST_MAX_SIZE;
		}
		HAL_GPIO_WritePin(LCD_BLK_GPIO_Port, LCD_BLK_Pin, GPIO_PIN_SET);
	}
	else if (orientation == SCREEN_VERTICAL_1)
	{
		ili9341_set_rotation(SCREEN_VERTICAL_1);
		ili9341_set_address(0, 0, ILI9341_SCREEN_HEIGHT, ILI9341_SCREEN_WIDTH);

		HAL_GPIO_WritePin(LCD_DC_GPIO_Port, LCD_DC_PIN, GPIO_PIN_SET);
		HAL_GPIO_WritePin(LCD_BLK_GPIO_Port, LCD_BLK_Pin, GPIO_PIN_RESET);

		uint8_t Temp_small_buffer[BURST_MAX_SIZE];
		uint32_t counter = 0;
		for (uint32_t i = 0; i < ILI9341_SCREEN_WIDTH * ILI9341_SCREEN_HEIGHT * 2 / BURST_MAX_SIZE; i++)
		{
			for (uint32_t k = 0; k < BURST_MAX_SIZE; k++)
			{
				Temp_small_buffer[k] = image_array[counter + k];
			}
			HAL_SPI_Transmit(HSPI_INSTANCE, (uint8_t *)Temp_small_buffer, BURST_MAX_SIZE, 10);
			counter += BURST_MAX_SIZE;
		}
		HAL_GPIO_WritePin(LCD_BLK_GPIO_Port, LCD_BLK_Pin, GPIO_PIN_SET);
	}
}



lcd_type lcd = 
{
	(&ili9341_reset),
	(&ili9341_set_rotation),
	(&ili9341_init),
	(&ili9341_fill_screen),
	(&ili9341_draw_pixel),
	(&ili9341_draw_colour_burst),
	(&ili9341_draw_rectangle),
	(&ili9341_draw_horizontal_line),
	(&ili9341_draw_vertical_line),
	(&ili9341_draw_hollow_circle),
	(&ili9341_draw_filled_circle),
	(&ili9341_draw_hollow_cectangle_coord),
	(&ili9341_draw_filled_rectangle_coord),
	(&ili9341_draw_char),
	(&ili9341_draw_text),
	(&ili9341_draw_image)
};
