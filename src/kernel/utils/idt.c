#include <system.h>
#include <stdint.h>

// Defines an IDT entry
struct idt_entry{
    uint16_t base_lo;
    uint16_t sel;
    unsigned char alwaysZero;
    unsigned char flags;
    unsigned short base_hi;   
} __attribute__((packed));

struct idt_ptr {
    unsigned short limit;
    unsigned int base;
} __attribute__((packed));

/* Declare an IDT of 256 entries. Although we will only use the
*  first 32 entries, the rest exists as a bit
*  of a trap. If any undefined IDT entry is hit, it normally
*  will cause an "Unhandled Interrupt" exception. Any descriptor
*  for which the 'presence' bit is cleared (0) will generate an
*  "Unhandled Interrupt" exception */

struct idt_entry idt[256];
struct idt_ptr idtp;

// This exists in boot.asm, and is used to load our IDT.
extern void _idt_load(void);

/* Use this function to set an entry in the IDT. Alot simpler
*  than twiddling with the GDT ;) */
void idt_set_gate(unsigned char num, uint64_t base, uint16_t sel, unsigned char flags)
{
    idt[num].base_lo = (base & 0xFFFF);
    idt[num].base_hi = (base >> 16) & 0xFFFF;
    idt[num].sel = sel;
    idt[num].alwaysZero = 0;
    idt[num].flags = flags;
}

void idt_install(void)
{
    // Sets the special IDT pointer up.
    idtp.limit = (sizeof(struct idt_entry) * 256) - 1;
    idtp.base = (unsigned int)&idt;

    //Clear out the entire IDT, initializing it to zeros
    memset(&idt, 0, sizeof(struct idt_entry) * 256 - 1);

    // Add any new ISRs to the IDT here using idt_set_gate.

    _idt_load();
}

