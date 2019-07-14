#include "ata.h"
#include "io.h"
#include "console.h"
#include "memory.h"
#include "mheap.h"

#define ATA_PRIMARY_IO_BASE 0x1f0
#define ATA_PRIMARY_IO_CONTROL_BASE 0x3f6
#define ATA_SECONDARY_IO_BASE 0x170
#define ATA_SECONDARY_IO_CONTROL_BASE 0x376

enum ATA_REGISTERS
{
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

struct Drive drives[4] =
{
	{
		0,
		0,
		true,
		{
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
		},
		{}, {0}
	},
	{
		1,
		0,
		false,
		{
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
		},
		{}, {0}
	},
	{
		2,
		0,
		true,
		{
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
		},
		{}, {0}
	},
	{
		3,
		0,
		false,
		{
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
		},
		{}, {0}
	}
};

enum ATA_COMMANDS
{
	ATA_IDENTIFY = 0xEC,
};

enum DeviceControlRegister
{
	nIEN = 1 << 1,
	SRST = 1 << 2,
	HOB = 1 << 7, //  	Set this to read back the High Order Byte of the last LBA48 value sent to an IO port.
};

enum DriveAddressRegister
{
	DriveSelect0 = 1 << 0,
	DriveSelect1 = 1 << 1,
	HeadSelect0 = 1 << 2,
	HeadSelect1 = 1 << 3,
	HeadSelect2 = 1 << 4,
	HeadSelect3 = 1 << 5,
	WriteGate = 1 << 6,
};

enum ErrorRegister
{
	AMNF = 1 << 0,
	TKZNF = 1 << 1,
	ABRT = 1 << 2,
	MCR = 1 << 3,
	IDNF = 1 << 4,
	MC = 1 << 5,
	UNC = 1 << 6,
	BBK = 1 << 7,
};

enum DriveRegister
{
	DriveNumber = 1 << 4,
	LBA = 1 << 6,
};

enum StatusRegister
{
	ERR = 1 << 0,
	IDX = 1 << 1,
	CORR = 1 << 2,
	DRQ = 1 << 3,
	SRV = 1 << 4,
	DF = 1 << 5,
	RDY = 1 << 6,
	BSY = 1 << 7,
};

inline static void setDeviceControlRegister(unsigned char id, enum DeviceControlRegister toggle)
{
	outb(drives[id].registers.device_control, inb(drives[id].registers.device_control) | toggle);
}

inline static void clearDeviceControlRegister(unsigned char id, enum DeviceControlRegister toggle)
{
	outb(drives[id].registers.device_control, inb(drives[id].registers.device_control & ~toggle));
}

static option_t ata_detect(struct Drive* drive)
{
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

	if(cl == 0x14 && ch == 0xEB)
	{
		return (option_t)
		{
			.ok = true,
			.result = (any_t)ATA_PATAPI,
		};
	}
	if(cl == 0x69 && ch == 0x96)
	{
		return (option_t)
		{
			.ok = true,
			.result = (any_t)ATA_SATAPI,
		};
	}

	if(cl == 0)
	{
		if(ch == 0)
		{
			return (option_t)
			{
				.ok = true,
				.result = (any_t)ATA_PATA,
			};
		}
		if(ch == 0xC3)
		{
			return (option_t)
			{
				.ok = true,
				.result = (any_t)ATA_SATA,
			};
		}
	}

	return (option_t)
	{
		.ok = false,
		.result = (any_t)ATA_UNKNOWN,
	};
}

static inline bool ata_identify(unsigned int id)
{
	outb(drives[id].registers.drive, 0xA0 | (drives[id].slave << 4));
	outb(drives[id].registers.lba_low, 0);
	outb(drives[id].registers.lba_mid, 0);
	outb(drives[id].registers.lba_high, 0);
	outb(drives[id].registers.sector_count, 0);

	outb(drives[id].registers.command, ATA_IDENTIFY);

	if(!inb(drives[id].registers.status))
	{
		drives[id].type = ATA_UNKNOWN;
		return false;
	}

	while(inb(drives[id].registers.status) & BSY);

	if(inb(drives[id].registers.lba_mid) || inb(drives[id].registers.lba_high))
	{
		drives[id].type = ATA_UNKNOWN;
		return false;
	}

	while(true)
	{
		unsigned char status = inb(drives[id].registers.status);
		if(status & ERR)
		{
			drives[id].type = ATA_UNKNOWN;
			return false;
		}

		if(status & DRQ)
		{
			break;
		}
	}

	for(unsigned int i = 0; i < 256; ++i)
	{
		((unsigned short*)&drives[id].identify)[i] = inw(drives[id].registers.data);
	}

	return true;
}

void ata_init(void)
{
	for(size_t i = 0; i < 4; ++i)
	{
		setDeviceControlRegister(i, nIEN); // disable interrupts
		option_t option;
		if((option = ata_detect(&drives[i])).ok)
		{
			drives[i].type = (int)option.result;

			if(!ata_identify(i))
			{
				continue;
			}

			if(drives[i].identify.Supported2.ExtendedUserAddressableSectors)
			{
				if(drives[i].identify.SectorCountExt > 0x0000FFFFFFFFFFFF)
				{
					printf("Sector Count exceeded maximum value");
				}
				else drives[i].data.ExtSectors = drives[i].identify.SectorCountExt;
			}

			drives[i].data.LowAddressableSectors = drives[i].identify.SectorCountLow;
			drives[i].data.HighAddressableSectors = drives[i].identify.SectorCount;

			if(!drives[i].identify.PhysicalLogicalSectorSize.one || drives[i].identify.PhysicalLogicalSectorSize.zero
				|| !drives[i].identify.PhysicalLogicalSectorSize.MultipleLogicalPerPhysical)
			{
				drives[i].data.LogicalSectorSize = 512;
			}
			else
			{
				drives[i].data.LogicalSectorAlignment = drives[i].identify.LogicalSectorAlignment.offset;
				drives[i].data.LogicalSectorMultiplier = 1 << drives[i].identify.PhysicalLogicalSectorSize.LogicalSectorsPerPhysical;
				drives[i].data.LogicalSectorSize = 512 * drives[i].data.LogicalSectorMultiplier;
			}

			printf("%d\n", drives[i].data.LogicalSectorSize);

		}
	}
}
