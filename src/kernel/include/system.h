#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#ifndef __SYSTEM_H
#define __SYSTEM_H
#define YCO 34
extern void terminal_putchar(char c);
extern void terminal_writestring(const char* data);
extern void memcpy (void *dest, const void *src, size_t size);
extern void memset (void *dest,const unsigned char src, size_t size);
extern void memsetw(void *dest,const unsigned short src, size_t size);
extern void terminal_initialize(void);
extern void outportb(unsigned short _port, unsigned char _data);
extern unsigned char inportb (unsigned short _port);
extern size_t strlen(const char* str);
extern void fill_terminal(const char* string);
extern void gdt_install(void);
extern void idt_install(void);
#endif