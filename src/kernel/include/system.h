#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#ifndef __SYSTEM_H
#define __SYSTEM_H

// This defines what the stack looks like after an ISR was running
struct regs
{
    unsigned int gs, fs, es, ds;                            // Pushed the segs last
    unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax;    // Pushed by 'pusha'
    unsigned int int_no, err_code;                          // Our 'push byte #' and ecodes do this
    unsigned int eip, cs, eflags, useresp, ss;               // pushed by the processor automatically
};

extern void terminal_initialize(void);
extern void terminal_putchar(char c);
extern void terminal_writestring(const char* data);
extern void terminal_setcolor(uint8_t color);
extern uint8_t terminal_getcurrcolor(void);
extern void fill_terminal(const char* string);

extern void memcpy (void *dest, const void *src, size_t size);
extern void memset (void *dest,const unsigned char src, size_t size);
extern void memsetw(void *dest,const unsigned short src, size_t size);
extern void outportb(unsigned short _port, unsigned char _data);
extern unsigned char inportb (unsigned short _port);

extern size_t strlen(const char* str);

extern void gdt_install(void);

extern void isrs_install(void);
extern void idt_install(void);
extern void idt_set_gate(unsigned char num, uint64_t base, uint16_t sel, unsigned char flags);

extern void irq_install(void);
extern void irq_install_handler(int irq, void (*handler)(struct regs *r));
extern void irq_uninstall_handler(int irq);

extern void timer_install(void);
extern void sleep(int ticks);
extern void timer_handler(struct regs *r);

extern void keyboard_install(void);
#endif