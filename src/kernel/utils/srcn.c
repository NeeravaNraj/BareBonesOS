#include <system.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// 16 Vga colors
enum vga_color {
    VGA_COLOR_BLACK = 0,
	VGA_COLOR_BLUE = 1,
	VGA_COLOR_GREEN = 2,
	VGA_COLOR_CYAN = 3,
	VGA_COLOR_RED = 4,
	VGA_COLOR_MAGENTA = 5,
	VGA_COLOR_BROWN = 6,
	VGA_COLOR_LIGHT_GREY = 7,
	VGA_COLOR_DARK_GREY = 8,
	VGA_COLOR_LIGHT_BLUE = 9,
	VGA_COLOR_LIGHT_GREEN = 10,
	VGA_COLOR_LIGHT_CYAN = 11,
	VGA_COLOR_LIGHT_RED = 12,
	VGA_COLOR_LIGHT_MAGENTA = 13,
	VGA_COLOR_LIGHT_BROWN = 14,
	VGA_COLOR_WHITE = 15,
};

// sets foreground and background colors
static inline uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg)
{
    return fg | bg << 4;
}

// enter a character with specified color
static inline uint16_t vga_entry(unsigned char uc, uint8_t color)
{
    return (uint16_t) uc | (uint16_t) color << 8;
}


static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;

size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color;
uint16_t* terminal_buffer;

// setting up environment
void terminal_initialize(void)
{
    terminal_row = 0;
    terminal_column = 0;
    terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GREEN, VGA_COLOR_BLACK);
    terminal_buffer = (uint16_t*) 0xB8000;
    for (size_t y = 0; y < VGA_HEIGHT; y++){
        for (size_t x = 0; x < VGA_WIDTH; x++){
            const size_t index = y * VGA_WIDTH + x;
            terminal_buffer[index] = vga_entry(' ', terminal_color);
        }
    }
}

void terminal_setcolor(uint8_t color)
{
    terminal_color = color;
}

uint8_t terminal_getcurrcolor(void)
{
    return terminal_color;
}

// scrolls the terminal when full
void scroll(void)
{
    unsigned short blank, temp;
    blank = vga_entry(' ', terminal_color);

    if (terminal_row >= VGA_HEIGHT){
        temp = terminal_row - 25 + 1;
        memcpy(terminal_buffer, terminal_buffer + temp *  VGA_WIDTH, (VGA_HEIGHT - temp) * VGA_WIDTH * 2);
        memsetw(terminal_buffer + (VGA_HEIGHT - temp) * VGA_WIDTH, blank, VGA_WIDTH);
        terminal_row = VGA_HEIGHT - 1;
    }

}

void fill_terminal(const char* string){
	int i;

	terminal_setcolor(VGA_COLOR_RED);
	for(i = 0; i < 2; i++){
		terminal_writestring(string);
	}
	terminal_setcolor(VGA_COLOR_WHITE);
	for(i = 0; i < 6; i++){
		terminal_writestring(string);
	}
	terminal_setcolor(VGA_COLOR_BLUE);
	for(i = 0; i < 17;i++){
		terminal_writestring(string);
	}
	terminal_setcolor(VGA_COLOR_MAGENTA);
	for(i = 0; i < 5; i++){
		terminal_writestring(string);
	}
}

// puts a character on screen at a specified location
void terminal_putentryat(char c, uint8_t color, size_t x, size_t y)
{
    const size_t index = y * VGA_WIDTH + x;
    terminal_buffer[index] = vga_entry(c, color);
}


void terminal_putchar(char c)
{
    terminal_putentryat(c, terminal_color, terminal_column, terminal_row);
    if (c == '\n'){
        terminal_column = 0;
        terminal_row++;
    }
    else if (c == 0x08){
        if (terminal_column != 0) terminal_column--;
    }
    else if (c == 0x09){
        terminal_column = (terminal_column + 8) & ~(8 - 1);
    }
    else if (++terminal_column == VGA_WIDTH){
        terminal_column = 0;
        terminal_row++;
    }
    scroll();
}

void terminal_write(const char* data)
{
    while (*data)
        terminal_putchar(*data++);
}

void terminal_writestring(const char* data)
{
    terminal_write(data);
}