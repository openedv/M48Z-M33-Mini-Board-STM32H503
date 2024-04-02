/**
 ****************************************************************************************************
 * @file        lcd.c
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

#include "lcd.h"
#include "stdint.h"
#include "lcdfont.h"
#include "spi.h"


/* LCD cache size setting, please note ！！！！
 * when changing this value Changing these values may affect
 * the following lcd_clear/lcd_fill/lcd_draw_line functions
 */
#define LCD_TOTAL_BUF_SIZE      (240 * 240 *2)
#define LCD_BUF_SIZE            1152
static uint8_t lcd_buf[LCD_BUF_SIZE];

uint16_t g_point_color = BLACK;     /* The brush color defaults to black */
uint16_t g_back_color = WHITE;      /* The background color defaults to white */

/**
 * @brief   LCD control interface initialization
 * @param   None.
 * @retval  None.
 */
static void lcd_gpio_init(void)
{
	LCD_CS(0);
	LCD_PWR(0);

    LCD_RST(0);
    HAL_Delay(120);                 /* Delay 120ms */
    LCD_RST(1);

    MX_SPI1_Init();                 /* Initialize the SPI1 interface */
}

/**
 * @brief   LCD underlying SPI data sending function
 * @param   data : The starting address of the data
 * @param   size : Send data size
 * @retval  None.
 */
static void lcd_spi_send(uint8_t *data, uint32_t size)
{
    uint32_t i;
    uint32_t delta;
    LCD_CS(0);                          /* Enable slice selection */
    delta = size / 0xFFFF;

    for (i = 0; i <= delta; i ++)
    {
        if ( i == delta )               /* The last frame is sent */
        {
            spi1_write_data(&data[i * 0xFFFF], size%0xFFFF);
        }
        else                            /* Very-long data sends 0xFFFF bytes of data at a time */
        {
            spi1_write_data(&data[i * 0xFFFF], 0xFFFF);
        }
    }
    LCD_CS(1);                          /* Disable slice selection */
}

/**
 * @brief   Writes commands to LCD
 * @param   cmd : Commands to send
 * @retval  None.
 */
static void lcd_write_cmd(uint8_t cmd)
{
    LCD_WR(0);

    lcd_spi_send(&cmd, 1);
}

/**
 * @brief   Writes data to LCD
 * @param   cmd : data to send
 * @retval  None.
 */
static void lcd_write_data(uint8_t data)
{
    LCD_WR(1);

    lcd_spi_send(&data, 1);
}

/**
 * @brief   Writes half a word of data to the LCD
 * @param   cmd : data to send
 * @retval  None.
 */
void lcd_write_halfword(const uint16_t da)
{
    uint8_t data[2] = {0};

    data[0] = da >> 8;
    data[1] = da;

    LCD_WR(1);
    lcd_spi_send(data, 2);
}

/**
 * @brief   Sets data to be written to the LCD cache area
 * @param   x1,y1 : starting coordinate
 * @param   x2,y2 : end coordinates
 * @retval  None.
 */
void lcd_address_set(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
    lcd_write_cmd(0x2a);
    lcd_write_data(x1 >> 8);
    lcd_write_data(x1);
    lcd_write_data(x2 >> 8);
    lcd_write_data(x2);

    lcd_write_cmd(0x2b);
    lcd_write_data(y1 >> 8);
    lcd_write_data(y1);
    lcd_write_data(y2 >> 8);
    lcd_write_data(y2);

    lcd_write_cmd(0x2C);
}

/**
 * @brief   Turn on the LCD display
 * @param   None.
 * @retval  None.
 */
void lcd_display_on(void)
{
    LCD_PWR(1);
}

/**
 * @brief   Turn off the LCD display
 * @param   None.
 * @retval  None.
 */
void lcd_display_off(void)
{
    LCD_PWR(0);
}

/**
 * @brief   Clears the LCD screen in one color
 * @param   Color : Clear screen color
 * @retval  None.
 */
void lcd_clear(uint16_t color)
{
    uint16_t i, j;
    uint8_t data[2] = {0};

    data[0] = color >> 8;
    data[1] = color;

    lcd_address_set(0, 0, LCD_WIDTH - 1, LCD_HEIGHT - 1);

    for (j = 0; j < LCD_BUF_SIZE / 2; j++)
    {
        lcd_buf[j * 2] =  data[0];
        lcd_buf[j * 2 + 1] =  data[1];
    }

    LCD_WR(1);

    for (i = 0; i < (LCD_TOTAL_BUF_SIZE / LCD_BUF_SIZE); i++)
    {
        lcd_spi_send(lcd_buf, LCD_BUF_SIZE);
    }
}

/**
 * @brief   Clears the LCD screen in one color
 * @param   x_start,y_start : coordinate of the starting point
 * @param   x_end,y_end     : coordinate of the endpoint
 * @param   color           : Fill color
 * @retval  None.
 */
void lcd_fill(uint16_t x_start, uint16_t y_start, uint16_t x_end, uint16_t y_end, uint16_t color)
{
    uint16_t i = 0;
    uint32_t size = 0, size_remain = 0;

    size = (x_end - x_start + 1) * (y_end - y_start + 1) * 2;

    if (size > LCD_BUF_SIZE)
    {
        size_remain = size - LCD_BUF_SIZE;
        size = LCD_BUF_SIZE;
    }

    lcd_address_set(x_start, y_start, x_end, y_end);

    while(1)
    {
        for(i = 0; i < size / 2; i++)
        {
            lcd_buf[2 * i] = color >> 8;
            lcd_buf[2 * i + 1] = color;
        }

        LCD_WR(1);
        lcd_spi_send(lcd_buf, size);

        if(size_remain == 0)
            break;

        if (size_remain > LCD_BUF_SIZE)
        {
            size_remain = size_remain - LCD_BUF_SIZE;
        }

        else
        {
            size = size_remain;
            size_remain = 0;
        }
    }
}

/**
 * @brief   Draw dot function
 * @param   x,y : Draws point coordinates
 * @retval  None.
 */
void lcd_draw_point(uint16_t x, uint16_t y, uint16_t color)
{
    lcd_address_set(x, y, x, y);
    lcd_write_halfword(color);
}

/**
 * @brief   Draw dot color function
 * @param   x,y : Draws point coordinates
 * @retval  None.
 */
void lcd_draw_color_point(uint16_t x, uint16_t y,uint16_t color)
{
    lcd_address_set(x, y, x, y);
    lcd_write_halfword(color);
}

/**
 * @brief   Line drawing function(straight,slash)
 * @param   x1,y1 : Starting point coordinates
 * @param   x2,y2 : Ending point coordinates
 * @retval  None.
 */
void lcd_draw_line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color)
{
    uint16_t t;
    int xerr = 0, yerr = 0, delta_x, delta_y, distance;
    int incx, incy, row, col;
    uint32_t i = 0;

    if (y1 == y2)
    {
        /* Draw horizontal lines quickly */
        lcd_address_set(x1, y1, x2, y2);

        for (i = 0; i < x2 - x1; i++)
        {
            lcd_buf[2 * i] = g_point_color >> 8;
            lcd_buf[2 * i + 1] = g_point_color;
        }

        LCD_WR(1);
        lcd_spi_send(lcd_buf, (x2 - x1) * 2);
        return;
    }

    delta_x = x2 - x1;
    delta_y = y2 - y1;
    row = x1;
    col = y1;

    if (delta_x > 0)
    {
        incx = 1;
    }
    else if (delta_x == 0)
    {
        incx = 0;
    }
    else
    {
        incx = -1;
        delta_x = -delta_x;
    }

    if (delta_y > 0)
    {
        incy = 1;
    }
    else if (delta_y == 0)
    {
        incy = 0;
    }
    else
    {
        incy = -1;
        delta_y = -delta_y;
    }

    if (delta_x > delta_y)
    {
        distance = delta_x;
    }
    else
    {
        distance = delta_y;
    }

    for (t = 0; t <= distance + 1; t++)
    {
        lcd_draw_point(row, col,color);
        xerr += delta_x ;
        yerr += delta_y ;

        if (xerr > distance)
        {
            xerr -= distance;
            row += incx;
        }

        if (yerr > distance)
        {
            yerr -= distance;
            col += incy;
        }
    }
}

/**
 * @brief   Draw a rectangle
 * @param   x1,y1 : Starting point coordinates
 * @param   x2,y2 : Ending point coordinates
 * @retval  None.
 */
void lcd_draw_rectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,uint16_t color)
{
    lcd_draw_line(x1, y1, x2, y1,color);
    lcd_draw_line(x1, y1, x1, y2,color);
    lcd_draw_line(x1, y2, x2, y2,color);
    lcd_draw_line(x2, y1, x2, y2,color);
}

/**
 * @brief   Draw a circle
 * @param   x0,y0 : Coordinate of center of circle
 * @param   r     : Circle radius
 * @retval  None.
 */
void lcd_draw_circle(uint16_t x0, uint16_t y0, uint8_t r,uint16_t color)
{
    int a, b;
    int di;
    a = 0;
    b = r;
    di = 3 - (r << 1);

    while (a <= b)
    {
        lcd_draw_point(x0 - b, y0 - a, color);
        lcd_draw_point(x0 + b, y0 - a, color);
        lcd_draw_point(x0 - a, y0 + b, color);
        lcd_draw_point(x0 - b, y0 - a, color);
        lcd_draw_point(x0 - a, y0 - b, color);
        lcd_draw_point(x0 + b, y0 + a, color);
        lcd_draw_point(x0 + a, y0 - b, color);
        lcd_draw_point(x0 + a, y0 + b, color);
        lcd_draw_point(x0 - b, y0 + a, color);
        a++;

        if (di < 0)
        {
            di += 4 * a + 6;
        }
        else
        {
            di += 10 + 4 * (a - b);
            b--;
        }

        lcd_draw_point(x0 + a, y0 + b, color);
    }
}

/**
 * @brief   Display a character function
 * @param   x,y  : Starting point coordinates
 * @param   chr  : The character to display
 * @param   size : Font size (16/24/32 point font supported)
 * @retval  None.
 */
void lcd_show_char(uint16_t x, uint16_t y, char chr, uint8_t size, uint8_t mode ,uint16_t color)
{
    uint8_t temp = 0, t1 = 0, t = 0;
    uint8_t *pfont = 0;
    uint8_t csize = 0;                            /* Get the number of bytes of a font character corresponding to the dot matrix set */
    uint16_t colortemp = 0;
    uint8_t sta = 0;

    chr = chr - ' ';                              /* Get the offset value (ASCII fonts start modulo Spaces, so - "is the font for the corresponding character) */

    if ((x > (LCD_WIDTH - size / 2)) || (y > (LCD_HEIGHT - size)))
    {
        return;
    }

    lcd_address_set(x, y, x + size / 2 - 1, y + size - 1); /* (x,y,x+8-1,y+16-1) */

    switch (size)
    {
        case 12:
            pfont = (uint8_t *)asc2_1206[(uint8_t)chr];             /* 1206 font */
            break;

        case 16:
            pfont = (uint8_t *)asc2_1608[(uint8_t)chr];             /* 1608 font */
            break;

        case 24:
            pfont = (uint8_t *)asc2_2412[(uint8_t)chr];             /* 2412 font */
            break;

        case 32:
            pfont = (uint8_t *)asc2_3216[(uint8_t)chr];             /* 3216 font */
            break;

        default:
            return ;
    }

    if (size != 24)
    {
        csize = (size / 8 + ((size % 8) ? 1 : 0)) * (size / 2);

        for (t = 0; t < csize; t++)
        {
            temp = pfont[t];                               /* Retrieves the dot matrix data of the character */

            for (t1 = 0; t1 < 8; t1++)
            {
                    if (temp & 0x80)
                    {
                        colortemp = color;
                    }
                    else if (mode == 0)                    /* Invalid point, not shown */
                    {
                        colortemp = g_back_color;
                    }

                    lcd_write_halfword(colortemp);
                    temp <<= 1;
            }
        }
    }
    else
    {
        csize = (size * 16) / 8;

        for (t = 0; t < csize; t++)
        {
            temp = asc2_2412[(uint8_t)chr][t];

            if (t % 2 == 0)
            {
                sta = 8;
            }
            else
            {
                sta = 4;
            }

            for (t1 = 0; t1 < sta; t1++)
            {
                if(temp & 0x80)
                {
                    colortemp = color;
                }
                else if (mode == 0)                                 /* Invalid point, not shown */
                {
                    colortemp = g_back_color;
                }

                lcd_write_halfword(colortemp);
                temp <<= 1;
            }
        }
    }
}

/**
 * @brief   m^n function
 * @param   m,n : Input parameters
 * @retval  m to the n
 */
static uint32_t lcd_pow(uint8_t m, uint8_t n)
{
    uint32_t result = 1;

    while(n--)result *= m;

    return result;
}

/**
 * @brief   Display len numbers
 * @param   x,y : The starting coordinate
 * @param   num : Number (0-2 ^32)
 * @param   len : Displays the number of bits
 * @param   size: Select font 12/16/24/32
 * @retval  None.
 */
void lcd_show_num(uint16_t x, uint16_t y, uint32_t num, uint8_t len, uint8_t size, uint16_t color)
{
    uint8_t t, temp;
    uint8_t enshow = 0;

    for (t = 0; t < len; t++)      /* Cycle by the total number of display bits */
    {
        temp = (num / lcd_pow(10, len - t - 1)) % 10;   /* Gets the number of the corresponding bit */

        if (enshow == 0 && t < (len - 1))   /* Display is not enabled, and there is still bit to display */
        {
            if (temp == 0)
            {
                lcd_show_char(x + (size / 2)*t, y, ' ', size, 0, color);/* Display space, placeholder */
                continue;                   /* Go to the next one */
            }
            else
            {
                enshow = 1;                 /* Enable display */
            }

        }

        lcd_show_char(x + (size / 2)*t, y, temp + '0', size, 0, color); /* Display character */
    }
}

/**
 * @brief   extension displays len numbers (high bits of 0 are also displayed)
 * @param   x,y : The starting coordinate
 * @param   num : Number (0-2 ^32)
 * @param   len : Displays the number of bits
 * @param   size: Select font 12/16/24/32
 * @param   mode: Display mode
 *          [7] : 0, no padding;  1, fill with 0.
 *          [6:1]:Reserved
 *          [0] : 0, non-overlay display; 1, overlay display.
 *
 * @retval  None.
 */
void lcd_show_xnum(uint16_t x, uint16_t y, uint32_t num, uint8_t len, uint8_t size, uint8_t mode, uint16_t color)
{
    uint8_t t, temp;
    uint8_t enshow = 0;

    for (t = 0; t < len; t++)   /* Cycle by the total number of display bits */
    {
        temp = (num / lcd_pow(10, len - t - 1)) % 10;    /* Gets the number of the corresponding bit */

        if (enshow == 0 && t < (len - 1))   /* Display is not enabled, and there is still bit to display */
        {
            if (temp == 0)
            {
                if (mode & 0X80)   /* The high bits need to be filled with zeros */
                {
                    lcd_show_char(x + (size / 2)*t, y, '0', size, mode & 0X01, color);  /* I'll put a 0 in there */
                }
                else
                {
                    lcd_show_char(x + (size / 2)*t, y, ' ', size, mode & 0X01, color);  /* Use Spaces for space */
                }

                continue;
            }
            else
            {
                enshow = 1;        /* Enable display */
            }

        }

        lcd_show_char(x + (size / 2)*t, y, temp + '0', size, mode & 0X01, color);
    }
}

/**
 * @brief   Displaying strings
 * @param   x,y         : The starting coordinate
 * @param   width,height: Region size
 * @param   size        : Choose 12/16/24/32
 * @param   p           : The first address of the string
 * @retval  None.
 */
void lcd_show_string(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t size, char *p, uint16_t color)
{
    uint8_t x0 = x;
    width += x;
    height += y;

    while ((*p <= '~') && (*p >= ' '))   /* Check whether it is an illegal character! */
    {
        if (x >= width)
        {
            x = x0;
            y += size;
        }

        if (y >= height)break;           /* Quit */

        lcd_show_char(x, y, *p, size, 0, color);
        x += size / 2;
        p++;
    }
}

/**
 * @brief   Display picture
 *
 * @remark  Image2Lcd take mode:
 *          C language data/horizontal scanning /
 *          16 bit true color (RGB565)/ high bits
 *          in the first other do not choose
 * @param   x,y   : The starting coordinate
 * @param   width : The image width
 * @param   height: The image height
 * @param   p     : Image cache data start address
 * @retval  None.
 */
void lcd_show_image(uint16_t x, uint16_t y, uint16_t width, uint16_t height, const uint8_t *p)
{
    if (x + width > LCD_WIDTH || y + height > LCD_HEIGHT)
    {
        return;
    }

    lcd_address_set(x, y, x + width - 1, y + height - 1);

    LCD_WR(1);

    lcd_spi_send((uint8_t *)p, width * height * 2);
}

/**
 * @brief   LCD initialization
 * @remark  None.
 * @retval  None.
 */
void lcd_init(void)
{
    lcd_gpio_init();            /* Hardware interface initialization */

    HAL_Delay(120);
    /* Sleep Out */
    lcd_write_cmd(0x11);
    /* wait for power stability */
    HAL_Delay(120);

    /* Memory Data Access Control */
    lcd_write_cmd(0x36);
    lcd_write_data(0x00);

    /* RGB 5-6-5-bit  */
    lcd_write_cmd(0x3A);
    lcd_write_data(0x65);

    /* Porch Setting */
    lcd_write_cmd(0xB2);
    lcd_write_data(0x0C);
    lcd_write_data(0x0C);
    lcd_write_data(0x00);
    lcd_write_data(0x33);
    lcd_write_data(0x33);

    /*  Gate Control */
    lcd_write_cmd(0xB7);
    lcd_write_data(0x72);

    /* VCOM Setting */
    lcd_write_cmd(0xBB);
    lcd_write_data(0x3D);

    /* LCM Control */
    lcd_write_cmd(0xC0);
    lcd_write_data(0x2C);

    /* VDV and VRH Command Enable */
    lcd_write_cmd(0xC2);
    lcd_write_data(0x01);

    /* VRH Set */
    lcd_write_cmd(0xC3);
    lcd_write_data(0x19);

    /* VDV Set */
    lcd_write_cmd(0xC4);
    lcd_write_data(0x20);

    /* Frame Rate Control in Normal Mode */
    lcd_write_cmd(0xC6);
    lcd_write_data(0x0F);

    /* Power Control 1 */
    lcd_write_cmd(0xD0);
    lcd_write_data(0xA4);
    lcd_write_data(0xA1);

    /* Positive Voltage Gamma Control */
    lcd_write_cmd(0xE0);
    lcd_write_data(0xD0);
    lcd_write_data(0x04);
    lcd_write_data(0x0D);
    lcd_write_data(0x11);
    lcd_write_data(0x13);
    lcd_write_data(0x2B);
    lcd_write_data(0x3F);
    lcd_write_data(0x54);
    lcd_write_data(0x4C);
    lcd_write_data(0x18);
    lcd_write_data(0x0D);
    lcd_write_data(0x0B);
    lcd_write_data(0x1F);
    lcd_write_data(0x23);

    /* Negative Voltage Gamma Control */
    lcd_write_cmd(0xE1);
    lcd_write_data(0xD0);
    lcd_write_data(0x04);
    lcd_write_data(0x0C);
    lcd_write_data(0x11);
    lcd_write_data(0x13);
    lcd_write_data(0x2C);
    lcd_write_data(0x3F);
    lcd_write_data(0x44);
    lcd_write_data(0x51);
    lcd_write_data(0x2F);
    lcd_write_data(0x1F);
    lcd_write_data(0x1F);
    lcd_write_data(0x20);
    lcd_write_data(0x23);

    /* Display Inversion On */
    lcd_write_cmd(0x21);

    lcd_write_cmd(0x29);

    lcd_address_set(0, 0, LCD_WIDTH - 1, LCD_HEIGHT - 1);

    lcd_clear(WHITE);

    LCD_PWR(1);                 /* Enable display */
}

