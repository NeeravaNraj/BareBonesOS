#include <system.h>
#include <stdint.h>


/* Defines a GDT entry. We say packed, because it prevents the
*  compiler from doing things that it thinks is best: Prevent
*  compiler "optimization" by packing */

struct gdt_entry
{
    uint16_t limit_low;
    uint16_t base_low;
    unsigned char base_middle;
    unsigned char access;
    unsigned char granularity;
    unsigned char base_high;
} __attribute__((packed));

/* Special pointer which includes the limit: The max bytes
*  taken up by the GDT, minus 1. Again, this NEEDS to be packed */

struct gdt_ptr{
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

/* Our GDT, with 3 entries, and finally our special GDT pointer */
struct gdt_entry gdt[3];
struct gdt_ptr gp;


/* This will be a function in boot.asm. We use this to properly
*  reload the new segment registers */
extern void _gdt_flush(void);

// Setup a descriptor in the GDT

void gdt_set_gate(int num, uint64_t base, uint64_t limit, unsigned char access, unsigned char gran)
{
    // Setup the descriptor base address
    gdt[num].base_low = (base & 0xFFFF);
    gdt[num].base_middle = (base >> 16) & 0xFF;
    gdt[num].base_high = (base >> 24) & 0xFF;

    // Setup the descriptor limits.
    gdt[num].limit_low = (limit & 0xFFFF); //0xFFFFFFFF & 0xFFFF = 0x0000FFFF
    gdt[num].granularity = ((limit >> 16) & 0xF);

    // Finally, set up the granularity and access flags
    gdt[num].granularity |= (gran & 0xF0);
    gdt[num].access = access;
}

/* This will setup the special GDT pointer,
*  set up the first 3 entries in our GDT, and then
*  finally call gdt_flush() in our assembler file in order
*  to tell the processor where the new GDT is and update the
*  new segment registers */
void gdt_install(void)
{
    // Setup the GDT pointer and limit
    gp.limit = (sizeof (struct gdt_entry) * 3) - 1;
    gp.base = (uint32_t)&gdt;
    
    // Our NULL descriptor
    gdt_set_gate(0, 0, 0, 0, 0);
    
    /* The second entry is our Code Segment. The base address
    *  is 0, the limit is 4GBytes, it uses 4KByte granularity,
    *  uses 32-bit opcodes, and is a Code Segment descriptor. */
    gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);

     /* The third entry is our Data Segment. It's EXACTLY the
    *  same as our code segment, but the descriptor type in
    *  this entry's access byte says it's a Data Segment */

    gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF);

    // Flush out the old GDT and install the new changes!
    _gdt_flush();
}
// access byte code : 10011010
// access byte data : 10010010
// granularity      : 11001111