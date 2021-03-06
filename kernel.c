#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#if defined(__linux__)
#error "It is advised that you use a cross-compiler for compiling this file."
#endif

#if !defined(__i386__)
#error "It is advised that you compile this file with a ix86-elf compiler."
#endif

enum vga_colour
{
	VGA_BLACK = 0,
	VGA_BLUE = 1,
	VGA_GREEN = 2,
	VGA_CYAN, // ETC
	VGA_RED,
	VGA_MAGENTA,
	VGA_BROWN,
	VGA_LIGHT_GREY,
	VGA_DARK_GREY,
	VGA_LIGHT_BLUE,
	VGA_LIGHT_GREEN,
	VGA_LIGHT_CYAN,
	VGA_LIGHT_RED,
	VGA_LIGHT_MAGENTA,
	VGA_LIGHT_BROWN,
	VGA_LIGHT_WHITE,
};

static inline uint8_t vga_entry_colour (enum vga_colour fg, enum vga_colour bg)
{
	return fg | bg << 4;
}

static inline uint16_t vga_entry (unsigned char uc, uint8_t color)
{
	return (uint16_t) uc | (uint16_t) color << 8;
}

size_t strlen(const char* str)
{
	size_t len = 0;
	while(str[len])
	    len++;
	return len;
}

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;

size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color;

volatile uint16_t* terminal_buffer;

void terminal_initialize(void)
{
	terminal_row = 0,
	terminal_column = 0;
	terminal_color = vga_entry_colour(VGA_LIGHT_GREY, VGA_BLACK);
	terminal_buffer = (uint16_t*) 0xb8000;
	for (size_t y = 0; y < VGA_HEIGHT; y++)
	{
		for (size_t x = 0; x < VGA_WIDTH; x++)
		{
			const size_t index = y * VGA_WIDTH + x;
			terminal_buffer[index] = vga_entry(' ', terminal_color);

		}
	}
}

void terminal_setcolor(uint8_t color)
{
	terminal_color = color;
}

void terminal_putentryat(char c, uint8_t color, size_t x, size_t y)
{
	const size_t index = y * VGA_WIDTH + x;
	if (c == '\n')
	{
		//const new_index = (y+1) * VGA_WIDTH;
		terminal_column = VGA_WIDTH - 1;
		//terminal_row++;
		//terminal_buffer[new_index] = vga_entry(' ', color);
	}
	else
		terminal_buffer[index] = vga_entry(c, color);
}

void terminal_putchar(char c)
{
	terminal_putentryat(c, terminal_color, terminal_column, terminal_row);
	if (++terminal_column == VGA_WIDTH)
	{
		terminal_column = 0;
		if (++terminal_row == VGA_HEIGHT)
			terminal_row = 0;
	}
}

void terminal_write(const char* data, size_t size)
{
	for(size_t i = 0; i < size; i++)
		terminal_putchar(data[i]);
}

void terminal_writestring(const char* data)
{
	terminal_write(data, strlen(data));
}

void kernel_main(void)
{
	terminal_initialize();
	terminal_writestring("Hello, kernel_world!\n");
	terminal_writestring("New line works.\n");
}
