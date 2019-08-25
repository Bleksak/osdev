#include "paging.h"
#include "console.h"
#include "gdt.h"
#include "interrupt.h"
#include "pit.h"
#include "io.h"
#include "cpu/cpuid.h"
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


#include "assert.h"

#include "string.h"

/*
    ****TODO LIST****

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
#include "align.h"

NORETURN void kernel_main(struct multiboot_info* mbd)
{
    gdt_install();
    idt_install();

    __asm__ volatile("sti");

    (void) mbd;

	cpu_init();

    // paging_init(mbd->mmap_addr, mbd->mmap_length);

    // BiosDataAreaInit();

    // printf("%x\n", pci_config_read_word(0, 0, 0, 0));
    // printf("%x\n", pci_get_header_type(0, 0, 0));

    // ata_init();

    // Result read = ata_read(grab_drive(1), 0, 1);

    // if(!read.ok)
    // {
    //     printf("NOK %s\n", read.result);
    // }

    // unsigned short* res = read.result;
    
    // printf("%x\n", res[255]);

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
        __asm__ volatile("hlt");
    }
}