#include "ata.h"
#include "../io.h"
#include "../console.h"
#include "../memory.h"
#include "../mheap.h"

#define ATA_PRIMARY_IO_BASE 0x1f0
#define ATA_PRIMARY_IO_CONTROL_BASE 0x3f6
#define ATA_SECONDARY_IO_BASE 0x170
#define ATA_SECONDARY_IO_CONTROL_BASE 0x376

enum ATA_REGISTERS {
	ATA_DATA_REGISTER = 0,
	ATA_ALTERNATE_STATUS_REGISTER = 0,
	ATA_DEVICE_CONTROL_REGISTER = 0,
	ATA_ERROR_REGISTER = 1,
	ATA_FEATURES_REGISTER = 1,
	ATA_DRIVE_ADDRESS_REGISTER = 1,
	ATA_SECTOR_COUNT_REGISTER = 2,
	ATA_LBA_LOW_REGISTER = 3,
	ATA_LBA_MID_REGISTER = 4,
	ATA_LBA_HIGH_REGISTER = 5,
	ATA_DRIVE_REGISTER = 6,
	ATA_STATUS_REGISTER = 7,
	ATA_COMMAND_REGISTER = 7,
};

static const struct ATARegisters primary_registers = {
	ATA_PRIMARY_IO_BASE + ATA_DATA_REGISTER,
	ATA_PRIMARY_IO_BASE + ATA_ERROR_REGISTER,
	ATA_PRIMARY_IO_BASE + ATA_FEATURES_REGISTER,
	ATA_PRIMARY_IO_BASE + ATA_SECTOR_COUNT_REGISTER,
	ATA_PRIMARY_IO_BASE + ATA_LBA_LOW_REGISTER,
	ATA_PRIMARY_IO_BASE + ATA_LBA_MID_REGISTER,
	ATA_PRIMARY_IO_BASE + ATA_LBA_HIGH_REGISTER,
	ATA_PRIMARY_IO_BASE + ATA_DRIVE_REGISTER,
	ATA_PRIMARY_IO_BASE + ATA_STATUS_REGISTER,
	ATA_PRIMARY_IO_BASE + ATA_COMMAND_REGISTER,
	ATA_PRIMARY_IO_CONTROL_BASE + ATA_ALTERNATE_STATUS_REGISTER,
	ATA_PRIMARY_IO_CONTROL_BASE + ATA_DEVICE_CONTROL_REGISTER,
	ATA_PRIMARY_IO_CONTROL_BASE + ATA_DRIVE_ADDRESS_REGISTER,
};

static const struct ATARegisters secondary_registers = {
	ATA_SECONDARY_IO_BASE + ATA_DATA_REGISTER,
	ATA_SECONDARY_IO_BASE + ATA_ERROR_REGISTER,
	ATA_SECONDARY_IO_BASE + ATA_FEATURES_REGISTER,
	ATA_SECONDARY_IO_BASE + ATA_SECTOR_COUNT_REGISTER,
	ATA_SECONDARY_IO_BASE + ATA_LBA_LOW_REGISTER,
	ATA_SECONDARY_IO_BASE + ATA_LBA_MID_REGISTER,
	ATA_SECONDARY_IO_BASE + ATA_LBA_HIGH_REGISTER,
	ATA_SECONDARY_IO_BASE + ATA_DRIVE_REGISTER,
	ATA_SECONDARY_IO_BASE + ATA_STATUS_REGISTER,
	ATA_SECONDARY_IO_BASE + ATA_COMMAND_REGISTER,
	ATA_SECONDARY_IO_CONTROL_BASE + ATA_ALTERNATE_STATUS_REGISTER,
	ATA_SECONDARY_IO_CONTROL_BASE + ATA_DEVICE_CONTROL_REGISTER,
	ATA_SECONDARY_IO_CONTROL_BASE + ATA_DRIVE_ADDRESS_REGISTER,
};

enum ATA_COMMANDS {
	ATA_IDENTIFY = 0xEC,
	ATA_READ_SECTORS = 0x20,
	ATA_READ_SECTORS_EXT = 0x24,
	ATA_WRITE_SECTORS = 0x30,
	ATA_WRITE_SECTORS_EXT = 0x34,
	ATA_FLUSH_CACHE = 0xE7,
};

enum DeviceControlRegister {
	nIEN = 1 << 1,
	SRST = 1 << 2,
	HOB = 1 << 7, //  	Set this to read back the High Order Byte of the last LBA48 value sent to an IO port.
};

enum DriveAddressRegister {
	DriveSelect0 = 1 << 0,
	DriveSelect1 = 1 << 1,
	HeadSelect0 = 1 << 2,
	HeadSelect1 = 1 << 3,
	HeadSelect2 = 1 << 4,
	HeadSelect3 = 1 << 5,
	WriteGate = 1 << 6,
};

enum ErrorRegister {
	AMNF = 1 << 0,
	TKZNF = 1 << 1,
	ABRT = 1 << 2,
	MCR = 1 << 3,
	IDNF = 1 << 4,
	MC = 1 << 5,
	UNC = 1 << 6,
	BBK = 1 << 7,
};

enum DriveRegister {
	DriveNumber = 1 << 4,
	LBA = 1 << 6,
};

enum StatusRegister {
	ERR = 1 << 0,
	IDX = 1 << 1,
	CORR = 1 << 2,
	DRQ = 1 << 3,
	SRV = 1 << 4,
	DF = 1 << 5,
	RDY = 1 << 6,
	BSY = 1 << 7,
};

inline static void setDeviceControlRegister(unsigned char id, enum DeviceControlRegister toggle) {
	outb(drives[id].registers.device_control, inb(drives[id].registers.device_control) | toggle);
}

inline static void clearDeviceControlRegister(unsigned char id, enum DeviceControlRegister toggle) {
	outb(drives[id].registers.device_control, inb(drives[id].registers.device_control & ~toggle));
}

static enum ATA_TYPE ata_detect(Drive* drive) {
	setDeviceControlRegister(drive->id, SRST); // do a software reset
	inb(drive->registers.device_control);
	clearDeviceControlRegister(drive->id, SRST);
	outb(drive->registers.drive, 0xA0 | (drive->slave << 4));

	inb(drive->registers.device_control);
	inb(drive->registers.device_control);
	inb(drive->registers.device_control);
	inb(drive->registers.device_control);

	unsigned int cl = inb(drive->registers.lba_mid);
	unsigned int ch = inb(drive->registers.lba_high);

	if(cl == 0x14 && ch == 0xEB) {
		return ATA_PATAPI;
	}

	if(cl == 0x69 && ch == 0x96) {
		return ATA_SATAPI;
	}

	if(cl == 0) {
		if(ch == 0) {
			return ATA_PATA;
		}
		if(ch == 0xC3) {
			return ATA_SATA;
		}
	}

	return ATA_UNKNOWN;
}

static bool ata_identify(unsigned int id) {
	outb(drives[id].registers.drive, 0xA0 | (drives[id].slave << 4));
	outb(drives[id].registers.lba_low, 0);
	outb(drives[id].registers.lba_mid, 0);
	outb(drives[id].registers.lba_high, 0);
	outb(drives[id].registers.sector_count, 0);

	outb(drives[id].registers.command, ATA_IDENTIFY);

	if(!inb(drives[id].registers.status)) {
		drives[id].type = ATA_UNKNOWN;
		return false;
	}

	while(inb(drives[id].registers.status) & BSY);

	if(inb(drives[id].registers.lba_mid) || inb(drives[id].registers.lba_high)) {
		drives[id].type = ATA_UNKNOWN;
		return false;
	}

	while(true) {
		unsigned char status = inb(drives[id].registers.status);
		if(status & ERR) {
			drives[id].type = ATA_UNKNOWN;
			return false;
		}

		if(status & DRQ) {
			break;
		}
	}

	for(unsigned int i = 0; i < 256; ++i) {
		((unsigned short*)&drives[id].identify)[i] = inw(drives[id].registers.data);
	}

	return true;
}

size_t ata_init(void) {

	size_t drive_count = 0;

	for(size_t i = 0; i < 4; ++i) {
		setDeviceControlRegister(i, nIEN); // disable interrupts
		enum ATA_TYPE type = ata_detect(&drives[i]);
		if(type == ATA_UNKNOWN) {
			continue;
		}

		drives[i].type = type;

		if(!ata_identify(i)) {
			continue;
		}

		if(drives[i].identify.Supported2.ExtendedUserAddressableSectors) {
			if(drives[i].identify.SectorCountExt > 0x0000FFFFFFFFFFFF) {
				printf("Sector Count exceeded maximum value");
			}
			else drives[i].data.ExtSectors = drives[i].identify.SectorCountExt;
		}
		else drives[i].data.ExtSectors = 0;

		drives[i].data.LowAddressableSectors = drives[i].identify.SectorCountLow;
		drives[i].data.HighAddressableSectors = drives[i].identify.SectorCount;

		if(!drives[i].identify.PhysicalLogicalSectorSize.one || drives[i].identify.PhysicalLogicalSectorSize.zero
			|| !drives[i].identify.PhysicalLogicalSectorSize.MultipleLogicalPerPhysical) {

			drives[i].data.PhysicalSectorSize = 512;
		}
		else {
			drives[i].data.LogicalSectorAlignment = drives[i].identify.LogicalSectorAlignment.offset;
			drives[i].data.LogicalSectorMultiplier = 1 << drives[i].identify.PhysicalLogicalSectorSize.LogicalSectorsPerPhysical;
			drives[i].data.PhysicalSectorSize = 512 * drives[i].data.LogicalSectorMultiplier;

		}

		drive_count++;
	}

	return drive_count;
}

static void ata_prepare_low_operation(const Drive* drive, unsigned long long lba, unsigned char count) {
	outb(drive->registers.lba_low, lba & 0xFF);
	outb(drive->registers.lba_mid, (lba >> 8) & 0xFF);
	outb(drive->registers.lba_high, (lba >> 16) & 0xFF);
	outb(drive->registers.sector_count, count);
}

static void ata_prepare_high_operation(const Drive* drive, unsigned long long lba, unsigned short count) {
	outb(drive->registers.sector_count, (count >> 8) & 0xFF);
	outb(drive->registers.lba_low, (lba >> 24) & 0xFF);
	outb(drive->registers.lba_mid, (lba >> 32) & 0xFF);
	outb(drive->registers.lba_high, (lba >> 40) & 0xFF);
	outb(drive->registers.sector_count, count & 0xFF);
	outb(drive->registers.lba_low, lba & 0xFF);
	outb(drive->registers.lba_mid, (lba >> 8) & 0xFF);
	outb(drive->registers.lba_high, (lba >> 16) & 0xFF);
}

static bool ata_poll(const Drive* drive) {
	unsigned char status;

	while(inb(drive->registers.status) & BSY);
	while(!((status = inb(drive->registers.status)) & DRQ)) {
		if(status & ERR) {
			return false;
		}
	}

	return true;
}

static Result ata_actual_read(const Drive* drive, unsigned char count) {
	unsigned short* buffer = malloc(512 * count);

	if(!buffer) {
		return Err("Out of memory");
	}

	for(unsigned int i = 0; i < count; ++i) {
		if(!ata_poll(drive)) {
			free(buffer);
			return Err("Read failed");
		}

		for(unsigned short j = 0; j < 256; ++j) {
			buffer[i * 256 + j] = inw(drive->registers.data);
		}
	}

	return Ok(buffer);
}

static bool ata_actual_write(const Drive* drive, unsigned char count, const unsigned short* buffer) {
	if(!ata_poll(drive)) {
		return false;
	}

	for(unsigned int i = 0; i < count; ++i) {
		for(unsigned int j = 0; j < 256; ++j) {
			outw(drive->registers.data, buffer[i * 256 + j]);
		}
	}

	outb(drive->registers.data, ATA_FLUSH_CACHE);
	return ata_poll(drive);
}

static Result ata_read_low(const Drive* drive, unsigned long long lba, unsigned char count) {
	outb(drive->registers.drive, ((lba >> 24) & 0xF) | (0xE0 | drive->slave << 4));
	ata_prepare_low_operation(drive, lba, count);
	outb(drive->registers.command, ATA_READ_SECTORS);

	return ata_actual_read(drive, count);
}

static Result ata_read_high(const Drive* drive, unsigned long long lba, unsigned short count) {
	outb(drive->registers.drive, 0x40 | (drive->slave << 4));
	ata_prepare_high_operation(drive, lba, count);
	outb(drive->registers.command, ATA_READ_SECTORS_EXT);

	return ata_actual_read(drive, count);
}

static bool ata_write_low(const Drive* drive, unsigned long long lba, unsigned char count, const unsigned short* buffer) {
	outb(drive->registers.drive, ((lba >> 24) & 0xF) | (0xE0 | drive->slave << 4));
	ata_prepare_low_operation(drive, lba, count);
	outb(drive->registers.command, ATA_WRITE_SECTORS);

	return ata_actual_write(drive, count, buffer);
}

static bool ata_write_high(const Drive* drive, unsigned long long lba, unsigned short count, const unsigned short* buffer) {
	outb(drive->registers.drive, 0x40 | (drive->slave << 4));
	ata_prepare_high_operation(drive, lba, count);
	outb(drive->registers.command, ATA_WRITE_SECTORS_EXT);

	return ata_actual_write(drive, count, buffer);
}

const Drive* grab_drive(unsigned char i) {
	return &drives[i];
}

Result ata_read(const Drive* drive, unsigned long long lba, unsigned short count) {
	// @TODO: http://ata.wiki.kernel.org/index.php/ATA_4KiB_sector_issues
	// Add logical/physical sector offsetting

	if(drive->type == ATA_UNKNOWN) {
		return Err("Trying to read from a non existant drive");
	}

	if(!count) {
		return Err("Cannot read 0 sectors");
	}

	unsigned long long lba_start = 512 * (lba + drive->data.LogicalSectorAlignment) / drive->data.PhysicalSectorSize;
	unsigned long long lba_end   = 512 * (lba + count + drive->data.LogicalSectorAlignment) / drive->data.PhysicalSectorSize;
	unsigned short actual_count  = lba_end - lba_start + 1;

	if(lba_end <= drive->data.LowAddressableSectors && actual_count <= 256) {
		return ata_read_low(drive, lba_start, (actual_count == 256) ? 0 : (unsigned char)actual_count);
	}

	if(lba_end > drive->data.HighAddressableSectors && !drive->data.ExtSectors) {
		return Err("Trying to read from LBA out of range");
	}

	return ata_read_high(drive, lba_start, actual_count);
}

Result ata_write(const Drive* drive, unsigned long long lba, unsigned short count, const unsigned short* buffer) {
	if(drive->type == ATA_UNKNOWN) {
		return Err("Trying to read from a non existant drive");
	}

	if(!count) {
		return Err("Cannot read 0 sectors");
	}

	// TODO: Need to fix writing after alignment or we will write garbage/to a wrong place

	unsigned long long lba_start = 512 * (lba + drive->data.LogicalSectorAlignment) / drive->data.PhysicalSectorSize;
	unsigned long long lba_end   = 512 * (lba + count + drive->data.LogicalSectorAlignment) / drive->data.PhysicalSectorSize;
	unsigned short actual_count  = lba_end - lba_start + 1;

	if(lba_end <= drive->data.LowAddressableSectors && actual_count <= 256) {
		return ata_write_low(drive, lba_start, (actual_count == 256) ? 0 : (unsigned char)actual_count, buffer) ? Ok(0) : Err("Failed to write");
	}

	if(lba_end > drive->data.HighAddressableSectors && !drive->data.ExtSectors) {
		return Err("Trying to write from LBA out of range");
	}

	return ata_write_high(drive, lba_start, (actual_count == 256) ? 0 : (unsigned char)actual_count, buffer) ? Ok(0) : Err("Failed to write");
}