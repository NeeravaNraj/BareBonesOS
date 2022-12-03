#include <stddef.h>
#include <stdint.h>
#include <system.h>

void memset(void *dest,const unsigned char src, size_t size)
{
    unsigned char* memdest = (unsigned char*)dest;
    for(; size != 0; size--) *memdest++ = src;
}

void memsetw(void *dest,const unsigned short src, size_t size)
{
    unsigned short* memdest = (unsigned short*)dest;
    for(; size != 0; size--) *memdest++ = src;
}
