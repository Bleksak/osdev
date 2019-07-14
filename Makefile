SRC := kernel/src/
INFILES := $(shell find $(SRC) -name '*.asm') $(shell find $(SRC) -name '*.c')

BIN := kernel/bin/
OUTFILES := $(INFILES:.c=.o)
OUTFILES := $(subst .asm,.o,$(OUTFILES))
OUTFILES := $(subst $(SRC),$(BIN),$(OUTFILES))

LOSETUP1 := 9
LOSETUP2 :=	$(shell expr $(LOSETUP1) \+ 1)

DEPS := $(OUTFILES:.o=.d)

CC := i686-elf-gcc
CC64 := x86_64-elf-gcc
CCFLAGS := -m32 -nostdlib -std=c11 -ffreestanding -Wall -Wextra -O0 -D _KERNEL_CPU_32 -include "attributes.h"

AS := nasm
ASFLAGS := -f elf32

LD64 := x86_64-elf-ld
LD := i686-elf-ld
LDFLAGS := -nostdlib -m32

KERNEL := $(BIN)jane.bin

all: $(KERNEL) kernel.iso
	@losetup /dev/loop$(LOSETUP1) kernel.iso -o 1048576
	@mount /dev/loop$(LOSETUP1) /mnt

	cp $(BIN)jane.bin isodir/boot/velex.iso
	cp -R isodir/boot/ /mnt/
	
	@umount /mnt
	@losetup -d /dev/loop$(LOSETUP1)

fix:
	-umount /mnt
	-losetup -d /dev/loop$(LOSETUP1)
	-losetup -d /dev/loop$(LOSETUP2)

kernel.iso : $(grub)

grub: $(KERNEL)
	qemu-img create kernel.iso 1G
	parted kernel.iso mklabel msdos \ mkpart primary 1MB 1GB
	
	-@losetup /dev/loop$(LOSETUP1) kernel.iso
	-@losetup /dev/loop$(LOSETUP2) kernel.iso -o 1048576
	
	mkdosfs -F32 -f 2 /dev/loop$(LOSETUP2)

	mount /dev/loop$(LOSETUP2) /mnt
	
	grub-install --root-directory=/mnt --target=i386-pc --no-floppy --modules="normal part_msdos ext2 multiboot fat" /dev/loop$(LOSETUP1)
	cp $(BIN)jane.bin isodir/boot/jane.iso
	cp -R isodir/boot/ /mnt/
	umount /mnt

	losetup -d /dev/loop$(LOSETUP1)
	losetup -d /dev/loop$(LOSETUP2)


run: kernel.iso
	qemu-system-i386 -smp cpus=2,threads=1 -drive format=raw,file=kernel.iso -m 2G

clean:
	-rm $(BIN)/*.o
	-rm $(BIN)/cpu/*.o
	-rm $(BIN)/disk/*.o
	-rm $(BIN)/disk/fs/*.o
	-rm $(BIN)/file/elf/*.o
	-rm $(BIN)/acpi/*.o

cd: kernel.iso
	qemu-system-x86_64 -cdrom kernel.iso -m 1G

$(KERNEL): $(OUTFILES) 
	$(CC) $(LDFLAGS) -I $(SRC) -T linker.ld $(OUTFILES) -o $@ -lgcc

$(BIN)%.o : $(SRC)%.asm 
	mkdir -p $(@D)
	$(AS) $(ASFLAGS) -o $@ $<

$(BIN)%.o : $(SRC)%.c
	mkdir -p $(@D)
	$(CC) $(CCFLAGS) -I $(SRC) -o $@ -c $< -lgcc
