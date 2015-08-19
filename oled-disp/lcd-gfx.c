#include "ssd1306.h"
#include "lcd-gfx.h"
#include "fonts/linux/font.h"

extern const struct font_desc *fonts[];

static int cur_x = 0, cur_y = 0, fontn = 0;

void lcd_setfont(int f)
{
    if (f < 0)
	f = 0;
    if (f > 9)
	f = 9;
    fontn = f;
}

int  lcd_getfontwidth(void)
{
    return 0;
}

int  lcd_getfontheight(void)
{
    return 0;
}

void lcd_home(void)
{
    cur_x = cur_y = 0;
}

void lcd_gotoxy(int x, int y)
{
    cur_x = x;
    cur_y = y;
}  

void lcd_putc(int c)
{
    lcd_putchar(c, 1);
}

void lcd_puts(char *s)
{
    while (*s) {
	lcd_putchar(*s++, 0);
    }
    display();
}

void lcd_putchar(int c, int update)
{
    int x, y, height, width;
    unsigned char *p;
    const struct font_desc *font;
  
    font = fonts[fontn];
    width = font->width;
    height = font->height;
    p = (unsigned char *) font->data;

    // Modify for fonts with .width bigger thatn 8 pixels
    // HERE!
    // 
    // 
    if (width <= 8)
	p += (c * height); // font height
    else
	p += (c * height * 2); // font height
    
    // font height
    for (y = 0; y < height; y++) {
	// font width
	for (x = 0; x < width; x++) {
	    if (width <= 8) {
		drawPixel(cur_x + x, cur_y + y,
		      ((1 << (8 - 1 - x)) & *(p + y)) ? WHITE : 0);
	    }
	    else {
		if (x < 8)
		    drawPixel(cur_x + x, cur_y + y,
			      ((1 << (8 - 1 - x)) & *(p + (y * 2))) ? WHITE : 0);
#if 1
		else
		    drawPixel(cur_x + x, cur_y + y,
			      ((1 << (16 - x - 1)) & *(p + (y * 2) + 1)) ? WHITE : 0);
#endif
	    }
	}
    }
    
    // update pixel cursor
    cur_x += width;
    if (cur_x > SSD1306_LCDWIDTH) {
	cur_x = 0;
	cur_y += height;
    }
    if (cur_y > SSD1306_LCDHEIGHT) {
	cur_y = 0;
    }

    if (update)
	display();
}
