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
#include "multiboot.h"

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

__attribute__((noreturn)) void kernel_main(multiboot_info_t* mbd) {
    gdt_install();
    idt_install();

    paging_init(mbd->mmap_addr, mbd->mmap_length);
    os_init();

    // for(size_t i = 0; i < os.apic.interrupt_override.length; ++i) {
	// 	printf("Bus: %d\n", os.apic.interrupt_override.entries[i].bus_source);
    //     printf("IRQ: %d\n", os.apic.interrupt_override.entries[i].irq_source);
    //     printf("Global Interrupt: %d\n", os.apic.interrupt_override.entries[i].global_system_interrupt);
	// }

    keyboard_install(); // keyboard is in IOAPIC mode

    // vga_init();

    // bda_init();
    
    // pci_init();

    // ioapic_override_fix();

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

    for(;;) {
        __asm__ volatile("hlt");
    }
}

