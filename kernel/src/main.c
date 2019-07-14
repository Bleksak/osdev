#include "paging.h"
#include "asm.h"
#include "console.h"
#include "gdt.h"
#include "idt.h"
#include "pit.h"
#include "io.h"
#include "cpu/cpu.h"
#include "disk/partition.h"

#include "memory.h"
#include "acpi/acpi.h"
#include "acpi/apic.h"
#include "multiboot.h"

#include "file/elf/elf.h"

#include "isr.h"
#include "mheap.h"
#include "pheap.h"

#include "ebda.h"

#include "tss.h"
#include "usermode/usermode_entry.h"

extern void test_func();

/*
    ****TODO LIST****

APM
ACPI
APIC
Memory allocation -> partially done
SATA
DMA
Syscall -> only present on some AMD CPUs
Filesystems(FAT, NTFS, EXT)
Executables(ELF, PE)
PCI
String manipulation
Sound
Networking
Keyboard
CLI
VGA
Real Time Clock
VeleX(rendering engine)
Userspace
Kernel panic

*/

NORETURN void kernel_main(struct multiboot_info* mbd)
{
    gdt_install();
    idt_install();

    asm volatile("sti");

	cpu_init();
    paging_init(mbd->mmap_addr, mbd->mmap_length);

    BiosDataAreaInit();

    ata_init();

    

    // if(AcpiInit())
    // {
    //     printf("cool");
    // }
    // else
    // {
    //     printf("not cool");
    // }
    


    //partition_init();
    
    for(;;)
    {
        asm volatile("hlt");
    }
}