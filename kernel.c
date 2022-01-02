

#include<stdbool.h>
#include<stddef.h>
#include<stdint.h>

#if defined(__linux__)
#error "no good comes from that"
#endif

#if !defined(__i386__)
#error "wrong compiler used"
#endif

enum VGA_COLOR {
	VGA_BLACK,
	VGA_BLUE,
	VGA_GREEN,
	VGA_CYAN,
	VGA_RED,
	VGA_PURPLE,
	VGA_BROWN,
	VGA_GRAY,
	VGA_DARK_GRAY,
	VGA_LIGHT_BLUE,
	VGA_LIGHT_GREEN,
	VGA_LIGHT_CYAN,
	VGA_LIGHT_RED,
	VGA_LIGHT_PURPLE,
	VGA_YELLOW,
	VGA_WHITE
};

//VGA text mode data format
static inline uint8_t vga_entry_color(enum VGA_COLOR fg, enum VGA_COLOR bg) {
	return fg | bg << 4;
}

static inline uint16_t vga_entry(unsigned char uc, uint8_t color) {
	return (uint16_t)uc | (uint16_t)color << 8;
}

size_t strlen(const char* str) {
	size_t len = 0;
	while (str[len]) 
		len++;
		return len;
}

//screen initialization
static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;

size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color;
uint16_t* terminal_buffer;

void terminal_initialize(void) {
	terminal_row = 0;
	terminal_column = 0;
	terminal_color = vga_entry_color(VGA_BROWN, VGA_DARK_GRAY);
	terminal_buffer = (uint16_t*)0xB8000;  //VGA text mode base addr
	for (size_t y = 0; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			const size_t index = y * VGA_WIDTH + x;
			terminal_buffer[index] = vga_entry(' ', terminal_color);
		}
	}
}

void terminal_setcolor(uint8_t color) {
	terminal_color = color;
}

void terminal_putentryat(char c, uint8_t color, size_t x, size_t y) {
	const size_t index = y * VGA_WIDTH + x;
	switch (y) {
	case(VGA_HEIGHT):
		scrolling();  //follow input in case it overflows screen defined height
		terminal_row--;
	default:
		terminal_buffer[index] = vga_entry(c, color);
			}
		}

		void terminal_putchar(char c) {
			terminal_putentryat(c, terminal_color, terminal_column, terminal_row);
			if (++terminal_column == VGA_WIDTH) {
				terminal_column = 0;
				if (++terminal_row == VGA_HEIGHT)
					terminal_row = 0;
			}
		}

		void terminal_write(const char* data, size_t size) {
			for (size_t i = 0; i < size; i++) {
				switch (data[i]) {
				case '\n':  //newline implementation
					terminal_row++;
					terminal_column = 0;

				default: terminal_putchar(data[i]);
				}
			}
		}
		
		void scrolling(void) { //follow input in case of height overflow
			for (size_t i = 0; i < VGA_HEIGHT; i++) {
				for (size_t j = 0; j < VGA_WIDTH; j++) {
					terminal_buffer[i * VGA_WIDTH + j] = terminal_buffer[(i + 1) * VGA_WIDTH + j];
				}
			}
		}

		void terminal_writestring(const char* data) {
			terminal_write(data, strlen(data));
		}

		void terminal_art(const char* data,size_t size) { //utilize all color in input
			uint8_t j = 1;
			for (size_t i = 0; i < size; i++) {
				switch (data[i]) {
				case '\n':
					terminal_row++;
					terminal_column = 0;

				default:
					terminal_putchar(data[i]);
					terminal_setcolor(j | ((j - 1) << 4));
					j++;
					if (j == 15) j = 1;
				}
			}
		}

		
		void terminal_doart(const char* data) {
			terminal_art(data, strlen(data));
		}

		void kernel_main(void) {
			terminal_initialize();

			terminal_doart("A\nB\nC\n********\n");
			//terminal_writestring("a\na\na\na\na\na\na\na\na\na\na\na\na\na\na\na\na\na\na\na\na\na\na\na\na\na\na\na\na\n\ndfgdfhfgjfghk");
		}
	
