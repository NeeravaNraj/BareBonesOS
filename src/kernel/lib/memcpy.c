#include <stddef.h>
#include <system.h>

void memcpy (void *dest, const void *src, size_t size)
{
    const unsigned char *memsrc = (const unsigned char*)src;
    unsigned char* memdest = (unsigned char*)dest;
    if (size <= 0) return;
    for (; size != 0; size--) *memdest++ = *memsrc++;
}