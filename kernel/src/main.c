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
#include "cpu/apic.h"
#include "multiboot.h"

#include "file/elf/elf.h"

#include "isr.h"
#include "mheap.h"
#include "assert.h"
#include "pheap.h"

#include "ebda.h"

/*
    ****TODO LIST****

APM
ACPI
Local APIC
Memory allocation
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

void kernel_main(struct multiboot_info* mbd)
{
    gdt_install();
    idt_install();

	cpu_init();
    paging_init(mbd->mmap_addr, mbd->mmap_length);

    ExtendedBDAInit();

    ata_init();

    AcpiInit();

    //partition_init();
    
	//init_acpi();
    
    for(;;)
    {
        asm volatile("hlt");
    }
}