LOSETUP1 := 9
LOSETUP2 := $(shell expr $(LOSETUP1) \+ 1)
all: 
	sudo qemu-img create kernel.iso 1G
	sudo parted kernel.iso mklabel msdos  mkpart primary 1MB 1GB
	-@sudo losetup /dev/loop$(LOSETUP1) kernel.iso
	-@sudo losetup /dev/loop$(LOSETUP2) kernel.iso -o 1048576
	sudo mkdosfs -F32 -f 2 /dev/loop$(LOSETUP2)
	sudo mount /dev/loop$(LOSETUP2) /mnt
	sudo grub-install --root-directory=/mnt --target=i386-pc --no-floppy --modules="normal part_msdos ext2 multiboot fat" /dev/loop$(LOSETUP1)
	sudo cp -R isodir/boot/ /mnt/
	sudo umount /mnt
	sudo losetup -d /dev/loop$(LOSETUP1)
	sudo losetup -d /dev/loop$(LOSETUP2)
	sudo chmod 777 -R isodir
	sudo chmod 777 kernel.iso
update: kernel.elf
	-@losetup /dev/loop$(LOSETUP1) kernel.iso -o 1048576
	@mount /dev/loop$(LOSETUP1) /mnt
	cp $(BIN)kernel.elf isodir/boot/velex.iso
	cp -R isodir/boot/ /mnt/
	@umount /mnt
	-@losetup -d /dev/loop$(LOSETUP1)
