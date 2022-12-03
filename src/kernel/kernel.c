#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <system.h>

// Check if the compiler thinks you are targeting the wrong operating system
#if defined(__linux__)
#error "You are not using a cross-compiler, you will most certainly run into trouble"
#endif

// This tutorial will only work for the 32-bit ix86 targets.
#if !defined(__i386__)
#error "This tutorial needs to be compiled with a ix86-elf compiler"
#endif


void kernel_main(void)
{
    gdt_install();
    idt_install();
    isrs_install();
    irq_install();
    timer_install();
    keyboard_install();
    __asm__ __volatile__ ("sti"); 
    terminal_initialize();
    // int color = 1;
    // while (true)
    // {
    //     if (color == 15)
    //         color = 1;
    //     terminal_writestring("Hello, I am kernel :D\n");
    //     terminal_setcolor(color);
    //     color++;
    //     sleep(10);
    // }
}