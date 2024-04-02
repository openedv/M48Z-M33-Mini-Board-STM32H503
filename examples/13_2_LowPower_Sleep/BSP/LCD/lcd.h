/**
 ****************************************************************************************************
 * @file        lcd.h
 * @author      ALIENTEK
 * @brief       LCD code
 * @license     Copyright (C) 2012-2024, ALIENTEK
 ****************************************************************************************************
 * @attention
 *
 * platform     : ALIENTEK M48-STM32H503 board
 * website      : www.alientek.com
 * forum        : www.openedv.com/forum.php
 *
 * change logs  ：
 * version      data         notes
 * V1.0         20240309     the first version
 *
 ****************************************************************************************************
 */

#ifndef BSP_LCD_LCD_H_
#define BSP_LCD_LCD_H_

#include "stdint.h"
#include "stm32h503xx.h"
#include "stm32h5xx_hal.h"

#define LCD_PWR(x)      do{ x ? \
                           (HAL_GPIO_WritePin(LCD_PWR_GPIO_Port, LCD_PWR_Pin, GPIO_PIN_SET)):       \
                           (HAL_GPIO_WritePin(LCD_PWR_GPIO_Port, LCD_PWR_Pin, GPIO_PIN_RESET));     \
                         }while(0)
#define LCD_RST(x)       do{ x ? \
                           (HAL_GPIO_WritePin(LCD_RST_GPIO_Port, LCD_RST_Pin, GPIO_PIN_SET)):       \
                           (HAL_GPIO_WritePin(LCD_RST_GPIO_Port, LCD_RST_Pin, GPIO_PIN_RESET));     \
                         }while(0)
#define LCD_WR(x)        do{ x ? \
                           (HAL_GPIO_WritePin(LCD_WR_GPIO_Port, LCD_WR_Pin, GPIO_PIN_SET)):         \
                           (HAL_GPIO_WritePin(LCD_WR_GPIO_Port, LCD_WR_Pin, GPIO_PIN_RESET));       \
                         }while(0)
#define LCD_CS(x)        do{ x ? \
                           (HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_SET)):         \
                           (HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_RESET));       \
                         }while(0)

/* Definition of width and height of LCD */
#define LCD_WIDTH           240
#define LCD_HEIGHT          240

/* Stroke color */
#define WHITE               0xFFFF
#define BLACK               0x0000
#define BLUE                0x001F
#define BRED                0XF81F
#define GRED                0XFFE0
#define GBLUE               0X07FF
#define RED                 0xF800
#define MAGENTA             0xF81F
#define GREEN               0x07E0
#define CYAN                0x7FFF
#define YELLOW              0xFFE0
#define BROWN               0XBC40      /* Brown */
#define BRRED               0XFC07      /* Brown red */
#define GRAY                0X8430      /* Gray */
#define DARKBLUE            0X01CF      /* Dark blue */
#define LIGHTBLUE           0X7D7C      /* Light blue */
#define GRAYBLUE            0X5458      /* Grey blue */
/* The above three colors are the PANEL colors  */
#define LIGHTGREEN          0X841F      /* Light green */
#define LGRAY               0XC618      /* Light grey(PANNEL),form background color */

#define LGRAYBLUE           0XA651      /* Light gray blue (middle layer color) */
#define LBBLUE              0X2B12      /* Light brown blue (the reverse color of selected items) */


void lcd_init(void);
void lcd_show_image(uint16_t x, uint16_t y, uint16_t width, uint16_t height, const uint8_t *p);
void lcd_show_string(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t size, char *p, uint16_t color);
void lcd_show_xnum(uint16_t x, uint16_t y, uint32_t num, uint8_t len, uint8_t size, uint8_t mode, uint16_t color);
void lcd_show_num(uint16_t x, uint16_t y, uint32_t num, uint8_t len, uint8_t size, uint16_t color);
void lcd_show_char(uint16_t x, uint16_t y, char chr, uint8_t size, uint8_t mode ,uint16_t color);
void lcd_draw_circle(uint16_t x0, uint16_t y0, uint8_t r,uint16_t color);
void lcd_draw_rectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,uint16_t color);
void lcd_draw_line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,uint16_t color);
void lcd_draw_color_point(uint16_t x, uint16_t y,uint16_t color);
void lcd_draw_point(uint16_t x, uint16_t y,uint16_t color);
void lcd_fill(uint16_t x_start, uint16_t y_start, uint16_t x_end, uint16_t y_end, uint16_t color);
void lcd_clear(uint16_t color);
void lcd_display_off(void);
void lcd_display_on(void);
void lcd_address_set(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
void lcd_write_halfword(const uint16_t da);


#endif /* BSP_LCD_LCD_H_ */
