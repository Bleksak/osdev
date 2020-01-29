# The set of languages for which implicit dependencies are needed:
set(CMAKE_DEPENDS_LANGUAGES
  "ASM_NASM"
  "C"
  )
# The set of files for implicit dependencies of each language:
set(CMAKE_DEPENDS_CHECK_ASM_NASM
  "/home/bleksak/dev/osdev/src/boot.asm" "/home/bleksak/dev/osdev/build/CMakeFiles/kernel.elf.dir/src/boot.asm.o"
  "/home/bleksak/dev/osdev/src/cpu/ext/fpu.asm" "/home/bleksak/dev/osdev/build/CMakeFiles/kernel.elf.dir/src/cpu/ext/fpu.asm.o"
  "/home/bleksak/dev/osdev/src/irq.asm" "/home/bleksak/dev/osdev/build/CMakeFiles/kernel.elf.dir/src/irq.asm.o"
  "/home/bleksak/dev/osdev/src/isr.asm" "/home/bleksak/dev/osdev/build/CMakeFiles/kernel.elf.dir/src/isr.asm.o"
  "/home/bleksak/dev/osdev/src/memory.asm" "/home/bleksak/dev/osdev/build/CMakeFiles/kernel.elf.dir/src/memory.asm.o"
  "/home/bleksak/dev/osdev/src/paging.asm" "/home/bleksak/dev/osdev/build/CMakeFiles/kernel.elf.dir/src/paging.asm.o"
  )
set(CMAKE_ASM_NASM_COMPILER_ID "NASM")

# The include file search paths:
set(CMAKE_ASM_NASM_TARGET_INCLUDE_PATH
  "../src"
  "../SYSTEM"
  )
set(CMAKE_DEPENDS_CHECK_C
  "/home/bleksak/dev/osdev/src/acpi/acpi.c" "/home/bleksak/dev/osdev/build/CMakeFiles/kernel.elf.dir/src/acpi/acpi.c.o"
  "/home/bleksak/dev/osdev/src/acpi/madt.c" "/home/bleksak/dev/osdev/build/CMakeFiles/kernel.elf.dir/src/acpi/madt.c.o"
  "/home/bleksak/dev/osdev/src/acpi/rsdp.c" "/home/bleksak/dev/osdev/build/CMakeFiles/kernel.elf.dir/src/acpi/rsdp.c.o"
  "/home/bleksak/dev/osdev/src/align.c" "/home/bleksak/dev/osdev/build/CMakeFiles/kernel.elf.dir/src/align.c.o"
  "/home/bleksak/dev/osdev/src/assert.c" "/home/bleksak/dev/osdev/build/CMakeFiles/kernel.elf.dir/src/assert.c.o"
  "/home/bleksak/dev/osdev/src/bda.c" "/home/bleksak/dev/osdev/build/CMakeFiles/kernel.elf.dir/src/bda.c.o"
  "/home/bleksak/dev/osdev/src/bound.c" "/home/bleksak/dev/osdev/build/CMakeFiles/kernel.elf.dir/src/bound.c.o"
  "/home/bleksak/dev/osdev/src/console.c" "/home/bleksak/dev/osdev/build/CMakeFiles/kernel.elf.dir/src/console.c.o"
  "/home/bleksak/dev/osdev/src/cpu/cpuid.c" "/home/bleksak/dev/osdev/build/CMakeFiles/kernel.elf.dir/src/cpu/cpuid.c.o"
  "/home/bleksak/dev/osdev/src/cpu/cr.c" "/home/bleksak/dev/osdev/build/CMakeFiles/kernel.elf.dir/src/cpu/cr.c.o"
  "/home/bleksak/dev/osdev/src/cpu/ext/avx.c" "/home/bleksak/dev/osdev/build/CMakeFiles/kernel.elf.dir/src/cpu/ext/avx.c.o"
  "/home/bleksak/dev/osdev/src/cpu/ext/fpu.c" "/home/bleksak/dev/osdev/build/CMakeFiles/kernel.elf.dir/src/cpu/ext/fpu.c.o"
  "/home/bleksak/dev/osdev/src/cpu/ext/sse.c" "/home/bleksak/dev/osdev/build/CMakeFiles/kernel.elf.dir/src/cpu/ext/sse.c.o"
  "/home/bleksak/dev/osdev/src/cpu/ext/xsave.c" "/home/bleksak/dev/osdev/build/CMakeFiles/kernel.elf.dir/src/cpu/ext/xsave.c.o"
  "/home/bleksak/dev/osdev/src/disk/ata.c" "/home/bleksak/dev/osdev/build/CMakeFiles/kernel.elf.dir/src/disk/ata.c.o"
  "/home/bleksak/dev/osdev/src/disk/partition.c" "/home/bleksak/dev/osdev/build/CMakeFiles/kernel.elf.dir/src/disk/partition.c.o"
  "/home/bleksak/dev/osdev/src/ebda.c" "/home/bleksak/dev/osdev/build/CMakeFiles/kernel.elf.dir/src/ebda.c.o"
  "/home/bleksak/dev/osdev/src/file/elf/elf.c" "/home/bleksak/dev/osdev/build/CMakeFiles/kernel.elf.dir/src/file/elf/elf.c.o"
  "/home/bleksak/dev/osdev/src/gdt.c" "/home/bleksak/dev/osdev/build/CMakeFiles/kernel.elf.dir/src/gdt.c.o"
  "/home/bleksak/dev/osdev/src/gfx/vga.c" "/home/bleksak/dev/osdev/build/CMakeFiles/kernel.elf.dir/src/gfx/vga.c.o"
  "/home/bleksak/dev/osdev/src/interrupt.c" "/home/bleksak/dev/osdev/build/CMakeFiles/kernel.elf.dir/src/interrupt.c.o"
  "/home/bleksak/dev/osdev/src/interrupt/8259.c" "/home/bleksak/dev/osdev/build/CMakeFiles/kernel.elf.dir/src/interrupt/8259.c.o"
  "/home/bleksak/dev/osdev/src/interrupt/apic.c" "/home/bleksak/dev/osdev/build/CMakeFiles/kernel.elf.dir/src/interrupt/apic.c.o"
  "/home/bleksak/dev/osdev/src/interrupt/interrupt.c" "/home/bleksak/dev/osdev/build/CMakeFiles/kernel.elf.dir/src/interrupt/interrupt.c.o"
  "/home/bleksak/dev/osdev/src/interrupt/ioapic.c" "/home/bleksak/dev/osdev/build/CMakeFiles/kernel.elf.dir/src/interrupt/ioapic.c.o"
  "/home/bleksak/dev/osdev/src/io.c" "/home/bleksak/dev/osdev/build/CMakeFiles/kernel.elf.dir/src/io.c.o"
  "/home/bleksak/dev/osdev/src/keyboard.c" "/home/bleksak/dev/osdev/build/CMakeFiles/kernel.elf.dir/src/keyboard.c.o"
  "/home/bleksak/dev/osdev/src/main.c" "/home/bleksak/dev/osdev/build/CMakeFiles/kernel.elf.dir/src/main.c.o"
  "/home/bleksak/dev/osdev/src/memory.c" "/home/bleksak/dev/osdev/build/CMakeFiles/kernel.elf.dir/src/memory.c.o"
  "/home/bleksak/dev/osdev/src/mheap.c" "/home/bleksak/dev/osdev/build/CMakeFiles/kernel.elf.dir/src/mheap.c.o"
  "/home/bleksak/dev/osdev/src/os.c" "/home/bleksak/dev/osdev/build/CMakeFiles/kernel.elf.dir/src/os.c.o"
  "/home/bleksak/dev/osdev/src/paging.c" "/home/bleksak/dev/osdev/build/CMakeFiles/kernel.elf.dir/src/paging.c.o"
  "/home/bleksak/dev/osdev/src/pci/pci.c" "/home/bleksak/dev/osdev/build/CMakeFiles/kernel.elf.dir/src/pci/pci.c.o"
  "/home/bleksak/dev/osdev/src/pheap.c" "/home/bleksak/dev/osdev/build/CMakeFiles/kernel.elf.dir/src/pheap.c.o"
  "/home/bleksak/dev/osdev/src/pit.c" "/home/bleksak/dev/osdev/build/CMakeFiles/kernel.elf.dir/src/pit.c.o"
  "/home/bleksak/dev/osdev/src/str.c" "/home/bleksak/dev/osdev/build/CMakeFiles/kernel.elf.dir/src/str.c.o"
  )
set(CMAKE_C_COMPILER_ID "GNU")

# The include file search paths:
set(CMAKE_C_TARGET_INCLUDE_PATH
  "../src"
  "../SYSTEM"
  )

# Targets to which this target links.
set(CMAKE_TARGET_LINKED_INFO_FILES
  )

# Fortran module output directory.
set(CMAKE_Fortran_TARGET_MODULE_DIR "")
