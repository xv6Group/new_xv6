#include "types.h"
#include "defs.h"
#include "memlayout.h"
#include "vesamode.h"

void vesamodeinit()
{
    unsigned int memaddr = KERNBASE + 0x1028;
    unsigned int physaddr = *((unsigned int*)memaddr);
    SCREEN_PHYSADDR = (unsigned short*)physaddr;
    SCREEN_WIDTH = *((unsigned short*)(KERNBASE + 0x1012));
    SCREEN_HEIGHT = *((unsigned short*)(KERNBASE + 0x1014));
    VESA_ADDR = SCREEN_PHYSADDR;

    cprintf("SCREEN PHYSICAL ADDRESS: %x\n", SCREEN_PHYSADDR);
    cprintf("SCREEN WIDTH: %d\n", SCREEN_WIDTH);
    cprintf("SCREEN HEIGHT: %d\n", SCREEN_HEIGHT);
    cprintf("SCREEN BPP: %d\n", *((uchar*)(KERNBASE + 0x1019)));
}
