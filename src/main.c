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

#include "gfx/vga.h"
#include "os.h"

/*
    ****TODO LIST****

ACPI
APIC
Memory allocation -> partially done
SATA
DMA
Filesystems(FAT, NTFS, EXT)
Executables(ELF, PE)
PCI
Sound
Networking
Keyboard
CLI
VGA
Userspace

*/

#include "pci/pci.h"

#include "keyboard.h"

NORETURN void kernel_main(multiboot_info_t* mbd) {
    gdt_install();
    idt_install();
    
    __asm__ volatile("sti");

    memset_classic(&os, 0, sizeof(struct OS));

	cpu_init();
    
    paging_init(mbd->mmap_addr, mbd->mmap_length);

    bda_init();
    
    // pci_init();

    if(acpi_init()) {
        printf("cool");

        keyboard_install_apic();
    }
    else {
        printf("not cool");
    }

    // __asm__ volatile("div %1" :: "a"(k), "d"(0), "r"(0));

    // printf("%s\n", r);

    // printf("Paging initialized\n");
    // printf("Usable memory: %d MB\n", heap_get_free_mem() / (1024 * 1024));

    // bda_init();

    // ata_init();

    // Result read = ata_read(&os.drives[1], 0, 1);


    // if(!read.ok)
    // {
    //     printf("NOK %s\n", read.result);
    // }
    
    

    // unsigned short* res = read.result;
    
    // printf("%x\n", res[255]);

    //partition_init();
    
    for(;;)
    {
        __asm__ volatile("hlt");
    }
}

