cmake_minimum_required(VERSION 2.8)
project(kernel)

# link_libraries(-nostdlib "-T ${PROJECT_SOURCE_DIR}/linker.ld" -lgcc)

set(CMAKE_ASM_NASM_LINK_EXECUTABLE "i686-elf-gcc <LINK_FLAGS> <OBJECTS> -o <TARGET> <LINK_LIBRARIES>")
set(CMAKE_ASM_NASM_OBJECT_FORMAT elf32)
enable_language(ASM_NASM)

# set(CMAKE_C_FLAGS "-D _x86 -x c -Wno-cpp -std=c17 -Wall -Wextra -ffreestanding -nostdlib -nostdinc -march=i686")

FILE(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/isodir)
FILE(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/isodir/boot)
FILE(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/isodir/boot/grub)
FILE(WRITE ${CMAKE_BINARY_DIR}/isodir/boot/grub/grub.cfg
    "set timeout=15\n"
    "set default=0\n"

    "menuentry \"VeleX\" {\n"
        "\tmultiboot /boot/velex.iso\n"
        "\tboot\n"
    "}\n"
)

FILE(WRITE ${CMAKE_BINARY_DIR}/grub.make
"LOSETUP1 := 9\n"
"LOSETUP2 := $(shell expr $(LOSETUP1) \\+ 1)\n"

"all: \n"
    "\tsudo qemu-img create kernel.iso 1G\n"
    "\tsudo parted kernel.iso mklabel msdos \ mkpart primary 1MB 1GB\n"

    "\t-@sudo losetup /dev/loop$(LOSETUP1) kernel.iso\n"
    "\t-@sudo losetup /dev/loop$(LOSETUP2) kernel.iso -o 1048576\n"

    "\tsudo mkdosfs -F32 -f 2 /dev/loop$(LOSETUP2)\n"

    "\tsudo mount /dev/loop$(LOSETUP2) /mnt\n"

    "\tsudo grub-install --root-directory=/mnt --target=i386-pc --no-floppy --modules=\"normal part_msdos ext2 multiboot fat\" /dev/loop$(LOSETUP1)\n"
    "\tsudo cp -R isodir/boot/ /mnt/\n"
    "\tsudo umount /mnt\n"

    "\tsudo losetup -d /dev/loop$(LOSETUP1)\n"
    "\tsudo losetup -d /dev/loop$(LOSETUP2)\n"
    "\tsudo chmod 777 -R isodir\n"
    "\tsudo chmod 777 kernel.iso\n"

"update: kernel.elf\n"
    "\t-@losetup /dev/loop$(LOSETUP1) kernel.iso -o 1048576\n"
    "\t@mount /dev/loop$(LOSETUP1) /mnt\n"

    "\tcp $(BIN)kernel.elf isodir/boot/velex.iso\n"
    "\tcp -R isodir/boot/ /mnt/\n"

    "\t@umount /mnt\n"
    "\t-@losetup -d /dev/loop$(LOSETUP1)\n"
)

add_custom_target(grub 
    make -f grub.make
)

add_custom_target(run
    sudo qemu-system-x86_64 -smp cpus=2,threads=1 -drive format=raw,file=kernel.iso -m 2G -cpu host --enable-kvm
)

add_executable(kernel.elf
    src/acpi/acpi.c
    src/acpi/acpi.h
    src/acpi/apic.asm
    src/acpi/apic.c
    src/acpi/apic.h
    src/acpi/ioapic.c
    src/acpi/ioapic.h
    src/acpi/madt.c
    src/acpi/madt.h
    src/acpi/rsdp.c
    src/acpi/rsdp.h

    src/cpu/ext/avx.c
    src/cpu/ext/avx.h
    src/cpu/ext/fpu.asm
    src/cpu/ext/fpu.c
    src/cpu/ext/fpu.h
    src/cpu/ext/sse.c
    src/cpu/ext/sse.h
    src/cpu/ext/xsave.c
    src/cpu/ext/xsave.h

    src/cpu/cpuid.c
    src/cpu/cpuid.h
    src/cpu/cr.c
    src/cpu/cr.h
    
    src/disk/ata.c
    src/disk/ata.h
    src/disk/partition.c
    src/disk/partition.h

    src/file/elf/elf.c
    src/file/elf/elf.h

    src/pci/pci.h
    src/pci/pci.c

    src/align.c
    src/align.h
    src/assert.c
    src/assert.h
    src/attributes.h
    src/bda.c
    src/bda.h
    src/boot.asm
    src/bound.c
    src/bound.h
    src/console.c
    src/console.h
    src/ebda.c
    src/ebda.h
    src/gdt.c
    src/gdt.h
    src/io.c
    src/io.h
    src/interrupt.c
    src/interrupt.h
    src/irq.asm
    src/isr.asm
    src/isr.c
    src/isr.h
    src/keyboard.c
    src/keyboard.h
    src/main.c
    src/memory.asm
    src/memory.c
    src/memory.h
    src/mheap.c
    src/mheap.h
    src/multiboot.h
    src/paging.asm
    src/paging.c
    src/paging.h
    src/panic.c
    src/panic.h
    src/pheap.c
    src/pheap.h
    src/pit.c
    src/pit.h
    src/result.h
    src/string.h
    src/string.c
    src/std.h
    src/os.h
    src/os.c
)

add_custom_command(TARGET kernel.elf
                   POST_BUILD
                   COMMAND sudo make -f grub.make update)